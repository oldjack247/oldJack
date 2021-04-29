#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

// 弾の構造体
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
	BULLETTYPE_ENEMY,		//敵の弾
	BULLETTYPE_MAX
}BULLETTYPE;

typedef struct
{
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
	D3DXVECTOR3 pos;	// 位置
	D3DXVECTOR3 move;	// 移動方向
	float fmove;		// 移動力
						//D3DXVECTOR3 rot;	// 向き
	int nLife;			// 寿命
	BULLETTYPE type;
	bool bUse;			// 使用しているかどうか
} BULLET;

//==============================================================================
// プロトタイプ宣言
//==============================================================================
HRESULT InitBullet(void);											// 弾の初期化処理
void UninitBullet(void);											// 弾の終了処理
void UpdateBullet(void);											// 弾の更新処理
void DrawBullet(void);												// 弾の描画処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife);		// 弾の設定
void HitBullet(void);												// 弾の命中
void SetVertexBullet(int nIdx);										// 弾の頂点座標取得
BULLET *GetBullet(void);

#endif