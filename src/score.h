#ifndef SCORE_H
#define SCORE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lib/common.h"

//---------------------------------------------------------------------------

typedef struct {
	u16 high;
	u16 now;

} ST_SCORE;


//---------------------------------------------------------------------------
void ScoreInit(void);
void ScoreSave(void);
void ScoreLoad(void);

u16  ScoreGetHigh(void);
void ScoreAddNow(u16 num);
u16  ScoreGetNow(void);


#ifdef __cplusplus
}
#endif
#endif
