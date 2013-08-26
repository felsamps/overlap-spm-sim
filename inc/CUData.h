#ifndef CUDATA_H
#define	CUDATA_H

#include <list>
#include <map>

#include "TypeDefs.h"
#include "PUData.h"

using namespace std;

class CUData {
private:
    map<UInt, PUData*> refsPU;
    UInt depth, x, y;
public:
    CUData(UInt x, UInt y, UInt depth);
    
    void insertPU(PUData* pu);
    
    void setY(UInt y);
    UInt getY() const;
    void setX(UInt x);
    UInt getX() const;
    void setDepth(UInt depth);
    UInt getDepth() const;    
    
};

#endif	/* CUDATA_H */

