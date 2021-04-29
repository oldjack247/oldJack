#ifndef _SUBPLAYER_H_
#define _SUBPLAYER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define SUBPLAYER_SIZE (20)
#define MOVE_SUBPLAYER (3)
#define MAX_SUBPLAYER (50) //子機の上限

//-----------------------------------------------------------------------------
// 状態
//-----------------------------------------------------------------------------
typedef enum
{
	SUBPLAYERSTATE_APPEAR = 0,
	SUBPLAYERSTATE_NORMAL,
	SUBPLAYERSTATE_BREAK,
	SUBPLAYERSTATE_MAX
}SUBPLAYERSTATE;

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posDest;	//目的の位置
	D3DXVECTOR3 move;		//移動速度
	bool bUse;
	SUBPLAYERSTATE State;
}SUBPLAYER;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitSubplayer(void);
void UninitSubplayer(void);
void UpdateSubplayer(void);
void DrawSubplayer(void);
SUBPLAYER* GetSubplayer(void);
void SetPosSubplayer(int nIdx, D3DXVECTOR3 pos);
void SetBulletSubplayer(int nIdx);
void SetSubplayer(D3DXVECTOR3 pos);
void SetVertexSubplayer(int nIdx);


#endif _SUBPLAYER_H_

