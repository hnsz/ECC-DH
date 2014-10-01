/*******
*	Multitplying as we did in the olden days.
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int ecc_mul(int a, int b);
int _ecc_mul(int factor, int *subSolution);
int _ecc_add(int factor1, int factor2, int *subSolution);
int normalAdd(int, int);


/*******
*	Simple version of multiplying by adding, on the normal axis.
*/
int ecc_mul(int a, int b)
{
	int *subSolution;
	int factor;
	int term;
	int result;
	//	Use the smallest number as factor to limit number of additions
	factor = (a < b)?a:b;
	term = (factor == a)?b:a;

	subSolution = malloc((factor + 1) * sizeof(int));
	memset(subSolution, 0, (factor + 1) * sizeof(int));


	//	If a value has been set on an element in the subSolution array
	//	the value will be factor * term
	//	This is why factor is used to index the array.
	//	The whole point of this is to only calculate the subSolutions we need.
	//	Set the solution for factor = 1 to term
	subSolution[1] = term;	
	
	result = _ecc_mul(factor, subSolution);

	free(subSolution);
	return result;
}
int _ecc_mul(int factor, int *subSolution)
{
	if(factor == 1) {

		return subSolution[1];
	} else
	if(subSolution[factor] == 0) {

		if(factor % 2) {

			subSolution[factor - 1] = _ecc_add(factor / 2, factor / 2, subSolution);
			subSolution[factor] = _ecc_add(factor - 1, 1, subSolution);
		} else {

			subSolution[factor] = _ecc_add(factor / 2, factor / 2, subSolution);
		}
	} 

	return subSolution[factor];
}
int _ecc_add(int factor1, int factor2, int *subSolution)
{
	int term1 = _ecc_mul(factor1, subSolution);
	int term2 = _ecc_mul(factor2, subSolution);

	return normalAdd(term1, term2);
}
int normalAdd(int t1, int t2)
{
	return t1+t2;
}
