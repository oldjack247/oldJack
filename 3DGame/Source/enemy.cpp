#include "enemy.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ENEMY	(8)
#define MOVE_ENEMY (1.5f)
#define MAX_TEXTURE (10)
#define SHADOW_SIZE_ENEMY (30.0f)

//=============================================================================
// グローバル変数
//=============================================================================
Enemy g_aEnemy[MAX_ENEMY];							// モデル情報
LPDIRECT3DTEXTURE9 g_aTextureEnemy[MAX_TEXTURE] = {};	//テクスチャへのポインタ
D3DXVECTOR3 g_vtxMinEnemy, g_vtxMaxEnemy;				// 各頂点座標の最小値、最大値
const char *g_aTextureNameEnemy[MAX_TEXTURE];
int g_nCntEffectGetE = 0;

//=============================================================================
// 敵の初期化処理
//=============================================================================
HRESULT InitEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの初期化
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++) {
		g_aTextureNameEnemy[nCntTex] = "";
	}
	//読み込むテクスチャの設定
	g_aTextureNameEnemy[0] = "data/TEXTURE/Joker_map_D.png";	//固定

																//テクスチャの読み込み
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++) {
		if (g_aTextureNameEnemy[nCntTex] != NULL) {
			D3DXCreateTextureFromFile(pDevice,
				g_aTextureNameEnemy[nCntTex],
				&g_aTextureEnemy[nCntTex]);
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//初期化
		g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		g_aEnemy[nCntEnemy].nNumMatEnemy = NULL;
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotDest = g_aEnemy[nCntEnemy].rot;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 50;
		g_aEnemy[nCntEnemy].nIdx = -1;
		g_aEnemy[nCntEnemy].bUse = false;
		for (int nCntTex = 0; nCntTex < 24; nCntTex++) {
			g_aEnemy[nCntEnemy].aTexType[nCntTex] = NULL;
		}

		//Xファイルの読み込み
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/JokerStatue.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCntEnemy].pBuffMatEnemy,
			NULL,
			&g_aEnemy[nCntEnemy].nNumMatEnemy,
			&g_aEnemy[nCntEnemy].pMeshEnemy)))
		{
			return E_FAIL;
		}
	}
	//敵の設置
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	///SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	//SetEnemy(D3DXVECTOR3(200.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	//SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// 敵の終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//メッシュの破棄
		if (g_aEnemy[nCntEnemy].pBuffMatEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMatEnemy->Release();
			g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		}

		//マテリアルの破棄
		if (g_aEnemy[nCntEnemy].pMeshEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pMeshEnemy->Release();
			g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		}
	}
}

//=============================================================================
// 敵の更新処理
//=============================================================================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = &g_aEnemy[0];
	BULLET *pBullet = GetBullet();

	// 変数宣言
	int nCntTrue = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, nCntEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			MoveEnemy(nCntEnemy);

			//プレイヤーが当たったら消す
			if ((pPlayer->pos.x - pPlayer->vtxMinPlayer.x) < (pEnemy->pos.x + pEnemy->vtxMaxEnemy.x) &&
				(pPlayer->pos.x + pPlayer->vtxMaxPlayer.x) > (pEnemy->pos.x - pEnemy->vtxMaxEnemy.x) &&
				(pPlayer->pos.z - pPlayer->vtxMinPlayer.z) < (pEnemy->pos.z + pEnemy->vtxMaxEnemy.z) &&
				(pPlayer->pos.z + pPlayer->vtxMaxPlayer.z) > (pEnemy->pos.z - pEnemy->vtxMaxEnemy.z))
			{
				if (pPlayer->nLife <= 100)
				{
					//プレイヤー即死
					HitPlayer(100);
				}
			}

			//弾が当たったら敵にダメージ
			if ((pBullet->pos.x) < (pEnemy->pos.x + pEnemy->vtxMaxEnemy.x) &&
				(pBullet->pos.x) > (pEnemy->pos.x - pEnemy->vtxMaxEnemy.x) &&
				(pBullet->pos.z) < (pEnemy->pos.z + pEnemy->vtxMaxEnemy.z) &&
				(pBullet->pos.z) > (pEnemy->pos.z - pEnemy->vtxMaxEnemy.z))
			{
				if (pBullet->type == BULLETTYPE_PLAYER)
				{
					HitEnemy(5);
				}
			}

			if (pEnemy->nLife <= 0)
			{
				//使った判定
				pEnemy->bUse = false;

				//影を消す
				//DeleteShadow(pEnemy->nIdx);
			}

			//使われているカウント
			nCntTrue++;
		}
	}
}

//=============================================================================
//敵の描画処理
//=============================================================================
void DrawEnemy(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatEnemy; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aTextureEnemy[g_aEnemy[nCntEnemy].aTexType[nCntMat]]);
				//モデル(パーツ)の描画
				g_aEnemy[nCntEnemy].pMeshEnemy->DrawSubset(nCntMat);
			}
			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// 敵の移動処理
//=============================================================================
void MoveEnemy(int nCntEnemy)
{
	//変数宣言
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 EnemyV;		//プレイヤーに向く

							//目的の回転角を制限
	if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
	}

	//向きの更新
	g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * -0.05f;

	//現在の回転角を制限
	if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14fより大きくなったら-3.14fにする
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	//敵がプレイヤーを見るようにする
	EnemyV = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

	//角度を求める
	float fAngle = (float)atan2(EnemyV.x, EnemyV.z);

	//自機と中心点の対角線の長さ
	float fLength = sqrtf((EnemyV.x * EnemyV.x) + (EnemyV.z * EnemyV.z));

	//プレイヤーの追尾
	g_aEnemy[nCntEnemy].move.x = MOVE_ENEMY * sinf(fAngle);
	g_aEnemy[nCntEnemy].move.z = MOVE_ENEMY * cosf(fAngle);

	//向きの更新
	g_aEnemy[nCntEnemy].rotDest.y = fAngle;

	//位置の更新
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
	g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;

	//影の移動
	SetPositionShadow(g_aEnemy[nCntEnemy].nIdx, g_aEnemy[nCntEnemy].pos);
}

//=============================================================================
// 敵の設置
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファへのポインタ
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ
	const char *Xfilename = NULL;//読み込むモデル名

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++) {
		if (g_aEnemy[nCntEnemy].bUse == false) {

			//モデルの情報の設定
			//g_aAIEnemy[nCntEnemy].pBuffMat = g_aEnemyData[ntype].pBuffMat;
			//g_aAIEnemy[nCntEnemy].nNumMat = g_aEnemyData[ntype].nNumMat;
			//g_aAIEnemy[nCntEnemy].pMesh = g_aAIEnemyData[ntype].pMesh;

			g_aEnemy[nCntEnemy].bUse = true;
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;

			//マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatEnemy; nCntMat++) {
				if (pMat[nCntMat].pTextureFilename != NULL) {
					for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++) {
						//文字列が一致でテクスチャの番号を決める
						if (strcmp(pMat[nCntMat].pTextureFilename, g_aTextureNameEnemy[nCntTex]) == 0) {
							g_aEnemy[nCntEnemy].aTexType[nCntMat] = nCntTex;
							break;
						}
					}
				}
			}

			//頂点数を取得
			nNumVtx = g_aEnemy[nCntEnemy].pMeshEnemy->GetNumVertices();
			//頂点フォーマットのサイズを取得
			sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMeshEnemy->GetFVF());
			//頂点バッファをロック
			g_aEnemy[nCntEnemy].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++) {
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; 	//頂点座標の代入
															//X座標
				if (g_aEnemy[nCntEnemy].vtxMinEnemy.x > vtx.x || g_aEnemy[nCntEnemy].vtxMinEnemy.x == NULL) {
					g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
				}
				if (g_aEnemy[nCntEnemy].vtxMaxEnemy.x < vtx.x || g_aEnemy[nCntEnemy].vtxMaxEnemy.x == NULL) {
					g_aEnemy[nCntEnemy].vtxMaxEnemy.x = vtx.x;
				}
				//Y座標
				if (g_aEnemy[nCntEnemy].vtxMinEnemy.y > vtx.y || g_aEnemy[nCntEnemy].vtxMinEnemy.y == NULL) {
					g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
				}
				if (g_aEnemy[nCntEnemy].vtxMaxEnemy.y < vtx.y || g_aEnemy[nCntEnemy].vtxMaxEnemy.y == NULL) {
					g_aEnemy[nCntEnemy].vtxMaxEnemy.y = vtx.y;
				}
				//Z座標
				if (g_aEnemy[nCntEnemy].vtxMinEnemy.z > vtx.z || g_aEnemy[nCntEnemy].vtxMinEnemy.z == NULL) {
					g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
				}
				if (g_aEnemy[nCntEnemy].vtxMaxEnemy.z < vtx.z || g_aEnemy[nCntEnemy].vtxMaxEnemy.z == NULL) {
					g_aEnemy[nCntEnemy].vtxMaxEnemy.z = vtx.z;
				}
				pVtxBuff += sizeFVF;	//頂点フォーマットのサイズ分ポインタを進める
			}
			//頂点バッファをアンロック
			g_aEnemy[nCntEnemy].pMeshEnemy->UnlockVertexBuffer();



			break;
		}
	}
}

//=============================================================================
// 敵の頂点座標
//=============================================================================
void VecEnemy(int nCntEnemy)
{
	//変数宣言
	int nNumVtx;			//頂点数
	DWORD sizeFVF;			//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファへのポインタ

							//頂点数の取得
	nNumVtx = g_aEnemy[nCntEnemy].pMeshEnemy->GetNumVertices();

	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMeshEnemy->GetFVF());

	//頂点バッファのロック
	g_aEnemy[nCntEnemy].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//全ての頂点を比較しモデルの最小値、最大値を抜き出す
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.x > vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y > vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.z > vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}

		pVtxBuff += sizeFVF;
	}
	//頂点バッファのアンロック
	g_aEnemy[0].pMeshEnemy->UnlockVertexBuffer();

	//0.0fより小さいとき-1.0倍する
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
		{
		}
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
			g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
}

//=============================================================================
// 敵の当たり判定
//=============================================================================
bool HitEnemy(int nDamage)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 50)
		{
			//プレイヤーダメージのマイナス
			g_aEnemy[nCntEnemy].nLife -= nDamage;
			return true;
		}
	}

	return false;
}

//=============================================================================
// モデルの位置情報の取得
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}