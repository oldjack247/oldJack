#ifndef _ITEM_H_
#define _ITEM_H_
#include "main.h"

//�}�N����`
#define MAX_ITEM (1000)				//�A�C�e���̍ő吔
#define ITEM_TYPE (1)				//�A�C�e���̎��
#define ITEM_HEIGHT (40.0f)		//�A�C�e���̏c�̑傫��
#define ITEM_WIDTH (40.0f)		//�A�C�e���̑傫��

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nType;			//���
	bool bUse;			//�g�����ǂ���
} Item;

//�v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, int ntype);
Item *GetItem(void);

#endif