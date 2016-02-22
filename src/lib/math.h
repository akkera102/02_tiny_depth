#ifndef MATH_H
#define MATH_H
#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"


//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void MathInit(void);

s8   MathSin(u8 deg);
s8   MathCos(u8 deg);

void MathRndSeed(u32 seed);
u32  MathRnd(void);


#ifdef __cplusplus
}
#endif
#endif
