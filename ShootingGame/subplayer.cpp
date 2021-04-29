#include "subplayer.h"
#include "player.h"
#include "sound.h"
#include "bullet.h"
#include "input.h"

//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_apTextureSubplayer = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSubplayer = NULL;
SUBPLAYER g_aSubplayer[MAX_SUBPLAYER];						//子機の情報
int nSubplayerCounter;

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitSubplayer(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\spaceship002.png", &g_apTextureSubplayer);

	//構造体初期化
	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++)
	{
		g_aSubplayer[nCntSubplayer].pos = D3DXVECTOR3(1920.0f, 0.0f, 0.0f);
		g_aSubplayer[nCntSubplayer].move = D3DXVECTOR3(1920.0f, 0.0f, 0.0f);
		g_aSubplayer[nCntSubplayer].bUse = false;
		g_aSubplayer[nCntSubplayer].State = SUBPLAYERSTATE_APPEAR;
	}

	nSubplayerCounter = 0;

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SUBPLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSubplayer, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pVtx += 4)
	{
		//頂点座標
		SetVertexSubplayer(nCntSubplayer);

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

		//頂点の情報の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	}
	//頂点バッファをアンロックする
	g_pVtxBuffSubplayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitSubplayer(void)
{
	//バッファの開放
	if (g_pVtxBuffSubplayer != NULL)
	{
		g_pVtxBuffSubplayer->Release();
		g_pVtxBuffSubplayer = NULL;
	}

	if (g_apTextureSubplayer != NULL)
	{
		g_apTextureSubplayer->Release();
		g_apTextureSubplayer = NULL;
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateSubplayer(void)
{
	SUBPLAYER* pSubplayer;
	pSubplayer = &g_aSubplayer[0];

	PLAYER* pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pSubplayer++)
	{
		if (pSubplayer->bUse == true)
		{
			switch (pSubplayer->State)
			{
			case SUBPLAYERSTATE_APPEAR:
				//位置の更新
				pSubplayer->pos.y += 5;

				if (pSubplayer->pos.y > SCREEN_HEIGHT)
				{
					pSubplayer->State = SUBPLAYERSTATE_BREAK;
				}


				break;

			case SUBPLAYERSTATE_NORMAL:

				//慣性
				pSubplayer->pos.x += (pSubplayer->posDest.x - pSubplayer->pos.x) * 0.09f;
				pSubplayer->pos.y += (pSubplayer->posDest.y - pSubplayer->pos.y) * 0.09f;

				break;

			case SUBPLAYERSTATE_BREAK:

				pSubplayer->bUse = false;

				break;

			}
			SetVertexSubplayer(nCntSubplayer);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSubplayer->Unlock();
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawSubplayer(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	SUBPLAYER* pSubplayer;
	pSubplayer = &g_aSubplayer[0];

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSubplayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pSubplayer++)
	{
		if (pSubplayer->bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSubplayer);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSubplayer * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// 子機の頂点座標設定
//-----------------------------------------------------------------------------
void SetVertexSubplayer(int nIdx)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffSubplayer->Unlock();

}

//-----------------------------------------------------------------------------
// 子機の設定
//-----------------------------------------------------------------------------
void SetSubplayer(D3DXVECTOR3 pos)
{
	SUBPLAYER* pSubplayer;
	pSubplayer = &g_aSubplayer[0];

	int nCntSubplayer;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pSubplayer++)
	{
		if (pSubplayer->bUse == false)
		{
			//位置
			pSubplayer->pos = pos;

			pSubplayer->State = SUBPLAYERSTATE_APPEAR;

			//頂点座標
			SetVertexSubplayer(nCntSubplayer);

			//使っている状態に
			pSubplayer->bUse = true;

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffSubplayer->Unlock(); //アンロック

}

//-----------------------------------------------------------------------------
// 子機の場所設定
//-----------------------------------------------------------------------------
void SetPosSubplayer(int nIdx, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//g_aSubplayer[nIdx].pos = D3DXVECTOR3(500, -100, 0.0f);

	//頂点座標
	g_aSubplayer[nIdx].posDest = pos;

	pVtx[0].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffSubplayer->Unlock();

}

//-----------------------------------------------------------------------------
// 子機の弾
//-----------------------------------------------------------------------------
void SetBulletSubplayer(int nIdx)
{
	SetBullet(g_aSubplayer[nIdx].pos, D3DXVECTOR3(0.0f, -11.5f, 0.0f), PLAYER_BULLET, BULLETTYPE_SUBPLAYER);
}

SUBPLAYER* GetSubplayer(void)
{
	return &g_aSubplayer[0];
}

