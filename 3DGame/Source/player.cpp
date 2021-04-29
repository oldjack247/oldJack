#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "bullet.h"
#include "math.h"
#include "effect.h"
#include "fade.h"


// マクロ定義
#define PLAYER_MOVE	(1.0f)
#define player_TURN	(0.1f)
#define SPEEDDOWN (0.2)
#define HIT_WALL (750.0f)
#define VTX_MINP (D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f))
#define VTX_MAXP (D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f))
#define PLAYER_LIFE		(100)

//==============================================================================
// グローバル変数
//==============================================================================
Player g_Player;
D3DXVECTOR3 g_lastRot[MODEL_PARTS];	//ひとつ前のg_Player.aModel[].rotの保存
D3DXVECTOR3 g_lastPos;
float g_fdeltaRot[MODEL_PARTS][3];//パーツごとのxyzの角度の差分
float g_fdeltaPos[3];//今のところ位置座標の変更は胴体のみなので１パーツ分のみ
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;
int g_nCntDeath;									//死亡時の待機
int g_nCntEffectDeath = 0;							//死亡エフェクト

// 初期化処理
HRESULT InitPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

													// テクスチャの読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data/TEXTURE/body_d.tga",
		&g_pTexturePlayer);

	// body
	D3DXLoadMeshFromX(
		"data/MODEL/00_body.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[0].pBuffMatModel,
		NULL,
		&g_Player.aModel[0].nNumMatModel,
		&g_Player.aModel[0].pMeshModel);

	// head
	D3DXLoadMeshFromX(
		"data/MODEL/01_head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[1].pBuffMatModel,
		NULL,
		&g_Player.aModel[1].nNumMatModel,
		&g_Player.aModel[1].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/02_armR.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[2].pBuffMatModel,
		NULL,
		&g_Player.aModel[2].nNumMatModel,
		&g_Player.aModel[2].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/03_handR.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[3].pBuffMatModel,
		NULL,
		&g_Player.aModel[3].nNumMatModel,
		&g_Player.aModel[3].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/04_armL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[4].pBuffMatModel,
		NULL,
		&g_Player.aModel[4].nNumMatModel,
		&g_Player.aModel[4].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/05_handL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[5].pBuffMatModel,
		NULL,
		&g_Player.aModel[5].nNumMatModel,
		&g_Player.aModel[5].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/06_legR.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[6].pBuffMatModel,
		NULL,
		&g_Player.aModel[6].nNumMatModel,
		&g_Player.aModel[6].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/07_footR.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[7].pBuffMatModel,
		NULL,
		&g_Player.aModel[7].nNumMatModel,
		&g_Player.aModel[7].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/08_legL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[8].pBuffMatModel,
		NULL,
		&g_Player.aModel[8].nNumMatModel,
		&g_Player.aModel[8].pMeshModel);

	D3DXLoadMeshFromX(
		"data/MODEL/09_footL.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player.aModel[9].pBuffMatModel,
		NULL,
		&g_Player.aModel[9].nNumMatModel,
		&g_Player.aModel[9].pMeshModel);


	//LOCKER
	int nNumVtx;
	DWORD sizeFVF;
	BYTE *pVtxBuff;

	//頂点数
	nNumVtx = g_Player.aModel[0].pMeshModel->GetNumVertices();

	sizeFVF = D3DXGetFVFVertexSize(g_Player.aModel[0].pMeshModel->GetFVF());

	//body parts
	//body
	g_Player.aModel[0].nIdxModelParent = -1;
	g_Player.aModel[0].pos = D3DXVECTOR3(0.0f, 53.0f, 0.0f);
	g_Player.aModel[0].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//head
	g_Player.aModel[1].nIdxModelParent = 0;
	g_Player.aModel[1].pos = D3DXVECTOR3(0.0f, 30.0f, 2.0f);
	g_Player.aModel[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[2].nIdxModelParent = 0;
	g_Player.aModel[2].pos = D3DXVECTOR3(7.5f, 35.0f, 2.5f);
	g_Player.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[3].nIdxModelParent = 2;
	g_Player.aModel[3].pos = D3DXVECTOR3(14.5f, -17.0f, 2.0f);
	g_Player.aModel[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[4].nIdxModelParent = 0;
	g_Player.aModel[4].pos = D3DXVECTOR3(-7.5f, 35.0f, 2.5f);
	g_Player.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[5].nIdxModelParent = 4;
	g_Player.aModel[5].pos = D3DXVECTOR3(-14.5f, -17.0f, 2.0f);
	g_Player.aModel[5].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[6].nIdxModelParent = 0;
	g_Player.aModel[6].pos = D3DXVECTOR3(4.5f, 5.0f, 0.0f);
	g_Player.aModel[6].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[7].nIdxModelParent = 6;
	g_Player.aModel[7].pos = D3DXVECTOR3(0.5f, -30.0f, 4.5f);
	g_Player.aModel[7].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[8].nIdxModelParent = 0;
	g_Player.aModel[8].pos = D3DXVECTOR3(-4.5f, 5.0f, 0.0f);
	g_Player.aModel[8].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player.aModel[9].nIdxModelParent = 8;
	g_Player.aModel[9].pos = D3DXVECTOR3(-0.5f, -30.0f, 4.5f);
	g_Player.aModel[9].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 変数の初期化
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rotDest = 0.0f;
	g_Player.nLife = PLAYER_LIFE;
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.vtxMaxPlayer = VTX_MAXP;
	g_Player.vtxMinPlayer = VTX_MINP;
	g_Player.nNumModel = MODEL_PARTS;
	//g_Player.nShadowIdx = SetShadow(g_Player.pos, SHADOW_SIZE_PLAYER, SHADOW_SIZE_PLAYER);
	g_Player.bMove = false;
	g_Player.bMoveMotion = false;
	g_Player.bFirstMotion = false;
	g_Player.bSetLast = false;
	g_Player.bLand = false;

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++) {
		g_Player.aMotionInfo[nCntMotion].bLoop = false;
		g_Player.aMotionInfo[nCntMotion].nNumKey = 0;
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY; nCntKeyInfo++)
		{
			g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame = 0;
			for (int nCntKey = 0; nCntKey < g_Player.nNumModel; nCntKey++)
			{
				g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosX = 0.0f;
				g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosY = 0.0f;
				g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fPosZ = 0.0f;
				g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotX = 0.0f;
				g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotY = 0.0f;
				g_Player.aMotionInfo[nCntMotion].aKeyInfo[nCntKeyInfo].aKey[nCntKey].fRotZ = 0.0f;
			}

		}
	}

	//Setting Motion

	//====================================================================
	//Idle Motion
	//====================================================================
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].bLoop = true;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey = 2;
	//Frame
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].nFrame = 30;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].nFrame = 30;
	//--------------
	//Key No.1
	//--------------
	//part 0
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotX = -0.05f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotY = -0.25f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[0].fRotZ = 0.00f;
	//part 1
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotX = -0.026;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotY = 0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[1].fRotZ = 0.00f;
	//part 2
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotX = -0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotY = 0.22f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[2].fRotZ = 0.17f;
	//part 3
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosX = -1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosY = -5.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fPosZ = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotX = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotY = -0.236f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[3].fRotZ = -0.09f;
	//part 4
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotX = 0.282f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotY = -0.565f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[4].fRotZ = -0.23f;
	//part 5
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosX = 1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosY = -6.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fPosZ = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotX = 0.38f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotY = 0.528f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[5].fRotZ = -0.067f;
	//part 6
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotX = 0.006f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotY = -0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[6].fRotZ = 0.3187f;
	//part 7
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotY = 0.7f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[7].fRotZ = -0.15f;
	//part 8
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotX = 0.697f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotY = 0.314f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[8].fRotZ = -0.22f;
	//part 9
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotX = -0.7f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotY = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[0].aKey[9].fRotZ = -0.03f;
	//--------------
	//Key No.2
	//--------------
	//part 0
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotX = -0.11f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotY = -0.25f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[0].fRotZ = 0.00f;
	//part 1
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotY = 0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[1].fRotZ = 0.00f;
	//part 2
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotX = -0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotY = 0.22f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[2].fRotZ = 0.3f;
	//part 3
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosX = -1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosY = -5.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotX = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotY = -0.236f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[3].fRotZ = 0.00f;
	//part 4
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotX = 0.282f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotY = -0.565f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[4].fRotZ = -0.3f;
	//part 5
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosX = 1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosY = -6.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fPosZ = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotX = 0.38f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotY = 0.528f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[5].fRotZ = -0.067f;
	//part 6 
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotX = 0.1f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotY = -0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[6].fRotZ = 0.3187f;
	//part 7 
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotY = 0.7f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[7].fRotZ = -0.15f;
	//part 8
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosY = 0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotX = 0.76f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotY = 0.314f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[8].fRotZ = -0.22f;
	//part 9
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotX = -0.7f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotY = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[9].fRotZ = 0.03f;
	//====================================================================
	//Walking Motion
	//====================================================================
	g_Player.aMotionInfo[MOTIONTYPE_WALK].bLoop = true;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].nNumKey = 6;
	//キーの速度設定
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].nFrame = 15;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].nFrame = 15;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].nFrame = 15;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].nFrame = 15;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].nFrame = 15;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].nFrame = 15;
	//キーの角度設定
	//--------------
	//0キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0].fPosY = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0].fRotX = -0.52f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[1].fRotZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2].fRotX = -0.04f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2].fRotY = 0.21f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[2].fRotZ = -0.045f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3].fRotX = 0.191f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3].fRotY = -0.426f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4].fRotX = -0.17f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4].fRotY = 0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[4].fRotZ = -0.14f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5].fPosX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5].fRotX = 0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5].fRotY = 0.121f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[5].fRotZ = 0.5f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6].fPosY = -1.5f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6].fPosZ = -1.7f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6].fRotX = -0.09f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6].fRotY = -0.06f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[6].fRotZ = 0.00f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7].fRotX = -0.81f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7].fRotY = 0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[7].fRotZ = 0.00f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8].fPosY = -1.5f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8].fPosZ = 3.5f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8].fRotX = 1.35f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8].fRotY = -0.004f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[8].fRotZ = 0.00f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9].fRotX = -0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[0].aKey[9].fRotZ = 0.00f;
	//--------------
	//1キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0].fPosY = -2.3f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0].fRotX = -0.6f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[1].fRotZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2].fRotX = 0.117f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2].fRotY = 0.21f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[2].fRotZ = -0.045f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3].fRotX = 0.191f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3].fRotY = -0.426f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4].fPosX = -3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4].fPosZ = 3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4].fRotX = 0.45f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4].fRotY = 0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[4].fRotZ = -0.147f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5].fPosX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5].fRotX = 0.81f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5].fRotY = -0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[5].fRotZ = 0.94f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6].fRotX = -0.05f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6].fRotY = 0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[6].fRotZ = 0.00f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7].fRotX = -0.05f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[7].fRotZ = 0.12f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8].fRotX = 1.8f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8].fRotY = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[8].fRotZ = 0.00f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9].fRotX = -1.6f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[1].aKey[9].fRotZ = 0.00f;
	//--------------
	//2キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0].fPosY = -2.3f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0].fRotX = -0.6f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[1].fRotZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2].fRotX = 0.117f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2].fRotY = 0.21f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[2].fRotZ = -0.045f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3].fRotX = 0.191f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3].fRotY = -0.426f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4].fPosX = -3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4].fPosZ = 3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4].fRotX = 1.08f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4].fRotY = 0.1f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[4].fRotZ = -0.14f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5].fPosX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5].fRotX = 0.25f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5].fRotY = -0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[5].fRotZ = 1.7f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6].fRotX = 0.54f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6].fRotY = -0.06f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[6].fRotZ = 0.00f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7].fRotX = -0.6f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[7].fRotZ = 0.12f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8].fRotX = 1.14f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8].fRotY = 0.064f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[8].fRotZ = 0.00f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9].fRotX = -2.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[2].aKey[9].fRotZ = 0.00f;
	//--------------
	//3キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0].fPosY = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0].fRotX = -0.52f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[1].fPosZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2].fRotX = -0.04f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2].fRotY = 0.21f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[2].fRotZ = -0.045f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3].fRotX = 0.191f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3].fRotY = -0.426f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4].fRotX = -0.17f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4].fRotY = 0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[4].fRotZ = -0.14f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5].fPosX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5].fRotX = 0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5].fRotY = 0.121f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[5].fRotZ = 0.5f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6].fPosY = -5.50f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6].fPosZ = 3.5f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6].fRotX = 1.35f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6].fRotY = 0.0028f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[6].fRotZ = 0.00f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7].fRotX = -0.51f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[7].fRotZ = 0.00f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8].fPosZ = -1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8].fRotX = -0.09f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8].fRotY = -0.06f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[8].fRotZ = 0.00f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9].fRotX = -0.81f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[3].aKey[9].fRotZ = 0.1f;
	//--------------
	//3キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[0].fPosY = -2.3f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[0].fRotX = -0.61f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[0].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[1].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[1].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[1].fRotZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[2].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[2].fRotX = 0.117f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[2].fRotY = 0.21f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[2].fRotZ = -0.045f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[3].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[3].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[3].fRotX = 0.191f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[3].fRotY = -0.426f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[4].fPosX = -3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[4].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[4].fPosZ = 3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[4].fRotX = 1.08f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[4].fRotY = 0.1f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[4].fRotZ = -0.14f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[5].fPosX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[5].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[5].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[5].fRotX = 0.788f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[5].fRotY = -0.31f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[5].fRotZ = 1.41f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[6].fPosY = -3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[6].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[6].fRotX = 1.81f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[6].fRotY = 0.04f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[6].fRotZ = 0.00f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[7].fRotX = -1.62f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[7].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[7].fRotZ = 0.00f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[8].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[8].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[8].fRotX = -0.05f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[8].fRotY = -0.011f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[8].fRotZ = 0.00f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[9].fRotX = 0.02f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[9].fRotY = -0.06f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[4].aKey[9].fRotZ = 0.00f;
	//--------------
	//5キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[0].fPosY = -2.3f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[0].fRotX = -0.6f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[0].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[1].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[1].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[1].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[1].fRotX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[1].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[1].fRotZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[2].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[2].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[2].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[2].fRotX = 0.117f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[2].fRotY = 0.21f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[2].fRotZ = -0.045f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[3].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[3].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[3].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[3].fRotX = 0.191f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[3].fRotY = -0.426f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[4].fPosX = -3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[4].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[4].fPosZ = 3.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[4].fRotX = 1.08f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[4].fRotY = 0.1f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[4].fRotZ = -0.14f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[5].fPosX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[5].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[5].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[5].fRotX = 0.85f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[5].fRotY = -0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[5].fRotZ = 1.7f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[6].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[6].fPosY = -4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[6].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[6].fRotX = 1.14f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[6].fRotY = 0.064f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[6].fRotZ = 0.00f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[7].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[7].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[7].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[7].fRotX = -2.0f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[7].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[7].fRotZ = 0.00f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[8].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[8].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[8].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[8].fRotX = 0.54f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[8].fRotY = -0.06f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[8].fRotZ = 0.00f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[9].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[9].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[9].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[9].fRotX = -0.6f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[9].fRotY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_WALK].aKeyInfo[5].aKey[9].fRotZ = 0.12f;

	g_Player.nNumMotion = MOTIONTYPE_MAX;
	g_Player.motionType = MOTIONTYPE_NEUTRAL;
	g_Player.bLoopMotion = g_Player.aMotionInfo[g_Player.motionType].bLoop;
	g_Player.nNumKey = g_Player.aMotionInfo[g_Player.motionType].nNumKey;
	g_Player.nKey = 0;
	g_Player.nCounterMotion = 0;

	//====================================================================
	//アクションモーション（パンチ）
	//====================================================================
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].bLoop = false;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].nNumKey = 3;
	//キーの速度設定
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].nFrame = 30;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].nFrame = 15;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].nFrame = 15;
	//キーの角度設定
	//--------------
	//0キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[0].fPosX = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[0].fPosY = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[0].fPosZ = 0.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[0].fRotX = -0.05f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[0].fRotY = -0.25f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[0].fRotZ = 0.00f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[1].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[1].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[1].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[1].fRotX = 0.026f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[1].fRotY = 0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[1].fRotZ = 0.00f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[2].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[2].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[2].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[2].fRotX = -0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[2].fRotY = 0.22f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[2].fRotZ = 0.17f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[3].fPosX = -1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[3].fPosY = -5.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[3].fPosZ = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[3].fRotX = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[3].fRotY = -0.236f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[3].fRotZ = -0.09f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[4].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[4].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[4].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[4].fRotX = 0.282f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[4].fRotY = -0.565f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[4].fRotZ = -0.23f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[5].fPosX = 1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[5].fPosY = -6.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[5].fPosZ = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[5].fRotX = 0.38f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[5].fRotY = 0.528f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[5].fRotZ = -0.067f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[6].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[6].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[6].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[6].fRotX = 0.006f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[6].fRotY = -0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[6].fRotZ = 0.3187f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[7].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[7].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[7].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[7].fRotX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[7].fRotY = 0.7f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[7].fRotZ = -0.15f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fRotX = 0.697f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fRotY = 0.314f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fRotZ = -0.22f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[8].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[9].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[9].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[9].fRotX = -0.7f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[9].fRotY = -0.03f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[0].aKey[9].fRotZ = 0.03f;
	//--------------
	//1キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[0].fPosX = 5.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[0].fPosY = -5.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[0].fPosZ = -9.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[0].fRotX = -0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[0].fRotY = -0.46f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[0].fRotZ = 0.02f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[1].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[1].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[1].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[1].fRotX = 0.17f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[1].fRotY = 0.244f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[1].fRotZ = -0.03f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[2].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[2].fPosY = -3.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[2].fPosZ = -4.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[2].fRotX = -0.86f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[2].fRotY = -0.098f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[2].fRotZ = 0.31f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[3].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[3].fPosY = -7.50f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[3].fPosZ = 3.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[3].fRotX = 2.03f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[3].fRotY = 0.095f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[3].fRotZ = 0.72f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[4].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[4].fPosY = -7.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[4].fPosZ = 4.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[4].fRotX = 1.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[4].fRotY = -0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[4].fRotZ = -0.5f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[5].fPosX = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[5].fPosY = -8.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[5].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[5].fRotX = 0.5f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[5].fRotY = -0.4f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[5].fRotZ = -0.45f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[6].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[6].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[6].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[6].fRotX = 0.28f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[6].fRotY = 0.12f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[6].fRotZ = 0.31f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[7].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[7].fPosY = 1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[7].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[7].fRotX = -0.71f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[7].fRotY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[7].fRotZ = 0.0f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[8].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[8].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[8].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[8].fRotX = 1.13f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[8].fRotY = 0.187f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[8].fRotZ = -0.255f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[9].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[9].fPosY = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[9].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[9].fRotX = -0.81f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[9].fRotY = -0.06f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[1].aKey[9].fRotZ = 0.0f;
	//--------------
	//2キー
	//--------------
	//0パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[0].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[0].fPosY = -2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[0].fPosZ = -9.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[0].fRotX = -0.8f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[0].fRotY = 0.3f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[0].fRotZ = 0.02f;
	//1パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[1].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[1].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[1].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[1].fRotX = 0.2f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[1].fRotY = -0.038f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[1].fRotZ = -0.03f;
	//2パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[2].fPosX = 5.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[2].fPosY = -3.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[2].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[2].fRotX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[2].fRotY = 1.78f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[2].fRotZ = 1.00f;
	//3パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[3].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[3].fPosY = -7.50f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[3].fPosZ = 3.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[3].fRotX = 2.03f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[3].fRotY = 0.97f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[3].fRotZ = 0.72f;
	//4パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[4].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[4].fPosY = -7.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[4].fPosZ = 4.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[4].fRotX = -0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[4].fRotY = -0.565f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[4].fRotZ = -0.23f;
	//5パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[5].fPosX = 2.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[5].fPosY = -8.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[5].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[5].fRotX = 0.85f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[5].fRotY = 0.025f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[5].fRotZ = 0.85f;
	//6パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[6].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[6].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[6].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[6].fRotX = 0.122f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[6].fRotY = 0.12f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[6].fRotZ = 0.09f;
	//7パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[7].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[7].fPosY = 1.00f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[7].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[7].fRotX = -0.17f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[7].fRotY = -0.15f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[7].fRotZ = 0.0314f;
	//8パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[8].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[8].fPosY = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[8].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[8].fRotX = 1.91f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[8].fRotY = -0.25f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[8].fRotZ = -0.255f;
	//9パーツ
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[9].fPosX = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[9].fPosY = 2.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[9].fPosZ = 0.0f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[9].fRotX = -2.2f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[9].fRotY = 2.48f;
	g_Player.aMotionInfo[MOTIONTYPE_ACTION].aKeyInfo[2].aKey[9].fRotZ = -2.73f;

	g_Player.nNumMotion = MOTIONTYPE_MAX;
	g_Player.motionType = MOTIONTYPE_NEUTRAL;
	g_Player.bLoopMotion = g_Player.aMotionInfo[g_Player.motionType].bLoop;
	g_Player.nNumKey = g_Player.aMotionInfo[g_Player.motionType].nNumKey;
	g_Player.nKey = 0;
	g_Player.nCounterMotion = 0;

	// 影の設定
	//g_Player.nIdx = SetShadow(D3DXVECTOR3(g_Player.pos.x, 0.0f, g_Player.pos.z), 10.0f, 10.0f);

	g_Player.aModel[0].pMeshModel->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//
		if (g_Player.vtxMaxPlayer.x < vtx.x)
		{
			g_Player.vtxMaxPlayer.x = vtx.x;
		}
		if (g_Player.vtxMaxPlayer.y < vtx.y)
		{
			g_Player.vtxMaxPlayer.y = vtx.y;
		}
		if (g_Player.vtxMaxPlayer.z < vtx.z)
		{
			g_Player.vtxMaxPlayer.z = vtx.z;
		}

		//
		if (g_Player.vtxMinPlayer.x > vtx.x)
		{
			g_Player.vtxMinPlayer.x = vtx.x;
		}
		if (g_Player.vtxMinPlayer.y > vtx.y)
		{
			g_Player.vtxMinPlayer.y = vtx.y;
		}
		if (g_Player.vtxMinPlayer.z > vtx.z)
		{
			g_Player.vtxMinPlayer.z = vtx.z;
		}

		pVtxBuff += sizeFVF;
	}

	g_Player.aModel[0].pMeshModel->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// 終了処理
//==============================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < g_Player.nNumModel; nCnt++)
	{
		// メッシュの破棄
		if (g_Player.aModel[nCnt].pMeshModel != NULL)
		{
			g_Player.aModel[nCnt].pMeshModel->Release();
			g_Player.aModel[nCnt].pMeshModel = NULL;
		}

		// マテリアルの破棄
		if (g_Player.aModel[nCnt].pBuffMatModel != NULL)
		{
			g_Player.aModel[nCnt].pBuffMatModel->Release();
			g_Player.aModel[nCnt].pBuffMatModel = NULL;
		}
	}
}

//==============================================================================
// 更新処理
//==============================================================================
void UpdatePlayer(void)
{
	// カメラの取得
	Camera camera = GetCamera();
	static int nCntUp, nCntDown, nCntRight, nCntLeft = 0;
	bool bPushKey = false;
	float fPushTime = 10;
	double dSqrt = sqrt(2);		// 2の平方根

	// 位置のリセット
	if (GetKeyboardTrigger(KEYINFO_PRESET) == true)
	{
		g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player.rotDest = 0.0f;
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// 弾の発射
	if (GetKeyboardPress(KEYINFO_SHOT) == true)
	{
		SetBullet(
			D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 88.0f, g_Player.pos.z),
			D3DXVECTOR3(sinf(D3DX_PI - g_Player.rot.y) * -5.0f, 0.0f, cosf(D3DX_PI - g_Player.rot.y) * 5.0f),
			100);
	}

	// 移動量の加算
	g_Player.pos += g_Player.move;

	// 影の追従
	SetPositionShadow(g_Player.nIdx, D3DXVECTOR3(g_Player.pos.x, 0.0f, g_Player.pos.z));

	// モデルの移動
	if (GetKeyboardPress(KEYINFO_UP) == true)
	{
		g_Player.move.x += sinf(camera.rot.y) * PLAYER_MOVE;
		g_Player.move.z += cosf(camera.rot.y) * PLAYER_MOVE;
		g_Player.rotDest = camera.rot.y + D3DX_PI;
		nCntDown = fPushTime;
		bPushKey = true;
		if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// 左上方向
			g_Player.move.x -= cosf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_Player.move.z += sinf(camera.rot.y + D3DX_PI / 4) * PLAYER_MOVE;
			g_Player.rotDest = camera.rot.y + (D3DX_PI * 3 / 4);
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// 右上方向
			g_Player.move.x += cosf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_Player.move.z -= sinf(camera.rot.y - D3DX_PI / 4) * PLAYER_MOVE;
			g_Player.rotDest = camera.rot.y - (D3DX_PI * 3 / 4);
		}
	}
	else if (GetKeyboardPress(KEYINFO_DOWN) == true)
	{
		g_Player.move.x += sinf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
		g_Player.move.z += cosf(camera.rot.y - D3DX_PI) * PLAYER_MOVE;
		g_Player.rotDest = camera.rot.y;
		nCntUp = fPushTime;
		bPushKey = true;
		if (GetKeyboardPress(KEYINFO_LEFT) == true)
		{// 左下方向
			g_Player.move.x += cosf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_Player.move.z -= sinf(camera.rot.y + D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_Player.rotDest = camera.rot.y + D3DX_PI / 4;
		}
		else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
		{// 右下方向
			g_Player.move.x -= cosf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_Player.move.z += sinf(camera.rot.y - D3DX_PI * 3 / 4) * PLAYER_MOVE;
			g_Player.rotDest = camera.rot.y + D3DX_PI / -4;
		}
	}
	else if (GetKeyboardPress(KEYINFO_LEFT) == true)
	{// 左方向
		g_Player.move.x -= cosf(camera.rot.y) * PLAYER_MOVE;
		g_Player.move.z += sinf(camera.rot.y) * PLAYER_MOVE;
		g_Player.rotDest = camera.rot.y + (D3DX_PI / 2);
		nCntRight = fPushTime;
		bPushKey = true;
	}
	else if (GetKeyboardPress(KEYINFO_RIGHT) == true)
	{// 右方向
		g_Player.move.x += cosf(camera.rot.y) * PLAYER_MOVE;
		g_Player.move.z -= sinf(camera.rot.y) * PLAYER_MOVE;
		g_Player.rotDest = camera.rot.y + (D3DX_PI / -2);
		nCntLeft = fPushTime;
		bPushKey = true;
	}

	// モデルの上昇・下降
	if (GetKeyboardPress(KEYINFO_UP_MODEL) == true)
	{
		g_Player.move.y = 2;
	}
	else if (GetKeyboardPress(KEYINFO_DOWN_MODEL) == true)
	{
		g_Player.move.y = -2;
	}
	else
	{
		g_Player.move.y = 0;
	}

	// モデルの旋回
	if (GetKeyboardPress(KEYINFO_TURN_LEFT_MODEL) == true)
	{
		g_Player.rotDest += D3DX_PI / 18;
	}
	if (GetKeyboardPress(KEYINFO_TURN_RIGHT_MODEL) == true)
	{
		g_Player.rotDest -= D3DX_PI / 18;
	}

	// 目的の回転角の上限
	if (g_Player.rotDest - g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rotDest += D3DX_PI * 2.0f;
	}
	else if (g_Player.rotDest - g_Player.rot.y > D3DX_PI)
	{
		g_Player.rotDest -= D3DX_PI * 2.0f;
	}

	// 向きの更新
	g_Player.rot.y += (g_Player.rotDest - g_Player.rot.y) * 0.1f;

	// 現在の回転角の上限
	if (g_Player.rot.y < -D3DX_PI)
	{
		g_Player.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_Player.rot.y > D3DX_PI)
	{
		g_Player.rot.y -= D3DX_PI * 2.0f;
	}

	//アクションモーション中は移動不可
	if (g_Player.motionType == MOTIONTYPE_ACTION) {
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		bPushKey = false;
		g_Player.bMoveMotion = false;
	}

	//移動の反映

	g_Player.bMove = bPushKey;

	//アクションモーション
	if (g_Player.motionType == MOTIONTYPE_ACTION) {
		g_Player.rotDest = g_Player.rot.y;
	}

	if (nCntUp > 0) {
		nCntUp--;
	}
	if (nCntDown > 0) {
		nCntDown--;
	}
	if (nCntRight > 0) {
		nCntRight--;
	}
	if (nCntLeft > 0) {
		nCntLeft--;
	}

	//パンチ
	if (GetKeyboardPress(KEYINFO_SHOT) && g_Player.motionType != MOTIONTYPE_ACTION) {
		SetMotion(MOTIONTYPE_ACTION);
	}

	//===================================================
	//モーション設定
	//===================================================

	//移動時と通常時の切り替え
	if (g_Player.bMoveMotion == false && g_Player.bMove == true) {
		SetMotion(MOTIONTYPE_WALK);
		//g_Player.bDash = false;
		g_Player.bMoveMotion = true;
	}
	if (g_Player.bMove == false && g_Player.bMoveMotion == true) {
		SetMotion(MOTIONTYPE_NEUTRAL);
		g_Player.bMoveMotion = false;
	}

	//今のモデルの角度から次のモーションの始めの角度へ回転
	if (g_Player.bFirstMotion == false) {
		g_Player.nCounterMotion++;
		//現在のモデルのパーツごとの位置と角度の保存
		if (g_Player.bSetLast == false) {
			for (int nCntModel = 0; nCntModel < MODEL_PARTS; nCntModel++) {
				g_lastRot[nCntModel] = g_Player.aModel[nCntModel].rot;
			}
			g_lastPos = g_Player.aModel[0].pos;
			g_Player.bSetLast = true;

			//現在の角度と目標の角度の差分を求める
			for (int nCntModel = 0; nCntModel < MODEL_PARTS; nCntModel++) {
				g_fdeltaRot[nCntModel][0] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotX - g_lastRot[nCntModel].x;
				g_fdeltaRot[nCntModel][1] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotY - g_lastRot[nCntModel].y;
				g_fdeltaRot[nCntModel][2] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotZ - g_lastRot[nCntModel].z;
			}
			//位置の差分
			g_fdeltaPos[0] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].aKey[0].fPosX - g_lastPos.x;
			g_fdeltaPos[1] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].aKey[0].fPosY - g_lastPos.y + 49.0f;
			g_fdeltaPos[2] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].aKey[0].fPosZ - g_lastPos.z;

		}

		//モデルのパーツごとの角度に差分を足していく
		for (int nCntModel = 0; nCntModel < MODEL_PARTS; nCntModel++) {
			g_Player.aModel[nCntModel].rot.x = g_lastRot[nCntModel].x + g_fdeltaRot[nCntModel][0] * g_Player.nCounterMotion / g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame;
			g_Player.aModel[nCntModel].rot.y = g_lastRot[nCntModel].y + g_fdeltaRot[nCntModel][1] * g_Player.nCounterMotion / g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame;
			g_Player.aModel[nCntModel].rot.z = g_lastRot[nCntModel].z + g_fdeltaRot[nCntModel][2] * g_Player.nCounterMotion / g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame;


		}
		//位置の反映
		g_Player.aModel[0].pos.x = g_lastPos.x + g_fdeltaPos[0] * g_Player.nCounterMotion / g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame;
		g_Player.aModel[0].pos.y = g_lastPos.y + g_fdeltaPos[1] * g_Player.nCounterMotion / g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame;
		g_Player.aModel[0].pos.z = g_lastPos.z + g_fdeltaPos[2] * g_Player.nCounterMotion / g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame;

		if (g_Player.nCounterMotion >= g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[0].nFrame) {
			g_Player.nKey = 0;
			g_Player.nCounterMotion = 0;
			g_Player.bFirstMotion = true;
		}
	}
	else if (g_Player.nKey < g_Player.aMotionInfo[g_Player.motionType].nNumKey - 1) {
		g_Player.nCounterMotion++;
		//現在の角度と目標の角度の差分を求める
		for (int nCntModel = 0; nCntModel < MODEL_PARTS; nCntModel++) {
			g_fdeltaRot[nCntModel][0] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[nCntModel].fRotX - g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotX;
			g_fdeltaRot[nCntModel][1] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[nCntModel].fRotY - g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotY;
			g_fdeltaRot[nCntModel][2] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[nCntModel].fRotZ - g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotZ;
		}
		//位置の差分
		g_fdeltaPos[0] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[0].fPosX - g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0].fPosX;
		g_fdeltaPos[1] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[0].fPosY - g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0].fPosY;
		g_fdeltaPos[2] = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[0].fPosZ - g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0].fPosZ;

		//モデルのパーツごとの角度に差分を足していく
		for (int nCntModel = 0; nCntModel < MODEL_PARTS; nCntModel++) {
			//g_Player.aModel[nCntModel].rot.x = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotX - g_fdeltaRot[nCntModel][0] * g_Player.nCounterMotion / 
			//	g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
			//g_Player.aModel[nCntModel].rot.y = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotY - g_fdeltaRot[nCntModel][1] * g_Player.nCounterMotion / 
			//	g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
			//g_Player.aModel[nCntModel].rot.z = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotZ - g_fdeltaRot[nCntModel][2] * g_Player.nCounterMotion / 
			//	g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;

			g_Player.aModel[nCntModel].rot.x = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotX + g_fdeltaRot[nCntModel][0] * g_Player.nCounterMotion /
				g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
			g_Player.aModel[nCntModel].rot.y = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotY + g_fdeltaRot[nCntModel][1] * g_Player.nCounterMotion /
				g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
			g_Player.aModel[nCntModel].rot.z = g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[nCntModel].fRotZ + g_fdeltaRot[nCntModel][2] * g_Player.nCounterMotion /
				g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
		}
		//位置の反映
		//g_Player.aModel[0].pos.x = 0.0f + g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[0].fPosX + g_fdeltaPos[0] * g_Player.nCounterMotion / 
		//	g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].nFrame;
		//g_Player.aModel[0].pos.y = 27.0f + g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[0].fPosY + g_fdeltaPos[1] * g_Player.nCounterMotion / 
		//	g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].nFrame;
		//g_Player.aModel[0].pos.z = 0.0f + g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].aKey[0].fPosZ + g_fdeltaPos[2] * g_Player.nCounterMotion / 
		//	g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].nFrame;

		g_Player.aModel[0].pos.x = 0.0f + g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0].fPosX + g_fdeltaPos[0] * g_Player.nCounterMotion /
			g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
		g_Player.aModel[0].pos.y = 49.0f + g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0].fPosY + g_fdeltaPos[1] * g_Player.nCounterMotion /
			g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;
		g_Player.aModel[0].pos.z = 0.0f + g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey].aKey[0].fPosZ + g_fdeltaPos[2] * g_Player.nCounterMotion /
			g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame;

		if (g_Player.nCounterMotion >= g_Player.aMotionInfo[g_Player.motionType].aKeyInfo[g_Player.nKey + 1].nFrame) {
			g_Player.nCounterMotion = 0;
			g_Player.nKey++;
		}
	}
	//最後のキー終了後
	else {
		//ループモーションの場合
		if (g_Player.bLoopMotion == true) {
			SetMotion(g_Player.motionType);
		}
		//ループモーションでは無い場合
		else {
			//ジャンプモーション
			if (g_Player.motionType == MOTIONTYPE_JUMP) {
				//着地まで硬直
			}
			//アクションや着地など
			else {
				if (g_Player.bMoveMotion == false) {
					SetMotion(MOTIONTYPE_NEUTRAL);
				}
				else if (g_Player.bMoveMotion == true) {
					SetMotion(MOTIONTYPE_WALK);
				}
			}
		}
	}

	// 加速後の減速処理
	g_Player.move.x += (0.0f - g_Player.move.x) * SPEEDDOWN;
	g_Player.move.z += (0.0f - g_Player.move.z) * SPEEDDOWN;

	//壁の設定
	if (g_Player.pos.x < -HIT_WALL)
	{
		//左端
		g_Player.pos.x = -HIT_WALL;
	}
	if (g_Player.pos.x > HIT_WALL)
	{
		//右端
		g_Player.pos.x = HIT_WALL;
	}
	if (g_Player.pos.z > HIT_WALL)
	{
		//奥
		g_Player.pos.z = HIT_WALL;
	}
	if (g_Player.pos.z < -HIT_WALL)
	{
		//手前
		g_Player.pos.z = -HIT_WALL;
	}
	if (g_Player.nLife <= 0)
	{
		g_Player.bUse = false;

		g_nCntDeath++;
		if (g_nCntDeath == 10)
		{
			SetFade(FADE_OUT, MODE_RESULT);	//リザルト画面に切り替え
		}
	}

}

//==============================================================================
// 描画処理
//==============================================================================
void DrawPlayer(void)
{
	// ローカル変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				// 計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

												// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	// 向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	for (int nCntModel = 0; nCntModel < g_Player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotModel, mtxTransModel;
		D3DXMATRIX mtxParent;

		D3DXMatrixIdentity(&g_Player.aModel[nCntModel].mtxWorld);
		// 向きの反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_Player.aModel[nCntModel].rot.y, g_Player.aModel[nCntModel].rot.x, g_Player.aModel[nCntModel].rot.z);
		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxRotModel);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTransModel, g_Player.aModel[nCntModel].pos.x, g_Player.aModel[nCntModel].pos.y, g_Player.aModel[nCntModel].pos.z);
		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxTransModel);

		if (g_Player.aModel[nCntModel].nIdxModelParent != -1)
		{
			if (g_Player.aModel[nCntModel].nIdxModelParent == 0)
			{
				mtxParent = g_Player.aModel[0].mtxWorld;
			}
			if (g_Player.aModel[nCntModel].nIdxModelParent != 0)
			{
				mtxParent = g_Player.aModel[nCntModel - 1].mtxWorld;
			}
		}
		else
		{
			mtxParent = g_Player.mtxWorld;
		}

		D3DXMatrixMultiply(&g_Player.aModel[nCntModel].mtxWorld, &g_Player.aModel[nCntModel].mtxWorld, &mtxParent);

		pDevice->SetTransform(D3DTS_WORLD, &g_Player.aModel[nCntModel].mtxWorld);

		pMat = (D3DXMATERIAL*)g_Player.aModel[nCntModel].pBuffMatModel->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_Player.aModel[nCntModel].nNumMatModel; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePlayer);

			// モデル(パーツ)の描画
			g_Player.aModel[nCntModel].pMeshModel->DrawSubset(nCntMat);
		}

	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

void SetMotion(MOTIONTYPE type) {
	//モーションタイプの設定
	g_Player.motionType = type;
	//ループするかどうかの設定
	g_Player.bLoopMotion = g_Player.aMotionInfo[type].bLoop;
	//キーの初期化
	g_Player.nKey = 0;
	//カウンターの初期化
	g_Player.nCounterMotion = 0;
	//モーションの継ぎ目
	g_Player.bFirstMotion = false;
	//モーションの前の角度の保存
	g_Player.bSetLast = false;
}

// プレイヤーの当たり判定
bool HitPlayer(int nDamage)
{
	if (g_Player.nLife <= 100)
	{
		//プレイヤーダメージのマイナス
		g_Player.nLife -= nDamage;
		return true;
	}

	if (g_Player.nLife = 0)
	{
		//使ったことにする
		g_Player.bUse = false;

		//影を消す
		//DeleteShadow(g_Player.nShadow);

		g_nCntEffectDeath++;

		if (g_nCntEffectDeath % 1 == 0)
		{
			for (int g_nCntEffect = 0; g_nCntEffect < 10; g_nCntEffect++)
			{
				//角度の設定
				float fAngle = ((float)(rand() % 800)) / 100.0f;
				float fmove = (float)(rand() % 1 + 1);

				//エフェクトの設定
				SetEffect(g_Player.pos,
					D3DXVECTOR3(sinf(fAngle) * fmove, 5, cosf(fAngle) * fmove),
					D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),
					1.0f,
					5.0f,
					0.01f,
					0.2f);
			}
		}
	}
	return false;
}


// モデルの取得
Player *GetPlayer(void)
{
	return &g_Player;
}