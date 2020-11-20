#pragma once
#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

// �}�N����`
#define SCORE_SIZEX (30)	//����
#define SCORE_SIZEY (50)	//�c��
#define MAX_SCORE (8)		//�X�R�A�̌�

// �\����
typedef struct
{
	D3DXVECTOR3 pos;
}SCORE;

// �v���g�^�C�v�錾
HRESULT InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SetTextureScore(int nIdx, int nNumber);

#endif _SCORE_H_

