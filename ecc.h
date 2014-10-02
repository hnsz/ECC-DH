/*******
*	ecc.h
*/
#include <openssl/bn.h>

//	PT represents a point on the elliptic curve
struct pt_t {
	BIGNUM *x;
	BIGNUM *y;
};

typedef struct pt_t PT;




//	CURVE is datatype that holds the curve constants a and b
//	It also holds the modulo p that is used for every 
//	point operation on the curve
struct curve_t {
	BIGNUM *a;
	BIGNUM *b;
	BIGNUM *p;
};

typedef struct curve_t CURVE;


//	For comments on functions see ecc.c
PT *ECC_ptNew(int, int);
void ECC_ptFree(PT *);
CURVE *ECC_curveNew(int, int, int);
void ECC_curveFree(CURVE *);
void ECC_ptSetFromStr(PT *, char *, char *);
void ECC_curveSetFromStr(CURVE *, char *, char *, char *);


int ECC_ptEq(PT *, PT *);
void ECC_ptCopy(PT *, PT *);
void ECC_ptAdd(PT *, PT *, PT *, CURVE *);
void ECC_fPrintPt(FILE *, PT *);

void ECC_createCTX();
void ECC_destroyCTX();

