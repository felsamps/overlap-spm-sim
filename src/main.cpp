#include <iostream>

#include "../inc/TypeDefs.h"
#include "../inc/TraceFileHandler.h"
#include "../inc/SearchEngine.h"

using namespace std;

int main(int argc, char** argv) {
	
	string traceFileName(argv[1]);
	
	TraceFileHandler* tfh = new TraceFileHandler(traceFileName);
	SPMManager* spm = new SPMManager(tfh);
	SearchEngine* se = new SearchEngine(tfh, spm);
	
	se->performSearch();
}

