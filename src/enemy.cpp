#include "enemy.h"
#include "lib/oled.h"
#include "lib/math.h"
#include "res/img.h"
#include "res/img2.h"
#include "bullet.h"
#include "beep.h"
#include "score.h"

//---------------------------------------------------------------------------
const unsigned char* EnemyChrList[] = {
	bitmapEnemy1_1,
	bitmapEnemy1_2,
	bitmapEnemy2_1,
	bitmapEnemy2_2,
	bitmapEnemy3_1,
	bitmapEnemy3_2,
	bitmapEnemy4_1,
	bitmapEnemy4_2,
};

//---------------------------------------------------------------------------
ST_ENEMY Enemy;


//---------------------------------------------------------------------------
void EnemyInit(void)
{
	_Memset(&Enemy, 0x00, sizeof(ST_ENEMY));
}
//---------------------------------------------------------------------------
void EnemyExec(void)
{
	EnemyMove();
	EnemyFire();

	EnemyMake();
}
//---------------------------------------------------------------------------
void EnemyDraw(void)
{
	u8 i;

	for(i=0; i<ENEMY_MAX_CHR_CNT; i++)
	{
		if(Enemy.c[i].isDraw == TRUE)
		{
			OledDrawBmp(FF(Enemy.c[i].fx), FF(Enemy.c[i].fy), EnemyChrList[Enemy.c[i].type]);
		}
	}
}
//---------------------------------------------------------------------------
void EnemyMove(void)
{
	u8 i;

	for(i=0; i<ENEMY_MAX_CHR_CNT; i++)
	{
		ST_ENEMY_CHR* p = &Enemy.c[i];

		if(p->isDraw == FALSE)
		{
			continue;
		}

		if(p->type == ENEMY_TYPE_4_1 || p->type == ENEMY_TYPE_4_2)
		{
			p->fy += MathSin(p->val++);
		}
		p->fx += p->fxs;


		if(FF(p->fx) <= -25 || FF(p->fx) >= OLED_SCREEN_CX)
		{
			p->isDraw = FALSE;
			Enemy.chrCnt--;
			continue;
		}

		if(BulletShipHit(FF(p->fx), FF(p->fy), p->cx, p->cy) == TRUE)
		{
			p->isDraw = FALSE;
			Enemy.chrCnt--;

			ScoreAddNow((p->type >> 1) + 1);
			BeepSeEnemyDestory();
			continue;
		}
	}
}
//---------------------------------------------------------------------------
void EnemyFire(void)
{
	s16 fx, fys;
	u8  i;

	for(i=0; i<ENEMY_MAX_CHR_CNT; i++)
	{
		ST_ENEMY_CHR* p = &Enemy.c[i];

		if(p->isDraw == FALSE)
		{
			continue;
		}

		if(p->type == ENEMY_TYPE_4_1 || p->type == ENEMY_TYPE_4_2)
		{
			continue;
		}

		if(MathRnd() % (Enemy.rnd * 8) != 0)
		{
			continue;
		}

		if(p->type == ENEMY_TYPE_3_1 || p->type == ENEMY_TYPE_3_2)
		{
			fys = 20 + Enemy.wave * 2;
		}
		else
		{
			fys = 40 + Enemy.wave * 2;
		}

		fx = p->fx + FT(p->cx/2);

		if(fx >= FT(5) && fx < FT(OLED_SCREEN_CX-5))
		{
			BulletEnemySet(fx, p->fy, fys);
		}
	}
}
//---------------------------------------------------------------------------
void EnemyMake(void)
{
	if(Enemy.make == 0)
	{
		return;
	}

	if(Enemy.wait != 0)
	{
		Enemy.wait--;
		return;
	}

	if(MathRnd() % Enemy.rnd != 0)
	{
		return;
	}


	u8 i;

	for(i=0; i<ENEMY_MAX_CHR_CNT; i++)
	{
		if(Enemy.c[i].isDraw == FALSE)
		{
			break;
		}
	}

	if(i >= ENEMY_MAX_CHR_CNT)
	{
		return;
	}


	ST_ENEMY_CHR* p = &Enemy.c[i];

	switch(MathRnd() % 18)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		p->type = ENEMY_TYPE_1_1;
		p->fx   = FT(-ENEMY1_CX + 1);
		p->fy   = FT(MathRnd() % 32 + 26);
		p->fxs  = 50 + Enemy.wave * 10;
		p->cx   = ENEMY1_CX;
		p->cy   = ENEMY1_CY;
		break;

	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		p->type = ENEMY_TYPE_1_2;
		p->fx   = FT(OLED_SCREEN_CX - 1);
		p->fy   = FT(MathRnd() % 32 + 26);
		p->fxs  = -50 - Enemy.wave * 10;
		p->cx   = ENEMY1_CX;
		p->cy   = ENEMY1_CY;
		break;

	case 10:
	case 11:
		p->type = ENEMY_TYPE_2_1;
		p->fx   = FT(-ENEMY2_CX + 1);
		p->fy   = FT(MathRnd() % 12 + 46);
		p->fxs  = 50 + Enemy.wave * 10;
		p->cx   = ENEMY2_CX;
		p->cy   = ENEMY2_CY;
		break;

	case 12:
	case 13:
		p->type = ENEMY_TYPE_2_2;
		p->fx   = FT(OLED_SCREEN_CX - 1);
		p->fy   = FT(MathRnd() % 12 + 46);
		p->fxs  = -50 - Enemy.wave * 10;
		p->cx   = ENEMY2_CX;
		p->cy   = ENEMY2_CY;
		break;

	case 14:
		p->type = ENEMY_TYPE_3_1;
		p->fx   = FT(-ENEMY3_CX + 1);
		p->fy   = FT(MathRnd() % 5 + 25);
		p->fxs  = 220 + Enemy.wave * 10;
		p->cx   = ENEMY3_CX;
		p->cy   = ENEMY3_CY;
		break;

	case 15:
		p->type = ENEMY_TYPE_3_2;
		p->fx   = FT(-OLED_SCREEN_CX - 1);
		p->fy   = FT(MathRnd() % 5 + 25);
		p->fxs  = -220 - Enemy.wave * 10;
		p->cx   = ENEMY3_CX;
		p->cy   = ENEMY3_CY;
		break;

	case 16:
		p->type = ENEMY_TYPE_4_1;
		p->fx   = FT(-ENEMY4_CX + 1);
		p->fy   = FT(20);
		p->fxs  = 40;
		p->cx   = ENEMY4_CX;
		p->cy   = ENEMY4_CY;
		p->val  = 0;
		break;

	case 17:
		p->type = ENEMY_TYPE_4_2;
		p->fx   = FT(-OLED_SCREEN_CX - 1);
		p->fy   = FT(20);
		p->fxs  = -40;
		p->cx   = ENEMY4_CX;
		p->cy   = ENEMY4_CY;
		p->val  = 0;
		break;
	}
	p->isDraw = TRUE;
	Enemy.chrCnt++;

	Enemy.make--;
	Enemy.wait = 5;
}
//---------------------------------------------------------------------------
void EnemySetWaveNext(void)
{
	Enemy.wave++;

	Enemy.make = 10  + Enemy.wave * 4;
	Enemy.rnd  = 150 - Enemy.wave * 10;

	if(Enemy.make > 100)
	{
		Enemy.make = 100;
	}

	if(Enemy.rnd < 50)
	{
		Enemy.rnd = 50;
	}
}
//---------------------------------------------------------------------------
u8 EnemyGetWave(void)
{
	return Enemy.wave;
}
//---------------------------------------------------------------------------
bool EnemyIsEnd(void)
{
	return (Enemy.make == 0 && Enemy.chrCnt == 0) ? TRUE : FALSE;
}
//---------------------------------------------------------------------------
void EnemyDecRnd(void)
{
	Enemy.rnd -= 20;

	if(Enemy.rnd < 30)
	{
		Enemy.rnd = 30;
	}
}
