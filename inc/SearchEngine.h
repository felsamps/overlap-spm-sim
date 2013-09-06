#ifndef SEARCHENGINE_H
#define	SEARCHENGINE_H

#include <cstdio>

#include "TraceFileHandler.h"
#include "SPMManager.h"
#include "TypeDefs.h"

class SearchEngine {
private:
	TraceFileHandler* tfh;
	SPMManager* spm;
    
	void xHandlePUAccess(PUData* pu, Int sizeCU, Int idTile);
	Int xGetMaxNumOfCTU(MotionEstimationData* meData);
	
public:
	SearchEngine(TraceFileHandler* tfh, SPMManager* spm);
	
	void performSearch();
};

#endif	/* SEARCHENGINE_H */

