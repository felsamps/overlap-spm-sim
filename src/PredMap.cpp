#include "../inc/PredMap.h"

Int STATIC_GOP_ME_ENTRIES[23][2] = {};

PredMap::PredMap(string fileName, Int wFrame, Int hFrame, Int sizeCU) {
	Int wInCU = wFrame / sizeCU;
	Int hInCU = hFrame / sizeCU;
	Int*** preds;
	
	this->fp.open(fileName.c_str(), fstream::in);
	
	preds = new Int**[wInCU];
	for (int i = 0; i < wInCU; i++) {
		preds[i] = new Int*[hInCU];
		for (int j = 0; j < hInCU; j++) {
			preds[i][j] = new Int[2];
		}
	}
	
	for (int idME = 0; idME < 23; idME++) {
		Int idCurrFrame = STATIC_GOP_ME_ENTRIES[idME][0];
		Int idRefFrame = STATIC_GOP_ME_ENTRIES[idME][1];
		for (int y = 0; y < hInCU; y++) {
			for (int x = 0; x < wInCU; x++) {
				fp >> preds[x][y][0] >> preds[x][y][1];
			}
		}
		this->predMap[make_pair<Int,Int>(idCurrFrame, idRefFrame)] = preds;
	}
	
}

Int*** PredMap::getPred(Int idCurrFrame, Int idRefFrame) {
	return this->predMap[make_pair<Int,Int>(idCurrFrame, idRefFrame)];
}
