#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"


// マクロ定義
#define MOVE_BULLET (5)				
#define BULLET_SIZE (20)			
#define MAX_BULLET (256)			
#define MAX_BULLET_TYPE (2)			
#define PLAYER_BULLET (1)
#define ENEMY_BULLET (0)

//弾の状態
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MACHINE,
	BULLETTYPE_MAX,
}BULLETTYPE;

// 構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nType;			//弾の種類
	BULLETTYPE Type;
	bool bUse;			//弾の使用状況
}BULLET;

// プロトタイプ宣言
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE Type);
void SetVertexBullet(int nIdx);

#endif