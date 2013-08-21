#include "../inc/TraceFileHandler.h"


TraceFileHandler::TraceFileHandler(string name) {
	this->fp.open(name.c_str(), fstream::in);
}

list<MotionEstimationData*> TraceFileHandler::parseNextFrame() {
	MotionEstimationData* meData;
	TileData* tileData;
	CTUData* ctuData;
	CUData* cuData;
	
	char command;
	
	while(1) {
		this->fp >> command;
		
		switch(command) {
			case 'I':	/*Begining of the frame (image)*/
				break;
			case 'i':	/*Ending of the frame (image)*/
				break;
			case 'T':	/*Begining of the tile*/
				break;
			case 't':	/*Ending of the tile*/
				break;
			case 'L':	/*Begining of CTU*/
				break;
			case 'U':	/*Begining of CU*/
				break;
			case 'P':	/*Begining of PU*/
				break;
			case 'F':	/*TZ First Search*/
				break;
			case 'C':	/*TZ Candidate Search*/
				break;
			case 'R':	/*TZ Raster Search*/
				break;
		}
	}
	//TODO implement it
	
	/*check frame begin*/
	/*check Tile begin*/
	
	/*check Tile end*/
	/*check frame end*/
}