#include "game.h"
#include "Player.h"
#include "background.h"
#include "Bullet.h"
#include "Explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "Effect.h"
#include "life.h"
#include "pause.h"
#include <stdlib.h>
#include <time.h>

//グローバル変数
bool g_bPause = false;

// 初期化処理
HRESULT InitGame(void)
{

	//背景の初期化
	InitBG();

	//ポリゴンの初期化処理
	InitPlayer();

	//弾の初期化処理
	InitBullet();

	//エフェクトの初期化処理
	InitEffect();

	//敵の初期化処理
	InitEnemy();

	//爆発の初期化
	InitExplosion();

	//スコアの初期化
	InitScore();

	//ライフテクスチャの初期化
	InitLife();

	//ポーズの初期化処理
	InitPause();

	//サウンドスタート
	PlaySound(SOUND_LABEL_BGM003);

	return S_OK;
}

// 終了処理
void UninitGame(void)
{
	//サウンドストップ
	StopSound();

	//ポーズの終了処理
	UninitPause();

	//ライフテクスチャの終了処理
	//UninitLife();

	//スコアの終了処理
	UninitScore();

	//爆破の終了処理
	UninitExplosion();

	//敵の終了処理
	UninitEnemy();

	//エフェクトの終了処理
	UninitEffect();

	//弾の終了処理
	UninitBullet();

	//プレイヤーの終了処理
	UninitPlayer();

	//体力表示の終了処理
	UninitLife();

	//背景の終了
	UninitBG();
}

// 更新処理
void UpdateGame(void)
{
	FADE *pFade;
	pFade = GetFade();

	srand((unsigned int)time(NULL));

	if (GetKeyboardTrigger(DIK_P) == true)
	{
		if (*pFade == FADE_NONE)
		{
			g_bPause = g_bPause ? false : true;
		}
	}

	SetPause(g_bPause);

	if (g_bPause == true)
	{
		//ポーズの更新処理
		UpdatePause();
	}
	else if (g_bPause == false)
	{
		//背景の更新処理
		UpdateBG();

		//ポリゴンの更新処理
		UpdatePlayer();

		//弾の更新処理
		UpdateBullet();

		//エフェクトの更新処理
		UpdateEffect();

		//敵の更新処理
		UpdateEnemy();

		//敵の更新処理
		UpdateExplosion();

		//ライフテクスチャの更新処理		
		UpdateLife();

		//スコアの更新処理
		UpdateScore();
	}

}

// 描画処理
void DrawGame(void)
{
	//背景の描画
	DrawBG();

	//ポリゴンの描画処理
	DrawPlayer();

	//弾の描画処理
	DrawBullet();

	//エフェクトの描画処理
	DrawEffect();

	//敵の描画処理
	DrawEnemy();

	//爆発の描画処理
	DrawExplosion();

	//ライフテクスチャの描画
	DrawLife();

	//スコアの描画処理
	DrawScore();


	if (g_bPause == true)
	{
		DrawPause();
	}

}

void SetPause(bool bpause)
{
	g_bPause = bpause;
}