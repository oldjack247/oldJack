#include "pause.h"
#include "fade.h"
#include "input.h"
#include "game.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;			//���_���
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_PAUSE] = {};		//�e�N�X�`��
PAUSE g_aPause[MAX_PAUSE];
int g_State;

// ����������
HRESULT InitPause(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, NULL, &g_pTexturePause[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause100.png", &g_pTexturePause[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause000.png", &g_pTexturePause[2]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause001.png", &g_pTexturePause[3]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/pause002.png", &g_pTexturePause[4]);

	//������
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		g_aPause[nCntPause].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPause[nCntPause].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	g_State = PAUSE_MENU_CONTINUE;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPause, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++, pVtx += 4)
	{

		SetVertexPause(nCntPause);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�̏��̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPause->Unlock();

	}
	return S_OK;
}

// �I������
void UninitPause(void)
{
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		if (g_pTexturePause[nCntPause] != NULL)
		{
			g_pTexturePause[nCntPause]->Release();
			g_pTexturePause[nCntPause] = NULL;
		}
	}
}

// �X�V����
void UpdatePause(void)
{
	VERTEX_2D *pVtx;

	if (GetKeyboardTrigger(DIK_S) == true)
	{
		g_State++;
		if (g_State > PAUSE_MENU_MAX - 1)
		{
			g_State = 0;
		}
	}

	if (GetKeyboardTrigger(DIK_W) == true)
	{
		g_State--;
		if (g_State < 0)
		{
			g_State = PAUSE_MENU_MAX - 1;
		}
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);


	switch (g_State)
	{
	case PAUSE_MENU_CONTINUE:

		g_aPause[3].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[4].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	case PAUSE_MENU_RETRY:

		g_aPause[2].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_aPause[4].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		break;

	case PAUSE_MENU_QUIT:

		g_aPause[2].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[3].col = D3DXCOLOR(0.75f, 0.75f, 0.75f, 1.0f);

		g_aPause[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		break;

	}
	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		//���_�̃J���[�ݒ�
		pVtx[0].col = g_aPause[nCntPause].col;
		pVtx[1].col = g_aPause[nCntPause].col;
		pVtx[2].col = g_aPause[nCntPause].col;
		pVtx[3].col = g_aPause[nCntPause].col;

		pVtx += 4;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (g_State == PAUSE_MENU_RETRY)
		{
			SetFade(FADE_OUT, MODE_GAME);
		}
		else if (g_State == PAUSE_MENU_QUIT)
		{
			SetFade(FADE_OUT, MODE_TITLE);
		}
		SetPause(false);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

}

// �`�揈��
void DrawPause(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntPause = 0; nCntPause < MAX_PAUSE; nCntPause++)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCntPause]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPause * 4, 2);

	}
}


void SetVertexPause(int nIdx)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	if (nIdx == 0)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_BG_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_BG_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_BG_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_BG_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_BG_SIZE_Y, 0.0f);

		//���_�̃J���[
		g_aPause[nIdx].col.r = 0.0f;
		g_aPause[nIdx].col.g = 0.0f;
		g_aPause[nIdx].col.b = 0.0f;
		g_aPause[nIdx].col.a = 0.5f;
	}
	else if (nIdx == 1)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_MENU_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_MENU_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y + PAUSE_MENU_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + PAUSE_MENU_SIZE_X, g_aPause[nIdx].pos.y - PAUSE_MENU_SIZE_Y, 0.0f);

		//���_�̃J���[
		g_aPause[nIdx].col.r = 0.0f;
		g_aPause[nIdx].col.g = 1.0f;
		g_aPause[nIdx].col.b = 1.0f;
		g_aPause[nIdx].col.a = 0.5;
	}
	else if (nIdx == 2)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(960.0f, 0.0f + (nIdx * 175), 0.0f);
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);

		//���_�̃J���[
		g_aPause[nIdx].col.r = 1.0f;
		g_aPause[nIdx].col.g = 1.0f;
		g_aPause[nIdx].col.b = 1.0f;
		g_aPause[nIdx].col.a = 1.0f;

	}
	else if (nIdx >= 3)
	{
		g_aPause[nIdx].pos = D3DXVECTOR3(960.0f, 0.0f + (nIdx * 175), 0.0f);
		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x - CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y + CONTINUE_SIZE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPause[nIdx].pos.x + CONTINUE_SIZE_X, g_aPause[nIdx].pos.y - CONTINUE_SIZE_Y, 0.0f);

		//���_�̃J���[
		g_aPause[nIdx].col.r = 0.75;
		g_aPause[nIdx].col.g = 0.75;
		g_aPause[nIdx].col.b = 0.75;
		g_aPause[nIdx].col.a = 1.0f;

	}

	//���_�̃J���[�ݒ�
	pVtx[0].col = g_aPause[nIdx].col;
	pVtx[1].col = g_aPause[nIdx].col;
	pVtx[2].col = g_aPause[nIdx].col;
	pVtx[3].col = g_aPause[nIdx].col;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

}