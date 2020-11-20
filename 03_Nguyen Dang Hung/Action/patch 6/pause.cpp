#include "pause.h"
#include "fade.h"
#include "input.h"
#include "game.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//頂点情報
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PAUSE] = {};		//テクスチャ
PAUSE g_aPause[MAX_PAUSE];
int g_State;

// 初期化処理
HRESULT InitPause(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause100.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &g_pTexturePause[4]);

	//初期化
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		g_aPause[nCntPause].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	g_State = PAUSE_MENU_CONTINUE;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++, pVtx += 4)
	{

		SetVertexPause(nCntPause);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffPause->Unlock();

	}
	return S_OK;
}

// 終了処理
void UninitPause(void)
{
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
}

// 更新処理
void UpdatePause(void)
{
	VERTEX_2D *pVtx;

	if (GetKeyboardTrigger(DIK_S) == true)
	{
		g_State++;
		if (g_State > PAUSE_MENU_MAX - 1)
		{
			g_State = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_W) == true)
	{
		g_State--;
		if (g_State < 0)
		{
			g_State = PAUSE_MENU_MAX - 1;
		}
	}

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);


	switch (g_State)
	{
	case PAUSE_MENU_CONTINUE:

		g_aPause[3].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[4].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	case PAUSE_MENU_RETRY:

		g_aPause[2].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aPause[4].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		break;

	case PAUSE_MENU_QUIT:

		g_aPause[2].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[3].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	}
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		//頂点のカラー設定
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;

		pVtx += 4;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (g_State == PAUSE_MENU_RETRY)
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
		else if (g_State == PAUSE_MENU_QUIT)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
		SetPause(false);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}

// 描画処理
void DrawPause(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);

	}
}


void SetVertexPause(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	if (nIdx == 0)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_BG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_BG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_BG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_BG_SIZE_Y, 0.0f);

		//頂点のカラー
		g_aPause[nIdx].col.r = 0.0f;
		g_aPause[nIdx].col.g = 0.0f;
		g_aPause[nIdx].col.b = 0.0f;
		g_aPause[nIdx].col.a = 0.5f;
	}
	else if (nIdx == 1)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_MENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_MENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_MENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_MENU_SIZE_Y, 0.0f);

		//頂点のカラー
		g_aPause[nIdx].col.r = 0.0f;
		g_aPause[nIdx].col.g = 1.0f;
		g_aPause[nIdx].col.b = 1.0f;
		g_aPause[nIdx].col.a = 0.5;
	}
	else if (nIdx == 2)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(960.0f, 0.0f + (nIdx * 175), 0.0f);
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);

		//頂点のカラー
		g_aPause[nIdx].col.r = 1.0f;
		g_aPause[nIdx].col.g = 1.0f;
		g_aPause[nIdx].col.b = 1.0f;
		g_aPause[nIdx].col.a = 1.0f;

	}
	else if (nIdx >= 3)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(960.0f, 0.0f + (nIdx * 175), 0.0f);
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);

		//頂点のカラー
		g_aPause[nIdx].col.r = 0.75;
		g_aPause[nIdx].col.g = 0.75;
		g_aPause[nIdx].col.b = 0.75;
		g_aPause[nIdx].col.a = 1.0f;

	}

	//頂点のカラー設定
	pVtx[0].col = g_aPause[nIdx].col;
	pVtx[1].col = g_aPause[nIdx].col;
	pVtx[2].col = g_aPause[nIdx].col;
	pVtx[3].col = g_aPause[nIdx].col;

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}