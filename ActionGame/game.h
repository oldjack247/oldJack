#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//�}�N����`
#define MAX_LINE (18)			//�u���b�N�̍ő�̍s
#define MAX_COLUMN (246)		//�u���b�N�̍ő�̗�

//�v���g�^�C�v�錾
HRESULT InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetPause(bool bPause);

#endif

