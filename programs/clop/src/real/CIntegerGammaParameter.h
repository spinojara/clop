/////////////////////////////////////////////////////////////////////////////
//
// CIntegerGammaParameter.h
//
// Rémi Coulom
//
// September, 2010
//
/////////////////////////////////////////////////////////////////////////////
#ifndef CIntegerGammaParameter_Declared
#define CIntegerGammaParameter_Declared

#include "CParameter.h"

class CIntegerGammaParameter : public CParameter // igparam
{
      private: ///////////////////////////////////////////////////////////////////
	const int Min;
	const int Max;

      public: ////////////////////////////////////////////////////////////////////
	CIntegerGammaParameter(const std::string &s, int Min, int Max);
	double TransformToQLR(double x) const;
	double TransformFromQLR(double x) const;
};

#endif
