#include <stdio.h>
#include "ecc_mul.h"

int main(int argc, char *argv[])
{
	int term1;
	int term2;

	sscanf(argv[1], "%d", &term1);
	sscanf(argv[2], "%d", &term2);
	

	printf("%d * %d = %d\n", term1, term2, ecc_mul(term1, term2));

	return 0;
}
