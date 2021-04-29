#include "time.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;
TIME g_aTime[MAX_TIME];							//タイマーの位置
int g_nTime;									//タイマー
int g_nCounterTime;								//タイマーカウンター
int g_aNumber[MAX_TIME];

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitTime(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//構造体初期化
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_aTime[nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//スコアの位置						
	}

	g_nTime = TIMER;

	g_nCounterTime = 0;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTime);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTime, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		//ポリゴンの位置
		g_aTime[nCntTime].pos = D3DXVECTOR3(910.0f + nCntTime * 50, 50.0f, 0.0f);

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_SIZEX, g_aTime[nCntTime].pos.y + TIME_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_SIZEX, g_aTime[nCntTime].pos.y - TIME_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_SIZEX, g_aTime[nCntTime].pos.y + TIME_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_SIZEX, g_aTime[nCntTime].pos.y - TIME_SIZEY, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//時間の計算
		g_aNumber[nCntTime] = (g_nTime / powf(10, MAX_TIME - (nCntTime + 1)));

		//頂点の情報の設定
		SetTextureTime(nCntTime);

	}

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitTime(void)
{
	//バッファの開放
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
	//テクスチャの開放
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateTime(void)
{
	g_nCounterTime++;

	if (g_nTime <= 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

	if ((g_nCounterTime % 60) == 0)
	{
		g_nTime--;

		for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			g_aNumber[nCntTime] = (g_nTime / powf(10, MAX_TIME - (nCntTime + 1)));

			//テクスチャ座標の設定
			SetTextureTime(nCntTime);
		}
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawTime(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTime);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// テクスチャ頂点座標の更新
//-----------------------------------------------------------------------------
void SetTextureTime(int nIdx)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aNumber[nIdx]), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f * g_aNumber[nIdx]), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f * g_aNumber[nIdx]), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aNumber[nIdx]), 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//-----------------------------------------------------------------------------
// タイマー情報の取得
//-----------------------------------------------------------------------------
int GetTime(void)
{
	return g_nTime;
}