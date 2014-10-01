#include <stdio.h>
#include "ecc.h"
#include "mul_by_add.h"

int main(int argc, char *argv[])
{


	ECC_createCTX();

	CURVE *curve;
	PT *p1;
	PT *p2;


	char k_str[] = "15";
	BIGNUM *k = BN_new();
	BN_dec2bn(&k, k_str);
	

	curve = ECC_curveNew(167, 11, 19);
	p1 = ECC_ptNew(2, 7);
	p2 = ECC_ptNew(0, 0);
	
	ecc_mul(p2, k, p1, curve);
	ECC_fPrintPt(stderr, p2);
	ECC_destroyCTX();

	return 0;
}
