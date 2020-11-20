#pragma once
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

// マクロ定義
#define SCORE_SIZEX (30)	//横幅
#define SCORE_SIZEY (50)	//縦幅
#define MAX_SCORE (8)		//スコアの桁

// 構造体
typedef struct
{
	D3DXVECTOR3 pos;
}SCORE;

// プロトタイプ宣言
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SetTextureScore(int nIdx, int nNumber);

#endif _SCORE_H_

