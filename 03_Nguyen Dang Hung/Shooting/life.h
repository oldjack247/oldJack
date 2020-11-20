#ifndef _LIFE_H_
#define _LIFE_H_

#include "main.h"

//���C�t�̏��
typedef enum
{
	LIFE_NORMAL = 0,
	LIFE_DAMAGE,
	LIFE_MAX
}LIFE_STATE;

// �\����
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}LIFE;

// �v���g�^�C�v�錾
HRESULT InitLife(void);
void UninitLife(void);
void UpdateLife(void);
void DrawLife(void);
#endif