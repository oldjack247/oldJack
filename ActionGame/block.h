#ifndef _BLOCK_H_
#define _BLOCK_H_
#include "main.h"

//�}�N����`
#define MAX_BLOCK (1000)			//�u���b�N�̍ő吔
#define BLOCK_TYPE (2)			//�u���b�N�̎�ނ̐�

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 PosOld;	//�O��̈ʒu�̃Z�[�u
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 SaveMove;	//�ړ���
	int nType;			//���
	int nLife;			//�̗�
	float fTexU;		//�e�N�X�`���̏c�̒���
	float fTexV;		//�e�N�X�`���̉��̒���
	float fBlockWidth;	//�u���b�N�̉���
	float fBlockHeight;	//�u���b�N�̏c��
	bool bUse;			//�g�����ǂ���
} BLOCK;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetMoveBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, int nLife, float fBlockWidth, float fBlockHeight, float fTexV, float fTexU);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pBeforePos, D3DXVECTOR3 *pMove, float fWidth, float fHeight);
BLOCK *GetBlock(void);
BLOCK *GetBlock2(void);
BLOCK *GetBlock3(void);

#endif 

