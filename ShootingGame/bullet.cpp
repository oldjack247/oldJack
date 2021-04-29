#include "Bullet.h"
#include "enemy.h"
#include "Player.h"
#include "score.h"
#include "effect.h"

//-----------------------------------------------------------------------------
// 構造体
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nType;			//弾の種類
	BULLETTYPE Type;
	bool bUse;			//弾の使用状況
}BULLET;


//-----------------------------------------------------------------------------
// グローバル変数
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet[2] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];							//弾の情報

//-----------------------------------------------------------------------------
// 弾の頂点座標の設定
//-----------------------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0);

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ammo000.png", &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ammo000.png", &g_pTextureBullet[1]);


	//構造体初期化
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nType = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標
		SetVertexBullet(nCntBullet);

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

		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock(); //アンロック

	return S_OK;
}

//-----------------------------------------------------------------------------
//　終了処理
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	//バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
	//テクスチャの開放
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET_TYPE; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	BULLET* pBullet;
	pBullet = &g_aBullet[0];



	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{

		if (pBullet->bUse == true)
		{

			//位置の更新
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			//頂点座標
			SetVertexBullet(nCntBullet);

			//寿命チェック
			/*pBullet->nLife--;
			if (pBullet->nLife == 0)
			{
				SetExplosion(pBullet->pos,D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
				pBullet->bUse = false;
			}*/

			//画面外チェック
			if (pBullet->pos.y < 0)
			{
				pBullet->bUse = false;
			}
			if (pBullet->pos.y > SCREEN_HEIGHT)
			{
				pBullet->bUse = false;
			}

			if (pBullet->pos.x > SCREEN_WIDTH)
			{
				pBullet->bUse = false;
			}

			if (pBullet->pos.x < 0)
			{
				pBullet->bUse = false;
			}

			//プレイヤーの弾
			if (pBullet->Type == BULLETTYPE_PLAYER)
			{

				//敵の情報
				ENEMY* pEnemy;
				pEnemy = GetEnemy();

				//自機の情報
				PLAYER* pPlayer;
				pPlayer = GetPlayer();

				//エフェクト情報
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.75f, 0.75f, 0.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵と弾の当たり判定
						if (pBullet->pos.y > pEnemy->pos.y - ENEMY_SIZE && pBullet->pos.y < pEnemy->pos.y + ENEMY_SIZE &&
							pBullet->pos.x > pEnemy->pos.x - ENEMY_SIZE && pBullet->pos.x < pEnemy->pos.x + ENEMY_SIZE)
						{
							HitEnemy(nCntEnemy, 1);

							pBullet->bUse = false;

						}

						if (pEnemy->nType == 2)
						{
							if (pBullet->pos.y + BULLET_SIZE > pEnemy->pos.y - BIG_ENEMY_SIZEX && pBullet->pos.y - BULLET_SIZE < pEnemy->pos.y + BIG_ENEMY_SIZEY &&
								pBullet->pos.x + BULLET_SIZE > pEnemy->pos.x - BIG_ENEMY_SIZEX && pBullet->pos.x - BULLET_SIZE < pEnemy->pos.x + BIG_ENEMY_SIZEY)
							{
								HitEnemy(nCntEnemy, 1);

								pBullet->bUse = false;
							}
						}
					}
				}
			}

			//敵の弾
			if (pBullet->Type == BULLETTYPE_ENEMY)
			{

				//敵の情報
				ENEMY* pEnemy;
				pEnemy = GetEnemy();

				//自機の情報
				PLAYER* pPlayer;
				pPlayer = GetPlayer();

				//エフェクト情報
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.75f, 0.75f, 0.75f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				if (pPlayer->State == PLAYERSTATE_NORMAL)
				{//自機と弾の当たり判定
					if (pBullet->pos.y + BULLET_SIZE > pPlayer->pos.y - PLAYER_SIZEY && pBullet->pos.y - BULLET_SIZE < pPlayer->pos.y + PLAYER_SIZEY &&
						pBullet->pos.x + BULLET_SIZE > pPlayer->pos.x - PLAYER_SIZEX && pBullet->pos.x - BULLET_SIZE < pPlayer->pos.x + PLAYER_SIZEX)
					{
						HitPlayer(1);
						pBullet->bUse = false;
					}
				}
			}

			//子機の弾
			if (pBullet->Type == BULLETTYPE_SUBPLAYER)
			{
				//敵の情報
				ENEMY* pEnemy;
				pEnemy = GetEnemy();

				//自機のの情報
				PLAYER* pPlayer;
				pPlayer = GetPlayer();

				//エフェクト情報
				SetEffect(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.85f));
				SetEffect(pBullet->pos, D3DXCOLOR(0.75f, 0.75f, 0.75f, 0.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//敵と弾の当たり判定
						if (pBullet->pos.y > pEnemy->pos.y - ENEMY_SIZE && pBullet->pos.y < pEnemy->pos.y + ENEMY_SIZE &&
							pBullet->pos.x > pEnemy->pos.x - ENEMY_SIZE && pBullet->pos.x < pEnemy->pos.x + ENEMY_SIZE)
						{

							HitEnemy(nCntEnemy, 1);

							pBullet->bUse = false;

						}

						if (pEnemy->nType == 2)
						{
							if (pBullet->pos.y + BULLET_SIZE > pEnemy->pos.y - BIG_ENEMY_SIZEX && pBullet->pos.y - BULLET_SIZE < pEnemy->pos.y + BIG_ENEMY_SIZEY &&
								pBullet->pos.x + BULLET_SIZE > pEnemy->pos.x - BIG_ENEMY_SIZEX && pBullet->pos.x - BULLET_SIZE < pEnemy->pos.x + BIG_ENEMY_SIZEY)
							{
								HitEnemy(nCntEnemy, 1);

								pBullet->bUse = false;
							}
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	//ローカル
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);



	//ポリゴンの描画
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nType]);

		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// 弾の設定
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, BULLETTYPE Type)
{
	BULLET* pBullet;
	pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//位置
			pBullet->pos = pos;

			//頂点座標
			SetVertexBullet(nCntBullet);

			//移動量
			pBullet->move.x = move.x;
			pBullet->move.y = move.y;

			pBullet->Type = Type;

			//寿命
			pBullet->nType = nType;

			pBullet->bUse = true;
			break;
		}
	}
}
