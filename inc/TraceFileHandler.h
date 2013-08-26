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
	
	UInt wFrame;
	UInt hFrame;
	UInt numTileColumns;
	UInt numTileRows;
	UInt searchRange;
	UInt numOfTiles;
	UInt numVerTilesBoundaries;
	UInt numHorTilesBoundaries;

	void xParseHeader();
	MotionEstimationData* xGetMotionEstimationDataEntry(UInt idRefFrame);
    
public:
	TraceFileHandler(string name);
	MotionEstimationData* parseNextFrame();
    
	UInt getNumHorTilesBoundaries() const;
    UInt getNumVerTilesBoundaries() const;
    UInt getNumOfTiles() const;
    UInt getSearchRange() const;
    UInt getNumTileRows() const;
    UInt getNumTileColumns() const;
    UInt getHFrame() const;
    UInt getWFrame() const;
};

#endif	/* TRACEFILEHANDLER_H */

