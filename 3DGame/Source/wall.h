#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"

//�ǂ̍\����
typedef struct {
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bUse;
} Wall;

//�v���g�^�C�v�錾
HRESULT InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);

#endif