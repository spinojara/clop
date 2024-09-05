/////////////////////////////////////////////////////////////////////////////
//
// CPFlat.h
//
// Rémi Coulom
//
// August, 2011
//
/////////////////////////////////////////////////////////////////////////////
#ifndef CPFlat_Declared
#define CPFlat_Declared

#include "CArtificialProblem.h"

class CPFlat : public CArtificialProblem // flat
{
      private: ///////////////////////////////////////////////////////////////////
	const int Dimensions;

      public: ////////////////////////////////////////////////////////////////////
	CPFlat(int Dimensions = 1) : Dimensions(Dimensions) {}

	double GetStrength(const double v[]) const;
	int GetDimensions() const { return Dimensions; }
	void GetOptimalParameters(double v[]) const;
};

#endif
