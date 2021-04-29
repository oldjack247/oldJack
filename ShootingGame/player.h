#pragma once
//-----------------------------------------------------
//Author:Nguyen Dang Hung
//-----------------------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MOVE_NUMBER (4)		//移動量
#define MOVE_SPEED (10)		//
#define PLAYER_SIZEX (40)	//プレイヤーの幅
#define PLAYER_SIZEY (40)	//プレイヤーの高さ
#define SUBPLAYER_HAVE (3)		//子機の持てる上限

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//出現
	PLAYERSTATE_NORMAL,		//通常
	PLAYERSTATE_DAMAGE,		//ダメージ
	PLAYERSTATE_WAIT,		//出現待ち
	PLAYERSTATE_DEATH,		//死亡
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	PLAYERSTATE State;	//状態
	int nCounterState;	//状態カウンター
	int nCounterDisp;	//表示カウンタ
	int nLife;			//体力
	int nRemaining;		//残機
	D3DXCOLOR col;		//色
	bool bDisp;			//表示ON/OFF
}PLAYER;

//
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
bool HitPlayer(int nDamage);
void SetVertexPlayer(void);


#endif
