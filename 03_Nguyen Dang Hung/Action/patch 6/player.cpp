#include "main.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "enemy.h"
#include "item.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePolygon[POLYGON_TYPE] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;			//頂点バッファへのポインタ
PLAYER g_Player;											//プレイヤー構造体
int g_nCntAnim;												//プレイヤーのアニメーションカウンタ
int g_nAnimSpan;
int g_nCounterAnim;											//プレイヤーのアニメーションカウンタ
int g_nCntReturn;											//プレイヤーの復帰
bool g_bAnim;												//アニメーションの切り替え
bool g_bMoveScrole;											//スクロールするための変数
float fScroll;

//初期化処理
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//初期化
	g_Player.fWidth = POLYGON_WIDTH;
	g_Player.fHeight = POLYGON_HEIGHT;
	g_Player.BeforePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.BeforeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.bJump = false;
	g_Player.nCounterState = 0;
	g_Player.State = PLAYERSTATE_NOMAL;
	g_nCntAnim = 0;
	g_nCounterAnim = 0;
	g_bAnim = false;
	g_bMoveScrole = false;

	g_Player.bJump = true;
	g_Player.bMoveR = false;
	g_Player.bMoveL = false;
	g_Player.nMoveTexX = 0;
	g_Player.nMoveTexY = 0;
	g_Player.nCntTex = 0;
	g_Player.bScroll = false;
	fScroll = 0.0f;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/santa000.png",
		&g_pTexturePolygon[0]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//srand((unsigned int)time(NULL));

	BLOCK *pBlock;
	//ブロックの取得
	pBlock = GetBlock();

	g_Player.pos = D3DXVECTOR3(g_Player.fWidth / 2, SCREEN_HEIGHT - 40.0f, 0.0f);


	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//終了処理
void UninitPlayer(void)
{
	int nCntPolygon;
	for (nCntPolygon = 0; nCntPolygon < POLYGON_TYPE; nCntPolygon++)
	{
		//テクスチャの破棄
		if (g_pTexturePolygon[nCntPolygon] != NULL)
		{
			g_pTexturePolygon[nCntPolygon]->Release();
			g_pTexturePolygon[nCntPolygon] = NULL;
		}
	}


	//頂点バッファの破棄
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//更新処理
void UpdatePlayer(void)
{
	BLOCK *pBlock;
	//ブロックの取得
	pBlock = GetBlock();
	static float fTexU = 0.0f;		//テクスチャのU座標
	static float fTexV = 0.0f;		//テクスチャのV座標
	int nAnimSpan;

	g_Player.bMoveR = false;
	g_Player.bMoveL = false;
	g_Player.bScroll = false;

	Enemy *pEnemy;
	pEnemy = GetEnemy();

	Item *pItem;
	pItem = GetItem();

	g_Player.BeforePos = g_Player.pos;
	g_Player.move.y += GRAVITY;		//重力

									//プレイヤーのアニメーション
	if (g_Player.move.x > 0.0f)
	{
		g_nCntAnim++;
		if (g_nCntAnim >= 8)
		{
			g_nCounterAnim++;
			if (g_nCounterAnim > 20)
			{
				g_nCounterAnim = 0;
			}
			VERTEX_2D *pVtx;		//頂点情報へのポインタ
									//頂点バッファをロックし、頂点データのポインタを取得
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);


			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 0.5f);


			//頂点バッファをアンロックする
			g_pVtxBuffPolygon->Unlock();
			g_nCntAnim = 0;
		}
	}
	else if (g_Player.move.x < 0.0f)
	{
		//プレイヤーのアニメーション
		g_nCntAnim++;
		if (g_nCntAnim >= 8)
		{
			g_nCounterAnim++;
			if (g_nCounterAnim > 20)
			{
				g_nCounterAnim = 0;
			}

			VERTEX_2D *pVtx;		//頂点情報へのポインタ
									//頂点バッファをロックし、頂点データのポインタを取得
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 1.0f);

			//頂点バッファをアンロックする
			g_pVtxBuffPolygon->Unlock();

			g_nCntAnim = 0;
		}
	}

	if (g_Player.move.x == 0.0f || g_Player.move.y < 0.0f)
	{
		VERTEX_2D *pVtx;		//頂点情報へのポインタ
								//頂点バッファをロックし、頂点データのポインタを取得
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		if (g_bAnim == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}

		//頂点バッファをアンロックする
		g_pVtxBuffPolygon->Unlock();
	}

	//任意のキーが押されたかどうか

	switch (g_Player.State)
	{
	case PLAYERSTATE_NOMAL:
		if ((GetKeyboardPress(DIK_RIGHT) == true || GetKeyboardPress(DIK_D) == true) && g_Player.pos.y - g_Player.fHeight <= SCREEN_HEIGHT)
		{
			if (g_Player.bJump = true)
			{
				g_Player.move.x = POLYGON_SPEED;
			}
			else if (g_Player.bJump = false)
			{
				g_Player.move.x = POLYGON_SPEED * 0.8;
			}
			g_bAnim = false;


			if (g_Player.pos.x >= SCROLE_START_POS)
			{
				for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
				{
					pBlock->pos.x += -g_Player.move.x;
				}
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					pEnemy->pos.x += -g_Player.move.x;
				}
				for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
				{
					pItem->pos.x += -g_Player.move.x;
				}
			}
		}
		else if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_A) == true)
		{
			if (g_Player.bJump = true)
			{
				g_Player.move.x = -POLYGON_SPEED;
			}
			else if (g_Player.bJump = false)
			{
				g_Player.move.x = -POLYGON_SPEED * 0.8;
			}
			g_bAnim = true;
		}
		else if (GetKeyboardPress(DIK_RIGHT) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_Player.move.x = 0.0f;
		}
		else if (GetKeyboardPress(DIK_LEFT) == false && GetKeyboardPress(DIK_A) == false)
		{
			g_Player.move.x = 0.0f;
		}

		break;

	case PLAYERSTATE_DAMAGE:
		g_Player.move.x = 0.0f;
		g_Player.nCounterState--;
		if (g_Player.nCounterState <= 0)
		{
			g_Player.State = PLAYERSTATE_NOMAL;

			VERTEX_2D *pVtx;		//頂点情報へのポインタ
			//頂点バッファをロックし、頂点データのポインタを取得
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//頂点バッファをアンロックする
			g_pVtxBuffPolygon->Unlock();
		}

		break;
	}


	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_nCntAnim = 29;
	}
	else if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_nCntAnim = 29;
	}


	g_Player.pos.y += g_Player.move.y;
	g_Player.pos.x += g_Player.move.x;

	//プレイヤーとブロックの当たり判定

	CollisionBlock(&g_Player.pos, &g_Player.BeforePos, &g_Player.move, g_Player.fWidth, g_Player.fHeight);

	if (CollisionBlock(&g_Player.pos, &g_Player.BeforePos, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true)	//着地していたら
	{
		g_Player.bJump = false;
	}

	//ブロックの上に乗るとプレイヤーも動く処理
	BLOCK *pBlock2;
	//ブロックの取得
	pBlock2 = GetBlock2();
	pEnemy = GetEnemy();

	g_Player.pos += pBlock2->SaveMove;
	if (g_Player.pos.x >= SCROLE_START_POS)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			pEnemy->pos += -pBlock2->SaveMove;
		}
	}

	//動くブロックに乗ってるときのスクロール処理
	pBlock = GetBlock();
	pEnemy = GetEnemy();
	pItem = GetItem();
	if (g_Player.pos.x >= SCROLE_START_POS)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
		{
			pBlock->pos.x += -pBlock2->move.x;
		}
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			pEnemy->pos.x += -pBlock2->move.x;
		}
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
		{
			pItem->pos.x += -pBlock2->move.x;
		}
		//CollisionBlock(&g_Player.pos, &g_Player.BeforePos, &g_Player.move, g_Player.fWidth, g_Player.fHeight);

	}

	//スペースキーを押したときの処理
	if (g_Player.State == PLAYERSTATE_NOMAL)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_Player.bJump == false)
			{
				g_Player.move.y = POLYGON_JUMP;
				g_Player.bJump = true;
			}
		}
	}

	//プレイヤーの移動制限
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Player.pos.x <= 0.0f + POLYGON_WIDTH / 2.0f)
	{
		g_Player.pos.x = 0.0f + POLYGON_WIDTH / 2.0f;		//位置の更新
	}
	else if (g_Player.pos.x >= SCROLE_START_POS)
	{
		g_Player.pos.x = SCROLE_START_POS;
	}
	if (g_Player.pos.y - g_Player.fHeight >= SCREEN_HEIGHT)
	{
		g_nCntReturn++;
		if (g_nCntReturn >= 50)
		{
			g_Player.pos = D3DXVECTOR3(g_Player.fWidth / 2, 0.0f, 0.0f);
			g_Player.move.y = 0.1f;
			g_Player.bJump = true;
			g_nCntReturn = 0;
		}
	}

	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();


}

//描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//デバイスのポインタ
	pDevice = GetDevice();						//デバイスを取得する
												//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//頂点フォーマットの設定

	pDevice->SetTexture(0, g_pTexturePolygon[0]);	//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,					//開始する頂点のインデックス
		2);					//描画するプリミティブ数
}

//ダメージ処理
void HitPlayer(void)
{
	g_Player.State = PLAYERSTATE_DAMAGE;
	g_Player.nCounterState = 50;

	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//取得処理
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

