#ifndef SPMMANAGER_H
#define	SPMMANAGER_H

#include <vector>

#include "OvSPM.h"
#include "PrivSPM.h"
#include "OverlapPredictor.h"
#include "ExternalMemory.h"
#include "TraceFileHandler.h"

using namespace std;

class SPMManager {
private:
	ExternalMemory* offChip;
	ExternalMemory* offChipWithoutSPM;
    vector<OvSPM*> verShared;
    vector<OvSPM*> horShared;
    vector<PrivSPM*> corePrivate;
	
	bool** ovMap;
	
	TraceFileHandler* tfh;
	
	void xParsePredFile(string fileName);
	pair<char,OvSPM*> xIsInsideAnyOverlap(Int xBU, Int yBU);
	
	long long int privHitCounter, privMissCounter;
	long long int ovHitCounter, ovMissCounter;
	long long int overallBUAcc;

public:
    SPMManager(TraceFileHandler* tfh);
	
	void init(Int D);
	
	void manageSPM_CTULevel(Int idTile, Int xCenter, Int yCenter);
	void manageSPMFrameLevel();
	void handleDataRequest(Int xReq, Int yReq, Int size, Int reqCore);
	
	void updatePowerCounters();
	void updateOverlapUsages();
	void report();
};

#endif	/* SPMMANAGER_H */

