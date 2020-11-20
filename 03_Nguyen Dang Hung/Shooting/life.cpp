#include "life.h"
#include "Player.h"

// マクロ定義
#define MAX_LIFE (4)		//背景の移動量
#define LIFE_SIZE (25)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	//頂点情報
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;	//テクスチャ
LIFE g_aLife[MAX_LIFE];
int g_nCntDamage;
int g_nDamageCounter;

// 初期化処理
HRESULT InitLife(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/life000.png", &g_pTextureLife);

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL)))
	{
		return E_FAIL;
	}

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLife[nCntLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	g_nCntDamage = 0;
	g_nDamageCounter = 0;

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < pPlayer->nLife; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3(50.0f + ((nCntLife + 1) * 60), 30.0f, 0.0f);

		//頂点の座標
		pVtx[0].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - LIFE_SIZE, g_aLife[nCntLife].pos.y + LIFE_SIZE, 0);
		pVtx[1].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - LIFE_SIZE, g_aLife[nCntLife].pos.y - LIFE_SIZE, 0);
		pVtx[2].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + LIFE_SIZE, g_aLife[nCntLife].pos.y + LIFE_SIZE, 0);
		pVtx[3].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + LIFE_SIZE, g_aLife[nCntLife].pos.y - LIFE_SIZE, 0);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点のカラー設定
		pVtx[0].col = g_aLife[nCntLife].col;
		pVtx[1].col = g_aLife[nCntLife].col;
		pVtx[2].col = g_aLife[nCntLife].col;
		pVtx[3].col = g_aLife[nCntLife].col;

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();

	return S_OK;
}
// 終了処理
void UninitLife(void)
{
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}

	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

}

// 更新処理
void UpdateLife(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->State == PLAYERSTATE_DAMAGE)
	{
		g_aLife[pPlayer->nLife].col.a = 0.0f;
	}

	else if (pPlayer->State == PLAYERSTATE_APPEAR)
	{
		for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
		{
			g_aLife[nCntLife].col.a = 1.0f;
		}
	}

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{

		//頂点のカラー設定
		pVtx[0].col = g_aLife[nCntLife].col;
		pVtx[1].col = g_aLife[nCntLife].col;
		pVtx[2].col = g_aLife[nCntLife].col;
		pVtx[3].col = g_aLife[nCntLife].col;

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffLife->Unlock();
}

//描画処理
void DrawLife(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
	}
}