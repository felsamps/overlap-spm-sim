#include "../inc/TraceFileHandler.h"

TraceFileHandler::TraceFileHandler(string name, UInt numOfTiles) {
	this->fp.open(name.c_str(), fstream::in);
	this->numOfTiles = numOfTiles;
}

MotionEstimationData* TraceFileHandler::parseNextFrame() {
	MotionEstimationData* meData;
	TileData* tileData;
	CTUData* ctuData;
	CUData* cuData;
	PUData* puData;
	Entry* entry;
	
	UInt idCurrFrame, idTile;
	UInt xCTU, yCTU;
	UInt xCU, yCU, idDepth;
	UInt idPart, sizePart, idRefFrame;
	
	Int xFS, yFS;
	Int xCand, yCand;
	Int xLeft, xRight, yTop, yBottom;
	
	char command;
	
	bool cuFlag = false, puFlag = false;
	
	while(1) {
		this->fp >> command;
		
		switch(command) {
			case 'I':	/*Begining of the frame (image)*/
				this->fp >> idCurrFrame;
				meData = new MotionEstimationData(idCurrFrame, this->numOfTiles);
				break;
			case 'i':	/*Ending of the frame (image)*/
				
				meData->report();
				getchar();
				return meData;
				break;
			case 'L':	/*Begining of CTU*/
				this->fp >> xCTU >> yCTU >> idTile;
				tileData = meData->getTile(idTile);				
				ctuData = new CTUData(xCTU, yCTU);
				break;
			case 'E':	/*Ending of CTU*/
				tileData->insertCTU(ctuData);
				break;
				
			case 'U':	/*Begining of CU*/
				if( cuFlag ) {
					ctuData->insertCU(cuData);
					cuFlag = true;
				}
				this->fp >> xCU >> yCU >> idDepth;
				cuData = new CUData(xCU, yCU, idDepth);
				
				break;
			case 'P':	/*Begining of PU*/
				if( puFlag and sizePart == 0) {
					cuData->insertPU(puData);
					puFlag = true;
				}
				this->fp >> idPart >> sizePart >> idRefFrame;
				puData = new PUData(idPart, sizePart, idRefFrame);
				meData->insertRefFrame(idRefFrame);
								
				break;
			case 'F':	/*TZ First Search*/
				fp >> xFS >> yFS;
				if( sizePart == 0 ) {
					entry = new Entry();
					entry->opcode = command;
					entry->xFS = xCU + xFS;
					entry->yFS = yCU + yFS;
					puData->insertEntry(entry);
				}
				break;
			case 'C':	/*TZ Candidate Search*/
				fp >> xCand >> yCand;
				if( sizePart == 0 ) {
					entry = new Entry();
					entry->opcode = command;
					entry->xCand = xCU + xCand;
					entry->yCand = yCU + yCand;
					puData->insertEntry(entry);
				}
				break;
			case 'R':	/*TZ Raster Search*/
				fp >> xLeft >> xRight >> yTop >> yBottom;
				if( sizePart == 0 ) {
					entry = new Entry();
					entry->opcode = command;
					entry->xLeft = xCU + xLeft;
					entry->xRight = xCU + xRight;
					entry->yTop = yCU + yTop;
					entry->yBottom = yCU + yBottom;
					puData->insertEntry(entry);
				}
				break;
		}
		
	}
	
}