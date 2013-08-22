#include <iostream>

#include "../inc/TypeDefs.h"
#include "../inc/TraceFileHandler.h"

using namespace std;

int main(int argc, char** argv) {
	
	UInt wFrame = atoi(argv[1]);
	UInt hFrame = atoi(argv[2]);
	UInt numOfTiles = atoi(argv[3]);
	UInt searchRange = atoi(argv[4]);
	string traceFileName(argv[5]);
	
	TraceFileHandler* tfh = new TraceFileHandler(traceFileName, numOfTiles);
	MotionEstimationData* med = tfh->parseNextFrame();
	
}

