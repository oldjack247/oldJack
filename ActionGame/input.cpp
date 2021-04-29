#include "input.h"

//�}�N����`
#define NUM_KEY_MAX (256)		//�L�[�̍ő吔

//�O���[�o���ϐ��錾
LPDIRECTINPUT8 g_pInput = NULL;					//DIRECTINPUT�I�u�W�F�N�g�̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		//���̓f�o�C�X�̃|�C���^
BYTE g_aKeystate[NUM_KEY_MAX];					//�L�[�{�[�h�̓��͏��
BYTE g_aKeystateTrigger[NUM_KEY_MAX];			//�L�[�{�[�h�̓��͏��
BYTE g_aKeystateRelease[NUM_KEY_MAX];			//�L�[�{�[�h�̓��͏��

//����������
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̍쐬
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// �}�E�X�Ƃ��͂����ɑ��₵�Ă���

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//�I������
void UninitKeyboard(void)
{
	//���̓f�o�C�X�̊J��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̊J��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//�X�V����
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//���̓f�o�C�X����f�[�^�擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeystateTrigger[nCntKey] = (g_aKeystate[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeystateRelease[nCntKey] = (g_aKeystate[nCntKey] & aKeyState[nCntKey]) ^ aKeyState[nCntKey];
			g_aKeystate[nCntKey] = aKeyState[nCntKey];	//�L�[�{�[�h�̓��͏��ۑ�
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}

//�L�[�{�[�h�̓��͏��擾
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
