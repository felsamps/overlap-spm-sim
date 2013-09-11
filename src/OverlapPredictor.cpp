#include "../inc/OverlapPredictor.h"


OverlapPredictor::OverlapPredictor(Int length, Int center) {
	this->ovLengthInBU = ceil((double)length / BU_SIZE);
	this->center = center;
	
	this->dispA = 0;
	this->dispB = 0;
	this->actualOvThicknessInBU = 0;
	
	this->overlapUsages = new Int[1000];
	this->sizeUsages = 0;
	
}

pair<double, double> OverlapPredictor::xCalcNormDistribution() {
	double mean = 0.0;
	double stdDev, acum = 0.0;
	Int total = this->sizeUsages;
	if(total == 0) {
		return make_pair<double,double>(0,0);
	}
	else if(total == 1) {
		return make_pair<double,double>((double)this->overlapUsages[0], 0.0);
	}
	else {
		for (int i = 0; i < total; i++) {	
			mean += this->overlapUsages[i];
		}
		mean /= total;

		for (int i = 0; i < total; i++) {
			acum += pow(this->overlapUsages[i]-mean,2);
		}
		stdDev = pow(acum/(total-1), 0.5);
		return make_pair<double,double>(stdDev, mean);
	}
}

void OverlapPredictor::predict(Int D, Int ovThickness) {
	
#if OVERLAP_PREDICTOR_EN
	pair<double,double> normDist = xCalcNormDistribution();
	double mean = normDist.first;
	double stdDev = normDist.second;
	if(mean == 0 and stdDev == 0) {
		this->actualOvThicknessInBU = ovThickness;
	}
	else {
		
		double ovPredSize = (D==1) ? mean + PRED_FACT_0*stdDev :
			(D>=2 and D<=3) ? mean + PRED_FACT_1*stdDev :
			(D>=4 and D<=7) ? mean + PRED_FACT_2*stdDev :
							  mean + PRED_FACT_3*stdDev; // (D>=8)
		Int ovPredSizeInBU = ceil(ovPredSize / (BU_SIZE*BU_SIZE));
		cout << "PREDICT " << mean << " " << stdDev << " " << ovPredSizeInBU << endl;
		this->actualOvThicknessInBU = (ceil(ovPredSizeInBU/this->ovLengthInBU) > ovThickness) ? ovThickness : ceil(ovPredSizeInBU/this->ovLengthInBU);
	}
#else
	this->actualOvThicknessInBU = ovThickness;
#endif

	this->dispA = (this->center/BU_SIZE - ceil((this->actualOvThicknessInBU)/2.0));
	this->dispB = (this->center/BU_SIZE + ceil((this->actualOvThicknessInBU)/2.0));
	
	cout << "OVERLAP PREDICTION " << this->dispA << " " << this->dispB << endl;
}


bool OverlapPredictor::isOverlap(Int lengthBU, Int thicknessBU) {
	return (thicknessBU >= this->dispA and thicknessBU < this->dispB);
}

void OverlapPredictor::insertOverlapUsage(Int usageOverlap) {
	this->overlapUsages[this->sizeUsages] = usageOverlap;
	sizeUsages += 1;
}

Int OverlapPredictor::getCenter() const {
	return center;
}

Int OverlapPredictor::getOvLengthInBU() const {
	return ovLengthInBU;
}

Int OverlapPredictor::getActualOvThicknessInBU() const {
	return actualOvThicknessInBU;
}

Int OverlapPredictor::getDispB() const {
	return dispB;
}

Int OverlapPredictor::getDispA() const {
	return dispA;
}
