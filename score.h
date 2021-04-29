#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//�}�N����`
#define MAX_SCORE_POLYGON (8)			//�X�R�A�̃|���S���̍ő吔
#define SCORE_POLYGON_HEIGHT (60.0f)	//�X�R�A�̃|���S���̏c�̑傫��
#define SCORE_POLYGON_WIDTH (30.0f)		//�X�R�A�̃|���S���̉��̑傫��
#define SCORE_RESULT_POS_WIDTH (800.0f)	//���U���g��ʂł̃|���S����X���W
#define SCORE_RESULT_POS_HEIGHT (320.0f)	//���U���g��ʂł̃|���S����Y���W

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
} Score;

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
int GetScore(void);

#endif 
