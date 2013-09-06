#ifndef PREDMAP_H
#define	PREDMAP_H

#include <utility>
#include <fstream>
#include <map>

#include "TypeDefs.h"

using namespace std;

class PredMap {
private:
	map<pair<Int, Int>, Int***> predMap;
	Int sizeCU;
	fstream fp;
		
public:
	PredMap(string fileName, Int wFrame, Int hFrame, Int sizeCU);
	
	Int*** getPred(Int idCurrFrame, Int idRefFrame);
};

#endif	/* PREDMAP_H */