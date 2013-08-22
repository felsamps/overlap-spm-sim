#include "../inc/CUData.h"

CUData::CUData(UInt depth, UInt x, UInt y) {
	this->depth = depth;
	this->x = x;
	this->y = y;
}

void CUData::insertPU(PUData* pu) {
	UInt idRefFrame = pu->getIdRefFrame();
	this->refsPU[idRefFrame] = pu;	
}

void CUData::setY(UInt y) {
	this->y = y;
}

UInt CUData::getY() const {
	return y;
}

void CUData::setX(UInt x) {
	this->x = x;
}

UInt CUData::getX() const {
	return x;
}

void CUData::setDepth(UInt depth) {
	this->depth = depth;
}

UInt CUData::getDepth() const {
	return depth;
}

