#ifndef _ENEMYCOUNTER_H_
#define _ENEMYCOUNTER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define COUNTER_SIZEX (30)	//����
#define COUNTER_SIZEY (50)	//�c��
#define MAX_COUNTER (2)		//�X�R�A�̌�
#define MAX_NUMBER (30)

//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;
}ENEMYCOUNTER;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitEnemyCounter(void);
void UninitEnemyCounter(void);
void UpdateEnemyCounter(void);
void DrawEnemyCounter(void);
void AddEnemyCounter(int EnemyCount);
void SetTextureEnemyCounter(int nIdx);

#endif _ENEMYCOUNTER_H_