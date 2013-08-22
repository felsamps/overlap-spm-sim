#include "../inc/MotionEstimationData.h"

MotionEstimationData::MotionEstimationData(UInt idCurrFrame, UInt numOfTiles) {
	this->idCurrFrame = idCurrFrame;
	this->numOfTiles = numOfTiles;
	this->refFrames.clear();
	
	tiles.resize(numOfTiles);
	for (int i = 0; i < numOfTiles; i++) {
		tiles[i] = NULL;
	}	
}

TileData* MotionEstimationData::getTile(UInt idTile) {
	if( tiles[idTile] == NULL ) {
		tiles[idTile] = new TileData();
	}
	return tiles[idTile];
}

void MotionEstimationData::insertRefFrame(UInt idRefFrame) {
	this->refFrames.insert(idRefFrame);
}

void MotionEstimationData::report() {
	for (int i = 0; i < this->tiles.size(); i++) {
		cout << "TILE " << i << endl;
		this->tiles[i]->report();

	}

}