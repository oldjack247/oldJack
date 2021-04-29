#ifndef _ENEMY_H_
#define _ENEMY_H_

#pragma once
#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define MAX_ENEMY_TYPE (3)
#define ENEMY_SIZE (50.0f)
#define BIG_ENEMY_SIZEX (70.0f)
#define BIG_ENEMY_SIZEY (60.0f)
#define MAX_ENEMY (256)				//�G�̏��
#define PLAYER_HITAREA (5)
#define ENEMY_COUNT (10)

//-----------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
}ENEMYSTATE;

//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�������x
	int nType;			//���
	ENEMYSTATE State;	//���
	int nCounterState;	//��ԃJ�E���^�[
	int nLife;			//�̗�
	D3DXCOLOR col;		//�F
	bool bUse;			//�g�p���
}ENEMY;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, D3DXCOLOR col);
void SetVertexEnemy(int nIdx);
bool HitEnemy(int nIdx, int nDamage);
int GetCount(void);
ENEMY* GetEnemy(void);

#endif _ENEMY_H_