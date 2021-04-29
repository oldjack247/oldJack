#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//マクロ定義
#define MAX_ENEMY (256)			//敵の最大数
#define ENEMY_TYPE (1)			//敵の種類の数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 BeforePos;	//前回の位置
	D3DXVECTOR3 move;	//移動量
	int nCounterAnim = 0;	//アニメーションカウンタ
	int nPatternAnim = 0;	//アニメーションパターンNo
	int nType;			//種類
	int nLife;			//体力
	float fEnemyWidth;	//敵の横幅
	float fEnemyHeight;	//敵の縦幅
	bool bUse;			//使うかどうか
} Enemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move, int nLife, float fEnemyWidth, float fEnemyHeight);
Enemy *GetEnemy(void);

#endif

