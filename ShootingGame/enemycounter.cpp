#include "enemycounter.h"
#include "enemy.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemyCounter = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyCounter = NULL;
ENEMYCOUNTER g_aEnemyCounter[MAX_COUNTER];						//敵のカウンターの位置
int g_nEnemyCounter;											//敵のカウンター
int g_aCount[MAX_COUNTER];

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitEnemyCounter(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//構造体初期化
	for (int nnCntEnemyCounter = 0; nnCntEnemyCounter < MAX_COUNTER; nnCntEnemyCounter++)
	{
		g_aEnemyCounter[nnCntEnemyCounter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//スコアの位置						
	}

	g_nEnemyCounter = ENEMY_COUNT;

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureEnemyCounter);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COUNTER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemyCounter, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyCounter = 0; nCntEnemyCounter < MAX_COUNTER; nCntEnemyCounter++, pVtx += 4)
	{
		//ポリゴンの位置
		g_aEnemyCounter[nCntEnemyCounter].pos = D3DXVECTOR3(COUNTER_SIZEX + 50 + nCntEnemyCounter * 50, 50.0f, 0.0f);

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x - COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y + COUNTER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x - COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y - COUNTER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x + COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y + COUNTER_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x + COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y - COUNTER_SIZEY, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

		//時間の計算
		g_aCount[nCntEnemyCounter] = (g_nEnemyCounter / powf(10, MAX_COUNTER - (nCntEnemyCounter + 1)));

		//頂点の情報の設定
		SetTextureEnemyCounter(nCntEnemyCounter);

	}

	//頂点バッファをアンロックする
	g_pVtxBuffEnemyCounter->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitEnemyCounter(void)
{
	//バッファの開放
	if (g_pVtxBuffEnemyCounter != NULL)
	{
		g_pVtxBuffEnemyCounter->Release();
		g_pVtxBuffEnemyCounter = NULL;
	}
	//テクスチャの開放
	if (g_pTextureEnemyCounter != NULL)
	{
		g_pTextureEnemyCounter->Release();
		g_pTextureEnemyCounter = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateEnemyCounter(void)
{
	if (g_nEnemyCounter <= 0)
	{
		g_nEnemyCounter = 0;
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawEnemyCounter(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEnemyCounter, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (int nCntTime = 0; nCntTime < MAX_COUNTER; nCntTime++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEnemyCounter);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// カウント処理
//-----------------------------------------------------------------------------
void AddEnemyCounter(int EnemyCount)
{
	g_nEnemyCounter -= EnemyCount;

	for (int nCntEnemyCounter = 0; nCntEnemyCounter < MAX_COUNTER; nCntEnemyCounter++)
	{
		g_aCount[nCntEnemyCounter] = (g_nEnemyCounter / powf(10, MAX_COUNTER - (nCntEnemyCounter + 1)));

		//テクスチャ座標の設定
		SetTextureEnemyCounter(nCntEnemyCounter);
	}
}

//-----------------------------------------------------------------------------
// テクスチャ頂点座標の更新
//-----------------------------------------------------------------------------
void SetTextureEnemyCounter(int nIdx)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemyCounter->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCount[nIdx]), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCount[nIdx]), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCount[nIdx]), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCount[nIdx]), 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffEnemyCounter->Unlock();
}