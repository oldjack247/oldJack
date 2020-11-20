#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"


// �}�N����`
#define MOVE_BULLET (5)				
#define BULLET_SIZE (20)			
#define MAX_BULLET (256)			
#define MAX_BULLET_TYPE (2)			
#define PLAYER_BULLET (1)
#define ENEMY_BULLET (0)

//�e�̏��
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MACHINE,
	BULLETTYPE_MAX,
}BULLETTYPE;

// �\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nType;			//�e�̎��
	BULLETTYPE Type;
	bool bUse;			//�e�̎g�p��
}BULLET;

// �v���g�^�C�v�錾
HRESULT InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE Type);
void SetVertexBullet(int nIdx);

#endif