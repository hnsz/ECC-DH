#include <stdio.h>
#include "ecc.h"
#include "mul_by_add.h"

int main(int argc, char *argv[])
{
/*
	int term1;
	int term2;
	sscanf(argv[1], "%d", &term1);
	sscanf(argv[2], "%d", &term2);
	printf("%d * %d = %d\n", term1, term2, ecc_mul(term1, term2));
*/
	ECC_createCTX();

	CURVE *curve;
	PT *p1;
	PT *p2;
	PT *p3;

	curve = ECC_curveNew(5, 0, 0);
	p1 = ECC_ptNew(1, 4);
	p2 = ECC_ptNew(3, 1);
	p3 = ECC_ptNew(0, 0);
	ECC_ptAdd(p3, p1, p2, curve);


	ECC_fPrintPt(stderr, p3);
	ECC_destroyCTX();
/*
*/
	return 0;
}
