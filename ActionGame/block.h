#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//マクロ定義
#define MAX_BLOCK (1000)			//ブロックの最大数
#define BLOCK_TYPE (2)			//ブロックの種類の数

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 PosOld;	//前回の位置のセーブ
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 SaveMove;	//移動量
	int nType;			//種類
	int nLife;			//体力
	float fTexU;		//テクスチャの縦の長さ
	float fTexV;		//テクスチャの横の長さ
	float fBlockWidth;	//ブロックの横幅
	float fBlockHeight;	//ブロックの縦幅
	bool bUse;			//使うかどうか
} BLOCK;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetMoveBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, int nLife, float fBlockWidth, float fBlockHeight, float fTexV, float fTexU);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pBeforePos, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
BLOCK *GetBlock(void);
BLOCK *GetBlock2(void);
BLOCK *GetBlock3(void);

#endif 

