#include "result.h"
#include "input.h"
#include "fade.h"
#include "player.h"

//構造体
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR  col;
}RESULT;


//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;			//頂点情報
LPDIRECT3DTEXTURE9 g_pTextureResult[MAX_RESULT] = {};		//テクスチャ
RESULT g_aResult[MAX_RESULT];
int g_StateResult;

// 初期化処理
HRESULT InitResult(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTextureResult[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameclear_logo.png", &g_pTextureResult[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/gameover_logo.png", &g_pTextureResult[2]);

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		g_aResult[nCntResult].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aResult[nCntResult].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	g_StateResult = RESULT_CLEAR;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RESULT, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffResult, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++, pVtx += 4)
	{

		SetVertexResult(nCntResult);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		pVtx[0].col = g_aResult[nCntResult].col;
		pVtx[1].col = g_aResult[nCntResult].col;
		pVtx[2].col = g_aResult[nCntResult].col;
		pVtx[3].col = g_aResult[nCntResult].col;

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffResult->Unlock();
	}
	return S_OK;

}

// 終了処理
void UninitResult(void)
{
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
}

// 更新処理
void UpdateResult(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	if (pPlayer->nRemaining <= 0)
	{
		g_StateResult = RESULT_GAMEOVER;
	}
	else
	{
		g_StateResult = RESULT_CLEAR;
	}


	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		switch (g_StateResult)
		{
		case RESULT_CLEAR:

			g_aResult[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_aResult[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aResult[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			break;

		case RESULT_GAMEOVER:

			g_aResult[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

			g_aResult[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			g_aResult[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			break;
		}

		//頂点のカラー設定
		pVtx[0].col = g_aResult[nCntResult].col;
		pVtx[1].col = g_aResult[nCntResult].col;
		pVtx[2].col = g_aResult[nCntResult].col;
		pVtx[3].col = g_aResult[nCntResult].col;

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	FADE *pFade;
	pFade = GetFade();

	if (GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
	{
		SetFade(FADE_OUT, MODE_TITLE);
	}
}

// 描画処理
void DrawResult(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntResult = 0; nCntResult < MAX_RESULT; nCntResult++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[nCntResult]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntResult * 4, 2);
	}
}

//  リザルトの頂点座標の設定
void SetVertexResult(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	if (nIdx == 0)
	{
		g_aResult[nIdx].pos = D3DXVECTOR3(960.0f, 540.0f, 0.0f);

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x - RESULT_BG_X, g_aResult[nIdx].pos.y + RESULT_BG_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x - RESULT_BG_X, g_aResult[nIdx].pos.y - RESULT_BG_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x + RESULT_BG_X, g_aResult[nIdx].pos.y + RESULT_BG_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x + RESULT_BG_X, g_aResult[nIdx].pos.y - RESULT_BG_Y, 0.0f);

		//頂点のカラー設定
		g_aResult[nIdx].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	}
	else if (nIdx == 1)
	{
		g_aResult[nIdx].pos = D3DXVECTOR3(960.0f, 450.0f, 0.0f);

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x - RESULT_SCREEN_X, g_aResult[nIdx].pos.y + RESULT_SCREEN_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x - RESULT_SCREEN_X, g_aResult[nIdx].pos.y - RESULT_SCREEN_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x + RESULT_SCREEN_X, g_aResult[nIdx].pos.y + RESULT_SCREEN_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x + RESULT_SCREEN_X, g_aResult[nIdx].pos.y - RESULT_SCREEN_Y, 0.0f);

		//頂点のカラー設定
		g_aResult[nIdx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}
	else if (nIdx == 2)
	{
		g_aResult[nIdx].pos = D3DXVECTOR3(960.0f, 450.0f, 0.0f);

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x - RESULT_SCREEN_X, g_aResult[nIdx].pos.y + RESULT_SCREEN_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x - RESULT_SCREEN_X, g_aResult[nIdx].pos.y - RESULT_SCREEN_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x + RESULT_SCREEN_X, g_aResult[nIdx].pos.y + RESULT_SCREEN_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResult[nIdx].pos.x + RESULT_SCREEN_X, g_aResult[nIdx].pos.y - RESULT_SCREEN_Y, 0.0f);

		//頂点のカラー設定
		g_aResult[nIdx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();
}


