#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//=============================================================================
// �\���̂̒�`
//=============================================================================
typedef struct
{
	LPD3DXMESH pMeshEnemy;			//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMatEnemy;		//�}�e���A��(�ގ����)�ւ̃|�C���^
	DWORD nNumMatEnemy = 0;			//�}�e���A���̐�
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;				//���S���W
	D3DXVECTOR3 rot;				//��]�p
	D3DXVECTOR3 rotDest;			//�ړI�̉�]�p
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 moverot;			//��]��
	D3DXVECTOR3 vtxMinEnemy;		//���W�̍ŏ��l
	D3DXVECTOR3 vtxMaxEnemy;		//���W�̍ő�l
	int nShadowIdx;					//�e�p�̃C���f�b�N�X
	int aTexType[24];
	int nLife;						//�̗�
	int nIdx;						//���Ԗڂ̉e��
	bool bUse;						//�g�p���Ă��邩�ǂ���
}Enemy;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void MoveEnemy(int nCntEnemy);		//�G�̈ړ�
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�A�C�e���̐ݒu
void VecEnemy(int nCntEnemy);	//�A�C�e���̒��_���W
Enemy *GetEnemy(void);

bool HitEnemy(int nDamage);

#endif