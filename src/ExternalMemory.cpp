#include "../inc/ExternalMemory.h"

ExternalMemory::ExternalMemory() {
	this->numOfReadBU = 0;
	this->numOfPageAct = 0;
}

void ExternalMemory::read(Int burstLengthInBU) {
	this->numOfReadBU += burstLengthInBU;
}

void ExternalMemory::report() {
	cout << "EXTERNAL MEMORY REPORT" << endl;
	cout << "READ_BU:\t" << this->numOfReadBU << endl;
	cout << "READ_MB:\t" << this->numOfReadBU * (BU_SIZE*BU_SIZE) / (1024.0*1024.0) << endl;
}