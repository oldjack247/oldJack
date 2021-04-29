#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "background.h"
#include "enemy.h"
#include "block.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "score.h"
#include "time.h"
#include "item.h"
#include <stdlib.h>
#include <time.h>

//グローバル変数
bool g_bPause = false;
int g_aMapData[MAX_LINE][MAX_COLUMN];

// 初期化処理
HRESULT InitGame(void)
{

	//背景の初期化
	InitBG();

	//ポリゴンの初期化処理
	InitPlayer();

	//ポーズの初期化処理
	InitPause();

	InitBlock();

	InitEnemy();

	InitScore();

	//InitTime();

	InitItem();

	//サウンドスタート
	//PlaySound(SOUND_LABEL_BGM003);

	//ロード処理
	FILE *pFile;

	pFile = fopen("data/STAGE/Stage2.csv", "r");
	int nCntX = 0, nCntY = 0;
	char aLine[1000];
	while (fgets(aLine, 1000, pFile) != NULL)
	{
		nCntX = 0;
		char *pToken = strtok(aLine, ",");
		while (pToken != NULL)
		{
			int nNum = atoi(pToken);
			g_aMapData[nCntY][nCntX] = nNum;
			pToken = strtok(NULL, ",");
			nCntX++;
		}
		nCntY++;
	}
	fclose(pFile);

	//ブロックの設置
	for (int nCntY = 0; nCntY < MAX_LINE; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_COLUMN; nCntX++)
		{
			if (g_aMapData[nCntY][nCntX] == 1)		//1*1ブロック
			{
				//D3DXVECTOR3 pos = D3DXVECTOR3(0.0f + 50.0f* nCntX, 0.0f + 50.0f * nCntY, 0);
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 40.0f, 1.0f, 1.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 2)
			{
				SetEnemy(D3DXVECTOR3(20.0f + 40.0f* nCntX, 40.0f + 40.0f * nCntY, 0.0f), 0, D3DXVECTOR3(-0.5f, 0.0f, 0.0f), 1, 40.0f, 40.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 3)
			{
				SetItem(D3DXVECTOR3(20.0f + 40.0f* nCntX, 40.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			}
			else if (g_aMapData[nCntY][nCntX] == 4)		//4*1ブロック
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 160.0f, 1.0f, 4.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 5)		//敵跳ね返す
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 40.0f, 40.0f, 1.0f, 1.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 6)		//6*1ブロック
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 240.0f, 1.0f, 6.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 7)		//特攻亀
			{
				SetEnemy(D3DXVECTOR3(20.0f + 40.0f* nCntX, 40.0f + 40.0f * nCntY, 0.0f), 0, D3DXVECTOR3(-2.0f, 0.0f, 0.0f), 1, 40.0f, 40.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 22)		//2*1ブロック
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 80.0f, 1.0f, 2.0f);
			}

		}
	}


	return S_OK;
}

// 終了処理
void UninitGame(void)
{


	//ポーズの終了処理
	UninitPause();

	//敵の終了処理
	//UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();

	UninitBlock();

	UninitEnemy();

	UninitScore();

	//UninitTime();

	UninitItem();

	//背景の終了
	UninitBG();
}

// 更新処理
void UpdateGame(void)
{
	FADE *pFade;
	pFade = GetFade();

	srand((unsigned int)time(NULL));

	if (GetKeyboardTrigger(DIK_P) == true)
	{
		if (*pFade == FADE_NONE)
		{
			g_bPause = g_bPause ? false : true;
		}
	}

	SetPause(g_bPause);

	if (g_bPause == true)
	{
		//ポーズの更新処理
		UpdatePause();
	}
	else if (g_bPause == false)
	{
		//背景の更新処理
		UpdateBG();

		//ポリゴンの更新処理
		UpdatePlayer();

		UpdateBlock();

		//UpdateTime();

		UpdateScore();

		UpdateItem();

		//敵の更新処理
		UpdateEnemy();

	}

}

// 描画処理
void DrawGame(void)
{
	//背景の描画
	DrawBG();

	//ポリゴンの描画処理
	DrawPlayer();

	//敵の描画処理
	DrawEnemy();

	DrawBlock();

	DrawItem();

	DrawScore();

//	DrawTime();


	if (g_bPause == true)
	{
		DrawPause();
	}

}

void SetPause(bool bpause)
{
	g_bPause = bpause;
}