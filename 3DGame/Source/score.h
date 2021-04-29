#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//==============================================================================
// �}�N����`
//==============================================================================
#define MAX_SCORE	(8)			// ���̍ő吔

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
HRESULT InitScore(void);					// �X�R�A�̏���������
void UninitScore(void);						// �X�R�A�̏I������
void UpdateScore(void);						// �X�R�A�̍X�V����
void DrawScore(void);						// �X�R�A�̕`�揈��
void AddScore(int nValue);					// �X�R�A�̉��Z����
void SetVertexScore(int nIdx, int nNumber);	// �X�R�A�̍��W���擾
int GetScore(void);							// �X�R�A���̎擾

#endif