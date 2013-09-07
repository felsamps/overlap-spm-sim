#include "../inc/SPMManager.h"

SPMManager::SPMManager(TraceFileHandler* tfh) {
	this->tfh = tfh;
	
	Int ovThickness = this->tfh->getSearchRange() * OV_THICKNESS_FACTOR;
	
	//TODO initialize External Memory
	this->offChip = new ExternalMemory();
	this->offChipWithoutSPM = new ExternalMemory();
	
	//TODO initialize PrivSPMs
	for (int p = 0; p < this->tfh->getNumOfTiles(); p++) {
		PrivSPM* priv = new PrivSPM(this->tfh->getSearchRange());
		this->corePrivate.push_back(priv);
	}
	//TODO initialize OvSPMs
	Int verOvStep = this->tfh->getWFrame() / (this->tfh->getNumVerTilesBoundaries()+1);
	Int horOvStep = this->tfh->getHFrame() / (this->tfh->getNumHorTilesBoundaries()+1);
	
	cout << "STEPS: " << verOvStep << " " << horOvStep << endl;
	
	for (int ver = 0; ver < this->tfh->getNumVerTilesBoundaries(); ver++) {
		OvSPM* ov = new OvSPM(this->tfh->getHFrame(), ovThickness, verOvStep*(ver+1), this->tfh->getNumOfTiles());
		this->verShared.push_back(ov);
	}
	for (int hor = 0; hor < this->tfh->getNumHorTilesBoundaries(); hor++) {
		OvSPM* ov = new OvSPM(this->tfh->getWFrame(), ovThickness, horOvStep*(hor+1), this->tfh->getNumOfTiles());
		this->horShared.push_back(ov);
	}
	
	this->privHitCounter = 0;
	this->ovHitCounter = 0;
	this->privMissCounter = 0;
	this->ovMissCounter = 0;
	this->overallBUAcc = 0;
	
}

void SPMManager::init(Int D) {
	//TODO run overlap predictors
	//TODO clear cache from previous ME
	for (int p = 0; p < this->tfh->getNumOfTiles(); p++) {
		this->corePrivate[p]->reset();
	}
	for (int ver = 0; ver < this->tfh->getNumVerTilesBoundaries(); ver++) {
		this->verShared[ver]->reset();
		this->verShared[ver]->getOvPred()->predict(D, this->verShared[ver]->getOvThicknessInBU());
	}
	for (int hor = 0; hor < this->tfh->getNumHorTilesBoundaries(); hor++) {
		this->horShared[hor]->reset();
		this->horShared[hor]->getOvPred()->predict(D, this->horShared[hor]->getOvThicknessInBU());
	}
	//
}


void SPMManager::manageSPM_CTULevel(Int idTile, Int xCenter, Int yCenter) {
	PrivSPM* priv = this->corePrivate[idTile];
	priv->initPowerStates(xCenter, yCenter, this->verShared, this->horShared);	
}

void SPMManager::manageSPMFrameLevel() {	
	for (int tiles = 0; tiles < this->tfh->getNumOfTiles(); tiles++) {
		this->corePrivate[tiles]->reset();
	}
	for (int ver = 0; ver < this->tfh->getNumVerTilesBoundaries(); ver++) {
		this->verShared[ver]->initPowerStates();
		this->verShared[ver]->reset();
	}
	for (int hor = 0; hor < this->tfh->getNumHorTilesBoundaries(); hor++) {
		this->horShared[hor]->initPowerStates();
		this->horShared[hor]->reset();
	}
	
}

pair<char,OvSPM*> SPMManager::xIsInsideAnyOverlap(Int xBU, Int yBU) {
	for (int ver = 0; ver < this->verShared.size(); ver++) {
		if(verShared[ver]->getOvPred()->isOverlap(yBU, xBU)) {
			return make_pair<char, OvSPM*>('V',verShared[ver]);
		}
	}
	for (int hor = 0; hor < this->horShared.size(); hor++) {
		if(horShared[hor]->getOvPred()->isOverlap(xBU, yBU)) {
			return make_pair<char, OvSPM*>('H',horShared[hor]);
		}
	}
	return make_pair<char, OvSPM*>('N',NULL);;
}

void SPMManager::xUpdateOverlapCounters() {
	for (int ver = 0; ver < this->tfh->getNumVerTilesBoundaries(); ver++) {
		this->verShared[ver]->updatePowerCounters();
	}
	for (int hor = 0; hor < this->tfh->getNumHorTilesBoundaries(); hor++) {
		this->horShared[hor]->updatePowerCounters();
	}
}

void SPMManager::handleDataRequest(Int xReq, Int yReq, Int size, Int reqCore) {
	/*Serialize the request into BU accesses*/
	for (int y = yReq/BU_SIZE; y < (yReq+size)/BU_SIZE; y+=1) {
		for (int x = xReq/BU_SIZE; x < (xReq+size)/BU_SIZE; x+=1) {
			/*Reading policy*/
			/*Burst reading for external memory*/
			pair<char,OvSPM*> targetOverlap = xIsInsideAnyOverlap(x, y);
			OvSPM* ovSPM = targetOverlap.second;
			PrivSPM* targetPriv = this->corePrivate[reqCore];
			this->overallBUAcc += 1;
			this->offChipWithoutSPM->read(1);
			if(targetOverlap.second != NULL) { //Overlap detected!
				//cout << "OVERLAP! " << x << " " << y << endl;
				//getchar();
				
				if(targetOverlap.first == 'V') {
					if(ovSPM->read(y, x, reqCore) == MISS) {
						this->ovMissCounter += 1;
						this->offChip->read(ovSPM->getOvPred()->getActualOvThicknessInBU());
						ovSPM->write(y,x);
					}
					else {
						this->ovHitCounter += 1;
					}
				}
				else { //if 'H'
					if(ovSPM->read(x, y, reqCore) == MISS) {
						this->ovMissCounter += 1;
						this->offChip->read(ovSPM->getOvPred()->getActualOvThicknessInBU());
						ovSPM->write(x,y);
					}
					else {
						this->ovHitCounter += 1;
					}
				}
			}
			else {  //No overlap
				if(targetPriv->read(x, y) == MISS) {
					this->privMissCounter += 1;
					this->offChip->read(1);
					targetPriv->write(x,y);
				}
				else {
					this->privHitCounter += 1;
				}
				xUpdateOverlapCounters();
			}	
			
		}
	}
		
	/*update power counters*/
}

void SPMManager::updateOverlapUsages() {
	for (int ver = 0; ver < this->tfh->getNumVerTilesBoundaries(); ver++) {
		this->verShared[ver]->updateOverlapUsage();
	}
	for (int hor = 0; hor < this->tfh->getNumHorTilesBoundaries(); hor++) {
		this->horShared[hor]->updateOverlapUsage();
	}
}

void SPMManager::report() {
	double overlapPctg = (this->ovHitCounter+this->ovMissCounter) / (double)this->overallBUAcc; 
	cout << "SPM Organization Report" << endl;
	cout << "PRIVATE: HIT: " << this->privHitCounter << " " << this->privHitCounter/(double)(this->privHitCounter+this->privMissCounter) << " ";
	cout << "MISS: " << this->privMissCounter << " " << this->privMissCounter/(double)(this->privHitCounter+this->privMissCounter) << " ";
	cout << "(" << 1-overlapPctg << ")" << endl;
	cout << "OVERLAP: HIT: " << this->ovHitCounter << " " << this->ovHitCounter/(double)(this->ovHitCounter+this->ovMissCounter) << " ";
	cout << "MISS: " << this->ovMissCounter << " "  << " " << this->ovMissCounter/(double)(this->ovHitCounter+this->ovMissCounter) << " ";
	cout << "(" << overlapPctg << ")" << endl;
	this->offChip->report();
	this->offChipWithoutSPM->report();
	
	pair<double,double> p;	
	double energyWOPG = 0, energyWithPG = 0;
	for (int c = 0; c < this->tfh->getNumOfTiles(); c++) {
		cout << "PRIV POWER " << c << endl;
		p = this->corePrivate[c]->reportPower();
		energyWOPG += p.first;
		energyWithPG += p.second;
	}
	for (int ver = 0; ver < this->tfh->getNumVerTilesBoundaries(); ver++) {
		cout << "VER OV POWER " << ver << endl;
		p = this->verShared[ver]->reportPower();
		energyWOPG += p.first;
		energyWithPG += p.second;
	}
	for (int hor = 0; hor < this->tfh->getNumHorTilesBoundaries(); hor++) {
		cout << "HOR OV POWER " << hor << endl;
		p = this->horShared[hor]->reportPower();
		energyWOPG += p.first;
		energyWithPG += p.second;
	}
	
	cout << "TOTAL ENERGY " << energyWOPG << " " << energyWithPG << endl;
	cout << "TOTAL SAVINGS " << (energyWOPG-energyWithPG)/energyWOPG << endl;
}

void SPMManager::reportPrivPowerStates() {
	this->corePrivate[0]->reportPowerStates();
	
}