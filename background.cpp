#include "background.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	//頂点情報
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg2 = NULL;	//頂点情報
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;			//テクスチャ情報
LPDIRECT3DTEXTURE9 g_pTextureBg2 = NULL;		//テクスチャ情報

//背景の初期化処理
HRESULT InitBG(void)
{
	//変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/snowbg.jpg", &g_pTextureBg);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/mountain000.png", &g_pTextureBg2);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0);
	pVtx[1].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0, 0);
	pVtx[2].pos = D3DXVECTOR3(0, 0 + SCREEN_HEIGHT, 0);
	pVtx[3].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0 + SCREEN_HEIGHT, 0);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg2, NULL)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBg2->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0);
	pVtx[1].pos = D3DXVECTOR3(0, 780, 0);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 780, 0);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点情報の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffBg2->Unlock();

	return S_OK;
}

//背景の終了処理
void UninitBG(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();	//開放
		g_pVtxBuffBg = NULL;		//初期化
	}
	if (g_pVtxBuffBg2 != NULL)
	{
		g_pVtxBuffBg2->Release();	//開放
		g_pVtxBuffBg2 = NULL;		//初期化
	}

	//テクスチャの開放
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();	//開放
		g_pTextureBg = NULL;		//初期化
	}
	if (g_pTextureBg2 != NULL)
	{
		g_pTextureBg2->Release();	//開放
		g_pTextureBg2 = NULL;		//初期化
	}
}

//背景の更新処理
void UpdateBG(void)
{
	VERTEX_2D *pVtx; //頂点情報へのポインタ
	static float fTexU = 0.0f;
	fTexU += 0.00005f;
	if (fTexU >= 0.75f) {
		fTexU = 0.0f;
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(fTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexU + 0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexU + 0.25f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffBg->Unlock();
}

//背景の描画処理
void DrawBG(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBg2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBg2);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}