#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//�}�N����`
#define MAX_RESULT (3)
#define RESULT_BG_X (960)
#define RESULT_BG_Y (540)
#define RESULT_SCREEN_X (700)
#define RESULT_SCREEN_Y (150)

typedef enum
{
	RESULT_CLEAR = 0,
	RESULT_GAMEOVER,
	RESULT_MAX
}RESULT_STATE;

// �v���g�^�C�v�錾
HRESULT InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);
void SetVertexResult(int nIdx);

#endif 
