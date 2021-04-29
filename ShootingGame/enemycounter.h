#ifndef _ENEMYCOUNTER_H_
#define _ENEMYCOUNTER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define COUNTER_SIZEX (30)	//横幅
#define COUNTER_SIZEY (50)	//縦幅
#define MAX_COUNTER (2)		//スコアの桁
#define MAX_NUMBER (30)

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
}ENEMYCOUNTER;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitEnemyCounter(void);
void UninitEnemyCounter(void);
void UpdateEnemyCounter(void);
void DrawEnemyCounter(void);
void AddEnemyCounter(int EnemyCount);
void SetTextureEnemyCounter(int nIdx);

#endif _ENEMYCOUNTER_H_