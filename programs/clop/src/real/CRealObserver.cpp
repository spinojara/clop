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

/////////////////////////////////////////////////////////////////////////////
// Display stats about current results
/////////////////////////////////////////////////////////////////////////////
void CRealObserver::OnOutcome(int i) {

	headers = headers % 20;
	if (headers == 0) {
		std::cout << '\n';
		std::cout << std::setw(8) << "Samples";
		std::cout << std::setw(13) << "Elo";
		for (int j = 0; j < paramcol.GetSize(); j++) {
			const CParameter &param = paramcol.GetParam(j);
			std::cout << std::setw(std::max((int)param.GetName().length() + 2, 17)) << param.GetName();
		}
		std::cout << "\n\n";
	}


	results.Refresh();
	std::cout << std::setw(8) << results.GetSamples();

	std::vector<double> v(paramcol.GetSize());
	me.ComputeLocalWeights();
	bool fMax = me.MaxParameter(&v[0]);
	double Rating, Variance;
	reg.GetPosteriorInfo(&v[0], Rating, Variance);

	std::cout << std::fixed << std::setprecision(1);
	if (fMax) {
		const double c = 1.96;

		double Deviation = std::sqrt(Variance);
		double PlusMinus = c * Deviation;
		double Elo = Rating * 400.0 / std::log(10.0);
		double EloPlusMinus = PlusMinus * 400.0 / std::log(10.0);
		
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(1);
		oss << Elo << "+-" << EloPlusMinus;
		std::cout << std::setw(13) << oss.str();

		std::vector<double> paramMax(paramcol.GetSize(), 0.0), paramMin(paramcol.GetSize(), 1.0);

		for (int i = results.GetSamples(); --i >= 0;)
			if (results.GetOutcome(i) < 3) {
				const double *v = results.GetSample(i);
				double Weight = reg.GetWeight(v);
				if (Weight > 0.5) {
					for (int j = 0; j < paramcol.GetSize(); j++) {
						if (paramMax[j] < v[j])
							paramMax[j] = v[j];
						if (paramMin[j] > v[j])
							paramMin[j] = v[j];
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

			std::cout << std::setw(std::max((int)param.GetName().length() + 2, 17)) << oss.str();
		}
	}

	std::cout << '\n';
	std::cout.flush();

	std::cout << std::defaultfloat;
	headers++;
}
