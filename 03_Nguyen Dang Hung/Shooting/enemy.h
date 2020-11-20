#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

// マクロ定義
#define MAX_ENEMY_TYPE (3)
#define ENEMY_SIZE (65.0f)
#define MAX_ENEMY (256)			
#define PLAYER_HITAREA (5)

// 状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
}ENEMYSTATE;

// 構造体
typedef struct
{
	D3DXVECTOR3 pos;	
	D3DXVECTOR3 move;	
	int nType;			
	ENEMYSTATE State;	
	int nCounterState;	
	int nLife;			
	D3DXCOLOR col;		
	bool bUse;			
}ENEMY;

// プロトタイプ宣言
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, D3DXCOLOR col);
void SetVertexEnemy(int nIdx);
bool HitEnemy(int nIdx, int nDamage);
ENEMY*GetEnemy(void);

#endif
