#ifndef EXTERNALMEMORY_H
#define	EXTERNALMEMORY_H

#include <iostream>

#include "TypeDefs.h"

using namespace std;

class ExternalMemory {
private:
	long long int numOfReadBU;
	long long int numOfPageAct;
	
public:
	ExternalMemory();
	
	void read(Int burstLengthInBU);
	void report();

};

#endif	/* EXTERNALMEMORY_H */

