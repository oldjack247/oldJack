#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256)		//キーの最大数

//グローバル変数宣言
LPDIRECTINPUT8 g_pInput = NULL;					//DIRECTINPUTオブジェクトのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//入力デバイスのポインタ
BYTE g_aKeystate[NUM_KEY_MAX];					//キーボードの入力情報
BYTE g_aKeystateTrigger[NUM_KEY_MAX];			//キーボードの入力情報
BYTE g_aKeystateRelease[NUM_KEY_MAX];			//キーボードの入力情報

//初期化処理
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// マウスとかはここに増やしていく

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//終了処理
void UninitKeyboard(void)
{
	//入力デバイスの開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//更新処理
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//入力デバイスからデータ取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeystateTrigger[nCntKey] = (g_aKeystate[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeystateRelease[nCntKey] = (g_aKeystate[nCntKey] & aKeyState[nCntKey]) ^ aKeyState[nCntKey];
			g_aKeystate[nCntKey] = aKeyState[nCntKey];	//キーボードの入力情報保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//キーボードの入力情報取得
bool GetKeyboardPress(int nKey)
{
	return(g_aKeystate[nKey] & 0x80) ? true : false;
}

bool GetKeyboardTrigger(int nKey)
{
	return(g_aKeystateTrigger[nKey] & 0x80) ? true : false;
}

bool GetKeyboardRelease(int nKey)
{
	return(g_aKeystateRelease[nKey] & 0x80) ? true : false;
}
