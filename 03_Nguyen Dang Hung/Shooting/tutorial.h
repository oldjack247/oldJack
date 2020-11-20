#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	bool bDisp;

}TUTORIAL;

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------

HRESULT InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);
void SetVertexTutorial(int nIdx);

#endif