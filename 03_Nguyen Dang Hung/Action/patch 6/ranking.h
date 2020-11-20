#ifndef _RANKING_H_
#define _RANKING_H_
#include "main.h"

//�}�N����`
#define MAX_RANKING_POLYGON (8)			//�X�R�A�̃|���S���̍ő吔
#define MAX_RANKING (6)					//�����L���O�̍ő吔
#define RANKING_POLYGON_HEIGHT (60.0f)	//�X�R�A�̃|���S���̏c�̑傫��
#define RANKING_POLYGON_WIDTH (45.0f)		//�X�R�A�̃|���S���̉��̑傫��
#define RANKING_RESULT_POS_WIDTH (800.0f)	//���U���g��ʂł̃|���S����X���W
#define RANKING_RESULT_POS_HEIGHT (320.0f)	//���U���g��ʂł̃|���S����Y���W

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int score;			//�X�R�A
	bool bUse;			//�g�����ǂ���
} Ranking;

//�v���g�^�C�v�錾
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(int nRanking);
#endif 
