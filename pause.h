#ifndef _PAUSE_H_
#define _PAUSE_H_
#include "main.h"

//マクロ定義
#define MAX_PAUSE (5)							//ポーズの画像の数
#define PAUSE_BG_SIZE_X (960)					//ポーズ画面の横幅
#define PAUSE_BG_SIZE_Y (540)					//ポーズ画面の立幅
#define PAUSE_MENU_SIZE_X (250)					//ポーズメニューのの横幅
#define PAUSE_MENU_SIZE_Y (300)					//ポーズメニューの立幅
#define CONTINUE_SIZE_X (200)					//コンテニューの横幅
#define CONTINUE_SIZE_Y (50)					//コンテニューの立幅

//ポーズメニュー
typedef enum
{
	PAUSE_MENU_CONTINUE = 0,	//コンティニュー
	PAUSE_MENU_RETRY,			//リトライ
	PAUSE_MENU_QUIT,			//終了
	PAUSE_MENU_MAX
}PAUSE_MENU;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR col;
}PAUSE;

// プロトタイプ宣言

HRESULT InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void SetVertexPause(int nIdx);

#endif 
