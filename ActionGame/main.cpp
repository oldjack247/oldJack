#include "main.h"
#include "input.h"
#include "sound.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "tutorial.h"


//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;							//Direct3D�I�u�W�F�N�g�̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;				//Direct3D�f�B�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL;							//�t�H���g�ւ̃|�C���^
int g_nCountFPS;									//FPS�J�E���^			
MODE g_mode = MODE_TITLE;							//���[�h�̎��

//���C���֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hlnstancePrev, LPSTR IpCmdLine, int nCmdShow)
{
	//�\���̂̐ݒ�(�E�B���h�E�ɂȂ�����)
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};
	HWND hWnd;
	MSG msg;
	//�E�B���h�E�̓o�^
	RegisterClassEx(&wcex);

	//�E�B���h�E�𐶐�(������E�B���h�E���E�B���h�E�n���h���ɔ[�߂Ċė�����)
	hWnd = CreateWindowEx(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//����������(�E�B���h�E�𐶐����Ă���s��)(DirectX�{�̂���̐������s���Ă���)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return-1;
	}
	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);                                     //�E�B���h�E�̕\��(�E�B���h�E�̕\����Ԃ�ݒ�)
	UpdateWindow(hWnd);                                             //�E�B���h�E�̔��f(�E�B���h�E�̃N���C�A���g�̈���X�V)
																	//���b�Z�[�W�O���[�v(���b�Z�[�W�L���[���烁�b�Z�[�W���擾)
	DWORD dwExecLastTime = 0;
	DWORD dwFPSLastTime = 0;
	DWORD dwCurrentTime = timeGetTime();
	DWORD dwFrameCount = 0;

	while (1)                                                       //���b�Z�[�W���擾���Ȃ�������0��Ԃ�
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)          //���b�Z�[�W���擾���Ȃ�������0��Ԃ�
		{
			//windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;                                              //"WM_QUIT"���b�Z�[�W�������Ă����烋�[�v�𔲂���
			}
			else
			{
				//���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);                             //���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ύX
				DispatchMessage(&msg);                              //�E�B���h�E�v���V�[�W�����b�Z�[�W�𑗏o
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if ((dwCurrentTime - dwExecLastTime) > (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				//DirectX�̏���
				//�X�V����
				Update();
				//�`�揈��
				Draw();

				dwFrameCount++;
			}
		}
	}

	//�I������
	Uninit();

	//�E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return(int)msg.wParam;
}

 //�E�B���h�v���V�[�W��
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	switch (uMsg)
	{
		//�E�B���h�E�j���̃��b�Z�[�W
	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W��Ԃ�
		PostQuitMessage(0);
		break;

	case WM_CLOSE:
		//
		nID = MessageBox(hWnd, "�I�����܂����H�H", "�I���H", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE: //[ESC]�L�[�������ꂽ
			DestroyWindow(hWnd);
			break;
		}
	}

	//�K��̏�����Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//����������
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;                                 //�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;                         //�v���[���e�[�V�����p�����[�^
														 //Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;               //�G���[���o��
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//����������
	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = bWindow;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &g_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &g_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &g_pD3DDevice)))
			{
				// ���������s
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Teminal", &g_pFont);


	//�L�[�{�[�h�̏���������
	InitKeyboard(hInstance, hWnd);

	//�T�E���h�̏���������
	InitSound(hWnd);

	//���[�h�̐ݒ�
	SetMode(g_mode);

	//�t�F�[�h�̐ݒ�
	InitFade(MODE_TITLE);

	return S_OK;
}

//�I������
void Uninit(void)
{

	//�L�[�{�[�h�̏I������
	UninitKeyboard();

	//���[�h�̐ݒ�
	SetMode(g_mode);

	//�t�F�[�h�̏�����
	UninitFade();

	//�T�E���h�̏I������
	UninitSound();


	//�e��I�u�W�F�N�g�̏I������
	//Direct3D�f�B�o�C�X�̊J��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̊J��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	//�t�H���g�I�u�W�F�N�g�̊J��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

}

//�X�V����
void Update(void)
{
	//�e��I�u�W�F�N�g�X�V����

	//�L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_TUTORIAL:
		UpdateTutorial();
		break;

	case MODE_GAME:
		UpdateGame();
		break;

	case MODE_RESULT:
		UpdateResult();
		break;

	}

	//�t�F�[�h�̍X�V����
	UpdateFade();

}

//�`�ʏ���
void Draw(void)
{//�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	//�`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//�e��I�u�W�F�N�g�̕`�揈��


		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_TUTORIAL:
			DrawTutorial();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESULT:
			DrawResult();
			break;

		}

		//�`�揈��
		DrawFade();

		//FPS�̕\��
		DrawFPS();

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}

// FPS�̕\��
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	g_pFont->DrawTextA(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

// ���[�h�̐ݒ�
void SetMode(MODE mode)
{

	//�I������
	switch (g_mode)
	{
	case MODE_TITLE:
		//�^�C�g����ʂ̏I������
		UninitTitle();
		break;

	case MODE_TUTORIAL:
		UninitTutorial();
		break;

	case MODE_GAME:
		//�Q�[����ʂ̏I������
		UninitGame();
		break;

	case MODE_RESULT:
		//���U���g��ʂ̏I������
		UninitResult();
		break;
	}

	switch (mode)
	{
	case MODE_TITLE:
		//�^�C�g����ʂ̏���������
		InitTitle();
		break;

	case MODE_TUTORIAL:
		InitTutorial();
	//	break;

	case MODE_GAME:
		// �Q�[����ʂ̏���������
		InitGame();
		break;

	case MODE_RESULT:
		//���U���g��ʂ̏���������
		InitResult();
		break;
	}

	g_mode = mode;
}

MODE GetMode(void)
{
	return g_mode;
}

LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

