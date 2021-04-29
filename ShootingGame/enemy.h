#ifndef _ENEMY_H_
#define _ENEMY_H_

#pragma once
#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_ENEMY_TYPE (3)
#define ENEMY_SIZE (50.0f)
#define BIG_ENEMY_SIZEX (70.0f)
#define BIG_ENEMY_SIZEY (60.0f)
#define MAX_ENEMY (256)				//敵の上限
#define PLAYER_HITAREA (5)
#define ENEMY_COUNT (10)

//-----------------------------------------------------------------------------
// 状態
//-----------------------------------------------------------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
}ENEMYSTATE;

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//動く速度
	int nType;			//種類
	ENEMYSTATE State;	//状態
	int nCounterState;	//状態カウンター
	int nLife;			//体力
	D3DXCOLOR col;		//色
	bool bUse;			//使用状態
}ENEMY;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, D3DXCOLOR col);
void SetVertexEnemy(int nIdx);
bool HitEnemy(int nIdx, int nDamage);
int GetCount(void);
ENEMY* GetEnemy(void);

#endif _ENEMY_H_