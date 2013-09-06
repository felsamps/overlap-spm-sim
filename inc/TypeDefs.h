#ifndef TYPEDEFS_H
#define	TYPEDEFS_H

typedef int Int;

#define DEBUG_EN 0

#define OV_THICKNESS_FACTOR 2.3
#define BU_SIZE 16
#define CTU_SIZE 64

#define SPM_MISS false
#define SPM_HIT true

#define PRED_FACT_0 1
#define PRED_FACT_1 2
#define PRED_FACT_2 3
#define PRED_FACT_3 4

enum SPMStatus {
	HIT, MISS
};

enum PowerState {
	S0, S1, S2, S3
};

#endif	/* TYPEDEFS_H */

