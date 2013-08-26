#include "../inc/CTUData.h"

CTUData::CTUData(UInt x, UInt y) {
	this->x = x;
	this->y = y;
	for (int i = 0; i < 4; i++) {
		vector<CUData*> vec;
		this->vectorCU.push_back(vec);
	}

}

void CTUData::insertCU(CUData* cu) {
	UInt depth = cu->getDepth();
	this->vectorCU[depth].push_back(cu);
}

void CTUData::setY(UInt y) {
	this->y = y;
}

UInt CTUData::getY() const {
	return y;
}

void CTUData::setX(UInt x) {
	this->x = x;
}

UInt CTUData::getX() const {
	return x;
}


void CTUData::report() {
	for (int i = 0; i < 4; i++) {
		cout << "Depth " << i << " " << vectorCU[i].size() << endl;
	}
}