#ifndef BULLET_H
#define BULLET_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------
#define BULLET_SHIP_MAX_CNT					5
#define BULLET_ENEMY_MAX_CNT				10


//---------------------------------------------------------------------------
typedef struct {
	bool isDraw;
	s16  fx;
	s16  fy;
} ST_BULLET_SHIP;

typedef struct {
	bool isDraw;
	s16  fx;
	s16  fy;
	s16  fys;
} ST_BULLET_ENEMY;

typedef struct {
	u8 shipCnt;
	u8 enemyCnt;
} ST_BULLET;

//---------------------------------------------------------------------------
void BulletInit(void);
void BulletExec(void);
void BulletDraw(void);

void BulletShipMove(void);
void BulletShipDraw(void);
void BulletShipSet(s16 fx, s16 fy);
bool BulletShipHit(s8 x, s8 y, s8 cx, s8 cy);

void BulletEnemyMove(void);
void BulletEnemyDraw(void);
void BulletEnemySet(s16 fx, s16 fy, s16 fys);
bool BulletEnemyHit(s8 x, s8 y, s8 cx, s8 cy);

bool BulletIsEnd(void);
bool BulletIsHit(s8 sx, s8 sy, s8 scx, s8 scy, s8 dx, s8 dy, s8 dcx, s8 dcy);


#ifdef __cplusplus
}
#endif
#endif
