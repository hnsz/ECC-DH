/*******
*	Multitplying as we did in the olden days.
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ecc.h"
#include <openssl/bn.h>

#define N_SUBSOLUTIONS 1000

void ecc_mul(PT * , BIGNUM *, PT *, CURVE *);


/*******
*	Simple version of multiplying by adding, on the normal axis.
*/
void ecc_mul(PT *p_out, BIGNUM *k, PT *p, CURVE *curve)
{

	BN_CTX *ctxx = BN_CTX_new();
	PT *subSolution[N_SUBSOLUTIONS];
	int n;
	BIGNUM *twoToThePowerOfN = BN_new();
	char two_str[] = "2";
	BIGNUM *two_bn = BN_new();
	BN_dec2bn(&two_bn, two_str);
	PT *tmp_ecc = ECC_ptNew(0,0);
	BIGNUM *tmp_bn = BN_new();
	PT *result = NULL;


	for(n = 0; n < N_SUBSOLUTIONS; ++n)
		subSolution[n] = NULL;
	//	Subsolutions for 2^idx
	//	subSolution[N_SUBSOLUTIONS];


	//	If a value has been set on an element in the subSolution array
	//	the value will be factor * term
	//	This is why factor is used to index the array.
	//	The whole point of this is to only calculate the subSolutions we need.
	//	Set the solution for factor = 1 to term
	subSolution[0] = p;
	
	while(! BN_is_zero(k)) {

		BN_copy(twoToThePowerOfN, two_bn);
		for(n = 1; BN_cmp(twoToThePowerOfN, k) <= 0; BN_sqr(twoToThePowerOfN, twoToThePowerOfN, ctxx), ++n) {
		printf("inner\n");
			if(subSolution[n] == NULL) {
				printf("not known\n");
				subSolution[n] = ECC_ptNew(0,0);
				ECC_ptAdd(subSolution[n], subSolution[n-1], subSolution[n-1], curve);
			}
			else {
				printf("known\n");
				continue;
			}
			ECC_fPrintPt(stdout, subSolution[n]);
		}
		if(result == NULL) {
			printf("copy first result to total\n");
			result = ECC_ptNew(0,0);
			ECC_ptCopy(result, subSolution[n-1]);
		}
		else {
			printf("add result to total\n");
			ECC_ptAdd(tmp_ecc, result, subSolution[n-1], curve);
			ECC_ptCopy(result, tmp_ecc);
		}

		if(BN_cmp(twoToThePowerOfN, two_bn) == 0) {
			break;
		}
		else {
			BN_rshift1(tmp_bn, twoToThePowerOfN);
			BN_copy(twoToThePowerOfN, tmp_bn);
			BN_sub(k, k, twoToThePowerOfN);
		}
		fprintf(stderr, "rest of factor k is:\n");
		BN_print_fp(stderr, k);
		fprintf(stderr, "\n");
	}
	ECC_ptCopy(p_out, result);
}

