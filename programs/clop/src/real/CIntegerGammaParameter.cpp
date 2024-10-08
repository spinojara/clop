/////////////////////////////////////////////////////////////////////////////
//
// CIntegerGammaParameter.cpp
//
// Rémi Coulom
//
// September, 2010
//
/////////////////////////////////////////////////////////////////////////////
#include "CIntegerGammaParameter.h"
#include <cmath>

/////////////////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////////////////
CIntegerGammaParameter::CIntegerGammaParameter(const std::string &s, int Min, int Max) : CParameter(s), Min(Min), Max(Max) {}

/////////////////////////////////////////////////////////////////////////////
// Transform x to the [-1, 1] range
/////////////////////////////////////////////////////////////////////////////
double CIntegerGammaParameter::TransformToQLR(double x) const {
	double lx = std::log(x);
	double lMin = std::log(double(Min));
	double lMax = std::log(double(Max));

	return -1.0 + 2.0 * (lx - lMin) / (lMax - lMin);
}

/////////////////////////////////////////////////////////////////////////////
// Transform x to the [Min, Max] range
/////////////////////////////////////////////////////////////////////////////
double CIntegerGammaParameter::TransformFromQLR(double x) const {
	double lMin = std::log(double(Min));
	double lMax = std::log(double(Max));
	double lx = lMin + 0.5 * (x + 1) * (lMax - lMin);
	int i = int(std::exp(lx) + 0.5);

	return i;
}
