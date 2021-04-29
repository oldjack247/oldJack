#include "item.h"
#include "game.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// グローバル変数
//==============================================================================
LPD3DXMESH g_pMeshItem = NULL;			// メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatItem = NULL;		// マテリアル(材質情報)へのポインタ
DWORD g_nNumMatItem = 0;				// マテリアルの数
ITEM g_aItem[MAX_ITEM];					// アイテムの情報
//int g_nAnimCnt;						// アニメーションカウント
//int g_nAnimCol;						// 色変化カウント

//==============================================================================
// アイテムの初期化処理
//==============================================================================
HRESULT InitItem(void)
{
	// ローカル変数宣言
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ITEM *pItem;
	int nRand;

	// 乱数処理から現時刻を除外
	srand((unsigned int)time(0));

	// デバイスの取得
	pDevice = GetDevice();

	// アイテムの取得
	pItem = &g_aItem[0];

	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		"data/MODEL/coin.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatItem,
		NULL,
		&g_nNumMatItem,
		&g_pMeshItem);

	nRand = RandItem();		// ハイスコアアイテムの選定

	// 構造体の初期化
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItem->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pItem->nType = NULL;
		pItem->bUse = false;
		pItem->vtxMaxObject = D3DXVECTOR3(-10000.0f,-10000.0f,-10000.0f);
		pItem->vtxMinObject = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		//pItem->fAnim = 0.0f;
		//if (nCntItem == nRand)
		//{
		//	pItem->bHiScore = true;
		//}
		//else
		//{
		//	pItem->bHiScore = false;
		//}
	}

	//// 変数の初期化
	//g_nAnimCnt = 0;
	//g_nAnimCol = 0;

	// ローカル変数宣言
	int nNumVtx;	// 頂点数
	DWORD sizeFVF;	// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;	// 頂点バッファへのポインタ

	// 頂点数を取得
	nNumVtx = g_pMeshItem->GetNumVertices();

	// 頂点フォーマットのサイズの取得
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshItem->GetFVF());

	// 頂点バッファをロック
	g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点座標の比較
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

														// 各座標の最大値の比較
			if (g_aItem[nCntItem].vtxMaxObject.x < vtx.x)
			{
				g_aItem[nCntItem].vtxMaxObject.x = vtx.x;
			}
			if (g_aItem[nCntItem].vtxMaxObject.y < vtx.y)
			{
				g_aItem[nCntItem].vtxMaxObject.y = vtx.y;
			}
			if (g_aItem[nCntItem].vtxMaxObject.z < vtx.z)
			{
				g_aItem[nCntItem].vtxMaxObject.z = vtx.z;
			}

			// 各座標の最小値の比較
			if (g_aItem[nCntItem].vtxMinObject.x > vtx.x)
			{
				g_aItem[nCntItem].vtxMinObject.x = vtx.x;
			}
			if (g_aItem[nCntItem].vtxMinObject.y > vtx.y)
			{
				g_aItem[nCntItem].vtxMinObject.y = vtx.y;
			}
			if (g_aItem[nCntItem].vtxMinObject.z > vtx.z)
			{
				g_aItem[nCntItem].vtxMinObject.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	// 頂点フォーマットのサイズ分ポインタを進める
		}
	}


	// 頂点バッファをアンロック
	g_pMeshItem->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// アイテムの終了処理
//==============================================================================
void UninitItem(void)
{
	// メッシュの破棄
	if (g_pMeshItem != NULL)
	{
		g_pMeshItem->Release();
		g_pMeshItem = NULL;
	}

	// マテリアルの破棄
	if (g_pBuffMatItem != NULL)
	{
		g_pBuffMatItem->Release();
		g_pBuffMatItem = NULL;
	}
}

//==============================================================================
// アイテムの更新処理
//==============================================================================
void UpdateItem(void)
{

}

//==============================================================================
// アイテムの描画処理
//==============================================================================
void DrawItem(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
		D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
		D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMatItem; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデル(パーツ)の描画
			g_pMeshItem->DrawSubset(nCntMat);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

	//// ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&object.mtxWorld);

	//// 向きの反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, object.rot.y, object.rot.x, object.rot.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxRot);

	//// 位置を反映
	//D3DXMatrixTranslation(&mtxTrans, object.pos.x, object.pos.y, object.pos.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxTrans);

	//// ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &object.mtxWorld);

	//// 現在のマテリアルを取得
	//pDevice->GetMaterial(&matDef);

	//// マテリアルデータへのポインタを取得
	//pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	//{
	//	// マテリアルの設定
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// テクスチャの設定
	//	pDevice->SetTexture(0, NULL);

	//	// モデル(パーツ)の描画
	//	g_pMeshObject->DrawSubset(nCntMat);
	//}

	//// 保存していたマテリアルを戻す
	//pDevice->SetMaterial(&matDef);
}

//==============================================================================
// アイテムの設定
//==============================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{

}

//==============================================================================
// 敵の取得
//==============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}

//==============================================================================
// ランダムでハイスコアアイテムの選定
//==============================================================================
int RandItem(void)
{
	//// ローカル変数宣言
	//ITEM *pItem;
	//int nStage = GetStage();
	int nHigh = 0;

	//// アイテムの取得
	//pItem = &g_aItem[0];

	//if (nStage == 0)
	//{
	//	nHigh = rand() % COIN_STAGE1;
	//}
	//else if (nStage == 1)
	//{
	//	nHigh = rand() % COIN_STAGE2;
	//}
	//else if(nStage == 2)
	//{
	//	nHigh = rand() % COIN_STAGE3;
	//}

	return nHigh;
}