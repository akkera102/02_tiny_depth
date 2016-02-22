#include "bullet.h"
#include "beep.h"
#include "score.h"
#include "lib/oled.h"
#include "lib/frame.h"
#include "res/img.h"
#include "res/img2.h"


//---------------------------------------------------------------------------
const unsigned char* BulletShipList[] = {
	bitmapBullet1_1,
	bitmapBullet1_2,
};

const unsigned char* BulletEnemyList[] = {
	bitmapBullet2_1,
	bitmapBullet2_2,
	bitmapBullet2_3,
};


//---------------------------------------------------------------------------
ST_BULLET       Bullet;
ST_BULLET_SHIP  BulletShip[BULLET_SHIP_MAX_CNT];
ST_BULLET_ENEMY BulletEnemy[BULLET_ENEMY_MAX_CNT];


//---------------------------------------------------------------------------
void BulletInit(void)
{
	_Memset(&Bullet, 0x00, sizeof(ST_BULLET));

	_Memset(&BulletShip, 0x00, sizeof(ST_BULLET_SHIP) * BULLET_SHIP_MAX_CNT);
	_Memset(&BulletEnemy, 0x00, sizeof(ST_BULLET_ENEMY) * BULLET_ENEMY_MAX_CNT);
}
//---------------------------------------------------------------------------
void BulletExec(void)
{
	BulletShipMove();
	BulletEnemyMove();
}
//---------------------------------------------------------------------------
void BulletDraw(void)
{
	BulletShipDraw();
	BulletEnemyDraw();
}
//---------------------------------------------------------------------------
void BulletShipMove(void)
{
	u8 i;

	for(i=0; i<BULLET_SHIP_MAX_CNT; i++)
	{
		if(BulletShip[i].isDraw == TRUE)
		{
			BulletShip[i].fy += 40;

			if(FF(BulletShip[i].fy) >= OLED_SCREEN_CY)
			{
				Bullet.shipCnt--;
				BulletShip[i].isDraw = FALSE;
			}
		}
	}
}
//---------------------------------------------------------------------------
void BulletShipDraw(void)
{
	u8 n = (FrameGetCnt() & 0x40) ? 0 : 1;
	u8 i;

	for(i=0; i<BULLET_SHIP_MAX_CNT; i++)
	{
		if(BulletShip[i].isDraw == TRUE)
		{
			OledDrawBmp(FF(BulletShip[i].fx), FF(BulletShip[i].fy), BulletShipList[n]);
		}
	}
}
//---------------------------------------------------------------------------
void BulletShipSet(s16 fx, s16 fy)
{
	u8 i;

	for(i=0; i<BULLET_SHIP_MAX_CNT; i++)
	{
		if(BulletShip[i].isDraw == FALSE)
		{
			BulletShip[i].fx = fx;
			BulletShip[i].fy = fy;
			BulletShip[i].isDraw = TRUE;

			Bullet.shipCnt++;
			BeepSeShipFire();
			return;
		}
	}
}
//---------------------------------------------------------------------------
bool BulletShipHit(s8 x, s8 y, s8 cx, s8 cy)
{
	u8 i;

	for(i=0; i<BULLET_SHIP_MAX_CNT; i++)
	{
		if(BulletShip[i].isDraw == TRUE)
		{
			if(BulletIsHit(FF(BulletShip[i].fx), FF(BulletShip[i].fy), BULLET1_CX, BULLET1_CY, x, y, cx, cy) == TRUE)
			{
				Bullet.shipCnt--;
				BulletShip[i].isDraw = FALSE;

				return TRUE;
			}
		}
	}

	return FALSE;
}
//---------------------------------------------------------------------------
void BulletEnemyMove(void)
{
	u8 i;

	for(i=0; i<BULLET_ENEMY_MAX_CNT; i++)
	{
		if(BulletEnemy[i].isDraw == TRUE)
		{
			BulletEnemy[i].fy -= BulletEnemy[i].fys;

			if(FF(BulletEnemy[i].fy) < 10)
			{
				Bullet.enemyCnt--;
				BulletEnemy[i].isDraw = FALSE;
			}
		}
	}
}
//---------------------------------------------------------------------------
void BulletEnemyDraw(void)
{
	u8 n = ((FrameGetCnt() & 0xf0) >> 4) % 3;
	u8 i;

	for(i=0; i<BULLET_ENEMY_MAX_CNT; i++)
	{
		if(BulletEnemy[i].isDraw == TRUE)
		{
			OledDrawBmp(FF(BulletEnemy[i].fx), FF(BulletEnemy[i].fy), BulletEnemyList[n]);
		}
	}
}
//---------------------------------------------------------------------------
void BulletEnemySet(s16 fx, s16 fy, s16 fys)
{
	u8 i;

	for(i=0; i<BULLET_ENEMY_MAX_CNT; i++)
	{
		if(BulletEnemy[i].isDraw == FALSE)
		{
			BulletEnemy[i].fx  = fx;
			BulletEnemy[i].fy  = fy;
			BulletEnemy[i].fys = fys;
			BulletEnemy[i].isDraw = TRUE;

			Bullet.enemyCnt++;
			return;
		}
	}
}
//---------------------------------------------------------------------------
bool BulletEnemyHit(s8 x, s8 y, s8 cx, s8 cy)
{
	u8 i;

	for(i=0; i<BULLET_ENEMY_MAX_CNT; i++)
	{
		if(BulletEnemy[i].isDraw == TRUE)
		{
			if(BulletIsHit(FF(BulletEnemy[i].fx), FF(BulletEnemy[i].fy), BULLET2_CX, BULLET2_CY, x, y, cx, cy) == TRUE)
			{
				Bullet.enemyCnt--;
				BulletEnemy[i].isDraw = FALSE;

				return TRUE;
			}
		}
	}

	return FALSE;
}
//---------------------------------------------------------------------------
bool BulletIsEnd(void)
{
	return (Bullet.enemyCnt == 0) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
bool BulletIsHit(s8 sx, s8 sy, s8 scx, s8 scy, s8 dx, s8 dy, s8 dcx, s8 dcy)
{
	if((sy < dy + dcy) && (dy < sy + scy))
	{
		if((sx < dx + dcx) && (dx < sx + scx))
		{
			return TRUE;
		}
	}

	return FALSE;
}
