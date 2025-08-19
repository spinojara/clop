/////////////////////////////////////////////////////////////////////////////
//
// CRealObserver.cpp
//
// Rémi Coulom
//
// November, 2009
//
/////////////////////////////////////////////////////////////////////////////
#include "CRealObserver.h"
#include "CResults.h"
#include "CParameterCollection.h"
#include "CMaxEstimator.h"
#include "CParameter.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

void CRealObserver::displayWinRate(double W, double D, double L) {
	double Total = W + D + L;
	double Score = W + 0.5 * D;
	double Rate = Score / Total;

	double TotalVariance =
		W * (1.0 - Rate) * (1.0 - Rate) + D * (0.5 - Rate) * (0.5 - Rate) + L * (0.0 - Rate) * (0.0 - Rate);
	double Margin = 1.96 * std::sqrt(TotalVariance) / Total;

	const double EloMul = 400.0 / std::log(10.0);

	double EloLower = EloMul * reg.Rating(Rate - Margin);
	double Elo = EloMul * reg.Rating(Rate);
	double EloUpper = EloMul * reg.Rating(Rate + Margin);

	double EloPlusMinus = std::max(Elo - EloLower, EloUpper - Elo);

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1);
	oss << Elo << "+-" << EloPlusMinus;
	std::cout << std::setw(13) << oss.str();

	std::cout << '\n';
}

void CRealObserver::displayMax(const double *v, std::vector<int> &width) {
	std::cout << std::fixed << std::setprecision(1);

	double Rating, Variance;
	reg.GetPosteriorInfo(&v[0], Rating, Variance);
	const double c = 1.96;

	double Deviation = std::sqrt(Variance);
	double PlusMinus = c * Deviation;
	double Elo = Rating * 400.0 / std::log(10.0);
	double EloPlusMinus = PlusMinus * 400.0 / std::log(10.0);

	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1);
	oss << Elo << "+-" << EloPlusMinus;
	std::cout << std::setw(13) << oss.str();

	std::vector<double> paramMax(paramcol.GetSize(), -1.0), paramMin(paramcol.GetSize(), 1.0);

	for (int i = results.GetSamples(); --i >= 0;)
		if (results.GetOutcome(i) < 3) {
			const double *v2 = results.GetSample(i);
			double Weight = reg.GetWeight(v2);
			if (Weight > 0.8) {
				for (int j = 0; j < paramcol.GetSize(); j++) {
					if (paramMax[j] < v2[j])
						paramMax[j] = v2[j];
					if (paramMin[j] > v2[j])
						paramMin[j] = v2[j];
				}
			}
		}

	std::cout << std::fixed << std::setprecision(2);
	for (int j = 0; j < paramcol.GetSize(); j++) {
		const CParameter &param = paramcol.GetParam(j);
		double value = param.TransformFromQLR(v[j]);
		double plus = param.TransformFromQLR(paramMax[j]) - value;
		double minus = value - param.TransformFromQLR(paramMin[j]);
		double plusminus = std::max(plus, minus);
		oss.str("");
		oss << std::fixed << std::setprecision(2);
		oss << value << "+-" << plusminus;

		std::cout << std::setw(std::max(std::max((int)param.GetName().length() + 2, 17), width[j])) << oss.str();
	}

	std::cout << '\n';
}

/////////////////////////////////////////////////////////////////////////////
// Display stats about current results
/////////////////////////////////////////////////////////////////////////////
void CRealObserver::OnOutcome(int i) {

	std::vector<int> width;
	std::vector<double> v(paramcol.GetSize());
	me.ComputeLocalWeights();
	bool fMax = me.MaxParameter(&v[0]);

	for (int j = 0; j < paramcol.GetSize(); j++) {
		if (fMax) {
			const CParameter &param = paramcol.GetParam(j);
			double upper = param.TransformFromQLR(1.0);
			double lower = param.TransformFromQLR(-1.0);
			int w = 1;
			if (upper > 0.5)
				w = std::lround(std::max(log10(upper), (double)w));
			if (upper < -0.5)
				w = std::lround(std::max(1 + log10(-upper), (double)w));
			if (lower > 0.5)
				w = std::lround(std::max(log10(lower), (double)w));
			if (lower < -0.5)
				w = std::lround(std::max(1 + log10(-lower), (double)w));
			width.push_back(2 * w + 3 + 3 + 2 + 2);
		}
		else {
			width.push_back(0);
		}
	}

	std::cout << '\n';
	std::cout << std::setw(8) << "Samples";
	std::cout << std::setw(9) << "";
	std::cout << std::setw(13) << "Elo";
	for (int j = 0; j < paramcol.GetSize(); j++) {
		const CParameter &param = paramcol.GetParam(j);
		std::cout << std::setw(std::max(std::max((int)param.GetName().length() + 2, 17), width[j])) << param.GetName();
	}
	std::cout << "\n\n";


	results.Refresh();
	if (fMax) {
		std::cout << std::setw(8) << results.GetSamples();
		std::cout << std::setw(9) << "Mean";
		displayMax(&v[0], width);
		if (reg.GetPF().GetMax(&reg.MAP()[0], &v[0])) {
			std::cout << std::setw(8) << "";
			std::cout << std::setw(9) << "Max";
			displayMax(&v[0], width);
		}
		std::cout << std::setw(8) << "";
		std::cout << std::setw(9) << "All";

		displayWinRate(reg.GetCount(COutcome::Win), reg.GetCount(COutcome::Draw), reg.GetCount(COutcome::Loss));

		//
		// Weighted winning rate
		//
		double tCount[3] = {0.0, 0.0, 0.0};
		double tCentralCount[3] = {0.0, 0.0, 0.0};
		for (int i = results.GetSamples(); --i >= 0;)
			if (results.GetOutcome(i) < 3) {
				const double *v2 = results.GetSample(i);
				double Weight = reg.GetWeight(v2);
				if (Weight == 1.0)
					tCentralCount[results.GetOutcome(i)] += Weight;
				tCount[results.GetOutcome(i)] += Weight;
			}
		std::cout << std::setw(8) << "";
		std::cout << std::setw(9) << "Weighted";
		displayWinRate(tCount[COutcome::Win], tCount[COutcome::Draw], tCount[COutcome::Loss]);
		std::cout << std::setw(8) << "";
		std::cout << std::setw(9) << "Central";
		displayWinRate(tCentralCount[COutcome::Win], tCentralCount[COutcome::Draw], tCentralCount[COutcome::Loss]);
	}

	std::cout.flush();

	std::cout << std::defaultfloat;
}
