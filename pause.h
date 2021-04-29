#ifndef _PAUSE_H_
#define _PAUSE_H_
#include "main.h"

//�}�N����`
#define MAX_PAUSE (5)							//�|�[�Y�̉摜�̐�
#define PAUSE_BG_SIZE_X (960)					//�|�[�Y��ʂ̉���
#define PAUSE_BG_SIZE_Y (540)					//�|�[�Y��ʂ̗���
#define PAUSE_MENU_SIZE_X (250)					//�|�[�Y���j���[�̂̉���
#define PAUSE_MENU_SIZE_Y (300)					//�|�[�Y���j���[�̗���
#define CONTINUE_SIZE_X (200)					//�R���e�j���[�̉���
#define CONTINUE_SIZE_Y (50)					//�R���e�j���[�̗���

//�|�[�Y���j���[
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	//�R���e�B�j���[
	PAUSE_MENU_RETRY,			//���g���C
	PAUSE_MENU_QUIT,			//�I��
	PAUSE_MENU_MAX
}PAUSE_MENU;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}PAUSE;

// �v���g�^�C�v�錾

HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetVertexPause(int nIdx);

#endif 
