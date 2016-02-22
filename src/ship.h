#ifndef SHIP_H
#define SHIP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
typedef struct {
	s16  fx;
	s16  fy;
	bool isEnd;
	u8   cnt;
} ST_SHIP;

//---------------------------------------------------------------------------
void ShipInit(void);
void ShipExec(void);
void ShipDraw(void);

void ShipMove(void);
void ShipFire(void);
bool ShipIsEnd(void);


#ifdef __cplusplus
}
#endif
#endif
