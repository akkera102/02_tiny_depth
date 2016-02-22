#include "game.h"
#include "lib/frame.h"
#include "lib/oled.h"
#include "menu.h"
#include "ship.h"
#include "enemy.h"
#include "beep.h"
#include "score.h"
#include "bullet.h"

//---------------------------------------------------------------------------
ST_GAME Game;


//---------------------------------------------------------------------------
void GameInit(void)
{
	_Memset(&Game, 0x00, sizeof(ST_GAME));
}
//---------------------------------------------------------------------------
void GameLoop(void)
{
	switch(Game.act)
	{
		case GAME_ACT_RESET: GameReset(); break;
		case GAME_ACT_MENU:  GameMenu();  break;
		case GAME_ACT_EXEC:  GameExec();  break;
		case GAME_ACT_WAVE:  GameWave();  break;
		case GAME_ACT_OVER:  GameOver();  break;

		default:
			SystemError("[Game act=%x]", Game.act);
	}


//	OledDrawStr(0, 0, "%03d", FrameGetCpuPercentMax());
//	OledDrawStr(0, 0, "%03d %x", FrameGetCpuPercentMax(), FrameGetCnt());
}
//---------------------------------------------------------------------------
void GameReset(void)
{
	MenuInit();
	BeepInit();
	ScoreInit();

	GameSetAct(GAME_ACT_MENU);
}
//---------------------------------------------------------------------------
void GameMenu(void)
{
	MenuExec();
	MenuDraw();

	if(MenuIsEnd() == TRUE)
	{
		ShipInit();
		EnemyInit();
		BulletInit();

		BeepBgmPlay();

		EnemySetWaveNext();
		GameSetAct(GAME_ACT_WAVE);
	}
}
//---------------------------------------------------------------------------
void GameExec(void)
{
	ShipDraw();
	EnemyDraw();
	BulletDraw();

	ShipExec();
	EnemyExec();
	BulletExec();

	if(++Game.step == 255)
	{
		EnemyDecRnd();
	}

	if(ShipIsEnd() == TRUE)
	{
		GameSetAct(GAME_ACT_OVER);
	}
	else if(EnemyIsEnd() == TRUE && BulletIsEnd() == TRUE)
	{
		if(EnemyGetWave() >= GAME_MAX_WAVE_CNT)
		{
			GameSetAct(GAME_ACT_OVER);
		}
		else
		{
			EnemySetWaveNext();
			GameSetAct(GAME_ACT_WAVE);
		}
	}
}
//---------------------------------------------------------------------------
void GameWave(void)
{
	ShipDraw();
	BulletDraw();

	ShipExec();
	BulletExec();

	if(Game.step / 18 % 2 == 0)
	{
		OledDrawStr(7, 4, "WAVE  %d", EnemyGetWave());
		OledDrawStr(7, 5, "SCORE %d", ScoreGetNow());
	}

	if(++Game.step >= 126)
	{
		GameSetAct(GAME_ACT_EXEC);
	}
}
//---------------------------------------------------------------------------
void GameOver(void)
{
	ShipDraw();
	EnemyDraw();
	BulletDraw();

	ShipExec();
	EnemyExec();
	BulletExec();


	if(EnemyGetWave() >= GAME_MAX_WAVE_CNT)
	{
		OledDrawStr(7, 4, "COMPLETE!");
	}
	else
	{
		OledDrawStr(7, 4, "GAME OVER");
	}
	OledDrawStr(7, 5, "SCORE %d", ScoreGetNow());


	if(++Game.step >= 250)
	{
		ScoreSave();
		GameSetAct(GAME_ACT_RESET);
	}
}
//---------------------------------------------------------------------------
void GameSetAct(u8 act)
{
	Game.act  = act;
	Game.step = 0;
}
