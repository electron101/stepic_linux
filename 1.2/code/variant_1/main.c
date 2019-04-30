#include "solution.h"
#include <stdio.h>

int main()
{
	char   *string = "Hello";
	size_t multiplier = 3;
	int    *count;

	int result;

	result = stringStat(string, multiplier, count);

	printf("result = %d\n", result);

	return 0;
}
