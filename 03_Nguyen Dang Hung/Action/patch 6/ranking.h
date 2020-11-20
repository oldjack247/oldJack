#ifndef _RANKING_H_
#define _RANKING_H_
#include "main.h"

//マクロ定義
#define MAX_RANKING_POLYGON (8)			//スコアのポリゴンの最大数
#define MAX_RANKING (6)					//ランキングの最大数
#define RANKING_POLYGON_HEIGHT (60.0f)	//スコアのポリゴンの縦の大きさ
#define RANKING_POLYGON_WIDTH (45.0f)		//スコアのポリゴンの横の大きさ
#define RANKING_RESULT_POS_WIDTH (800.0f)	//リザルト画面でのポリゴンのX座標
#define RANKING_RESULT_POS_HEIGHT (320.0f)	//リザルト画面でのポリゴンのY座標

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int score;			//スコア
	bool bUse;			//使うかどうか
} Ranking;

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(int nRanking);
#endif 
