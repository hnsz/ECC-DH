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

	//	Subsolutions for 2^n * point on the elliptic curve
	PT *subSolution[N_SUBSOLUTIONS];

	//	n represents both the power, and the index of subSolutions
	int n;

	//	the highest power of two reached so far
	//	(so it is known which subsolutions are known
	int nMax;

	//	holds the actual value of 2^n
	BIGNUM *twoToTheN = BN_new();

	//	a bignum with the value 2
	BIGNUM *two_bn = BN_new();
	char two_str[] = "2";
	BN_dec2bn(&two_bn, two_str);

	//	a tmp bignum that is used as a temp in 2 step operations
	BIGNUM *tmp_bn = BN_new();

	//	a tmp PT * that is used for 2 step operations on points
	PT *tmp_pt = ECC_ptNew(0,0);

	//	Holds cumulation of intermediate results for k*p
	//	Is set to NULL so that the first intermediate result is 
	//	placed in it instead of added to it. 
	//	Will be copied to p_out in the end.
	PT *result = NULL;



	
	//	2^0 * p = 1*p
	//	  ^         ^
	//	Set solution for 0 to p
	subSolution[0] = ECC_ptNew(0,0);
	ECC_ptCopy(subSolution[0], p);

	//	highest power for which a subsolution is know is 0
	nMax = 0;

	while(! BN_is_zero(k)) {

		//	Always start with 2^1
		BN_copy(twoToTheN, two_bn);

		//	Keep multiplying by 2 until greater than k, 
		//	multiplication happens at end of body
		for(n = 1; BN_cmp(twoToTheN, k) <= 0; ++n) {

			if(n > nMax) {
				nMax = n;

				subSolution[n] = ECC_ptNew(0,0);
				ECC_ptAdd(subSolution[n], subSolution[n-1], subSolution[n-1], curve);
			}


			//	multiply by 2
			BN_lshift1(tmp_bn, twoToTheN);
			BN_swap(twoToTheN, tmp_bn);	
			//nb. swap is more efficient than copy and it doesn't matter what value tmp_bn
			//holds after this
		}



		if(result == NULL) {

			result = ECC_ptNew(0,0);
			ECC_ptCopy(result, subSolution[n-1]);
		}
		else {

			ECC_ptAdd(tmp_pt, result, subSolution[n-1], curve);
			ECC_ptCopy(result, tmp_pt);
		}

		//	Always shoots over once so devide by 2
		BN_rshift1(tmp_bn, twoToTheN);
		BN_swap(twoToTheN, tmp_bn);


		//	Subtract from k and put rest in k for new round
		BN_sub(tmp_bn, k, twoToTheN);
		BN_swap(k, tmp_bn);

	}

	//	Copy result to the destination pt that was given as an argument
	ECC_ptCopy(p_out, result);



	//	clean up

	for(n = 0; n < nMax; ++n) {
		ECC_ptFree(subSolution[n]);
	}

	BN_free(twoToTheN);
	BN_free(two_bn);
	BN_free(tmp_bn);
	ECC_ptFree(tmp_pt);
	ECC_ptFree(result);
}

