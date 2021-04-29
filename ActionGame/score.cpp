#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include "main.h"
#include "score.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Score g_aScorePolygon[MAX_SCORE_POLYGON];			//�X�R�A
int g_Score;										//�X�R�A

//����������
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�ŃA�C�X�̃|�C���^
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�X�R�A�̈ʒu�̏�����
	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		g_aScorePolygon[nCntScore].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore) + (SCORE_POLYGON_WIDTH / 2)), SCORE_POLYGON_HEIGHT / 2, 0.0f);
	}

	g_Score = 0;


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data//TEXTURE//score.png",
		&g_pTextureScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_POLYGON_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_POLYGON_HEIGHT, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

}

//�I������
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//�X�V����
void UpdateScore(void)
{
	int nCntScore;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		if (GetMode() == MODE_RESULT)
		{
			//���_���W
			pVtx[0].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_RESULT_POS_HEIGHT - SCORE_POLYGON_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_RESULT_POS_HEIGHT - SCORE_POLYGON_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_RESULT_POS_HEIGHT + SCORE_POLYGON_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCORE_RESULT_POS_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_RESULT_POS_HEIGHT + SCORE_POLYGON_HEIGHT / 2, 0.0f);
		}
		else
		{
			//���_���W
			pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore)), SCORE_POLYGON_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (SCORE_POLYGON_WIDTH*(MAX_SCORE_POLYGON - nCntScore - 1)), SCORE_POLYGON_HEIGHT, 0.0f);
		}

		if (nCntScore == 0)
		{
			pVtx[0].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Score / (int)powf(10, 7)*0.1f + 0.1f, 1.0f);
		}
		else if (nCntScore == 7)
		{
			pVtx[0].tex = D3DXVECTOR2(g_Score % 10 * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Score % 10 * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Score % 10 * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Score % 10 * 0.1f + 0.1f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore) *0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore)*0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore)*0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Score % (int)powf(10, MAX_SCORE_POLYGON - nCntScore) / (int)powf(10, 7 - nCntScore)*0.1f + 0.1f, 1.0f);
		}

		pVtx += 4;
	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();

}

//�`�揈��
void DrawScore(void)
{
	int nCntScore;

	LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();						//�f�o�C�X���擾����
												//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�

	for (nCntScore = 0; nCntScore < MAX_SCORE_POLYGON; nCntScore++)
	{
		//�e�N�X�`��
		pDevice->SetTexture(0, g_pTextureScore);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntScore * 4,					//�`����J�n���钸�_�̃C���f�b�N�X
			2);					//�`�悷��v���~�e�B�u��
	}

}

//�ݒ菈��
void SetScore(int nScore)
{
	g_Score += nScore;
}

//�擾����
int GetScore(void)
{
	if (GetMode() == MODE_TITLE)
	{
		g_Score = 0;
	}

	return g_Score;
}
