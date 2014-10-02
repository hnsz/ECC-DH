/*******
*	ecc.c
*/
#include "ecc.h"
#include <stdio.h>


//	This is a datatype that is used by openssl for
//	effiency purposes. It should be reused for every
//	openssl function that requires it.
//	This variable is only used internally by ecc.c but
//	it should be initialised with ECC_createCTX() before
//	using ECC_ functions.
//	Afterwards is can be destroyed with ECC_destroyCTX();
BN_CTX *ctx;

//	See above
void ECC_createCTX()
{
	ctx = BN_CTX_new();
}
//	See above
void ECC_destroyCTX()
{
	BN_CTX_free(ctx);
}


/*******
*	PT functions
*/

//	Allocates memory for a PT sets it to the given values
//	returns pointer
PT *ECC_ptNew(int x, int y)
{
	PT *p = malloc(sizeof(PT));
	char x_str[11];
	char y_str[11];
	p->x = BN_new();
	p->y = BN_new();

	sprintf(x_str, "%d", x);
	sprintf(y_str, "%d", y);

	ECC_ptSetFromStr(p, x_str, y_str);

	return p;
}

//	Frees up a PT and its contents
void ECC_ptFree(PT *p)
{
	BN_free(p->x);
	BN_free(p->y);
	free(p);
}

//	Sets a PT to the decimal values given by argument strings
void ECC_ptSetFromStr(PT *p, char *x_str, char *y_str)
{
	BN_dec2bn(&(p->x), x_str);
	BN_dec2bn(&(p->y), y_str);
}

//	Copies the x,y values from one PT to another PT
void ECC_ptCopy(PT *p_dst, PT *p_src)
{
	//	DEBUG
	fprintf(stderr, "copying\n");
	ECC_fPrintPt(stderr, p_src);
	fprintf(stderr, "to pt that currently holds\n");
	ECC_fPrintPt(stderr, p_dst);
	//	END DEBUG
	BN_copy(p_dst->x, p_src->x);
	BN_copy(p_dst->y, p_src->y);
}
//	Returns 1 if two PT's are equal, 0 if they are not equal
int ECC_ptEq(PT *p1, PT *p2)
{
	if(BN_cmp(p1->x, p2->x) || BN_cmp(p1->y, p2->y))
		return 0;
	else
		return 1;	
}
//	Adds p1 and p2 on the curve and puts result in p3
//	Curve data and modulo are to be supplied by curve
void ECC_ptAdd(PT *p3, PT *p1, PT *p2, CURVE *curve)
{

	BIGNUM *two = BN_new();
	BIGNUM *three = BN_new();
	BIGNUM *m = BN_new();
	BIGNUM *tmp1 = BN_new();
	BIGNUM *tmp2 = BN_new();
	BIGNUM *tmp3 = BN_new();
	BIGNUM *tmp4 = BN_new();
	BIGNUM *tmp5 = BN_new();

	//	DEBUG
	fprintf(stderr, "Add points\n");
	ECC_fPrintPt(stderr, p1);
	ECC_fPrintPt(stderr, p2);
	//	EN DEBUG


	BN_dec2bn(&two, "2");
	BN_dec2bn(&three, "3");

	//	m = (3x1^2 + a) * (2y1)^-1 (mod p)	| P1 == P2
	if(ECC_ptEq(p1, p2)) {
		BN_mod_sqr(tmp1, p1->x, curve->p, ctx);
		BN_mod_mul(tmp2, three, tmp1, curve->p, ctx);
		BN_mod_add(tmp3, tmp2, curve->a, curve->p, ctx);

		BN_mod_mul(tmp4, two, p1->y, curve->p, ctx);
		BN_mod_inverse(tmp5, tmp4, curve->p, ctx);

		BN_mod_mul(m, tmp3, tmp5, curve->p, ctx);
	}
	//	m = (y2 - y1) * (x2 - x1)^-1 (mod p)	| P1 =/= P2
	else {
		BN_mod_sub(tmp1, p2->y, p1->y, curve->p, ctx);

		BN_mod_sub(tmp2, p2->x, p1->x, curve->p, ctx);
		BN_mod_inverse(tmp3, tmp2, curve->p, ctx);

		BN_mod_mul(m, tmp1, tmp3, curve->p, ctx);
	}
	//	All tmp variables can be used again

	//	x3 = m^2 - x1 - x2 (mod p)
	BN_mod_sqr(tmp1, m, curve->p, ctx);
	BN_mod_sub(tmp2, tmp1, p1->x, curve->p, ctx);
	BN_mod_sub(p3->x, tmp2, p2->x,  curve->p, ctx);
	//	All tmp variables can be used again

	
	//	y3 = m*(x1 - x3) - y1 (mod p)
	BN_mod_sub(tmp1, p1->x, p3->x, curve->p, ctx);
	BN_mod_mul(tmp2, m, tmp1, curve->p, ctx);
	BN_mod_sub(p3->y, tmp2, p1->y, curve->p, ctx);

	//	DEBUG
	fprintf(stderr, "result = ");
	ECC_fPrintPt(stderr, p3);
	//	END DEBUG

	//	clean up
	BN_free(two);
	BN_free(three);
	BN_free(tmp1);
	BN_free(tmp2);
	BN_free(tmp3);
	BN_free(tmp4);
	BN_free(tmp5);
	BN_free(m);

}
//	prints data from p  to out
void ECC_fPrintPt(FILE *out, PT *p)
{
	char *x;
	char *y;

	x = BN_bn2dec(p->x);
	y = BN_bn2dec(p->y);

	fprintf(out, "(%s, %s)\n", x, y);
	OPENSSL_free(x);
	OPENSSL_free(y);
}


/*******
*	CURVE functions
*/
//	Allocates memory for a CURVE and initialises it with
//	modulo p, and curve constants a and b
CURVE *ECC_curveNew(int p, int a, int b)
{
	CURVE *curve = malloc(sizeof(CURVE));
	curve->p = BN_new();
	curve->a = BN_new();
	curve->b = BN_new();
	char p_str[11];
	char a_str[11];
	char b_str[11];

	sprintf(p_str, "%d", p);
	sprintf(a_str, "%d", a);
	sprintf(b_str, "%d", b);

	ECC_curveSetFromStr(curve, p_str, a_str, b_str);

	return curve;
}
//	Frees curve and all its contents
void ECC_curveFree(CURVE *curve)
{
	BN_free(curve->p);
	BN_free(curve->a);
	BN_free(curve->b);
	free(curve);
}
//	Sets curve tp data given by decimal strings
void ECC_curveSetFromStr(CURVE *curve, char *p_str, char *a_str, char *b_str)
{
	BN_dec2bn(&(curve->p), p_str);
	BN_dec2bn(&(curve->a), a_str);
	BN_dec2bn(&(curve->b), b_str);
}
