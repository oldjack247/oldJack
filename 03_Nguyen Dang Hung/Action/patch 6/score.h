#ifndef _SCORE_H_
#define _SCORE_H_
#include "main.h"

//マクロ定義
#define MAX_SCORE_POLYGON (8)			//スコアのポリゴンの最大数
#define SCORE_POLYGON_HEIGHT (60.0f)	//スコアのポリゴンの縦の大きさ
#define SCORE_POLYGON_WIDTH (30.0f)		//スコアのポリゴンの横の大きさ
#define SCORE_RESULT_POS_WIDTH (800.0f)	//リザルト画面でのポリゴンのX座標
#define SCORE_RESULT_POS_HEIGHT (320.0f)	//リザルト画面でのポリゴンのY座標

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
} Score;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
int GetScore(void);

#endif 
