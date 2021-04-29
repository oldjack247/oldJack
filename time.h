#ifndef _TIME_H_
#define _TIME_H_
#include "main.h"

//�}�N����`
#define MAX_TIME_POLYGON (3)			//�X�R�A�̃|���S���̍ő吔
#define TIME_POLYGON_HEIGHT (70.0f)	//�X�R�A�̃|���S���̏c�̑傫��
#define TIME_POLYGON_WIDTH (35.0f)		//�X�R�A�̃|���S���̉��̑傫��

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
} Time;

//�v���g�^�C�v�錾
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
//Time *GetTime(void)

#endif 