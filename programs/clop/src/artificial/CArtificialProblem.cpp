/////////////////////////////////////////////////////////////////////////////
//
// CArtificialProblem.cpp
//
// Rémi Coulom
//
// October, 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "CArtificialProblem.h"
#include "CLogistic.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Draw a game outcome at random
/////////////////////////////////////////////////////////////////////////////
COutcome CArtificialProblem::GetOutcome(const double v[]) {
	if (rnd.NextDouble() < GetProba(v))
		return COutcome::Win;
	else
		return COutcome::Loss;
}

/////////////////////////////////////////////////////////////////////////////
// Compute probability
/////////////////////////////////////////////////////////////////////////////
double CArtificialProblem::GetProba(const double v[]) const { return CLogistic::f(GetStrength(v)); }

/////////////////////////////////////////////////////////////////////////////
// Probability of optimal parameters
/////////////////////////////////////////////////////////////////////////////
double CArtificialProblem::GetOptimalProba() const {
	std::vector<double> v(GetDimensions());
	GetOptimalParameters(&v[0]);
	return GetProba(&v[0]);
}
