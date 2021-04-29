#pragma once
//-----------------------------------------------------
//Author:Nguyen Dang Hung
//-----------------------------------------------------

#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//-----------------------------------------------------------------------------
// �}�N����`
//-----------------------------------------------------------------------------
#define MOVE_NUMBER (4)		//�ړ���
#define MOVE_SPEED (10)		//
#define PLAYER_SIZEX (40)	//�v���C���[�̕�
#define PLAYER_SIZEY (40)	//�v���C���[�̍���
#define SUBPLAYER_HAVE (3)		//�q�@�̎��Ă���

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,	//�o��
	PLAYERSTATE_NORMAL,		//�ʏ�
	PLAYERSTATE_DAMAGE,		//�_���[�W
	PLAYERSTATE_WAIT,		//�o���҂�
	PLAYERSTATE_DEATH,		//���S
	PLAYERSTATE_MAX,
}PLAYERSTATE;

//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	PLAYERSTATE State;	//���
	int nCounterState;	//��ԃJ�E���^�[
	int nCounterDisp;	//�\���J�E���^
	int nLife;			//�̗�
	int nRemaining;		//�c�@
	D3DXCOLOR col;		//�F
	bool bDisp;			//�\��ON/OFF
}PLAYER;

//
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER* GetPlayer(void);
bool HitPlayer(int nDamage);
void SetVertexPlayer(void);


#endif
