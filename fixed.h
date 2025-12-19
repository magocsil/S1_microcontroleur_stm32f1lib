#ifndef STM32F1LIB_FIXED_H
#define STM32F1LIB_FIXED_H

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

#endif
