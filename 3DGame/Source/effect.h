#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

// �\���̂̒�`
typedef struct
{
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXCOLOR col;			//�F
	float fRadius;			//���a
	float fRemove;			//�ړ��ʂ̌�����
	float nCntAlpha;		//���l�̈ړ�
	float fGravity;			// �d��
	int nLife;				//����
	bool bUse;				//�g�p���Ă��邩�ǂ���
} EFFECT;

// �v���g�^�C�v�錾
HRESULT InitEffect(void);	// �G�t�F�N�g�̏���������
void UninitEffect(void);	// �G�t�F�N�g�̏I������
void UpdateEffect(void);	// �G�t�F�N�g�̍X�V����
void DrawEffect(void);		// �G�t�F�N�g�̕`�揈
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,
	float fRemove, float fRadius, float fCntAlpha, float fGravity);	// �G�t�F�N�g�̐ݒ�

#endif