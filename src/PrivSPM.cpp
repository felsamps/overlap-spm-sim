#include "../inc/PrivSPM.h"

PrivSPM::PrivSPM(Int searchRange) {
	this->searchWindowInBU = ((searchRange * 2) + CTU_SIZE) / BU_SIZE;
	
	this->powerMap = new PowerState* [this->searchWindowInBU];
	this->contentMap = new pair<Int,Int>*[this->searchWindowInBU];
	for (int x = 0; x < this->searchWindowInBU; x++) {
		this->powerMap[x] = new PowerState [this->searchWindowInBU];
		this->contentMap[x] = new pair<Int,Int>[this->searchWindowInBU];
	}	
	
	cout << "Private SPM Created: " << this->searchWindowInBU << endl;

	this->writeAcum = 0;
	this->readAcum = 0;
	
	
	this->acumS0 = 0;	
	this->acumS1 = 0;	
	this->acumS2 = 0;	
	this->acumS3 = 0;	
	this->acumW03 = 0;	
	this->acumW13 = 0;	
	this->acumW23 = 0;	
	this->stateSet.resize(4);
	this->acumTimeInstant = 0;
}

void PrivSPM::reset() {
	this->contentSet.clear();
	for (int x = 0; x < this->searchWindowInBU; x++) {
		for (int y = 0; y < this->searchWindowInBU; y++) {
			this->contentMap[x][y] = make_pair<Int,Int>(-999,-999);
		}
	}

}

void PrivSPM::initPowerStates(Int xCenter, Int yCenter, vector<OvSPM*> verOvSPM, vector<OvSPM*> horOvSPM) {
	//TODO fix the centering displacement
	this->xCenter = xCenter / BU_SIZE;
	this->yCenter = yCenter / BU_SIZE;
		
	for (int yBU = 0; yBU < this->searchWindowInBU; yBU++) {
		for (int xBU = 0; xBU < this->searchWindowInBU; xBU++) {
			bool overlapFlag = false;
			for (int ver = 0; ver < verOvSPM.size(); ver++) {
				OverlapPredictor* op = verOvSPM[ver]->getOvPred();
				overlapFlag = (op->isOverlap(yBU+yCenter, xBU+xCenter)) ? true : overlapFlag;
			}
			for (int hor = 0; hor < horOvSPM.size(); hor++) {
				OverlapPredictor* op = horOvSPM[hor]->getOvPred();
				overlapFlag = (op->isOverlap(xBU+xCenter, yBU+yCenter)) ? true : overlapFlag;
			}
			
			pair<Int,Int> p(xBU,yBU);
			if(overlapFlag) {
				this->stateSet[this->powerMap[xBU][yBU]].erase(p);
				this->stateSet[S0].insert(p);
				this->powerMap[xBU][yBU] = S0;
			}
			else {
				xUpdatePowerState(make_pair<Int,Int>(xBU,yBU));
			}
			xUpdatePowerCounters();
			
		}
	}
}

void PrivSPM::xUpdatePowerState(pair<Int,Int> acc) {
	Int lPos = acc.first;
	Int tPos = acc.second;
	switch(this->powerMap[lPos][tPos]) {
		case S0:
			this->acumW03 += 1;
			this->stateSet[S0].erase(acc);
			this->stateSet[S3].insert(acc);
			break;

		case S1:
			this->acumW13 += 1;
			this->stateSet[S1].erase(acc);
			this->stateSet[S3].insert(acc);
			break;
		case S2:
			this->acumW23 += 1;
			this->stateSet[S2].erase(acc);
			this->stateSet[S3].insert(acc);
			break;
		case S3:
			break;
	}
	this->powerMap[lPos][tPos] = S3;

}


void PrivSPM::xUpdatePowerCounters() {
	this->acumS0 += this->stateSet[S0].size();
	this->acumS1 += this->stateSet[S1].size();
	this->acumS2 += this->stateSet[S2].size();
	this->acumS3 += this->stateSet[S3].size();

	this->acumTimeInstant += 1;
}


SPMStatus PrivSPM::read(Int xBU, Int yBU) {
	pair<Int,Int> p(xBU, yBU);
	
	if(this->contentSet.find(p) != this->contentSet.end()) { //HIT
		this->readAcum += BU_SIZE * BU_SIZE;
		xUpdatePowerCounters();
		return HIT;
		
	}
	else {
		return MISS;
	}
}

void PrivSPM::write(Int xBU, Int yBU) {
	
	Int xPos = (xBU - this->xCenter) + (this->searchWindowInBU - (CTU_SIZE/BU_SIZE)) / 2;
	Int yPos = (yBU - this->yCenter) + (this->searchWindowInBU - (CTU_SIZE/BU_SIZE)) / 2;
	
	if(xPos >= 0 and xPos < this->searchWindowInBU and yPos >= 0 and yPos < this->searchWindowInBU) {
		this->contentSet.insert(make_pair<Int,Int>(xBU, yBU));
		this->contentSet.erase(this->contentMap[xPos][yPos]);
		this->contentMap[xPos][yPos] = make_pair<Int,Int>(xBU, yBU);
		this->writeAcum += BU_SIZE * BU_SIZE;
	}
	
	
#if DEBUG_EN		
	cout << endl << "CENTER " << this->xCenter << " " << this->yCenter << endl;
	report();
#endif
	
}

void PrivSPM::report() {
	for (int y = 0; y < this->searchWindowInBU; y++) {
		for (int x = 0; x < this->searchWindowInBU; x++) {
			cout << "(" << this->contentMap[x][y].first << "," << this->contentMap[x][y].second << ")";
		}
		cout << endl;
	}
	cout << endl;
	getchar();
}

void PrivSPM::reportPower() {
	cout << "PrivSPM Report!" << endl;
	cout << "S0 " << this->acumS0 << endl;
	cout << "S1 " << this->acumS1 << endl;
	cout << "S2 " << this->acumS2 << endl;
	cout << "S3 " << this->acumS3 << endl;
	cout << "W03 " << this->acumW03 << endl;
	cout << "W23 " << this->acumW23 << endl;
	cout << "W13 " << this->acumW13 << endl;
}
