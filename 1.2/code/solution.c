#include <stddef.h>
#include <stdio.h>
#include <string.h>

int stringStat(const char *string, size_t multiplier, int *count);

int stringStat(const char *string, size_t multiplier, int *count)
{
	/* *count++; */
	*count = *count + 1;
	return strlen(string) * multiplier;
}
