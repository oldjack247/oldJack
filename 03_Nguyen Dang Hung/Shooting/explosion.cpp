#include "explosion.h"

// �}�N����`		
#define EXPLOSION_SIZE (50)			//�����̑傫��
#define MAX_EXPLOSION (256)			//�����̏��

// �\����
typedef struct
{
	D3DXVECTOR3 pos;						
	int nCounterAnimEx;						
	int nPatternAnimEx;						
	D3DXCOLOR col;
	bool bUse;								
}EXPLOSION;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION];							


// ����������
HRESULT InitExplosion(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/explosion000.png", &g_pTextureExplosion);

	//�\���̏�����
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].nCounterAnimEx = 0;
		g_aExplosion[nCntExplosion].nPatternAnimEx = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W
		SetVertexExplosion(nCntExplosion);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�F�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_�̏��̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.125f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f, 0.125f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 0.0f);

		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock(); //�A�����b�N

	return S_OK;
}

//�@�I������
void UninitExplosion(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
	//�e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
}

// �X�V����
void UpdateExplosion(void)
{
	EXPLOSION *pExplosion;
	pExplosion = &g_aExplosion[0];

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_EXPLOSION; nCntBullet++, pExplosion++)
	{
		if (pExplosion->bUse == true)
		{

			pExplosion->nCounterAnimEx++;												//�A�j���[�V�����J�E���^�[�X�V
			if ((pExplosion->nCounterAnimEx % 3) == 0)
			{
				pExplosion->nPatternAnimEx = (pExplosion->nPatternAnimEx + 1) % 8;		//�A�j���[�V�����p�^�[��No.�X�V
			}

			//���_���W
			SetVertexExplosion(nCntBullet);

			//�e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.125f * pExplosion->nPatternAnimEx), 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f + (0.125f * pExplosion->nPatternAnimEx), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f + (0.125f * pExplosion->nPatternAnimEx), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f + (0.125f * pExplosion->nPatternAnimEx), 0.0f);

			if (pExplosion->nPatternAnimEx == 7)
			{
				pExplosion->bUse = false;
			}
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}

// �`�揈��
void DrawExplosion(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	//�|���S���̕`��
	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

// �����̐ݒ�
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	EXPLOSION *pExplosion;
	pExplosion = &g_aExplosion[0];

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse == false)
		{

			pExplosion->pos = pos;

			pExplosion->nCounterAnimEx = 0;
			pExplosion->nPatternAnimEx = 0;

			//���_���W
			SetVertexExplosion(nCntExplosion);

			pExplosion->col = col;
			pVtx[0].col = pExplosion->col;
			pVtx[1].col = pExplosion->col;
			pVtx[2].col = pExplosion->col;
			pVtx[3].col = pExplosion->col;

			pExplosion->bUse = true;
			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}

// �����̒��_���W�̐ݒ�
void SetVertexExplosion(int nIdx)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y + EXPLOSION_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x - EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y - EXPLOSION_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y + EXPLOSION_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nIdx].pos.x + EXPLOSION_SIZE, g_aExplosion[nIdx].pos.y - EXPLOSION_SIZE, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffExplosion->Unlock();
}
