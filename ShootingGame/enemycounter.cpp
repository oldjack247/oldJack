#include "enemycounter.h"
#include "enemy.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureEnemyCounter = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyCounter = NULL;
ENEMYCOUNTER g_aEnemyCounter[MAX_COUNTER];						//�G�̃J�E���^�[�̈ʒu
int g_nEnemyCounter;											//�G�̃J�E���^�[
int g_aCount[MAX_COUNTER];

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT InitEnemyCounter(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�\���̏�����
	for (int nnCntEnemyCounter = 0; nnCntEnemyCounter < MAX_COUNTER; nnCntEnemyCounter++)
	{
		g_aEnemyCounter[nnCntEnemyCounter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�X�R�A�̈ʒu						
	}

	g_nEnemyCounter = ENEMY_COUNT;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureEnemyCounter);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_COUNTER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemyCounter, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyCounter->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemyCounter = 0; nCntEnemyCounter < MAX_COUNTER; nCntEnemyCounter++, pVtx += 4)
	{
		//�|���S���̈ʒu
		g_aEnemyCounter[nCntEnemyCounter].pos = D3DXVECTOR3(COUNTER_SIZEX + 50 + nCntEnemyCounter * 50, 50.0f, 0.0f);

		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x - COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y + COUNTER_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x - COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y - COUNTER_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x + COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y + COUNTER_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemyCounter[nCntEnemyCounter].pos.x + COUNTER_SIZEX, g_aEnemyCounter[nCntEnemyCounter].pos.y - COUNTER_SIZEY, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�̃J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

		//���Ԃ̌v�Z
		g_aCount[nCntEnemyCounter] = (g_nEnemyCounter / powf(10, MAX_COUNTER - (nCntEnemyCounter + 1)));

		//���_�̏��̐ݒ�
		SetTextureEnemyCounter(nCntEnemyCounter);

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemyCounter->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�@�I������
//-----------------------------------------------------------------------------
void UninitEnemyCounter(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffEnemyCounter != NULL)
	{
		g_pVtxBuffEnemyCounter->Release();
		g_pVtxBuffEnemyCounter = NULL;
	}
	//�e�N�X�`���̊J��
	if (g_pTextureEnemyCounter != NULL)
	{
		g_pTextureEnemyCounter->Release();
		g_pTextureEnemyCounter = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateEnemyCounter(void)
{
	if (g_nEnemyCounter <= 0)
	{
		g_nEnemyCounter = 0;
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawEnemyCounter(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemyCounter, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntTime = 0; nCntTime < MAX_COUNTER; nCntTime++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEnemyCounter);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// �J�E���g����
//-----------------------------------------------------------------------------
void AddEnemyCounter(int EnemyCount)
{
	g_nEnemyCounter -= EnemyCount;

	for (int nCntEnemyCounter = 0; nCntEnemyCounter < MAX_COUNTER; nCntEnemyCounter++)
	{
		g_aCount[nCntEnemyCounter] = (g_nEnemyCounter / powf(10, MAX_COUNTER - (nCntEnemyCounter + 1)));

		//�e�N�X�`�����W�̐ݒ�
		SetTextureEnemyCounter(nCntEnemyCounter);
	}
}

//-----------------------------------------------------------------------------
// �e�N�X�`�����_���W�̍X�V
//-----------------------------------------------------------------------------
void SetTextureEnemyCounter(int nIdx)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemyCounter->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCount[nIdx]), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f * g_aCount[nIdx]), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCount[nIdx]), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aCount[nIdx]), 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemyCounter->Unlock();
}