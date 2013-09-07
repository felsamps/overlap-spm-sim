#ifndef OVSPM_H
#define	OVSPM_H

#include <cmath>
#include <set>
#include "TypeDefs.h"
#include "OverlapPredictor.h"

using namespace std;

class OvSPM {
private:
	Int ovThicknessInBU, ovLengthInBU;
	PowerState **powerMap;
	Int** statMap;
	OverlapPredictor* ovPred;
	long long int readAcum;
	long long int writeAcum;
	Int usage;
	Int numOfCores;
	bool*** coreMap;
	
	vector<set<pair<Int,Int> > > stateSet;
	long long int acumS0, acumS1, acumS2, acumS3;
	long long int acumW03, acumW13, acumW23;
	long long int acumTimeInstant;
	
	void xUpdatePowerState(pair<Int,Int> acc);
	
public:
    OvSPM(Int length, Int thickness, Int center, Int numOfCores);
	
	void reset();
	void initPowerStates();
	void managePowerStatesVer(vector<bool**> priv, Int** ovMap);
	void managePowerStatesHor(vector<bool**> priv, Int** ovMap);
	void updateOverlapUsage();
	void updatePowerCounters();
	
	SPMStatus read(Int lBU, Int tBU, Int reqCore);
	void write(Int lBU, Int tBU);
	
    OverlapPredictor* getOvPred() const;
    Int getUsage() const;
    long long int getWriteAcum() const;
    long long int getReadAcum() const;
    Int getOvLengthInBU() const;
    Int getOvThicknessInBU() const;
	
	void report();
	pair<double,double> reportPower();
};

#endif	/* OVSPM_H */

