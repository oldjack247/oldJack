#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

// グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitlelogo = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitleEnter = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;
D3DXVECTOR3 g_posTitle;
D3DXVECTOR3 g_posTitlelogo;
D3DXVECTOR3 g_posTitleEnter;
int g_nCntMoveLogo;
ENTER Enter;
int g_nSignCounter;

//タイトル画面の初期化処理
HRESULT InitTitle(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	//サウンドスタート
	//PlaySound(SOUND_LABEL_BGM001);

	// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wallpaper000.png", &g_pTextureTitle);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo000.png", &g_pTextureTitlelogo);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTextureTitleEnter);

	// 頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	// タイトル画面の中心座標
	g_posTitle.x = SCREEN_WIDTH / 2;
	g_posTitle.y = SCREEN_HEIGHT / 2;

	// タイトルロゴの初期中心座標
	g_posTitlelogo.x = TITLE_LOGO_X;
	g_posTitlelogo.y = -50;

	g_nCntMoveLogo = 0;

	// エンターサインの中心座標
	g_posTitleEnter.x = TITLE_ENTER_X;
	g_posTitleEnter.y = TITLE_ENTER_Y;

	// エンターサインの初期化
	Enter = ENTER_NORMAL;
	g_nSignCounter = 0;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// タイトル画面の中心座標
	g_posTitle = D3DXVECTOR3(g_posTitle.x, g_posTitle.y, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);

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

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// タイトル画面の中心座標
	g_posTitle = D3DXVECTOR3(g_posTitlelogo.x, g_posTitlelogo.y, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y - 80, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y - 80, 0.0f);

	// rhwの設定
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点座標の設定
	pVtx[4].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[5].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[6].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[7].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// エンターサインの中心座標
	g_posTitleEnter = D3DXVECTOR3(g_posTitleEnter.x, g_posTitleEnter.y, 0.0f);

	// ポリゴンの各頂点座標
	pVtx[8].pos = D3DXVECTOR3(g_posTitleEnter.x - 240, g_posTitleEnter.y + 40, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posTitleEnter.x - 240, g_posTitleEnter.y - 40, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posTitleEnter.x + 240, g_posTitleEnter.y + 40, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posTitleEnter.x + 240, g_posTitleEnter.y - 40, 0.0f);

	// rhwの設定
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// 各頂点カラーの設定
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// 頂点座標の設定
	pVtx[8].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[9].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[10].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[11].tex = D3DXVECTOR2(1.0, 0.0);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	return S_OK;
}

// タイトル画面の終了処理
void UninitTitle(void)
{
	// 頂点バッファの開放
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// テクスチャの開放
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	if (g_pTextureTitlelogo != NULL)
	{
		g_pTextureTitlelogo->Release();
		g_pTextureTitlelogo = NULL;
	}

	if (g_pTextureTitleEnter != NULL)
	{
		g_pTextureTitleEnter->Release();
		g_pTextureTitleEnter = NULL;
	}
}

// タイトル画面の更新処理
void UpdateTitle(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	int nSign = 255;

	// エンターサインカウンターの増加
	g_nSignCounter++;

	// タイトルロゴの移動
	if (g_posTitlelogo.y != TITLE_LOGO_Y)
	{
		g_nCntMoveLogo++;
		if (g_nCntMoveLogo % 2 == 0)
		{
			g_posTitlelogo.y += 2;
			if (g_posTitlelogo.y >= TITLE_LOGO_Y)
			{
				g_posTitlelogo.y = TITLE_LOGO_Y;
			}
		}
	}

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの各頂点座標
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y - 80, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y - 80, 0.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();


	if (GetKeyboardTrigger(DIK_RETURN) == true && g_posTitlelogo.y != TITLE_LOGO_Y)
	{
		PlaySound(SOUND_LABEL_SE_HIT);
		g_posTitlelogo.y = TITLE_LOGO_Y - 1;
	}

	// エンターキー入力でゲーム画面へ遷移
	if (GetKeyboardTrigger(DIK_RETURN) == true && g_posTitlelogo.y == TITLE_LOGO_Y)
	{
		PlaySound(SOUND_LABEL_SE_HIT);
		// フェードアウト
		SetFade(FADE_OUT, MODE_GAME);

		// エンターサインの状態変化
		Enter = ENTER_PRESS;
	}

	// エンターサインの状態
	switch (Enter)
	{
	case ENTER_NORMAL:
		if (g_nSignCounter % 60 == 0)
		{
			// 透明度0へ
			nSign = 255;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 30 == 0 && g_nSignCounter % 60 != 0)
		{
			// 透明度最大へ
			nSign = 0;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}
		break;

	case ENTER_PRESS:
		if (g_nSignCounter % 4 == 0)
		{
			// 透明度0へ
			nSign = 255;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 2 == 0 && g_nSignCounter % 4 != 0)
		{
			// 透明度最大へ
			nSign = 0;

			// 頂点バッファをロックし、頂点情報へのポインタを取得
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// 各頂点カラーの設定
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// 頂点バッファをアンロックする
			g_pVtxBuffTitle->Unlock();
		}
		break;

	default:
		break;
	}
}

// タイトル画面の描画処理
void DrawTitle(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		// テクスチャの設定
		if (nCnt == 0)
		{
			pDevice->SetTexture(0, g_pTextureTitle);
		}
		else if (nCnt == 1)
		{
			pDevice->SetTexture(0, g_pTextureTitlelogo);
		}
		else if (nCnt == 2)
		{
			pDevice->SetTexture(0, g_pTextureTitleEnter);
		}

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCnt * 4,
			2);
	}
}