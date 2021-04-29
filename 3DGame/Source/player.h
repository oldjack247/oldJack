#ifndef _PLAYER_H
#define _PLAYER_H
#include "main.h"

#define MODEL_PARTS (10)
#define MAX_KEY (10)	
#define MAX_MOTION (10)	

typedef struct
{
	LPD3DXMESH pMeshModel;
	LPD3DXBUFFER pBuffMatModel;
	DWORD nNumMatModel;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	int nIdxModelParent;
}Model;


//モーションの種類
typedef enum {
	MOTIONTYPE_NEUTRAL = 0,	//ニュートラルモーション
	MOTIONTYPE_WALK,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_MAX
} MOTIONTYPE;

//キーの構造体
typedef struct
{
	float fPosX;	//位置X
	float fPosY;;	//位置Y
	float fPosZ;	//位置Z
	float fRotX;	//向きX
	float fRotY;	//向きY
	float fRotZ;	//向きZ
} KEY;

//キー情報の構造体
typedef struct {
	int nFrame;	//再生フレーム
	KEY aKey[MODEL_PARTS];	//各パーツのキー要素
} KEY_INFO;

//モーション情報の構造体
typedef struct {
	bool bLoop;	//ループするかどうか
	int nNumKey;	//キーの総数
	KEY_INFO aKeyInfo[MAX_KEY];	//キー情報
}MOTION_INFO;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	float rotDest;
	//float fdestRot;
	int nIdx;
	int nShadow;
	int nLife;
	bool bLand;
	Model aModel[MODEL_PARTS];
	int nNumModel = MODEL_PARTS;				//Number of body parts
	MOTION_INFO aMotionInfo[MAX_MOTION];
	int nNumMotion;
	MOTIONTYPE motionType;
	bool bLoopMotion;
	int nNumKey;
	int nKey;
	int nCounterMotion;
	bool bUse;
	bool bMove;
	bool bMoveMotion;
	bool bFirstMotion;				//モーションの移り変わりのはじめ
	bool bSetLast;					//lastRot,lastPosを設定したかどうか
	D3DXVECTOR3 vtxMinPlayer, vtxMaxPlayer;
}Player;


HRESULT InitPlayer(void);	// 3Dモデルの初期化処理
void UninitPlayer(void);	// 3Dモデルの終了処理
void UpdatePlayer(void);	// 3Dモデルの更新処理
void DrawPlayer(void);		// 3Dモデルの描画処理
void SetMotion(MOTIONTYPE type);
D3DXVECTOR3 GetPlayerRot(void);
Player *GetPlayer(void);		// 3Dモデルの取得
bool HitPlayer(int nDamage);

#endif