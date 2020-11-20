#include "life.h"
#include "Player.h"

// �}�N����`
#define MAX_LIFE (4)		//�w�i�̈ړ���
#define LIFE_SIZE (25)

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;	//���_���
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;	//�e�N�X�`��
LIFE g_aLife[MAX_LIFE];
int g_nCntDamage;
int g_nDamageCounter;

// ����������
HRESULT InitLife(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/life000.png", &g_pTextureLife);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL)))
	{
		return E_FAIL;
	}

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aLife[nCntLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	g_nCntDamage = 0;
	g_nDamageCounter = 0;

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntLife = 0; nCntLife < pPlayer->nLife; nCntLife++)
	{
		g_aLife[nCntLife].pos = D3DXVECTOR3(50.0f + ((nCntLife + 1) * 60), 30.0f, 0.0f);

		//���_�̍��W
		pVtx[0].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - LIFE_SIZE, g_aLife[nCntLife].pos.y + LIFE_SIZE, 0);
		pVtx[1].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x - LIFE_SIZE, g_aLife[nCntLife].pos.y - LIFE_SIZE, 0);
		pVtx[2].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + LIFE_SIZE, g_aLife[nCntLife].pos.y + LIFE_SIZE, 0);
		pVtx[3].pos = D3DXVECTOR3(g_aLife[nCntLife].pos.x + LIFE_SIZE, g_aLife[nCntLife].pos.y - LIFE_SIZE, 0);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�̃J���[�ݒ�
		pVtx[0].col = g_aLife[nCntLife].col;
		pVtx[1].col = g_aLife[nCntLife].col;
		pVtx[2].col = g_aLife[nCntLife].col;
		pVtx[3].col = g_aLife[nCntLife].col;

		//���_�̏��̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();

	return S_OK;
}
// �I������
void UninitLife(void)
{
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}

	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

}

// �X�V����
void UpdateLife(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	if (pPlayer->State == PLAYERSTATE_DAMAGE)
	{
		g_aLife[pPlayer->nLife].col.a = 0.0f;
	}

	else if (pPlayer->State == PLAYERSTATE_APPEAR)
	{
		for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
		{
			g_aLife[nCntLife].col.a = 1.0f;
		}
	}

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{

		//���_�̃J���[�ݒ�
		pVtx[0].col = g_aLife[nCntLife].col;
		pVtx[1].col = g_aLife[nCntLife].col;
		pVtx[2].col = g_aLife[nCntLife].col;
		pVtx[3].col = g_aLife[nCntLife].col;

		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//�`�揈��
void DrawLife(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
	}
}