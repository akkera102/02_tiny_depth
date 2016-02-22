#ifndef GAME_H
#define GAME_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------
#define GAME_MAX_WAVE_CNT		10


//---------------------------------------------------------------------------

enum {
	GAME_ACT_RESET,
	GAME_ACT_MENU,
	GAME_ACT_EXEC,
	GAME_ACT_WAVE,
	GAME_ACT_OVER,
};

typedef struct {
	u8 act;
	u8 step;

} ST_GAME;



//---------------------------------------------------------------------------
void GameInit(void);
void GameLoop(void);

void GameReset(void);
void GameMenu(void);
void GameExec(void);
void GameWave(void);
void GameOver(void);

void GameSetAct(u8 act);


#ifdef __cplusplus
}
#endif
#endif
