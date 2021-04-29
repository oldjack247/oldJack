#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"

//==============================================================================
// マクロ定義
//==============================================================================
#define MAX_ITEM		(128)		// アイテムの最大数
#define MAX_TYPE_ITEM	(3)			// アイテムの種類
#define ITEM_SIZEX		(15)		// アイテムの幅
#define ITEM_SIZEY		(30)		// アイテムの高さ

//==============================================================================
// アイテムの構造体
//==============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXCOLOR col;		// 色
	int nType;			// 種類
	bool bUse;			// 使用しているかどうか
	D3DXVECTOR3 vtxMinObject, vtxMaxObject;	// モデルの各座標の最大値・最小値
	//float fAnim;		// テクスチャアニメーションカット数
	//bool bHiScore;		// 高スコア状態のコイン
} ITEM;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitItem(void);						// アイテムの初期化処理
void UninitItem(void);						// アイテムの終了処理
void UpdateItem(void);						// アイテムの更新処理
void DrawItem(void);						// アイテムの描画処理
void SetItem(D3DXVECTOR3 pos, int nType);	// アイテムの配置
void SetVertexItem(int nIdx);				// アイテムの頂点座標取得
ITEM *GetItem(void);						// アイテムの情報
int RandItem(void);						// ランダムでハイスコアアイテムの選定

#endif
