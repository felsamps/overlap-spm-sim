#include "../inc/PrivSPM.h"

PrivSPM::PrivSPM(Int wFrame, Int hFrame, Int searchRange) {
	this->searchWindowInBU = ((searchRange * 2) + CTU_SIZE) / BU_SIZE;
	
	this->powerMap = new PowerState* [this->searchWindowInBU];
	this->contentMap = new pair<Int,Int>*[this->searchWindowInBU];
	for (int x = 0; x < this->searchWindowInBU; x++) {
		this->powerMap[x] = new PowerState [this->searchWindowInBU];
		this->contentMap[x] = new pair<Int,Int>[this->searchWindowInBU];
	}	
	
	this->wFrameInBU = wFrame / BU_SIZE;
	this->hFrameInBU = hFrame / BU_SIZE;
	this->searchLimits = new bool*[this->wFrameInBU];
	for (int i = 0; i < this->wFrameInBU; i++) {
		this->searchLimits[i] = new bool[this->hFrameInBU];
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

void PrivSPM::xUpdateSearchLimits() {
	Int xLeft = this->xCenter - this->searchWindowInBU/2;
	Int xRight = this->xCenter - this->searchWindowInBU/2;
	Int yTop = this->yCenter + this->searchWindowInBU/2;
	Int yBottom = this->yCenter + this->searchWindowInBU/2;
	
	for (int x = 0; x < this->wFrameInBU; x++) {
		for (int y = 0; y < this->hFrameInBU; y++) {
			this->searchLimits[x][y] = 
					(x >= xLeft and x < xRight and y >= yTop and y < yBottom) ? 
						true : false;
		}
	}
}

bool PrivSPM::checkLimitsHorPerspective(Int lBU, Int tBU) {
	Int xLeft = this->xCenter - this->searchWindowInBU/2;
	Int xRight = this->xCenter - this->searchWindowInBU/2;
	Int yTop = this->yCenter + this->searchWindowInBU/2;
	Int yBottom = this->yCenter + this->searchWindowInBU/2;
	
	return (tBU >= xLeft and tBU < xRight and lBU >= yTop and lBU < yBottom);
}

bool PrivSPM::checkLimitsVerPerspective(Int lBU, Int tBU) {
	Int xLeft = this->xCenter - this->searchWindowInBU/2;
	Int xRight = this->xCenter - this->searchWindowInBU/2;
	Int yTop = this->yCenter + this->searchWindowInBU/2;
	Int yBottom = this->yCenter + this->searchWindowInBU/2;
	
	return (lBU >= xLeft and lBU < xRight and tBU >= yTop and tBU < yBottom);
}

void PrivSPM::initPowerStates(Int xCenter, Int yCenter, vector<OvSPM*> verOvSPM, vector<OvSPM*> horOvSPM) {
	//TODO fix the centering displacement
	this->xCenter = xCenter / BU_SIZE;
	this->yCenter = yCenter / BU_SIZE;
	
	this->xUpdateSearchLimits();
		
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
	
	cout << "PrivSPM Report!" << endl;
	cout << "S0 " << this->acumS0 << endl;
	cout << "S1 " << this->acumS1 << endl;
	cout << "S2 " << this->acumS2 << endl;
	cout << "S3 " << this->acumS3 << endl;
	cout << "W03 " << this->acumW03 << endl;
	cout << "W23 " << this->acumW23 << endl;
	cout << "W13 " << this->acumW13 << endl;
}

pair<double,double> PrivSPM::reportPower() {
	double energyWOPG = this->acumTimeInstant * this->searchWindowInBU * this->searchWindowInBU * E_S3; //always in FULL VDD
	double energyWithPG = this->acumS0 * E_S0 +
							this->acumS1 * E_S1 +
							this->acumS2 * E_S2 +
							this->acumS3 * E_S3 +
							this->acumW03 * E_W03 +
							this->acumW13 * E_W13 +
							this->acumW23 * E_W23;
	double savings = (energyWOPG-energyWithPG) / energyWOPG;
	cout << "E(WO PG) " << energyWOPG << endl;
	cout << "E(With PG) " << energyWithPG << endl;
	cout << "SAVINGS " << savings << endl;
	
	return make_pair<double,double>(energyWOPG, energyWithPG);
}

void PrivSPM::reportPowerStates() {
	cerr << this->stateSet[S0].size() << " ";
	cerr << this->stateSet[S1].size() << " ";
	cerr << this->stateSet[S2].size() << " ";
	cerr << this->stateSet[S3].size() << endl;
}