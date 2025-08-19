/////////////////////////////////////////////////////////////////////////////
//
// CRealObserver.h
//
// Rémi Coulom
//
// November, 2009
//
/////////////////////////////////////////////////////////////////////////////
#ifndef CRealObserver_Declared
#define CRealObserver_Declared

#include "CObserver.h"
#include "CRegression.h"
class CParameterCollection;
class CMaxEstimator;

class CRealObserver : public CObserver // robs
{
      private: ///////////////////////////////////////////////////////////////////
	const CParameterCollection &paramcol;
	const CMaxEstimator &me;
	CRegression &reg;
	void displayWinRate(double W, double D, double L);
	void displayMax(const double *v, std::vector<int> &width);

      public: ////////////////////////////////////////////////////////////////////
	CRealObserver(CResults &results, const CParameterCollection &paramcol, const CMaxEstimator &me, CRegression &reg)
	    : CObserver(results), paramcol(paramcol), me(me), reg(reg) {}

	void OnOutcome(int i);
};

#endif
