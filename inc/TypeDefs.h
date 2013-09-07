#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

typedef int Int;

#define DEBUG_EN 0

#define OV_THICKNESS_FACTOR 2.3
#define BU_SIZE 16
#define CTU_SIZE 64

#define SPM_MISS false
#define SPM_HIT true

#define PRED_FACT_0 0
#define PRED_FACT_1 1
#define PRED_FACT_2 2
#define PRED_FACT_3 3

#define E_S3 1
#define E_S2 0.5
#define E_S1 0.3
#define E_S0 0

#define E_WAKE 4

#define E_W23 E_WAKE * 0.35
#define E_W12 E_WAKE * 0.35
#define E_W13 E_WAKE * 0.6
#define E_W03 E_WAKE



enum SPMStatus {
	HIT, MISS
};

enum PowerState {
	S0, S1, S2, S3
};

#endif	/* TYPEDEFS_H */

