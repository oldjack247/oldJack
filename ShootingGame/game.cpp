//-----------------------------------------------------------------------------
//Author:Nguyen Dang Hung
//-----------------------------------------------------------------------------
#include "game.h"
#include "player.h"
#include "background.h"
#include "Bullet.h"
#include "Explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "Effect.h"
#include "subplayer.h"
#include "life.h"
#include "pause.h"
#include "Time.h"
#include "EnemyCounter.h"

//-----------------------------------------------------------------------------
//グローバル変数
//-----------------------------------------------------------------------------
bool g_bPause = false;

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
HRESULT InitGame(void)
{
	//背景の初期化
	InitBG();

	//ポリゴンの初期化処理
	InitPlayer();

	//子機の初期化処理
	//InitSubplayer();

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

	//時間の初期化
	InitTime();

	//命の初期化
	InitLife();

	//敵のカウンターの初期化
	InitEnemyCounter();

	//ポーズの初期化処理
	InitPause();

	

	//サウンドスタート
	PlaySound(SOUND_LABEL_BGM003);

	return S_OK;
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void UninitGame(void)
{
	//サウンドストップ
	StopSound();

	//ポーズの終了処理
	UninitPause();

	//スコアの終了処理
	UninitScore();

	//時間の終了処理
	UninitTime();

	//敵のカウンターの終了処理
	UninitEnemyCounter();

	//爆破の終了処理
	UninitExplosion();

	//敵の終了処理
	UninitEnemy();

	//命の終了処理
	UninitLife();

	//エフェクトの終了処理
	UninitEffect();

	//弾の終了処理
	UninitBullet();

	//子機の終了処理
	//UninitSubplayer();

	//プレイヤーの終了処理
	UninitPlayer();

	//背景の終了
	UninitBG();
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void UpdateGame(void)
{

	FADE* pFade;
	pFade = GetFade();

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

	else if (g_bPause == false && *pFade == FADE_NONE)
	{
		//背景の更新処理
		UpdateBG();

		//ポリゴンの更新処理
		UpdatePlayer();

		//子機の更新処理
		//UpdateSubplayer();

		//弾の更新処理
		UpdateBullet();

		//エフェクトの更新処理
		UpdateEffect();

		//敵の更新処理
		UpdateEnemy();

		//命の更新処理
		UpdateLife();

		//敵の更新処理
		UpdateExplosion();

		//スコアの更新処理
		UpdateScore();

		//時間の更新処理
		UpdateTime();

		//敵のカウンターの更新処理
		UpdateEnemyCounter();
	}

	/*if (GetKeyboardTrigger(DIK_L) == true)
	{
	SetFade(FADE_OUT, MODE_RESULT);
	}*/
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void DrawGame(void)
{
	//背景の描画
	DrawBG();

	//ポリゴンの描画処理
	DrawPlayer();

	//子機の描画処理
	//DrawSubplayer();

	//弾の描画処理
	DrawBullet();

	//エフェクトの描画処理
	DrawEffect();

	//敵の描画処理
	DrawEnemy();

	//命の描画処理
	DrawLife();

	//爆発の描画処理
	DrawExplosion();

	//スコアの描画処理
	DrawScore();

	//時間の描画処理
	DrawTime();

	//敵のカウンターの描画処理
	DrawEnemyCounter();

	if (g_bPause == true)
	{
		DrawPause();
	}

}

void SetPause(bool bpause)
{
	g_bPause = bpause;
}