#ifndef _TITLE_H
#define _TITLE_H
#include "main.h"

//マクロ定義
#define TITLE_LOGO_X		(700)
#define TITLE_LOGO_Y		(300)
#define TITLE_LOGO_SIZE_X	(1000)
#define TITLE_LOGO_SIZE_Y	(950)
#define TITLE_ENTER_X		(700)
#define TITLE_ENTER_Y		(450)

//Press Enter
typedef enum
{
	ENTER_NORMAL = 0,	//Normal state
	ENTER_PRESS,		//Pressing Enter
	ENTER_MAX
}ENTER;

// プロトタイプ宣言
HRESULT InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(void);
#endif