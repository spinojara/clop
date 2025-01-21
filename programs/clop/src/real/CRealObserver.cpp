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
		std::cout << std::setw(10) << "95% LCB";
		std::cout << std::setw(10) << "Elo";
		std::cout << std::setw(10) << "95% UCB";
		for (int j = 0; j < paramcol.GetSize(); j++) {
			const CParameter &param = paramcol.GetParam(j);
			std::cout << std::setw(std::max((int)param.GetName().length() + 2, 10)) << param.GetName();
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

	if (fMax) {
		const double c = 1.96;

		double Deviation = std::sqrt(Variance);
		double LowerRating = Rating - c * Deviation;
		double UpperRating = Rating + c * Deviation;
		double Elo = Rating * 400.0 / std::log(10.0);
		double LowerElo = LowerRating * 400.0 / std::log(10.0);
		double UpperElo = UpperRating * 400.0 / std::log(10.0);
		
		std::cout << std::setw(10) << LowerElo;
		std::cout << std::setw(10) << Elo;
		std::cout << std::setw(10) << UpperElo;

		for (int j = 0; j < paramcol.GetSize(); j++) {
			const CParameter &param = paramcol.GetParam(j);
			std::cout << std::setw(std::max((int)param.GetName().length() + 2, 10)) << param.TransformFromQLR(v[j]);
		}
	}
	else {
		int W = results.CountOutcomes(COutcome::Win);
		int D = results.CountOutcomes(COutcome::Draw);
		int L = results.CountOutcomes(COutcome::Loss);
		double Total = W + D + L;
		double Score = W + 0.5 * D;
		double Rate = Score / Total;

		double TotalVariance =
			W * (1.0 - Rate) * (1.0 - Rate) + D * (0.5 - Rate) * (0.5 - Rate) + L * (0.0 - Rate) * (0.0 - Rate);
		double Margin = 1.96 * std::sqrt(TotalVariance) / Total;

		const double EloMul = 400.0 / std::log(10.0);

		std::cout << std::setw(10) << reg.Rating(Rate - Margin) * EloMul;
		std::cout << std::setw(10) << reg.Rating(Rate) * EloMul;
		std::cout << std::setw(10) << reg.Rating(Rate + Margin) * EloMul;
	}

	std::cout << '\n';
	std::cout.flush();
	headers++;
}
