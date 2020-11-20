#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "life.h"
#include "score.h"

// グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//頂点情報
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//テクスチャ
int g_nCounterAnim;									//アニメーションカウンター
int g_nPatternAnim;									//アニメーションパターンNo.
D3DXVECTOR3 g_rotPlayer;							//
float g_fLengthPlayer;								//対角線の長さ
float g_fAnglePlayer;								//対角線の角度
PLAYER g_Player;
int g_nCounterPlayer;
int g_nCounterWait;
int g_nCounterDeath;

// プレイヤーの初期化処理
HRESULT InitPlayer(void)
{
	//ローカル変数
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spaceship002.png", &g_pTexturePlayer);

	//構造体初期化
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.State = PLAYERSTATE_APPEAR;				//状態
	g_Player.nCounterState = 0;							//状態カウンター
	g_Player.nLife = 4;									//体力
	g_Player.nRemaining = 3;							//残機
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	g_Player.bDisp = true;								//表示状態
	g_nCounterPlayer = 0;
	g_nCounterDeath = 0;


	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点ポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	SetVertexPlayer();

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

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

// プレイヤーの終了処理
void UninitPlayer(void)
{
	//バッファの開放
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//テクスチャの開放
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

// プレイヤーの更新処理
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//Aを押したとき
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += -MOVE_NUMBER;
		g_Player.pos.y += 0;
		g_Player.pos.z += 0.0f;

		//加速量
		g_Player.move.x += -sinf(g_fAnglePlayer) * 2;
	}

	//AとWを同時に押したとき
	if (GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += -sinf(D3DX_PI / 4) * 0.2f;
		g_Player.pos.y += -cosf(D3DX_PI / 2) * 0.2f;
	}

	//AとSを同時に押したとき
	if (GetKeyboardPress(DIK_S) == true && GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += -sinf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
		g_Player.pos.y += cosf(D3DX_PI / 2) * MOVE_NUMBER * 0.2f;
	}

	//Dを押したとき
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += MOVE_NUMBER;
		g_Player.pos.y += 0;
		g_Player.pos.z += 0.0f;

		//加速量
		g_Player.move.x += sinf(g_fAnglePlayer) * 2;
	}

	//DとWを同時に押したとき
	if (GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += sinf(D3DX_PI / 2) * MOVE_NUMBER * 0.2f;
		g_Player.pos.y += -cosf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
	}

	//DとSを同時に押したとき
	if (GetKeyboardPress(DIK_S) == true && GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += sinf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
		g_Player.pos.y += cosf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
	}

	//Wを押したとき
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_Player.pos.x += 0;
			g_Player.pos.y += -MOVE_NUMBER;
			g_Player.pos.z += 0.0f;
		}

		//加速量
		g_Player.move.y -= cosf(g_fAnglePlayer) * 2;
	}

	//Sを押したとき
	if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_Player.pos.x += 0;
			g_Player.pos.y += MOVE_NUMBER;
			g_Player.pos.z += 0.0f;
		}
		//加速量
		g_Player.move.y += cosf(g_fAnglePlayer) * 2;
	}

	switch (g_Player.State)
	{
	case PLAYERSTATE_APPEAR:

		g_Player.nCounterDisp++;

		g_Player.bDisp = true;

		g_Player.nLife = 4;

		g_Player.nCounterState++;

		if ((g_Player.nCounterDisp % 5) == 0)
		{
			g_Player.bDisp = g_Player.bDisp ? false : true;
		}
		if (g_Player.nCounterState >= 100)
		{
			g_Player.State = PLAYERSTATE_NORMAL;
		}

		//弾の発射
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -11.5f, 0.0f), PLAYER_BULLET, BULLETTYPE_PLAYER);
			//SetBullet(D3DXVECTOR3(g_Player.pos.x - 10.0f, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -15.0f, 0.0f), 50, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_SHOT);
		}

		break;

	case PLAYERSTATE_NORMAL:

		//弾の発射
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -11.5f, 0.0f), PLAYER_BULLET, BULLETTYPE_PLAYER);

			//SetBullet(D3DXVECTOR3(g_Player.pos.x - 10.0f, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -15.0f, 0.0f), 50, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_SHOT);
		}

		g_Player.bDisp = true;

		break;

	case PLAYERSTATE_DAMAGE:

		g_Player.nCounterState--;

		if (g_Player.nCounterState <= 0)
		{//プレイヤーをノーマルに戻す
			g_Player.State = PLAYERSTATE_NORMAL;
			//色を元に戻す
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		break;

		//待ち時間
	case PLAYERSTATE_WAIT:

		//出現位置
		g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 1000, 0.0f);

		g_Player.nCounterState = 0;

		g_nCounterWait++;

		//位置更新させないための処理
		g_Player.pos.x += 0;
		g_Player.pos.y += 0;
		g_Player.pos.z += 0;

		//出現までの時間
		if (g_nCounterWait >= 60)
		{
			g_Player.State = PLAYERSTATE_APPEAR;
		}

		//色の更新
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		break;

		//プレイヤー死亡状態
	case PLAYERSTATE_DEATH:

		g_nCounterDeath++;

		//リザルトへの待機時間
		if (g_nCounterDeath == 60)
		{
			//リザルトへ
			SetFade(FADE_OUT, MODE_RESULT);
		}

		break;
	}

	//位置更新
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;
	g_Player.pos.z += g_Player.move.z;

	//移動量減衰
	g_Player.move.x += (0 - g_Player.move.x) * 0.2f;
	g_Player.move.y += (0 - g_Player.move.y) * 0.2f;

	//画面外に出ないようにする設定
	if (g_Player.pos.x > SCREEN_WIDTH - PLAYER_SIZEX)
	{
		g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZEX;
	}

	if (g_Player.pos.y > SCREEN_HEIGHT - PLAYER_SIZEY)
	{
		g_Player.pos.y = SCREEN_HEIGHT - PLAYER_SIZEY;
	}

	if (g_Player.pos.x < 0 + PLAYER_SIZEX)
	{
		g_Player.pos.x = 0 + PLAYER_SIZEX;
	}

	if (g_Player.pos.y < 0 + PLAYER_SIZEY)
	{
		g_Player.pos.y = 0 + PLAYER_SIZEY;
	}

	//頂点座標
	pVtx[0].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[1].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[2].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[3].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.z = (g_Player.pos.z = 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();

}

// プレイヤーの描画処理
void DrawPlayer(void)
{
	//ローカル変数
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//弾が当たった時
bool HitPlayer(int nDamage)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//体力マイナス
	g_Player.nLife -= nDamage;

	//体力があるかどうかの判定
	if (g_Player.nLife <= 0)
	{
		g_nCounterWait = 0;

		if (g_Player.bDisp == true)
		{
			g_Player.nLife = 0;

			//残機マイナス
			g_Player.nRemaining--;

			//爆発音　
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			//爆発アニメーション
			SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (g_Player.nRemaining > 0)
			{//残機残ってたら

				g_Player.State = PLAYERSTATE_WAIT;
			}
			else if (g_Player.nRemaining <= 0)
			{//残機なし
				g_Player.State = PLAYERSTATE_DEATH;
			}
			g_Player.bDisp = false;	//プレイヤー非表示

		}

		else
		{

			//ヒット音
			PlaySound(SOUND_LABEL_SE_HIT);
			if (g_Player.State == PLAYERSTATE_NORMAL)
			{
				//プレイヤーの状態をダメージに
				g_Player.State = PLAYERSTATE_DAMAGE;
			}

			//色変わる時間
			g_Player.nCounterState = 5;

			//色の更新
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

			return false;

		}
		g_pVtxBuffPlayer->Unlock(); //アンロック
	}
}

// プレイヤーの頂点座標設定
void SetVertexPlayer(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点ポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//ポリゴンの位置
	g_Player.pos = D3DXVECTOR3(1000, 1000, 0.0f);

	//対角線の長さ
	g_fLengthPlayer = sqrtf(PLAYER_SIZEX * PLAYER_SIZEX + PLAYER_SIZEY * PLAYER_SIZEY);

	//対角線の角度
	g_fAnglePlayer = atan2f(PLAYER_SIZEX, PLAYER_SIZEY);

	//頂点座標
	pVtx[0].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[1].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[2].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[3].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.z = (g_Player.pos.z = 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//プレイヤー情報
PLAYER *GetPlayer(void)
{
	return &g_Player;
}
