#include "main.h"
#include "item.h"
#include "player.h"
#include "score.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTextureItem[ITEM_TYPE] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;		//頂点バッファへのポインタ
Item g_aItem[MAX_ITEM];

//初期化処理
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//でアイスのポインタ
	int nCntItem;

	//デバイスの取得
	pDevice = GetDevice();

	//アイテムの初期化
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].nType = 0;
		g_aItem[nCntItem].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/item000.png",
		&g_apTextureItem[0]);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//アイテムの終了処理
void UninitItem(void)
{
	int nCntItem;
	for (nCntItem = 0; nCntItem < ITEM_TYPE; nCntItem++)
	{
		if (g_apTextureItem[nCntItem] != NULL)
		{
			//テクスチャの破棄
			g_apTextureItem[nCntItem]->Release();
			g_apTextureItem[nCntItem] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//アイテムの更新処理
void UpdateItem(void)
{
	int nCntItem;
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
							//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{

		if (g_aItem[nCntItem].bUse == true)
		{

			g_aItem[nCntItem].pos += g_aItem[nCntItem].move;

			//アイテムとプレイヤーの当たり判定
			PLAYER *pPlayer;
			pPlayer = GetPlayer();

			if (g_aItem[nCntItem].pos.x - ITEM_WIDTH / 2.0f <= pPlayer->pos.x + pPlayer->fWidth / 2.0f &&
				g_aItem[nCntItem].pos.x + ITEM_WIDTH / 2.0f >= pPlayer->pos.x - pPlayer->fWidth / 2.0f &&
				g_aItem[nCntItem].pos.y >= pPlayer->pos.y - pPlayer->fHeight &&
				g_aItem[nCntItem].pos.y - ITEM_HEIGHT <= pPlayer->pos.y)
			{
				//SetExplosion(g_posMother, (1.0f, 0.0f, 0.0f, 0.0f));
				//PlaySound(SOUND_LABEL_ITEM_CARROT);
				//g_aItem[nCntItem].bUse = false;
				//HitPlayer();
				SetScore(100);
				g_aItem[nCntItem].bUse = false;
			}


			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y - ITEM_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y - ITEM_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y, 0.0f);

			if (g_aItem[nCntItem].pos.x <= 0 - ITEM_WIDTH)
			{
				g_aItem[nCntItem].bUse = false;		//使用していない状態にする
			}

		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffItem->Unlock();
}

//描画処理
void DrawItem(void)
{
	int nCntItem;

	LPDIRECT3DDEVICE9 pDevice;					//デバイスのポインタ
	pDevice = GetDevice();						//デバイスを取得する
												//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCntItem].nType]);	//テクスチャの設定
																				//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,							//プリミティブの種類
				nCntItem * 4,													//描画を開始する頂点のインデックス
				2);																//描画するプリミティブ数
		}
	}
}

//アイテムの設定処理
void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, int type)
{
	int nCntItem;
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].pos = pos;
			//頂点座標の設定

			VERTEX_2D *pVtx;		//頂点情報へのポインタ
									//頂点バッファをロックし、頂点データのポインタを取得
			g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += (nCntItem * 4);

			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y - ITEM_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y - ITEM_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + ITEM_WIDTH / 2.0f, g_aItem[nCntItem].pos.y, 0.0f);

			//頂点バッファをアンロックする
			g_pVtxBuffItem->Unlock();

			g_aItem[nCntItem].move = move;
			g_aItem[nCntItem].nType = type;
			g_aItem[nCntItem].bUse = true;
			break;
		}
	}
}

//取得処理
Item *GetItem(void)
{
	return &g_aItem[0];
}