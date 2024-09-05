/////////////////////////////////////////////////////////////////////////////
//
// CParametricFunction.h
//
// Rémi Coulom
//
// February, 2010
//
/////////////////////////////////////////////////////////////////////////////
#include "CParametricFunction.h"
#include "CDFParametric.h"

#include <vector>

/////////////////////////////////////////////////////////////////////////////
// Constructor
/////////////////////////////////////////////////////////////////////////////
CParametricFunction::CParametricFunction(int Dimensions, int Parameters)
    : PriorStrength(1e-2), Dimensions(Dimensions), Parameters(Parameters) {}

/////////////////////////////////////////////////////////////////////////////
// Prior
/////////////////////////////////////////////////////////////////////////////
void CParametricFunction::GetPriorParam(double *vParam) const {
	for (int i = Parameters; --i >= 0;)
		vParam[i] = 0.0;
}

/////////////////////////////////////////////////////////////////////////////
// Prior
/////////////////////////////////////////////////////////////////////////////
double CParametricFunction::GetPrior(const double *vParam) const {
	double Result = 0;

	for (int i = Parameters; --i >= 0;)
		Result -= PriorStrength * vParam[i] * vParam[i] * 0.5;

	return Result;
}

/////////////////////////////////////////////////////////////////////////////
// Prior gradient
/////////////////////////////////////////////////////////////////////////////
void CParametricFunction::GetPriorGradient(const double *restrict vParam, double *restrict vGradient) const {
	for (int i = Parameters; --i >= 0;)
		vGradient[i] = -PriorStrength * vParam[i];
}

/////////////////////////////////////////////////////////////////////////////
// Prior Hessian (opposite of)
/////////////////////////////////////////////////////////////////////////////
void CParametricFunction::GetPriorHessian(const double *vParam, double *vHessian) const {
	for (int i = Parameters * Parameters; --i >= 0;)
		vHessian[i] = 0.0;
	for (int i = Parameters; --i >= 0;)
		vHessian[i * (Parameters + 1)] = PriorStrength;
}

/////////////////////////////////////////////////////////////////////////////
// Dot product
/////////////////////////////////////////////////////////////////////////////
double CParametricFunction::DotProduct(const double *vx, const double *vy) const {
	double Result = 0.0;
	for (int i = Parameters; --i >= 0;)
		Result += vx[i] * vy[i];
	return Result;
}

/////////////////////////////////////////////////////////////////////////////
// Get Value. By default, scalar product with monomials
/////////////////////////////////////////////////////////////////////////////
double CParametricFunction::GetValue(const double *vParam, const double *vx) const {
	std::vector<double> vMonomials(Parameters);
	GetMonomials(vx, &vMonomials[0]);
	return DotProduct(vParam, &vMonomials[0]);
}

/////////////////////////////////////////////////////////////////////////////
// Maximum
/////////////////////////////////////////////////////////////////////////////
bool CParametricFunction::GetMax(const double *vParam, double *vx) const {
	CDFParametric dfp(*this, vParam);
	dfp.CG(vx);
	return true;
}
