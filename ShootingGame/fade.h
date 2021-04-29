#ifndef _FADE_H_
#define _FADE_H_
#include "main.h"

// �}�N����`
#define MOVE_FADE (0.02f)	
#define MOVE_FADE_OUT (0.05f)


// �\����
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
}FADE;

// �v���g�^�C�v�錾
HRESULT InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(FADE fade, MODE modeNext);
FADE *GetFade(void);

#endif 
