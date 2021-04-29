#include "title.h"
#include "input.h"
#include "fade.h"
#include "sound.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitlelogo = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitleEnter = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;
D3DXVECTOR3 g_posTitle;
D3DXVECTOR3 g_posTitlelogo;
D3DXVECTOR3 g_posTitleEnter;
int g_nCntMoveLogo;
ENTER Enter;
int g_nSignCounter;

//�^�C�g����ʂ̏���������
HRESULT InitTitle(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	//�T�E���h�X�^�[�g
	//PlaySound(SOUND_LABEL_BGM001);

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/wallpaper000.png", &g_pTextureTitle);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title_logo000.png", &g_pTextureTitlelogo);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/press_enter.png", &g_pTextureTitleEnter);

	// ���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * 3,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL)))
	{
		return E_FAIL;
	}

	// �^�C�g����ʂ̒��S���W
	g_posTitle.x = SCREEN_WIDTH / 2;
	g_posTitle.y = SCREEN_HEIGHT / 2;

	// �^�C�g�����S�̏������S���W
	g_posTitlelogo.x = TITLE_LOGO_X;
	g_posTitlelogo.y = -50;

	g_nCntMoveLogo = 0;

	// �G���^�[�T�C���̒��S���W
	g_posTitleEnter.x = TITLE_ENTER_X;
	g_posTitleEnter.y = TITLE_ENTER_Y;

	// �G���^�[�T�C���̏�����
	Enter = ENTER_NORMAL;
	g_nSignCounter = 0;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �^�C�g����ʂ̒��S���W
	g_posTitle = D3DXVECTOR3(g_posTitle.x, g_posTitle.y, 0.0f);

	// �|���S���̊e���_���W
	pVtx[0].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posTitle.x - (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y + (SCREEN_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posTitle.x + (SCREEN_WIDTH / 2), g_posTitle.y - (SCREEN_HEIGHT / 2), 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[1].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �^�C�g����ʂ̒��S���W
	g_posTitle = D3DXVECTOR3(g_posTitlelogo.x, g_posTitlelogo.y, 0.0f);

	// �|���S���̊e���_���W
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y - 80, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y - 80, 0.0f);

	// rhw�̐ݒ�
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_���W�̐ݒ�
	pVtx[4].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[5].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[6].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[7].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();


	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �G���^�[�T�C���̒��S���W
	g_posTitleEnter = D3DXVECTOR3(g_posTitleEnter.x, g_posTitleEnter.y, 0.0f);

	// �|���S���̊e���_���W
	pVtx[8].pos = D3DXVECTOR3(g_posTitleEnter.x - 240, g_posTitleEnter.y + 40, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(g_posTitleEnter.x - 240, g_posTitleEnter.y - 40, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(g_posTitleEnter.x + 240, g_posTitleEnter.y + 40, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(g_posTitleEnter.x + 240, g_posTitleEnter.y - 40, 0.0f);

	// rhw�̐ݒ�
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;

	// �e���_�J���[�̐ݒ�
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// ���_���W�̐ݒ�
	pVtx[8].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[9].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[10].tex = D3DXVECTOR2(1.0, 1.0);
	pVtx[11].tex = D3DXVECTOR2(1.0, 0.0);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	return S_OK;
}

// �^�C�g����ʂ̏I������
void UninitTitle(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// �e�N�X�`���̊J��
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}

	if (g_pTextureTitlelogo != NULL)
	{
		g_pTextureTitlelogo->Release();
		g_pTextureTitlelogo = NULL;
	}

	if (g_pTextureTitleEnter != NULL)
	{
		g_pTextureTitleEnter->Release();
		g_pTextureTitleEnter = NULL;
	}
}

// �^�C�g����ʂ̍X�V����
void UpdateTitle(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	int nSign = 255;

	// �G���^�[�T�C���J�E���^�[�̑���
	g_nSignCounter++;

	// �^�C�g�����S�̈ړ�
	if (g_posTitlelogo.y != TITLE_LOGO_Y)
	{
		g_nCntMoveLogo++;
		if (g_nCntMoveLogo % 2 == 0)
		{
			g_posTitlelogo.y += 2;
			if (g_posTitlelogo.y >= TITLE_LOGO_Y)
			{
				g_posTitlelogo.y = TITLE_LOGO_Y;
			}
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	// �|���S���̊e���_���W
	pVtx[4].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(g_posTitlelogo.x - 480, g_posTitlelogo.y - 80, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y + 80, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(g_posTitlelogo.x + 480, g_posTitlelogo.y - 80, 0.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();


	if (GetKeyboardTrigger(DIK_RETURN) == true && g_posTitlelogo.y != TITLE_LOGO_Y)
	{
		PlaySound(SOUND_LABEL_SE_HIT);
		g_posTitlelogo.y = TITLE_LOGO_Y - 1;
	}

	// �G���^�[�L�[���͂ŃQ�[����ʂ֑J��
	if (GetKeyboardTrigger(DIK_RETURN) == true && g_posTitlelogo.y == TITLE_LOGO_Y)
	{
		PlaySound(SOUND_LABEL_SE_HIT);
		// �t�F�[�h�A�E�g
		SetFade(FADE_OUT, MODE_GAME);

		// �G���^�[�T�C���̏�ԕω�
		Enter = ENTER_PRESS;
	}

	// �G���^�[�T�C���̏��
	switch (Enter)
	{
	case ENTER_NORMAL:
		if (g_nSignCounter % 60 == 0)
		{
			// �����x0��
			nSign = 255;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 30 == 0 && g_nSignCounter % 60 != 0)
		{
			// �����x�ő��
			nSign = 0;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}
		break;

	case ENTER_PRESS:
		if (g_nSignCounter % 4 == 0)
		{
			// �����x0��
			nSign = 255;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}
		else if (g_nSignCounter % 2 == 0 && g_nSignCounter % 4 != 0)
		{
			// �����x�ő��
			nSign = 0;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

			// �e���_�J���[�̐ݒ�
			pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, nSign);
			pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, nSign);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTitle->Unlock();
		}
		break;

	default:
		break;
	}
}

// �^�C�g����ʂ̕`�揈��
void DrawTitle(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTitle,
		0,
		sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		if (nCnt == 0)
		{
			pDevice->SetTexture(0, g_pTextureTitle);
		}
		else if (nCnt == 1)
		{
			pDevice->SetTexture(0, g_pTextureTitlelogo);
		}
		else if (nCnt == 2)
		{
			pDevice->SetTexture(0, g_pTextureTitleEnter);
		}

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			nCnt * 4,
			2);
	}
}