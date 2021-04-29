#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
//-----------------------------------------------------------------------------
// マクロ定義
//---------------------------------------------------------------------------
#define MOVE_BULLET (5)				//弾の速度
#define BULLET_SIZE (10)			//弾の大きさ
#define MAX_BULLET (256)			//弾の上限
#define MAX_BULLET_TYPE (2)			//弾の種類
#define PLAYER_BULLET (1)
#define ENEMY_BULLET (0)
//弾の状態
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_SUBPLAYER,
	BULLETTYPE_MAX,
}BULLETTYPE;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE Type);
void SetVertexBullet(int nIdx);

#endif _BULLET_H_