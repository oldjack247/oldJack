#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

//マクロ定義
#define MAX_TUTORIAL (2)				//タイトルの画像の数
#define TUTORIAL_BG_SIZE_X (960)		//タイトル画面の横幅
#define TUTORIAL_BG_SIZE_Y (540)		//タイトル画面の立幅
#define TUTORIAL_ENTER_SIZE_X (100)		//プレスエンターの横幅
#define TUTORIAL_ENTER_SIZE_Y (50)		//プレスエンターの立幅

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;		//頂点情報
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TUTORIAL] = {};	//テクスチャ
TUTORIAL g_aTutorial[MAX_TUTORIAL];
int g_nCounterTutorial = 30;
int g_TutorialFadeCounter;
int g_nTutorialCounter;
bool g_bTutorialEnter;

// 初期化処理
HRESULT InitTutorial(void)
{

	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/tutorial000.png", &g_pTextureTutorial[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTextureTutorial[1]);

	g_nCounterTutorial = 30;
	g_bTutorialEnter = false;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TUTORIAL, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTutorial, NULL)))
	{
		return E_FAIL;
	}

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		g_aTutorial[nCntTutorial].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTutorial[nCntTutorial].bDisp = true;
	}

	g_TutorialFadeCounter = 0;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++, pVtx += 4)
	{

		SetVertexTutorial(nCntTutorial);

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
		g_pVtxBuffTutorial->Unlock();

	}
	return S_OK;
}

// 終了処理
void UninitTutorial(void)
{
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		if (g_pTextureTutorial[nCntTutorial] != NULL)
		{
			g_pTextureTutorial[nCntTutorial]->Release();
			g_pTextureTutorial[nCntTutorial] = NULL;
		}
	}
}

// 更新処理
void UpdateTutorial(void)
{
	FADE *pFade;
	pFade = GetFade();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4;

	g_nTutorialCounter++;

	if ((g_nTutorialCounter % g_nCounterTutorial) == 0)
	{
		g_aTutorial[1].bDisp = g_aTutorial[1].bDisp ? false : true;	//プレスエンターの点滅
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true && *pFade == FADE_NONE)
	{

		if (g_bTutorialEnter == false)
		{
			g_nCounterTutorial -= 25;
			g_bTutorialEnter = true;
		}
	}

	if (g_bTutorialEnter == true)
	{
		if ((g_nTutorialCounter % 2) == 0)
		{
			g_TutorialFadeCounter++;

		}
	}
	if (g_TutorialFadeCounter == 30)
	{
		//サウンドストップ
		StopSound();
		SetFade(FADE_OUT, MODE_GAME);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
}

// 描画処理
void DrawTutorial(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorial = 0; nCntTutorial < MAX_TUTORIAL; nCntTutorial++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorial[nCntTutorial]);

		if (g_aTutorial[nCntTutorial].bDisp == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorial * 4, 2);
		}
	}

}

// タイトルの頂点座標の設定
void SetVertexTutorial(int nIdx)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	if (nIdx == 0)
	{

		g_aTutorial[nIdx].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x - TUTORIAL_BG_SIZE_X, g_aTutorial[nIdx].pos.y + TUTORIAL_BG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x - TUTORIAL_BG_SIZE_X, g_aTutorial[nIdx].pos.y - TUTORIAL_BG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x + TUTORIAL_BG_SIZE_X, g_aTutorial[nIdx].pos.y + TUTORIAL_BG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x + TUTORIAL_BG_SIZE_X, g_aTutorial[nIdx].pos.y - TUTORIAL_BG_SIZE_Y, 0.0f);

		//頂点のカラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	if (nIdx == 1)
	{
		g_aTutorial[nIdx].pos = D3DXVECTOR3(1395.0f, 850.0f, 0.0f);

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x - TUTORIAL_ENTER_SIZE_X, g_aTutorial[nIdx].pos.y + TUTORIAL_ENTER_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x - TUTORIAL_ENTER_SIZE_X, g_aTutorial[nIdx].pos.y - TUTORIAL_ENTER_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x + TUTORIAL_ENTER_SIZE_X, g_aTutorial[nIdx].pos.y + TUTORIAL_ENTER_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorial[nIdx].pos.x + TUTORIAL_ENTER_SIZE_X, g_aTutorial[nIdx].pos.y - TUTORIAL_ENTER_SIZE_Y, 0.0f);

		//頂点のカラー設定
		pVtx[0].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 255, 255, 255);

	}
}