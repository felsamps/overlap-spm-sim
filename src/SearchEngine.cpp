#include "../inc/SearchEngine.h"

#define NUM_TZ_FIRST_SEARCH_16 44
#define NUM_TZ_FIRST_SEARCH_32 60
#define NUM_TZ_FIRST_SEARCH_64 76
#define NUM_TZ_FIRST_SEARCH_96 76
#define NUM_TZ_FIRST_SEARCH_128 76

Int TZ_SEARCH_CANDIDATES[76][2] = {{0,-1}, {-1,0}, {1,0}, {0,1}, {0,-2}, {-1,-1}, {1,-1}, {-2,0}, {2,0}, {-1,1}, {1,1}, {0,2}, {0,-4}, {-2,-2}, {2,-2}, {-4,0}, {4,0}, {-2,2}, {2,2}, {0,4}, {0,-8}, {-4,-4}, {4,-4}, {-8,0}, {8,0}, {-4,4}, {4,4}, {0,8}, {0,-16}, {-16,0}, {16,0}, {0,16}, {-4,-12}, {4,-12}, {-4,12}, {4,12}, {-8,-8}, {8,-8}, {-8,8}, {8,8}, {-12,-4}, {12,-4}, {-12,4}, {12,4}, {0,-32}, {-32,0}, {32,0}, {0,32}, {-8,-24}, {8,-24}, {-8,24}, {8,24}, {-16,-16}, {16,-16}, {-16,16}, {16,16}, {-24,-8}, {24,-8}, {-24,8}, {24,8}, {0,-64}, {-64,0}, {64,0}, {0,64}, {-16,-48}, {16,-48}, {-16,48}, {16,48}, {-32,-32}, {32,-32}, {-32,32}, {32,32}, {-48,-16}, {48,-16}, {-48,16}, {48,16}};

SearchEngine::SearchEngine(TraceFileHandler* tfh, SPMManager* spm) {
	this->tfh = tfh;
	this->spm = spm;
}

Int SearchEngine::xGetMaxNumOfCTU(MotionEstimationData* meData) {
	Int returnable = 0;
	for (int idTile = 0; idTile < this->tfh->getNumOfTiles(); idTile++) {
		TileData* tileData = meData->getTile(idTile);
		if(tileData->getNumOfCTU() > returnable) {
			returnable = tileData->getNumOfCTU();
		}
	}
	return returnable;
}

void SearchEngine::performSearch() {
	
	MotionEstimationData* meData = this->tfh->parseNextFrame();
	while(meData != NULL) {
		set<Int> refsME = meData->getRefFrames();
		for(set<Int>::iterator it = refsME.begin(); it != refsME.end(); it++) {
			Int idRefFrame = (*it);
			Int D = abs(meData->getIdCurrFrame() - idRefFrame);
	
			/*for debugging*/
			cout << "\nCURR FRAME " << meData->getIdCurrFrame() << " REF FRAME " << idRefFrame  << " (" << D << ")" << endl;

			this->spm->init(D);
			this->spm->manageSPMFrameLevel();
			
			Int numOfCTU = this->xGetMaxNumOfCTU(meData);
			for (int idCTU = 0; idCTU < numOfCTU; idCTU++) {
				for (int idTile = 0; idTile < this->tfh->getNumOfTiles(); idTile++) {
					TileData* tileData = meData->getTile(idTile);
					CTUData* ctuData = tileData->getCTU(idCTU);
					
					if(ctuData != NULL) {
						for (int idCU = 0; idCU < ctuData->getNumOfCU(); idCU++) {
							CUData* cuData = ctuData->getCU(idCU);
							PUData* puData = cuData->getPU(idRefFrame);	/*only 2Nx2N is supported*/
							if(puData != NULL) {
								this->xHandlePUAccess(puData, cuData->getSize(), idTile);
							}
						}
					}
				}
			}
			this->spm->updateOverlapUsages();
		}
		meData = this->tfh->parseNextFrame(	);
	}
	this->spm->report();
	
}

void SearchEngine::xHandlePUAccess(PUData* pu, Int sizeCU, Int idTile) {
	Entry* e = pu->getEntry();
	Int numOfCandidates;
	
	this->spm->manageSPM_CTULevel(idTile, pu->getXCenter(), pu->getYCenter());
			
	while(e != NULL) {
		switch(e->opcode) {
			case 'F':
				numOfCandidates = (this->tfh->getSearchRange() == 16) ? NUM_TZ_FIRST_SEARCH_16 :
						(this->tfh->getSearchRange() == 32) ? NUM_TZ_FIRST_SEARCH_32 :
						(this->tfh->getSearchRange() == 64) ? NUM_TZ_FIRST_SEARCH_64 :
						(this->tfh->getSearchRange() == 96) ? NUM_TZ_FIRST_SEARCH_96 :
						NUM_TZ_FIRST_SEARCH_128;
				
				for (int c = 0; c < numOfCandidates; c++) {
					Int xFS = e->xFS + TZ_SEARCH_CANDIDATES[c][0];
					Int yFS = e->yFS + TZ_SEARCH_CANDIDATES[c][1];
					this->spm->handleDataRequest(xFS, yFS, sizeCU, idTile);
				}

				break;
			case 'C':
				this->spm->handleDataRequest(e->xCand, e->yCand, sizeCU, idTile);
				break;
			case 'R':
				/*for (int y = e->yTop; y < e->yBottom; y++) {
					for (int x = e->xLeft; x < e->xRight; x++) {
						this->spm->handleDataRequest(x, y, sizeCU, idTile);
					}
				}*/

				break;
		}
		e = pu->getEntry();
	}
}