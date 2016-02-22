#include "beep.h"
#include "lib/snd.h"
#include "res/mid.h"


//---------------------------------------------------------------------------
ST_BEEP Beep;

//---------------------------------------------------------------------------
void BeepInit(void)
{
	_Memset(&Beep, 0x00, sizeof(ST_BEEP));

	Beep.isSe = TRUE;
}
//---------------------------------------------------------------------------
void BeepBgmPlay(void)
{
	if(Beep.isBgm == FALSE)
	{
		return;
	}

	SndPlayScore(midiS1);
}
//---------------------------------------------------------------------------
void BeepBgmStop(void)
{
	SndStopScore();
}
//---------------------------------------------------------------------------
void BeepSePlay(u16 freq, u32 duration)
{
	if(Beep.isSe == FALSE)
	{
		return;
	}

	SndPlayTone(freq, duration);
}
//---------------------------------------------------------------------------
void BeepSeShipFire(void)
{
	BeepSePlay(330, 100);
}
//---------------------------------------------------------------------------
void BeepSeEnemyDestory(void)
{
	BeepSePlay(175, 100);
}
//---------------------------------------------------------------------------
void BeepSetBgm(bool is)
{
	Beep.isBgm = is;
}
//---------------------------------------------------------------------------
void BeepSetSe(bool is)
{
	Beep.isSe = is;
}
//---------------------------------------------------------------------------
bool BeepIsBgm(void)
{
	return Beep.isBgm;
}
//---------------------------------------------------------------------------
bool BeepIsSe(void)
{
	return Beep.isSe;
}
