#include <stdio.h>
#include "ecc.h"
#include "mul_by_add.h"

int main(int argc, char *argv[])
{


	ECC_createCTX();

	BIGNUM *k = BN_new();
	CURVE *curve;
	PT *p1;
	PT *p2;


	char k_str[] = "486035459702866949106113048381182";
	char x_str[] = "97339010987059066523156133908935";
	char y_str[] = "149670372846169285760682371978898";
	char p_str[] = "564538252084441556247016902735257";
	char a_str[] = "321094768129147601892514872825668";
	char b_str[] = "430782315140218274262276694323197";
	char k_str_rev[] = "281183840311601949668207954530684";


	BN_dec2bn(&k, k_str_rev);
	

	curve = ECC_curveNew(0, 0, 0);
	p1 = ECC_ptNew(0, 0);
	p2 = ECC_ptNew(0, 0);


	ECC_curveSetFromStr(curve, p_str, a_str, b_str);
	ECC_ptSetFromStr(p1, x_str, y_str);

	
	ecc_mul(p2, k, p1, curve);
	ECC_fPrintPt(stdout, p2);

/*	In the context of ECC the parameters a = 10, b = -21, p = 41, P1 = (3,6) are given. 
	Assuming that Alice uses multiplier 44 and Bob uses multiplier 57 
	their shared secret will be the x-coordinate of 44 * 57 * P1.
*/



	ECC_destroyCTX();




	return 0;
}
