#include "background.h"
#include "player.h"
#include "input.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;	
LPDIRECT3DTEXTURE9 g_pTextureBG[MAX_BG] = { NULL, NULL, NULL };
D3DXVECTOR3 g_posBg[MAX_BG];		
int g_nCntAnim;						
float g_fPatternAnim[MAX_BG];		
float g_fMoveBgX[MAX_BG];
float g_fMoveBgY[MAX_BG];

// �|���S���̏���������
HRESULT InitBG(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg100.png", &g_pTextureBG[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg101.png", &g_pTextureBG[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bg102.png", &g_pTextureBG[2]);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BG,	
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL)))
	{
		return E_FAIL;
	}

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		g_posBg[nCnt].x = SCREEN_WIDTH / 2;
		g_posBg[nCnt].y = SCREEN_HEIGHT / 2;
		g_fMoveBgX[nCnt] = 0.0f;
		g_fMoveBgY[nCnt] = 0.0f;
	}

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// �w�i�̒��S���W
		g_posBg[nCnt] = D3DXVECTOR3(g_posBg[nCnt].x, g_posBg[nCnt].y, 0.0f);

		// �|���S���̊e���_���W
		pVtx[0].pos = D3DXVECTOR3(g_posBg[nCnt].x - (SCREEN_WIDTH / 2), g_posBg[nCnt].y + (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posBg[nCnt].x - (SCREEN_WIDTH / 2), g_posBg[nCnt].y - (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posBg[nCnt].x + (SCREEN_WIDTH / 2), g_posBg[nCnt].y + (SCREEN_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posBg[nCnt].x + (SCREEN_WIDTH / 2), g_posBg[nCnt].y - (SCREEN_HEIGHT / 2), 0.0f);

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

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

	return S_OK;
}

//�|���S���̏I������
void UninitBG(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	// �e�N�X�`���̊J��
	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		if (g_pTextureBG[nCnt] != NULL)
		{
			g_pTextureBG[nCnt]->Release();
			g_pTextureBG[nCnt] = NULL;
		}
	}
}

//�|���S���̍X�V����
void UpdateBG(void)
{
	VERTEX_2D *pVtx;

	g_nCntAnim++;	// �A�j���[�V�����J�E���^�[�X�V

	// �v���C���[�̈ړ��ɍ��킹�Ĕw�i�e�N�X�`���̐؂�ւ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_fMoveBgY[0] += MOVE_BG_A;
		g_fMoveBgY[1] += MOVE_BG_B;
		g_fMoveBgY[2] += MOVE_BG_C;

		if (GetKeyboardPress(DIK_A) == true)
		{
			g_fMoveBgX[0] -= MOVE_BG_A;
			g_fMoveBgX[1] -= MOVE_BG_B;
			g_fMoveBgX[2] -= MOVE_BG_C;
			g_fMoveBgY[0] += MOVE_BG_A;
			g_fMoveBgY[1] += MOVE_BG_B;
			g_fMoveBgY[2] += MOVE_BG_C;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
			g_fMoveBgY[0] += MOVE_BG_A;
			g_fMoveBgY[1] += MOVE_BG_B;
			g_fMoveBgY[2] += MOVE_BG_C;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;
		}
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		g_fMoveBgY[0] -= MOVE_BG_A;
		g_fMoveBgY[1] -= MOVE_BG_B;
		g_fMoveBgY[2] -= MOVE_BG_C;

		if (GetKeyboardPress(DIK_A) == true)
		{
			g_fMoveBgX[0] -= MOVE_BG_A;
			g_fMoveBgX[1] -= MOVE_BG_B;
			g_fMoveBgX[2] -= MOVE_BG_C;
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
			g_fMoveBgY[0] -= MOVE_BG_A;
			g_fMoveBgY[1] -= MOVE_BG_B;
			g_fMoveBgY[2] -= MOVE_BG_C;
		}
	}
	else if (GetKeyboardPress(DIK_A) == true)
	{
		g_fMoveBgX[0] -= MOVE_BG_A;
		g_fMoveBgX[1] -= MOVE_BG_B;
		g_fMoveBgX[2] -= MOVE_BG_C;

		if (GetKeyboardPress(DIK_D) == true)
		{
			g_fMoveBgX[0] += MOVE_BG_A;
			g_fMoveBgX[1] += MOVE_BG_B;
			g_fMoveBgX[2] += MOVE_BG_C;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{
		g_fMoveBgX[0] += MOVE_BG_A;
		g_fMoveBgX[1] += MOVE_BG_B;
		g_fMoveBgX[2] += MOVE_BG_C;
	}


	// �e�N�X�`���̍X�V
	if ((g_nCntAnim % 2) == 0)
	{
		// �e�N�X�`���̃p�^�[���̍X�V
		g_fPatternAnim[0] -= 0.001f;
		g_fPatternAnim[1] -= 0.003f;
		g_fPatternAnim[2] -= 0.005f;

		// ���_�o�b�t�@�����b�N����
		g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < MAX_BG; nCnt++, pVtx = pVtx + 4)
		{
			// �e�N�X�`���̒��_���W�̍X�V
			pVtx[0].tex = D3DXVECTOR2(0.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt]);
			pVtx[1].tex = D3DXVECTOR2(0.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt] - 1.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt]);
			pVtx[3].tex = D3DXVECTOR2(1.0f + g_fMoveBgX[nCnt], g_fPatternAnim[nCnt] + g_fMoveBgY[nCnt] - 1.0f);
		}

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffBG->Unlock();
	}
}

//�|���S���̕`�揈��
void DrawBG(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_BG; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBG[nCnt]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	nCnt * 4, 2);					
	}
}