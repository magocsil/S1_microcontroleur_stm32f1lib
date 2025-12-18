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
#define F_STR "%s%d.%.*d"

#define SCALE SCALING_FACTOR
#define N 4
#define str(x) #x
#define xstr(x) str(x)  // convert N to "N"

void fixedFindMultiplier(long *multiplier, unsigned char *digits);
long fixedAdd(long p, long q);
long fixedSubt(long positive, long negative);
long fixedMul(long p, long q);
long fixedDiv(long nominator, long denominator, long valueIfError);
long fixedSq(long p);
long fixedSqrt(long p, long valueIfError);
int pow10int(int n);

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
	long sq = fixedSq(b);
	long sqrt = fixedSqrt(a, a);
	
	printf(F_STR ", " F_STR " -> " F_STR "; " F_STR "; " F_STR "; " F_STR "; " F_STR "; " F_STR "\n", DISP(a, digits, multiplier), DISP(b, digits, multiplier), DISP(sum, digits, multiplier), DISP(diff, digits, multiplier), DISP(prod, digits, multiplier), /*"", INT(rate), digits, FRAC(rate, multiplier)*/DISP(rate, digits, multiplier), DISP(sq, digits, multiplier), DISP(sqrt, digits, multiplier));
	return 0;
}

// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
 
int pow10int(int n)
{long x=1,k=n;
 while (k>0) {x*=10;k--;}
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
	long long tmp;
	tmp=(long long)p*(long long)q;
	tmp/=SCALE;
	
	return((long)tmp);

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

long fixedSq(long p)
{
	return fixedMul(p, p);
}

long fixedSqrt(long p, long valueIfError)
{
	if(p < 0)
	{
		return valueIfError;
	}
	
	long pNew = SCALING_FACTOR;
	for(int i = 0; i < 16; i++)
	{
		pNew = fixedMul(SCALING_FACTOR >> 1, fixedAdd(pNew, fixedDiv(p, pNew, 0)));
	}
	
	return pNew;
}
