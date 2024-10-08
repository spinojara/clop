/////////////////////////////////////////////////////////////////////////////
//
// CResults.cpp
//
// Rémi Coulom
//
// October, 2008
//
/////////////////////////////////////////////////////////////////////////////
#include "CResults.h"
#include "CObserver.h"
#include "debug.h"

#include <algorithm>

#define OBSERVER_LOOP                                                                          \
	for (std::list<CObserver *>::iterator iter = lObs.begin(); iter != lObs.end(); iter++) \
	(*iter)

/////////////////////////////////////////////////////////////////////////////
// Add a new sample
/////////////////////////////////////////////////////////////////////////////
int CResults::AddSample(const double *v) {
	Samples++;

	if (vOutcome.size() < Samples) {
		vOutcome.resize(Samples);
		vSample.resize(Samples * Dimensions);
	}

	std::copy(v, v + Dimensions, &vSample[(Samples - 1) * Dimensions]);
	vOutcome[Samples - 1] = COutcome::InProgress;

	OBSERVER_LOOP->OnSample();

	return Samples - 1;
}

/////////////////////////////////////////////////////////////////////////////
// Record the outcome of a sample that was previously added
/////////////////////////////////////////////////////////////////////////////
void CResults::AddOutcome(int i, COutcome outcome) {
	vOutcome[i] = outcome;

	OBSERVER_LOOP->OnOutcome(i);
}

/////////////////////////////////////////////////////////////////////////////
// Reset
/////////////////////////////////////////////////////////////////////////////
void CResults::Reset() {
	Samples = 0;

	OBSERVER_LOOP->OnReset();
}

/////////////////////////////////////////////////////////////////////////////
// Refresh (called at end of run, or before measuring anything)
/////////////////////////////////////////////////////////////////////////////
void CResults::Refresh() { OBSERVER_LOOP->OnRefresh(); }

/////////////////////////////////////////////////////////////////////////////
// Shortcut to add sample and outcome at the same time
/////////////////////////////////////////////////////////////////////////////
void CResults::AddSample(const double *v, COutcome outcome) { AddOutcome(AddSample(v), outcome); }

/////////////////////////////////////////////////////////////////////////////
// Count outcomes
/////////////////////////////////////////////////////////////////////////////
int CResults::CountOutcomes(COutcome outcome, int MinIndex) const {
	int Result = 0;
	for (int i = Samples; --i >= MinIndex;)
		if (vOutcome[i] == outcome)
			Result++;
	return Result;
}

/////////////////////////////////////////////////////////////////////////////
// Reserve memory ahead of time
/////////////////////////////////////////////////////////////////////////////
void CResults::Reserve(unsigned n) {
	vSample.resize(n * Dimensions);
	vOutcome.resize(n);
}

/////////////////////////////////////////////////////////////////////////////
// Destructor: issue fatal error if still has observers
/////////////////////////////////////////////////////////////////////////////
CResults::~CResults() { FATAL(!lObs.empty()); }
