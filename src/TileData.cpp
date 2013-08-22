#include "../inc/TileData.h"

TileData::TileData() {
	listCTU.clear();
}

void TileData::insertCTU(CTUData* ctu) {
	listCTU.push_back(ctu);
}

CTUData* TileData::getNextCTU() {
	CTUData* returnable = this->listCTU.front();
	this->listCTU.pop_front();
	return returnable;
}

void TileData::report() {
	cout << "CTUS " << this->listCTU.size() << endl;
	for(list<CTUData*>::iterator it = listCTU.begin(); it != listCTU.end(); it++) {
		(*it)->report();
	}
}