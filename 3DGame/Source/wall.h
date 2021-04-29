#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//壁の構造体
typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bUse;
} Wall;

//プロトタイプ宣言
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

#endif