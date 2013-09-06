#include <set>

#include "../inc/OvSPM.h"

OvSPM::OvSPM(Int length, Int thickness, Int center, Int numOfCores) {
	this->ovThicknessInBU = ceil((double)thickness / BU_SIZE);
	this->ovLengthInBU = ceil((double)length / BU_SIZE);
	this->numOfCores = numOfCores;
	
	this->powerMap = new PowerState* [ovLengthInBU];
	this->statMap = new Int* [ovLengthInBU];
	this->coreMap = new bool** [ovLengthInBU];
	for (int i = 0; i < ovLengthInBU; i++) {
		this->powerMap[i] = new PowerState[ovThicknessInBU];
		this->statMap[i] = new Int[ovThicknessInBU];
		this->coreMap[i] = new bool* [ovThicknessInBU];
		for (int t = 0; t < this->ovThicknessInBU; t++) {
			this->coreMap[i][t] = new bool[this->numOfCores];
		}
	}
	
	this->ovPred = new OverlapPredictor(length, center);
	
	cout << "Overlap SPM Created: " << center << " " << this->ovLengthInBU << " " << this->ovThicknessInBU << endl;
	
	this->readAcum = 0;
	this->writeAcum = 0;
	
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

void OvSPM::reset() {
	this->usage = 0;
}

void OvSPM::initPowerStates() {	
	for (int l = 0; l < this->ovLengthInBU; l++) {
		for (int t = 0; t < this->ovThicknessInBU; t++) {
			if(t < this->ovPred->getActualOvThicknessInBU()) {
				xUpdatePowerState(make_pair<Int,Int>(l,t));
			}
			else {
				this->stateSet[this->powerMap[l][t]].erase(make_pair<Int,Int>(l,t));
				this->powerMap[l][t] = S0;
				this->stateSet[S0].insert(make_pair<Int,Int>(l,t));
				
			}			
			this->statMap[l][t] = 0;
			for (int c = 0; c < numOfCores; c++) {
				this->coreMap[l][t][c] = false;
			}
		}
	}
}

void OvSPM::updatePowerStates() {
	//TODO implement it
}

void OvSPM::xUpdatePowerState(pair<Int,Int> acc) {
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

void OvSPM::xUpdatePowerCounters() {
	this->acumS0 += this->stateSet[S0].size();
	this->acumS1 += this->stateSet[S1].size();
	this->acumS2 += this->stateSet[S2].size();
	this->acumS3 += this->stateSet[S3].size();

	this->acumTimeInstant += 1;
}

SPMStatus OvSPM::read(Int lBU, Int tBU, Int reqCore) {
	
	if(lBU >=0 and lBU < this->ovLengthInBU) {
		Int tPos = tBU - this->ovPred->getDispA();
		Int lPos = lBU;						
		SPMStatus returnable = (statMap[lPos][tPos] == 0) ? MISS : HIT;

		if(returnable == HIT) {
			this->statMap[lPos][tPos] += 1;
			this->readAcum += BU_SIZE*BU_SIZE;
		}
		this->coreMap[lPos][tPos][reqCore] = true;							
		xUpdatePowerState(make_pair<int,int>(lPos, tPos));
		xUpdatePowerCounters();
		return returnable;
	}
		
}

void OvSPM::write(Int lBU, Int tBU) {
	for (int t = 0; t < this->ovPred->getActualOvThicknessInBU(); t++) {
		statMap[lBU][t] += 1;
		powerMap[lBU][t] = S3;
		this->writeAcum += BU_SIZE*BU_SIZE;
	}

}

void OvSPM::updateOverlapUsage() {
	this->usage = 0;
	for (int l = 0; l < this->ovLengthInBU; l++) {
		for (int t = 0; t < this->ovThicknessInBU; t++) {
			Int acum = 0;
			for (int i = 0; i < this->numOfCores; i++) {
				acum += (this->coreMap[l][t][i]) ? 1 : 0;
			}
			this->usage += (acum > 1) ? BU_SIZE*BU_SIZE : 0;	
		}
	}
	this->ovPred->insertOverlapUsage(this->usage);
	cout << "USAGE " << this->usage  << " " << (this->usage)/(double)(this->ovLengthInBU*this->ovThicknessInBU*BU_SIZE*BU_SIZE) << endl;
}

OverlapPredictor* OvSPM::getOvPred() const {
	return ovPred;
}

Int OvSPM::getUsage() const {
	return usage;
}

long long int OvSPM::getWriteAcum() const {
	return writeAcum;
}

long long int OvSPM::getReadAcum() const {
	return readAcum;
}

Int OvSPM::getOvLengthInBU() const {
	return ovLengthInBU;
}

Int OvSPM::getOvThicknessInBU() const {
	return ovThicknessInBU;
}

void OvSPM::report() {
	cout << "OvSPM Report!" << endl;
	cout << "S0 " << this->acumS0 << endl;
	cout << "S1 " << this->acumS1 << endl;
	cout << "S2 " << this->acumS2 << endl;
	cout << "S3 " << this->acumS3 << endl;
	cout << "W03 " << this->acumW03 << endl;
	cout << "W23 " << this->acumW23 << endl;
	cout << "W13 " << this->acumW13 << endl;
	
}