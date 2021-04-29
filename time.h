#ifndef _TIME_H_
#define _TIME_H_
#include "main.h"

//マクロ定義
#define MAX_TIME_POLYGON (3)			//スコアのポリゴンの最大数
#define TIME_POLYGON_HEIGHT (70.0f)	//スコアのポリゴンの縦の大きさ
#define TIME_POLYGON_WIDTH (35.0f)		//スコアのポリゴンの横の大きさ

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
} Time;

//プロトタイプ宣言
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
void SetTime(int nTime);
//Time *GetTime(void)

#endif 