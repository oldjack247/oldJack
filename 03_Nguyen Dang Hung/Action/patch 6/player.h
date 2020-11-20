#ifndef _PLAYER_H_
#define _PLAYER_H_

//マクロ定義
#define POLYGON_WIDTH (50.0f)		//プレイヤーの幅
#define POLYGON_HEIGHT (50.0f)		//プレイヤーの高さ
#define POLYGON_SPEED (5.0f)			//プレイヤーの速さ
#define POLYGON_JUMP_SPEED (1.5f)		//ジャンプ中プレイヤーの速さ
#define GRAVITY (0.5f)					//重力
#define POLYGON_JUMP (-14.0f)			//ジャンプの高さ
#define POLYGON_TYPE (1)				//プレイヤーの種類
#define SCROLE_START_POS (SCREEN_WIDTH / 2.0f)		//スクロールが始まる位置

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NOMAL = 0,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 BeforePos;	//前の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 BeforeMove;	//前の移動量
	float fWidth;			//ブロックの横幅
	float fHeight;			//ブロックの縦幅
	bool bUse;				//使うかどうか
	bool bJump;				//ジャンプ制限
	int nCntAnime;			// アニメーションカウント
	int nAnimSpan;
	bool bRight;			// 左右の向き判定
	float fAnime;			// プレイヤーのアニメーション遷移
	PLAYERSTATE State;		//プレイヤーの状態
	int nRemaining;			//残機
	int nCounterState;		//状態管理のカウンター
	bool bMoveR;			//右向き
	bool bMoveL;			//左向き
	int nMoveTexX;			//横に移動
	int nMoveTexY;			//左右振り向き
	int nCntTex;			//横移動のカウンター
	bool bScroll;			//スクロール
} PLAYER;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(void);
void PlayerAnim(void);					
PLAYER *GetPlayer(void);

#endif // !_PLAYER_H_