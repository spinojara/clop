/////////////////////////////////////////////////////////////////////////////
//
// CPNonQuadratic2D.cpp
//
// Rémi Coulom
//
// February, 2009
//
/////////////////////////////////////////////////////////////////////////////
#include "CPNonQuadratic2D.h"

#include <cmath>

/////////////////////////////////////////////////////////////////////////////
// Quadratic strength function
/////////////////////////////////////////////////////////////////////////////
double CPNonQuadratic2D::GetStrength(const double v[]) const {
	double x = v[0];
	double y = v[1];

	double a = 0.5 * (x + y);
	double b = 0.5 * (x - y);

	double z = 1.0 + a * 4.0;
	double g;
	if (z < -1)
		g = 6.0 - 6.0 * z * z;
	else
		g = std::log(z + 1.01) - std::log(0.01) - 0.7 * z;

	return g - 3.0 - 6.0 * b * b;
}

/////////////////////////////////////////////////////////////////////////////
// Optimal Parameters
/////////////////////////////////////////////////////////////////////////////
void CPNonQuadratic2D::GetOptimalParameters(double v[]) const {
	double a = (1.0 / 0.7 - 1.01 - 1.0) / 4.0;
	double b = 0.0;

	v[0] = a + b;
	v[1] = a - b;
}
