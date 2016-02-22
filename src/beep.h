#ifndef BEEP_H
#define BEEP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
typedef struct {
	bool isBgm;
	bool isSe;
} ST_BEEP;


//---------------------------------------------------------------------------
void BeepInit(void);

void BeepBgmPlay(void);
void BeepBgmStop(void);

void BeepSePlay(u16 freq, u32 duration);
void BeepSeShipFire(void);
void BeepSeEnemyDestory(void);

void BeepSetBgm(bool is);
void BeepSetSe(bool is);
bool BeepIsBgm(void);
bool BeepIsSe(void);


#ifdef __cplusplus
}
#endif
#endif
