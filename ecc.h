#include <openssl/bn.h>

struct pt_t {
	BIGNUM *x;
	BIGNUM *y;
};


//	Curve constant a
//	Curve constant b
//	Modulo p
struct curve_t {
	BIGNUM *a;
	BIGNUM *b;
	BIGNUM *p;
};

typedef struct pt_t PT;
typedef struct curve_t CURVE;



PT *ECC_ptNew(int, int);
CURVE *ECC_curveNew(int, int, int);

int ECC_ptEq(PT *, PT *);
void ECC_ptCopy(PT *, PT *);
void ECC_ptAdd(PT *, PT *, PT *, CURVE *);
void ECC_fPrintPt(FILE *, PT *);

void ECC_createCTX();
void ECC_destroyCTX();

