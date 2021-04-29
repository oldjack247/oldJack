#include "time.h"
#include "fade.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;
TIME g_aTime[MAX_TIME];							//�^�C�}�[�̈ʒu
int g_nTime;									//�^�C�}�[
int g_nCounterTime;								//�^�C�}�[�J�E���^�[
int g_aNumber[MAX_TIME];

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT InitTime(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�\���̏�����
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		g_aTime[nCntTime].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�X�R�A�̈ʒu						
	}

	g_nTime = TIMER;

	g_nCounterTime = 0;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number000.png", &g_pTextureTime);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffTime, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++, pVtx += 4)
	{
		//�|���S���̈ʒu
		g_aTime[nCntTime].pos = D3DXVECTOR3(910.0f + nCntTime * 50, 50.0f, 0.0f);

		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_SIZEX, g_aTime[nCntTime].pos.y + TIME_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x - TIME_SIZEX, g_aTime[nCntTime].pos.y - TIME_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_SIZEX, g_aTime[nCntTime].pos.y + TIME_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTime[nCntTime].pos.x + TIME_SIZEX, g_aTime[nCntTime].pos.y - TIME_SIZEY, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�̃J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���Ԃ̌v�Z
		g_aNumber[nCntTime] = (g_nTime / powf(10, MAX_TIME - (nCntTime + 1)));

		//���_�̏��̐ݒ�
		SetTextureTime(nCntTime);

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�@�I������
//-----------------------------------------------------------------------------
void UninitTime(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
	//�e�N�X�`���̊J��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateTime(void)
{
	g_nCounterTime++;

	if (g_nTime <= 0)
	{
		SetFade(FADE_OUT, MODE_RESULT);
	}

	if ((g_nCounterTime % 60) == 0)
	{
		g_nTime--;

		for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{
			g_aNumber[nCntTime] = (g_nTime / powf(10, MAX_TIME - (nCntTime + 1)));

			//�e�N�X�`�����W�̐ݒ�
			SetTextureTime(nCntTime);
		}
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawTime(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTime);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//-----------------------------------------------------------------------------
// �e�N�X�`�����_���W�̍X�V
//-----------------------------------------------------------------------------
void SetTextureTime(int nIdx)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nIdx * 4);

	pVtx[0].tex = D3DXVECTOR2(0.0f + (0.1f * g_aNumber[nIdx]), 1.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f + (0.1f * g_aNumber[nIdx]), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.1f + (0.1f * g_aNumber[nIdx]), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * g_aNumber[nIdx]), 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//-----------------------------------------------------------------------------
// �^�C�}�[���̎擾
//-----------------------------------------------------------------------------
int GetTime(void)
{
	return g_nTime;
}