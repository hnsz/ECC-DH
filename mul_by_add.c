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

	//	Holds cumulation of intermediate results of k*p
	//	Is set to NULL so that the first intermediate result is 
	//	placed in it instead of added to it. 
	//	Will be copied to p_out in the end.
	PT *result = NULL;



	//	2^0 * p = 1*p
	subSolution[0] = p;
	//	highest power for which subsolution is know is 0
	nMax = 0;

	while(! BN_is_zero(k)) {


		BN_copy(twoToTheN, two_bn);
		for(n = 1; BN_cmp(twoToTheN, k) <= 0; ++n) {

			if(n > nMax) {
				nMax = n;
				//	DEBUG
				fprintf(stderr, "Subsolution for 2^%d NOT known\n", n);
				//	END DEBUG
				subSolution[n] = ECC_ptNew(0,0);
				ECC_ptAdd(subSolution[n], subSolution[n-1], subSolution[n-1], curve);
			}
			else {
				//	DEBUG
				fprintf(stderr, "Subsolution for 2^%d IS known\n", n);
				//	END DEBUG
			}
			//	DEBUG
			ECC_fPrintPt(stderr, subSolution[n]);
			//	END DEBUG

			//	multiply by 2
			BN_lshift1(tmp_bn, twoToTheN);
			BN_swap(twoToTheN, tmp_bn);
		}
		if(result == NULL) {
			//	DEBUG
			fprintf(stderr, "copy first result to total\n");
			//	END DEBUG
			result = ECC_ptNew(0,0);
			ECC_ptCopy(result, subSolution[n-1]);
		}
		else {
			//	DEBUG
			fprintf(stderr, "add result to total\n");
			//	END DEBUG
			ECC_ptAdd(tmp_pt, result, subSolution[n-1], curve);
			ECC_ptCopy(result, tmp_pt);
		}

		//	Always shoots over once so devide by 2
		BN_rshift1(tmp_bn, twoToTheN);
		BN_swap(twoToTheN, tmp_bn);

		//	DEBUG
		fprintf(stderr, "2 to the power of n is (hex):\n");
		BN_print_fp(stderr, twoToTheN);
		fprintf(stderr, "\n");
		//	END DEBUG

		BN_sub(tmp_bn, k, twoToTheN);
		BN_swap(k, tmp_bn);

		//	DEBUG
		fprintf(stderr, "rest of factor k is:\n");
		BN_print_fp(stderr, k);
		fprintf(stderr, "\n");
		//	END DEBUG
	}
	ECC_ptCopy(p_out, result);
}

