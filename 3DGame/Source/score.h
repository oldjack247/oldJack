#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_SCORE	(8)			// 桁の最大数

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitScore(void);					// スコアの初期化処理
void UninitScore(void);						// スコアの終了処理
void UpdateScore(void);						// スコアの更新処理
void DrawScore(void);						// スコアの描画処理
void AddScore(int nValue);					// スコアの加算処理
void SetVertexScore(int nIdx, int nNumber);	// スコアの座標情報取得
int GetScore(void);							// スコア情報の取得

#endif