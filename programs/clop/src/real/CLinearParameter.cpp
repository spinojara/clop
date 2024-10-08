/////////////////////////////////////////////////////////////////////////////
//
// CLinearParameter.cpp
//
// Rémi Coulom
//
// August, 2009
//
/////////////////////////////////////////////////////////////////////////////
#include "CLinearParameter.h"

/////////////////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////////////////
CLinearParameter::CLinearParameter(const std::string &s, double Min, double Max) : CParameter(s), Min(Min), Max(Max) {}

/////////////////////////////////////////////////////////////////////////////
// Transform x to the [-1, 1] range
/////////////////////////////////////////////////////////////////////////////
double CLinearParameter::TransformToQLR(double x) const {
	if (Max == Min)
		return 0.0;
	else
		return -1.0 + 2.0 * (x - Min) / (Max - Min);
}

/////////////////////////////////////////////////////////////////////////////
// Transform x to the [Min, Max] range
/////////////////////////////////////////////////////////////////////////////
double CLinearParameter::TransformFromQLR(double x) const { return Min + (Max - Min) * (x + 1.0) * 0.5; }
