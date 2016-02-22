#ifndef MENU_H
#define MENU_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------

typedef struct {
	u8   cursor;
	bool isEnd;

} ST_MENU;



//---------------------------------------------------------------------------
void MenuInit(void);
void MenuExec(void);
void MenuDraw(void);

bool MenuIsEnd(void);


#ifdef __cplusplus
}
#endif
#endif
