#ifndef PRIVSPM_H
#define	PRIVSPM_H

#include <vector>
#include <set>

#include "TypeDefs.h"
#include "OverlapPredictor.h"
#include "OvSPM.h"


using namespace std;

class PrivSPM {
private:
	PowerState** powerMap;
	Int** statMap;
	pair<Int,Int>**contentMap;
	
	set<pair<Int,Int> > contentSet;
	Int xCenter, yCenter;
	Int searchWindowInBU;
	
	Int wFrameInBU, hFrameInBU;
	bool** searchLimits;
	
	long long int writeAcum, readAcum;
	
	vector<set<pair<Int,Int> > > stateSet;
	long long int acumS0, acumS1, acumS2, acumS3;
	long long int acumW03, acumW13, acumW23;
	long long int acumTimeInstant;
	
	void xUpdatePowerState(pair<Int,Int> acc);
	void xUpdatePowerCounters();
	void xUpdateSearchLimits();
public:
    PrivSPM(Int wFrame, Int hFrame, Int searchRange);
	
	void reset();
	void initPowerStates(Int xCenter, Int yCenter, vector<OvSPM*> verOvSPM, vector<OvSPM*> horOvSPM);
	
	SPMStatus read(Int lBU, Int tBU);
	void write(Int lBU, Int tBU);
	
	bool checkLimitsHorPerspective(Int lBU, Int tBU);
	bool checkLimitsVerPerspective(Int lBU, Int tBU);

	void report();
	pair<double,double> reportPower();
	void reportPowerStates();
	
	bool** getSearchLimits() { return this->searchLimits; }
};

#endif	/* PRIVSPM_H */

