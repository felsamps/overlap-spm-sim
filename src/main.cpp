#include <iostream>

#include "../inc/TypeDefs.h"
#include "../inc/TraceFileHandler.h"

using namespace std;

int main(int argc, char** argv) {
	
	string traceFileName(argv[1]);
	
	TraceFileHandler* tfh = new TraceFileHandler(traceFileName);
	MotionEstimationData* med = tfh->parseNextFrame();
	
}

