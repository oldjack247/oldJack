#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

//ライフの状態
typedef enum
{
	LIFE_NORMAL = 0,
	LIFE_DAMAGE,
	LIFE_MAX
}LIFE_STATE;

// 構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}LIFE;

// プロトタイプ宣言
HRESULT InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
#endif