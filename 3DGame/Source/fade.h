#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

//==============================================================================
// �t�F�[�h�̏��
//==============================================================================
typedef enum
{
	FADE_NONE = 0,	// �������Ȃ�
	FADE_IN,		// �t�F�[�h�C��
	FADE_OUT,		// �t�F�[�h�A�E�g
	FADE_MAX
}FADE;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitFade(MODE modeNext);		// �t�F�[�h��ʂ̏���������
void UninitFade(void);					// �t�F�[�h��ʂ̏I������
void UpdateFade(void);					// �t�F�[�h��ʂ̍X�V����
void DrawFade(void);					// �t�F�[�h��ʂ̕`�揈��
void SetFade(FADE fade, MODE modeNext);	// �t�F�[�h�̐ݒ�
FADE GetFade(void);						// �t�F�[�h���̎擾

#endif