#ifndef OVERLAPPREDICTOR_H
#define	OVERLAPPREDICTOR_H

#include <iostream>
#include <cmath>
#include <vector>

#include "TypeDefs.h"
#include "PredMap.h"

using namespace std;

class OverlapPredictor {
private:
	Int* overlapUsages;
	Int sizeUsages;
	Int dispA;
	Int dispB;
	Int actualOvThicknessInBU;
	Int ovLengthInBU;
	Int center;
	
	pair<double, double> xCalcNormDistribution();
public:
	OverlapPredictor(Int length, Int center);
	
	void insertOverlapUsage(Int usageOverlap);
	void predict(Int D, Int ovThickness);
	
	bool isOverlap(Int xBU, Int yBU);
	
	Int getCenter() const;
	Int getOvLengthInBU() const;
	Int getActualOvThicknessInBU() const;
	Int getDispB() const;
	Int getDispA() const;
	
};

#endif	/* OVERLAPPREDICTOR_H */

