#ifndef MOTIONESTIMATIONDATA_H
#define	MOTIONESTIMATIONDATA_H

#include <vector>
#include <cstdlib>
#include <set>

#include "TypeDefs.h"
#include "TileData.h"


using namespace std;

class MotionEstimationData {
private:
    vector<TileData*> tiles;
    set<UInt> refFrames;
    
    UInt numOfTiles, idCurrFrame;
	    
public:
    MotionEstimationData(UInt idCurrFrame, UInt numOfTiles);
    
    TileData* getTile(UInt idTile);
    void insertRefFrame(UInt idRefFrame);
    
    void report();
    
};

#endif	/* MOTIONESTIMATIONDATA_H */

