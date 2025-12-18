#include <stdio.h>
#include <stdlib.h>

#define SCALING_FACTOR (1000)
#define UPSCALED(i) (i * SCALING_FACTOR)
#define DOWNSCALED(i) (i / SCALING_FACTOR)
#define INT(i) DOWNSCALED(i)
#define FRAC(i) (i - UPSCALED(DOWNSCALED(i)))

#define SCALE SCALING_FACTOR
#define N 4
#define str(x) #x
#define xstr(x) str(x)  // convert N to "N"

void fixedFindMultiplier(unsigned long *multiplier, unsigned char *digits);
long fixedAdd(long p, long q);
long fixedSubt(long positive, long negative);
long fixedMul(long p, long q);
long fixedDiv(long nominator, long denominator);
long fixedSq(long p);
long fixedSqrt(long p);
int pow10int(int n);

int main(int *arg, char *args[])
{
	long a = (long)UPSCALED(0.6);
	long b = (long)UPSCALED(4);
	unsigned long multiplier = 1;
	unsigned char digits = 1;
	fixedFindMultiplier(&multiplier, &digits);

	long sum = fixedAdd(a, b);
	long diff = fixedSubt(a, b);
	long prod = fixedMul(a, b);
	long rate = fixedDiv(a, b);
	long sq = fixedSq(b);
	long sqrt = fixedSqrt(UPSCALED(atoi(args[1])));
	
	printf("%d.%.*d, %d.%.*d -> %d.%.*d; %d.%.*d; %d.%.*d; %s%d.%.*d; %d.%d\n", INT(a), digits, FRAC(a), INT(b), digits, FRAC(b), INT(sum), digits, FRAC(sum), INT(diff), digits, FRAC(diff), INT(prod), digits, FRAC(prod), (INT(rate) < 0) || (FRAC(rate) < 0) ? "-" : "", (INT(rate) < 0) ? -INT(rate) : INT(rate), digits, (FRAC(rate) < 0) ? -FRAC(rate) : FRAC(rate), INT(sq), FRAC(sq));
	printf("%d.%d\n", INT(sqrt), FRAC(sqrt));
	return 0;
}

// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
 
int pow10int(int n)
{long x=1,k=n;
 while (k>0) {x*=10;k--;}
}

void fixedFindMultiplier(unsigned long *multiplier, unsigned char *digits)
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

long fixedDiv(long nominator, long denominator)
{
	long result = (long)(UPSCALED((long long)nominator) / (long long)denominator);
	return result;
	/*
	long p = FRAC(result);
	long i = UPSCALED(INT(result));
	
	p *= (*multiplier) * 10;
	
	p /= (SCALING_FACTOR - 1);
	
	if(p % 10 >= 5)
	{
		p += 10;
	}
	p /= 10;
	
	while(p > SCALING_FACTOR)
	{
		p /= 10;
		(*multiplier) /= 10;
		(*digits)--;
	}

	return i + p;
	*/
}

long fixedSq(long p)
{
	return fixedMul(p, p);
}

long fixedSqrt(long p)
{
	long pi = SCALING_FACTOR;
	for(int i = 0; i < 16; i++)
	{
		pi = fixedMul(SCALING_FACTOR >> 1, fixedAdd(pi, fixedDiv(p, pi)));
		
	}
	
	printf("%d.%d\n", INT(pi), FRAC(pi));
	return pi;
}
