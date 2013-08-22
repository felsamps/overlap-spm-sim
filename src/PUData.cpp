#include "../inc/PUData.h"

PUData::PUData(UInt idPart, UInt sizePart, UInt idRefFrame){
	this->idPart = idPart;
	this->sizePart = sizePart;
	this->idRefFrame = idRefFrame;
}

void PUData::insertEntry(Entry* e) {
	entries.push_back(e);
}

Entry* PUData::getEntry() {
	Entry* returnable = this->entries.front();
	this->entries.pop_front();
	return returnable;
}

void PUData::setIdRefFrame(UInt idRefFrame) {
	this->idRefFrame = idRefFrame;
}

UInt PUData::getIdRefFrame() const {
	return idRefFrame;
}

void PUData::setSizePart(UInt sizePart) {
	this->sizePart = sizePart;
}

UInt PUData::getSizePart() const {
	return sizePart;
}

void PUData::setIdPart(UInt idPart) {
	this->idPart = idPart;
}

UInt PUData::getIdPart() const {
	return idPart;
}

