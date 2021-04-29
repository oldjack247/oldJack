#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

//==============================================================================
// フェードの状態
//==============================================================================
typedef enum
{
	FADE_NONE = 0,	// 何もしない
	FADE_IN,		// フェードイン
	FADE_OUT,		// フェードアウト
	FADE_MAX
}FADE;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitFade(MODE modeNext);		// フェード画面の初期化処理
void UninitFade(void);					// フェード画面の終了処理
void UpdateFade(void);					// フェード画面の更新処理
void DrawFade(void);					// フェード画面の描画処理
void SetFade(FADE fade, MODE modeNext);	// フェードの設定
FADE GetFade(void);						// フェード情報の取得

#endif