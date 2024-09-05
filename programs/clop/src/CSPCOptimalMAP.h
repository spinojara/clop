/////////////////////////////////////////////////////////////////////////////
//
// CSPCOptimalMAP.h
//
// Rémi Coulom
//
// July, 2009
//
/////////////////////////////////////////////////////////////////////////////
#ifndef CSPCOptimalMAP_Declared
#define CSPCOptimalMAP_Declared

#include "CSamplingPolicy.h"

#include <vector>

class CRegression;

class CSPCOptimalMAP : public CSamplingPolicy // coptmap
{
      private: ////////////////////////////////////////////////////////////////////
	CRegression &reg;
	std::vector<double> v;

      public: ////////////////////////////////////////////////////////////////////
	explicit CSPCOptimalMAP(CRegression &reg);

	const double *NextSample(int i);
};

#endif
