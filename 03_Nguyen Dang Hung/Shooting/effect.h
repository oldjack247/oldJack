#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "main.h"

// �}�N����`	
#define EFFECT_SIZE (17)			
#define MAX_EFFECT (10000)		

// �\����
typedef struct
{
	D3DXVECTOR3 pos;						//�ʒu
	D3DXCOLOR col;
	bool bUse;								//�����̎g�p��
}EFFECT;

// �v���g�^�C�v�錾
HRESULT InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col);
void SetVertexEffect(int nIdx);
EFFECT *GetEffect(void);

#endif
