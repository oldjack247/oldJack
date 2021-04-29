#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//マクロ定義
#define MAX_LINE (18)			//ブロックの最大の行
#define MAX_COLUMN (246)		//ブロックの最大の列

//プロトタイプ宣言
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetPause(bool bPause);

#endif

