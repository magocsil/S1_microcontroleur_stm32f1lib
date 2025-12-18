#include <stdio.h>
#include <stdlib.h>

#define SCALING_FACTOR (1 << 13)
#define UPSCALED(i) (i * SCALING_FACTOR)
#define DOWNSCALED(i) (i / SCALING_FACTOR)
#define INT(i) DOWNSCALED(i)

#if SCALING_FACTOR%10
	#define FRAC(i,m) (DOWNSCALED(i * m) - (INT(i) * m))
#else
	#define FRAC(i,m) (i - UPSCALED(DOWNSCALED(i)))
#endif

#define DISP(i,d,m) ((INT(i) < 0) || (FRAC(i,m) < 0)) ? "-" : "", (INT(i) < 0) ? -INT(i) : INT(i), d, (FRAC(i,m) < 0) ? -FRAC(i,m) : FRAC(i,m)
#define FIXED_STR "%s%d.%.*d"

void fixedFindMultiplier(long *multiplier, unsigned char *digits);
long fixedAdd(long p, long q);
long fixedSubt(long positive, long negative);
long fixedMul(long p, long q);
long fixedDiv(long nominator, long denominator, long valueIfError);
long fixedPow(long p, char radicand);
long fixedRoot(long p, char radical, long valueIfError);

int main(int *arg, char *args[])
{
	long a = (long)UPSCALED((long) atoi(args[1]));
	long b = (long)UPSCALED((long) atoi(args[2]));
	long multiplier = 1;
	unsigned char digits = 1;
	fixedFindMultiplier(&multiplier, &digits);

	long sum = fixedAdd(a, b);
	long diff = fixedSubt(a, b);
	long prod = fixedMul(a, b);
	long rate = fixedDiv(a, b, b);
	
	printf(FIXED_STR ", " FIXED_STR " -> " FIXED_STR "; " FIXED_STR "; " FIXED_STR "; " FIXED_STR "\n", DISP(a, digits, multiplier), DISP(b, digits, multiplier), DISP(sum, digits, multiplier), DISP(diff, digits, multiplier), DISP(prod, digits, multiplier), DISP(rate, digits, multiplier));
	
	for(int i = 5; i > -5; i--)
	{
		printf(FIXED_STR ", ", DISP(fixedPow(b, i), digits, multiplier));
		printf(FIXED_STR "\n", DISP(fixedRoot(b, i, 0), digits, multiplier));
	}
	
	return 0;
}

void fixedFindMultiplier(long *multiplier, unsigned char *digits)
{
	for((*multiplier) = 1, (*digits) = 0; (*multiplier) < SCALING_FACTOR; (*multiplier) *= 10, (*digits)++);
}

long fixedAdd(long p, long q)
{
	return p + q;
}

long fixedSubt(long positive, long negative)
{
	return fixedAdd(positive, -negative);
}

long fixedMul(long p, long q)
{
	return (long)DOWNSCALED((long long)p * (long long)q);
}

long fixedDiv(long nominator, long denominator, long valueIfError)
{
	if(!denominator)
	{
		return valueIfError;
	}

	long result = (long)(UPSCALED((long long)nominator) / (long long)denominator);
	return result;
}

long fixedPow(long p, char radicand)
{
	char isNegative = 0;
	if(radicand < 0)
	{
		radicand *= -1;
		isNegative++;
	}
	long result = SCALING_FACTOR;
	for(int i = 0; i < radicand; i++)
	{
		result = fixedMul(result, p);
	}
	
	if(isNegative)
	{
		result = fixedDiv(SCALING_FACTOR, result, 0);
	}
	
	return result;
}

long fixedRoot(long p, char radical, long valueIfError)
{
	if(((p < 0) && !(radical & 1)) || !radical)
	{
		return valueIfError;
	}
	
	char isNegative = 0;
	if(radical < 0)
	{
		radical *= -1;
		isNegative++;
	}
	
	long result = SCALING_FACTOR;
	long change = result;
	while(change)
	{
		long temp = fixedMul(SCALING_FACTOR / radical, fixedAdd(result * (radical - 1), fixedDiv(p, fixedPow(result, (radical - 1)), 0)));
		change = result - temp;
		result = temp;
	}
	
	if(isNegative)
	{
		result = fixedDiv(SCALING_FACTOR, result, 0);
	}
	
	return result;
}
