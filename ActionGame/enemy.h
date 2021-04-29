#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "main.h"

//�}�N����`
#define MAX_ENEMY (256)			//�G�̍ő吔
#define ENEMY_TYPE (1)			//�G�̎�ނ̐�

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 BeforePos;	//�O��̈ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nCounterAnim = 0;	//�A�j���[�V�����J�E���^
	int nPatternAnim = 0;	//�A�j���[�V�����p�^�[��No
	int nType;			//���
	int nLife;			//�̗�
	float fEnemyWidth;	//�G�̉���
	float fEnemyHeight;	//�G�̏c��
	bool bUse;			//�g�����ǂ���
} Enemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move, int nLife, float fEnemyWidth, float fEnemyHeight);
Enemy *GetEnemy(void);

#endif

