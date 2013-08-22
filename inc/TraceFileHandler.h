#ifndef TRACEFILEHANDLER_H
#define	TRACEFILEHANDLER_H

#include <fstream>
#include <vector>
#include <map>

#include "MotionEstimationData.h"
#include "TypeDefs.h"
#include "CTUData.h"
#include "CUData.h"
#include "TileData.h"
#include "PUData.h"

using namespace std;

class TraceFileHandler {
private:
    fstream fp;
    map<UInt, MotionEstimationData*> mapME;
    UInt numOfTiles;

    MotionEstimationData* xGetMotionEstimationDataEntry(UInt idRefFrame);
    
public:
    TraceFileHandler(string name, UInt numOfTiles);
    MotionEstimationData* parseNextFrame();
};

#endif	/* TRACEFILEHANDLER_H */

