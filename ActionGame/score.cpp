#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include "main.h"
#include "score.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファへのポインタ
Score g_aScorePolygon[MAX_SCORE_POLYGON];			//スコア
int g_Score;										//スコア

//初期化処理
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//でアイスのポインタ
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//スコアの位置の初期化
	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		g_aScorePolygon[nCntScore].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore) + (SCORE_POLYGON_WIDTH / 2)), SCORE_POLYGON_HEIGHT / 2, 0.0f);
	}

	g_Score = 0;


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//score.png",
		&g_pTextureScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_POLYGON_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_POLYGON_HEIGHT, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

	}
	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();

}

//終了処理
void UninitScore(void)
{
	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//更新処理
void UpdateScore(void)
{
	int nCntScore;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		if (GetMode() == MODE_RESULT)
		{
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_RESULT_POS_HEIGHT - SCORE_POLYGON_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_RESULT_POS_HEIGHT - SCORE_POLYGON_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_RESULT_POS_HEIGHT + SCORE_POLYGON_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_RESULT_POS_HEIGHT + SCORE_POLYGON_HEIGHT / 2, 0.0f);
		}
		else
		{
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_POLYGON_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_POLYGON_HEIGHT, 0.0f);
		}

		if (nCntScore == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f + 0.1f, 1.0f);
		}
		else if (nCntScore == 7)
		{
			pVtx[0].tex = D3DXVECTOR2(g_Score % 10 * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Score % 10 * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Score % 10 * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Score % 10 * 0.1f + 0.1f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore) *0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore)*0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore)*0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore)*0.1f + 0.1f, 1.0f);
		}

		pVtx += 4;
	}


	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();

}

//描画処理
void DrawScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice;					//デバイスのポインタ
	pDevice = GetDevice();						//デバイスを取得する
												//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定

	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		//テクスチャ
		pDevice->SetTexture(0, g_pTextureScore);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
			nCntScore * 4,					//描画を開始する頂点のインデックス
			2);					//描画するプリミティブ数
	}

}

//設定処理
void SetScore(int nScore)
{
	g_Score += nScore;
}

//取得処理
int GetScore(void)
{
	if (GetMode() == MODE_TITLE)
	{
		g_Score = 0;
	}

	return g_Score;
}
