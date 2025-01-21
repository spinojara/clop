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
	int headers;

      public: ////////////////////////////////////////////////////////////////////
	CRealObserver(CResults &results, const CParameterCollection &paramcol, const CMaxEstimator &me, CRegression &reg)
	    : CObserver(results), paramcol(paramcol), me(me), reg(reg), headers(0) {}

	void OnOutcome(int i);
};

#endif
