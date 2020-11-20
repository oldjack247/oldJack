#include "background.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg = NULL;	//���_���
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBg2 = NULL;	//���_���
LPDIRECT3DTEXTURE9 g_pTextureBg = NULL;			//�e�N�X�`�����
LPDIRECT3DTEXTURE9 g_pTextureBg2 = NULL;		//�e�N�X�`�����

//�w�i�̏���������
HRESULT InitBG(void)
{
	//�ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/snowbg.jpg", &g_pTextureBg);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/mountain000.png", &g_pTextureBg2);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0);
	pVtx[1].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0, 0);
	pVtx[2].pos = D3DXVECTOR3(0, 0 + SCREEN_HEIGHT, 0);
	pVtx[3].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0 + SCREEN_HEIGHT, 0);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.25f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBg2, NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBg2->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0);
	pVtx[1].pos = D3DXVECTOR3(0, 780, 0);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, 780, 0);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg2->Unlock();

	return S_OK;
}

//�w�i�̏I������
void UninitBG(void)
{
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffBg != NULL)
	{
		g_pVtxBuffBg->Release();	//�J��
		g_pVtxBuffBg = NULL;		//������
	}
	if (g_pVtxBuffBg2 != NULL)
	{
		g_pVtxBuffBg2->Release();	//�J��
		g_pVtxBuffBg2 = NULL;		//������
	}

	//�e�N�X�`���̊J��
	if (g_pTextureBg != NULL)
	{
		g_pTextureBg->Release();	//�J��
		g_pTextureBg = NULL;		//������
	}
	if (g_pTextureBg2 != NULL)
	{
		g_pTextureBg2->Release();	//�J��
		g_pTextureBg2 = NULL;		//������
	}
}

//�w�i�̍X�V����
void UpdateBG(void)
{
	VERTEX_2D *pVtx; //���_���ւ̃|�C���^
	static float fTexU = 0.0f;
	fTexU += 0.00005f;
	if (fTexU >= 0.75f) {
		fTexU = 0.0f;
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBg->Lock(0, 0, (void**)&pVtx, 0);
	//���_���W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(fTexU, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexU + 0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexU, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexU + 0.25f, 1.0f);
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBg->Unlock();
}

//�w�i�̕`�揈��
void DrawBG(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBg2, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBg2);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}