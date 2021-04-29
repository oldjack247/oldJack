#ifndef _POLYGON_H_
#define _POLYGON_H_
#include "main.h"

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitPolygon(void);		// 3Dポリゴンの初期化処理
void UninitPolygon(void);		// 3Dポリゴンの終了処理
void UpdatePolygon(void);		// 3Dポリゴンの更新処理
void DrawPolygon(void);			// 3Dポリゴンの描画処理

#endif
