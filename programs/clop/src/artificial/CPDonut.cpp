/////////////////////////////////////////////////////////////////////////////
//
// CPDonut.cpp
//
// Rémi Coulom
//
// February, 2009
//
/////////////////////////////////////////////////////////////////////////////
#include "CPDonut.h"

#include <cmath>
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Quadratic strength function
/////////////////////////////////////////////////////////////////////////////
double CPDonut::GetStrength(const double v[]) const {
	double r2 = 0.0;

	for (int i = Dimensions; --i >= 0;)
		r2 += v[i] * v[i];

	r2 *= 9.0;

	return 10 * r2 * std::exp(-r2) - 2;
}

/////////////////////////////////////////////////////////////////////////////
// Optimal Parameters
/////////////////////////////////////////////////////////////////////////////
void CPDonut::GetOptimalParameters(double v[]) const {
	v[0] = 1.0 / 3.0;
	for (int i = Dimensions; --i > 0;)
		v[i] = 0.0;
}
