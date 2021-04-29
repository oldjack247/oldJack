#include "wall.h"
#include "input.h"

//マクロ定義
#define POLYGON_WIDTH (100.0f)
#define POLYGON_HEIGHT (100.0f)
#define MAX_WALL (36)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;		//テクスチャへのポインタ
Wall g_aWall[MAX_WALL];

HRESULT InitWall(void) {
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//デバイスの取得
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/wall000.jpg",
		&g_pTextureWall);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++) {
		g_aWall[nCntWall].bUse = false;
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-100.0f, 100.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffWall->Unlock();

	//Up Front
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(400.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(600.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(700.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-200.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-400.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-600.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-700.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	

	//Right Side
	SetWall(D3DXVECTOR3(800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(800.0f, 0.0f, -700.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f, 100.0f);

	//Left Side
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, 600.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, 700.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, -600.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-800.0f, 0.0f, -700.0f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 100.0f, 100.0f);

	//Down Side
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(400.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(600.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(700.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-400.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-600.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);
	SetWall(D3DXVECTOR3(-700.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100.0f, 100.0f);

	return S_OK;
}

void UninitWall(void) {
	//テクスチャの破棄
	if (g_pTextureWall != NULL) {
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWall != NULL) {
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

void UpdateWall(void) {

}

void DrawWall(void) {
	//描画処理
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

									//デバイスの取得
	pDevice = GetDevice();
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++) {
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aWall[nCntWall].mtxWorld);
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxRot);
		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtxWorld, &g_aWall[nCntWall].mtxWorld, &mtxTrans);
		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtxWorld);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall); //テクスチャの設定
												//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			nCntWall * 4,
			2);
	}
}

void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight) {
	VERTEX_3D *pVtx; //頂点情報へのポインタ

	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++) {
		if (g_aWall[nCntWall].bUse == false) {
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;

			//頂点バッファをロックし、頂点データへのポインタを取得
			/*
			pVtx[0].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - fWidth, g_aWall[nCntWall].pos.y + fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + fWidth, g_aWall[nCntWall].pos.y + fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x - fWidth, g_aWall[nCntWall].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aWall[nCntWall].pos.x + fWidth, g_aWall[nCntWall].pos.y, 0.0f);
			*/

			g_pVtxBuffWall->Unlock();

			g_aWall[nCntWall].bUse = true;
			break;
		}
		pVtx += 4;
	}
}