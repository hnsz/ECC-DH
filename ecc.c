#include "ecc.h"
#include <stdio.h>


BN_CTX *ctx;


void ECC_createCTX()
{
	ctx = BN_CTX_new();
}
void ECC_destroyCTX()
{
	BN_CTX_free(ctx);
}
void ECC_ptSetFromStr(PT *p, char *x_str, char *y_str)
{
	BN_dec2bn(&(p->x), x_str);
	BN_dec2bn(&(p->y), y_str);
}
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
int ECC_ptEq(PT *p1, PT *p2)
{
	if(BN_cmp(p1->x, p2->x) || BN_cmp(p1->y, p2->y))
		return 0;
	else
		return 1;	
}
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
void ECC_ptFree(PT *p)
{
	BN_free(p->x);
	BN_free(p->y);
	free(p);
}
void ECC_curveSetFromStr(CURVE *curve, char *p_str, char *a_str, char *b_str)
{
	BN_dec2bn(&(curve->p), p_str);
	BN_dec2bn(&(curve->a), a_str);
	BN_dec2bn(&(curve->b), b_str);
}
CURVE *ECC_curveNew(int p, int a, int b)
{
	CURVE *curve = malloc(sizeof(CURVE));
	char p_str[11];
	char a_str[11];
	char b_str[11];

	sprintf(p_str, "%d", p);
	sprintf(a_str, "%d", a);
	sprintf(b_str, "%d", b);

	ECC_curveSetFromStr(curve, p_str, a_str, b_str);

	return curve;
}
void ECC_curveFree(CURVE *curve)
{
	BN_free(curve->p);
	BN_free(curve->a);
	BN_free(curve->b);
	free(curve);
}
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

	//	m = (y2 - y1) * (x2 - x1)^-1 (mod p)	| P1 =/= P2
	if(BN_cmp(p1->x, p2->x) || BN_cmp(p1->y, p2->y)) {
		BN_mod_sub(tmp1, p2->y, p1->y, curve->p, ctx);

		BN_mod_sub(tmp2, p2->x, p1->x, curve->p, ctx);
		BN_mod_inverse(tmp3, tmp2, curve->p, ctx);

		BN_mod_mul(m, tmp1, tmp3, curve->p, ctx);
	}
	//	m = (3x1^2 + a) * (2y1)^-1 (mod p)	| P1 == P2
	else {
		BN_mod_sqr(tmp1, p1->x, curve->p, ctx);
		BN_mod_mul(tmp2, three, tmp1, curve->p, ctx);
		BN_mod_add(tmp3, tmp2, curve->a, curve->p, ctx);

		BN_mod_mul(tmp4, two, p1->y, curve->p, ctx);
		BN_mod_inverse(tmp5, tmp4, curve->p, ctx);

		BN_mod_mul(m, tmp3, tmp5, curve->p, ctx);
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
