#include "ship.h"
#include "lib/oled.h"
#include "lib/key.h"
#include "lib/frame.h"
#include "res/img.h"
#include "res/img2.h"
#include "bullet.h"


//---------------------------------------------------------------------------
const unsigned char* ShipExplosionList[] = {
	bitmapExplosion1_1,
	bitmapExplosion1_2,
	bitmapExplosion1_3,
	bitmapExplosion1_3,
	bitmapExplosion1_3,
	bitmapExplosion1_2,
	bitmapExplosion1_1,
	bitmapNone,
};

//---------------------------------------------------------------------------
ST_SHIP Ship;

//---------------------------------------------------------------------------
void ShipInit(void)
{
	_Memset(&Ship, 0x00, sizeof(ST_SHIP));

	Ship.fx = FT(OLED_SCREEN_CX/2 - SHIP_CX/2);
	Ship.fy = 0;
}
//---------------------------------------------------------------------------
void ShipExec(void)
{
	if(Ship.isEnd == TRUE)
	{
		return;
	}

	ShipMove();
	ShipFire();
}
//---------------------------------------------------------------------------
void ShipDraw(void)
{
	u8 n = (FrameGetCnt() & 0x40) ? 0 : 1;
	OledDrawLineH(FF(Ship.fy) + SHIP_CY - 1 + n);


	if(Ship.isEnd == FALSE)
	{
		OledDrawBmp(FF(Ship.fx), FF(Ship.fy) + n, bitmapShip);
	}
	else
	{
		OledDrawBmp(FF(Ship.fx), FF(Ship.fy) + n, ShipExplosionList[(Ship.cnt++ & 0x70) >> 4]);

		if(Ship.cnt >= 0x70)
		{
			Ship.cnt = 0x70;
		}
	}
}
//---------------------------------------------------------------------------
void ShipMove(void)
{
	u8 cnt = KeyGetCnt();

	if(cnt & KEY_R && FF(Ship.fx) <= OLED_SCREEN_CX - SHIP_CX - 2)
	{
		Ship.fx += 200;
	}

	if(cnt & KEY_L && FF(Ship.fx) >= 2)
	{
		Ship.fx -= 200;
	}

	Ship.isEnd = BulletEnemyHit(FF(Ship.fx)+4, FF(Ship.fy), SHIP_CX-4, SHIP_CY);
//	Ship.isEnd = FALSE;
}
//---------------------------------------------------------------------------
void ShipFire(void)
{
	u8 trg = KeyGetTrg();

	if(!(trg & KEY_A || trg & KEY_B))
	{
		return;
	}

	s16 fx;

	if(trg & KEY_A)
	{
		fx = Ship.fx + FT(SHIP_CX);
	}
	else
	{
		fx = Ship.fx - FT(BULLET1_CX);
	}

	BulletShipSet(fx, Ship.fy + (FrameGetCnt() & 0x40) ? 0 : FT(1));
}
//---------------------------------------------------------------------------
bool ShipIsEnd(void)
{
	return Ship.isEnd;
}
