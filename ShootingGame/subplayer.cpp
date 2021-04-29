#include "subplayer.h"
#include "player.h"
#include "sound.h"
#include "bullet.h"
#include "input.h"

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_apTextureSubplayer = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSubplayer = NULL;
SUBPLAYER g_aSubplayer[MAX_SUBPLAYER];						//�q�@�̏��
int nSubplayerCounter;

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT InitSubplayer(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\spaceship002.png", &g_apTextureSubplayer);

	//�\���̏�����
	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++)
	{
		g_aSubplayer[nCntSubplayer].pos = D3DXVECTOR3(1920.0f, 0.0f, 0.0f);
		g_aSubplayer[nCntSubplayer].move = D3DXVECTOR3(1920.0f, 0.0f, 0.0f);
		g_aSubplayer[nCntSubplayer].bUse = false;
		g_aSubplayer[nCntSubplayer].State = SUBPLAYERSTATE_APPEAR;
	}

	nSubplayerCounter = 0;

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SUBPLAYER, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffSubplayer, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pVtx += 4)
	{
		//���_���W
		SetVertexSubplayer(nCntSubplayer);

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

		//���_�̏��̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);

	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSubplayer->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------
//�@�I������
//-----------------------------------------------------------------------------
void UninitSubplayer(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffSubplayer != NULL)
	{
		g_pVtxBuffSubplayer->Release();
		g_pVtxBuffSubplayer = NULL;
	}

	if (g_apTextureSubplayer != NULL)
	{
		g_apTextureSubplayer->Release();
		g_apTextureSubplayer = NULL;
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateSubplayer(void)
{
	SUBPLAYER* pSubplayer;
	pSubplayer = &g_aSubplayer[0];

	PLAYER* pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pSubplayer++)
	{
		if (pSubplayer->bUse == true)
		{
			switch (pSubplayer->State)
			{
			case SUBPLAYERSTATE_APPEAR:
				//�ʒu�̍X�V
				pSubplayer->pos.y += 5;

				if (pSubplayer->pos.y > SCREEN_HEIGHT)
				{
					pSubplayer->State = SUBPLAYERSTATE_BREAK;
				}


				break;

			case SUBPLAYERSTATE_NORMAL:

				//����
				pSubplayer->pos.x += (pSubplayer->posDest.x - pSubplayer->pos.x) * 0.09f;
				pSubplayer->pos.y += (pSubplayer->posDest.y - pSubplayer->pos.y) * 0.09f;

				break;

			case SUBPLAYERSTATE_BREAK:

				pSubplayer->bUse = false;

				break;

			}
			SetVertexSubplayer(nCntSubplayer);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSubplayer->Unlock();
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawSubplayer(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	SUBPLAYER* pSubplayer;
	pSubplayer = &g_aSubplayer[0];

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSubplayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pSubplayer++)
	{
		if (pSubplayer->bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureSubplayer);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntSubplayer * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// �q�@�̒��_���W�ݒ�
//-----------------------------------------------------------------------------
void SetVertexSubplayer(int nIdx)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSubplayer->Unlock();

}

//-----------------------------------------------------------------------------
// �q�@�̐ݒ�
//-----------------------------------------------------------------------------
void SetSubplayer(D3DXVECTOR3 pos)
{
	SUBPLAYER* pSubplayer;
	pSubplayer = &g_aSubplayer[0];

	int nCntSubplayer;

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntSubplayer = 0; nCntSubplayer < MAX_SUBPLAYER; nCntSubplayer++, pSubplayer++)
	{
		if (pSubplayer->bUse == false)
		{
			//�ʒu
			pSubplayer->pos = pos;

			pSubplayer->State = SUBPLAYERSTATE_APPEAR;

			//���_���W
			SetVertexSubplayer(nCntSubplayer);

			//�g���Ă����Ԃ�
			pSubplayer->bUse = true;

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffSubplayer->Unlock(); //�A�����b�N

}

//-----------------------------------------------------------------------------
// �q�@�̏ꏊ�ݒ�
//-----------------------------------------------------------------------------
void SetPosSubplayer(int nIdx, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSubplayer->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//g_aSubplayer[nIdx].pos = D3DXVECTOR3(500, -100, 0.0f);

	//���_���W
	g_aSubplayer[nIdx].posDest = pos;

	pVtx[0].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x - SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y + SUBPLAYER_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aSubplayer[nIdx].pos.x + SUBPLAYER_SIZE, g_aSubplayer[nIdx].pos.y - SUBPLAYER_SIZE, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSubplayer->Unlock();

}

//-----------------------------------------------------------------------------
// �q�@�̒e
//-----------------------------------------------------------------------------
void SetBulletSubplayer(int nIdx)
{
	SetBullet(g_aSubplayer[nIdx].pos, D3DXVECTOR3(0.0f, -11.5f, 0.0f), PLAYER_BULLET, BULLETTYPE_SUBPLAYER);
}

SUBPLAYER* GetSubplayer(void)
{
	return &g_aSubplayer[0];
}

