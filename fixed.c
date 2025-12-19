#include "fixed.h"

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
