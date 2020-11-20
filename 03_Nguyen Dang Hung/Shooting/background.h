#ifndef _BG_H_
#define _BG_H_

#include "main.h"

// マクロ定義
#define MAX_BG	(3)	
#define MOVE_BG_A	(0.001f)
#define MOVE_BG_B	(0.002f)
#define MOVE_BG_C	(0.003f)

//プロトタイプ宣言
HRESULT InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif