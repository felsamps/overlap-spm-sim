#ifndef TRACEFILEHANDLER_H
#define	TRACEFILEHANDLER_H

#include <fstream>
#include <list>
#include "MotionEstimationData.h"

using namespace std;

class TraceFileHandler {
private:
    fstream fp;

public:
    TraceFileHandler(string name);
    list<MotionEstimationData*> parseNextFrame();
};

#endif	/* TRACEFILEHANDLER_H */

