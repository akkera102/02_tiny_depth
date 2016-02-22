#include "menu.h"
#include "lib/key.h"
#include "lib/oled.h"
#include "res/img.h"
#include "res/img2.h"
#include "beep.h"
#include "score.h"

//---------------------------------------------------------------------------
ST_MENU Menu;


//---------------------------------------------------------------------------
void MenuInit(void)
{
	_Memset(&Menu, 0x00, sizeof(ST_MENU));
}
//---------------------------------------------------------------------------
void MenuExec(void)
{
	u8 trg = KeyGetTrg();

	if(trg & KEY_U && Menu.cursor != 0)
	{
		Menu.cursor--;
	}

	if(trg & KEY_D && Menu.cursor < 2)
	{
		Menu.cursor++;
	}

	if(trg & KEY_A)
	{
		switch(Menu.cursor)
		{
		case 0:
			Menu.isEnd = TRUE;
			break;

		case 1:
			BeepSetBgm((BeepIsBgm() == TRUE) ? FALSE : TRUE);
			break;

		case 2:
			BeepSetSe((BeepIsSe() == TRUE) ? FALSE : TRUE);
			break;
		}
	}
}
//---------------------------------------------------------------------------
void MenuDraw(void)
{
	OledDrawBmp(OLED_SCREEN_CX/2 - TITLE_CX/2, 0, bitmapTitle);

	OledDrawChr(6, 3+Menu.cursor, '>');

	OledDrawStr(8, 3, "START");
	OledDrawStr(8, 4, "BGM %s", (BeepIsBgm() == TRUE) ? "ON" : "OFF");
	OledDrawStr(8, 5, "SE  %s", (BeepIsSe()  == TRUE) ? "ON" : "OFF");

	OledDrawStr(0, 7, "SCORE %d", ScoreGetHigh());
}
//---------------------------------------------------------------------------
bool MenuIsEnd(void)
{
	return Menu.isEnd;
}
