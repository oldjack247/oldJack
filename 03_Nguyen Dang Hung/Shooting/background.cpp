#include "background.h"
#include "player.h"
#include "input.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;	
LPDIRECT3DTEXTURE9 g_pTextureBG[MAX_BG] = { NULL, NULL, NULL };
D3DXVECTOR3 g_posBg[MAX_BG];		
int g_nCntAnim;						
float g_fPatternAnim[MAX_BG];		
float g_fMoveBgX[MAX_BG];
float g_fMoveBgY[MAX_BG];

// ポリゴンの初期化処理
HRESULT InitBG(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureBG[2]);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,	
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		g_posBg[nCnt].x = SCREEN_WIDTH / 2;
		g_posBg[nCnt].y = SCREEN_HEIGHT / 2;
		g_fMoveBgX[nCnt] = 0.0f;
		g_fMoveBgY[nCnt] = 0.0f;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// 背景の中心座標
		g_posBg[nCnt] = D3DXVECTOR3(g_posBg[nCnt].x, g_posBg[nCnt].y, 0.0f);

		// ポリゴンの各頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_posBg[nCnt].x - (SCREEN_WIDTH / 2), g_posBg[nCnt].y + (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posBg[nCnt].x - (SCREEN_WIDTH / 2), g_posBg[nCnt].y - (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posBg[nCnt].x + (SCREEN_WIDTH / 2), g_posBg[nCnt].y + (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posBg[nCnt].x + (SCREEN_WIDTH / 2), g_posBg[nCnt].y - (SCREEN_HEIGHT / 2), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 各頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// 頂点座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
		pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
		pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
		pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffBG->Unlock();

	return S_OK;
}

//ポリゴンの終了処理
void UninitBG(void)
{
	//頂点バッファの開放
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	// テクスチャの開放
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (g_pTextureBG[nCnt] != NULL)
		{
			g_pTextureBG[nCnt]->Release();
			g_pTextureBG[nCnt] = NULL;
		}
	}
}

//ポリゴンの更新処理
void UpdateBG(void)
{
	VERTEX_2D *pVtx;

	g_nCntAnim++;	// アニメーションカウンター更新

	// プレイヤーの移動に合わせて背景テクスチャの切り替え
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_fMoveBgY[0] += MOVE_BG_A;
		g_fMoveBgY[1] += MOVE_BG_B;
		g_fMoveBgY[2] += MOVE_BG_C;

		if (GetKeyboardPress(DIK_A) == true)
		{
			g_fMoveBgX[0] -= MOVE_BG_A;
			g_fMoveBgX[1] -= MOVE_BG_B;
			g_fMoveBgX[2] -= MOVE_BG_C;
			g_fMoveBgY[0] += MOVE_BG_A;
			g_fMoveBgY[1] += MOVE_BG_B;
			g_fMoveBgY[2] += MOVE_BG_C;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
			g_fMoveBgY[0] += MOVE_BG_A;
			g_fMoveBgY[1] += MOVE_BG_B;
			g_fMoveBgY[2] += MOVE_BG_C;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_fMoveBgY[0] -= MOVE_BG_A;
		g_fMoveBgY[1] -= MOVE_BG_B;
		g_fMoveBgY[2] -= MOVE_BG_C;

		if (GetKeyboardPress(DIK_A) == true)
		{
			g_fMoveBgX[0] -= MOVE_BG_A;
			g_fMoveBgX[1] -= MOVE_BG_B;
			g_fMoveBgX[2] -= MOVE_BG_C;
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_fMoveBgX[0] -= MOVE_BG_A;
		g_fMoveBgX[1] -= MOVE_BG_B;
		g_fMoveBgX[2] -= MOVE_BG_C;

		if (GetKeyboardPress(DIK_D) == true)
		{
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_fMoveBgX[0] += MOVE_BG_A;
		g_fMoveBgX[1] += MOVE_BG_B;
		g_fMoveBgX[2] += MOVE_BG_C;
	}


	// テクスチャの更新
	if ((g_nCntAnim % 2) == 0)
	{
		// テクスチャのパターンの更新
		g_fPatternAnim[0] -= 0.001f;
		g_fPatternAnim[1] -= 0.003f;
		g_fPatternAnim[2] -= 0.005f;

		// 頂点バッファをロックする
		g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < MAX_BG; nCnt++, pVtx = pVtx + 4)
		{
			// テクスチャの頂点座標の更新
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt]);
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt] - 1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt]);
			pVtx[3].tex = D3DXVECTOR2(1.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt] - 1.0f);
		}

		// 頂点バッファをアンロックする
		g_pVtxBuffBG->Unlock();
	}
}

//ポリゴンの描画処理
void DrawBG(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBG[nCnt]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	nCnt * 4, 2);					
	}
}