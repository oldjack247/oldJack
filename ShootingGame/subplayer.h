#ifndef _SUBPLAYER_H_
#define _SUBPLAYER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define SUBPLAYER_SIZE (20)
#define MOVE_SUBPLAYER (3)
#define MAX_SUBPLAYER (50) //�q�@�̏��

//-----------------------------------------------------------------------------
// ���
//-----------------------------------------------------------------------------
typedef enum
{
	SUBPLAYERSTATE_APPEAR = 0,
	SUBPLAYERSTATE_NORMAL,
	SUBPLAYERSTATE_BREAK,
	SUBPLAYERSTATE_MAX
}SUBPLAYERSTATE;

//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posDest;	//�ړI�̈ʒu
	D3DXVECTOR3 move;		//�ړ����x
	bool bUse;
	SUBPLAYERSTATE State;
}SUBPLAYER;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
HRESULT InitSubplayer(void);
void UninitSubplayer(void);
void UpdateSubplayer(void);
void DrawSubplayer(void);
SUBPLAYER* GetSubplayer(void);
void SetPosSubplayer(int nIdx, D3DXVECTOR3 pos);
void SetBulletSubplayer(int nIdx);
void SetSubplayer(D3DXVECTOR3 pos);
void SetVertexSubplayer(int nIdx);


#endif _SUBPLAYER_H_

