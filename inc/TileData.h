#ifndef TILEDATA_H
#define	TILEDATA_H

#include <list>

#include "CTUData.h"

using namespace std;

class TileData {
private:
    list<CTUData*> listCTU;
public:
    TileData();
    
    void insertCTU(CTUData* ctu);
    CTUData* getNextCTU();
    
    void report();
};

#endif	/* TILEDATA_H */

