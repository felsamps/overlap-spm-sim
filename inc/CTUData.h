#ifndef CTUDATA_H
#define	CTUDATA_H

#include <iostream>
#include <vector>
#include "CUData.h"

using namespace std;

class CTUData {
private:
    vector<vector<CUData*> > vectorCU;
    UInt x, y;
public:
    CTUData(UInt x, UInt y);
    
    void insertCU(CUData* cu);
    
    void getCU();
    
    void setY(UInt y);
    UInt getY() const;
    void setX(UInt x);
    UInt getX() const;
    
    void report();
    
};

#endif	/* CTUDATA_H */

