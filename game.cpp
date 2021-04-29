#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "game.h"
#include "player.h"
#include "background.h"
#include "enemy.h"
#include "block.h"
#include "sound.h"
#include "fade.h"
#include "input.h"
#include "pause.h"
#include "score.h"
#include "time.h"
#include "item.h"
#include <stdlib.h>
#include <time.h>

//�O���[�o���ϐ�
bool g_bPause = false;
int g_aMapData[MAX_LINE][MAX_COLUMN];

// ����������
HRESULT InitGame(void)
{

	//�w�i�̏�����
	InitBG();

	//�|���S���̏���������
	InitPlayer();

	//�|�[�Y�̏���������
	InitPause();

	InitBlock();

	InitEnemy();

	InitScore();

	//InitTime();

	InitItem();

	//�T�E���h�X�^�[�g
	//PlaySound(SOUND_LABEL_BGM003);

	//���[�h����
	FILE *pFile;

	pFile = fopen("data/STAGE/Stage2.csv", "r");
	int nCntX = 0, nCntY = 0;
	char aLine[1000];
	while (fgets(aLine, 1000, pFile) != NULL)
	{
		nCntX = 0;
		char *pToken = strtok(aLine, ",");
		while (pToken != NULL)
		{
			int nNum = atoi(pToken);
			g_aMapData[nCntY][nCntX] = nNum;
			pToken = strtok(NULL, ",");
			nCntX++;
		}
		nCntY++;
	}
	fclose(pFile);

	//�u���b�N�̐ݒu
	for (int nCntY = 0; nCntY < MAX_LINE; nCntY++)
	{
		for (int nCntX = 0; nCntX < MAX_COLUMN; nCntX++)
		{
			if (g_aMapData[nCntY][nCntX] == 1)		//1*1�u���b�N
			{
				//D3DXVECTOR3 pos = D3DXVECTOR3(0.0f + 50.0f* nCntX, 0.0f + 50.0f * nCntY, 0);
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 40.0f, 1.0f, 1.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 2)
			{
				SetEnemy(D3DXVECTOR3(20.0f + 40.0f* nCntX, 40.0f + 40.0f * nCntY, 0.0f), 0, D3DXVECTOR3(-0.5f, 0.0f, 0.0f), 1, 40.0f, 40.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 3)
			{
				SetItem(D3DXVECTOR3(20.0f + 40.0f* nCntX, 40.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
			}
			else if (g_aMapData[nCntY][nCntX] == 4)		//4*1�u���b�N
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 160.0f, 1.0f, 4.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 5)		//�G���˕Ԃ�
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, 1, 40.0f, 40.0f, 1.0f, 1.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 6)		//6*1�u���b�N
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 240.0f, 1.0f, 6.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 7)		//���U�T
			{
				SetEnemy(D3DXVECTOR3(20.0f + 40.0f* nCntX, 40.0f + 40.0f * nCntY, 0.0f), 0, D3DXVECTOR3(-2.0f, 0.0f, 0.0f), 1, 40.0f, 40.0f);
			}
			else if (g_aMapData[nCntY][nCntX] == 22)		//2*1�u���b�N
			{
				SetMoveBlock(D3DXVECTOR3(0.0f + 40.0f* nCntX, 0.0f + 40.0f * nCntY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 1, 40.0f, 80.0f, 1.0f, 2.0f);
			}

		}
	}


	return S_OK;
}

// �I������
void UninitGame(void)
{


	//�|�[�Y�̏I������
	UninitPause();

	//�G�̏I������
	//UninitEnemy();

	//�v���C���[�̏I������
	UninitPlayer();

	UninitBlock();

	UninitEnemy();

	UninitScore();

	//UninitTime();

	UninitItem();

	//�w�i�̏I��
	UninitBG();
}

// �X�V����
void UpdateGame(void)
{
	FADE *pFade;
	pFade = GetFade();

	srand((unsigned int)time(NULL));

	if (GetKeyboardTrigger(DIK_P) == true)
	{
		if (*pFade == FADE_NONE)
		{
			g_bPause = g_bPause ? false : true;
		}
	}

	SetPause(g_bPause);

	if (g_bPause == true)
	{
		//�|�[�Y�̍X�V����
		UpdatePause();
	}
	else if (g_bPause == false)
	{
		//�w�i�̍X�V����
		UpdateBG();

		//�|���S���̍X�V����
		UpdatePlayer();

		UpdateBlock();

		//UpdateTime();

		UpdateScore();

		UpdateItem();

		//�G�̍X�V����
		UpdateEnemy();

	}

}

// �`�揈��
void DrawGame(void)
{
	//�w�i�̕`��
	DrawBG();

	//�|���S���̕`�揈��
	DrawPlayer();

	//�G�̕`�揈��
	DrawEnemy();

	DrawBlock();

	DrawItem();

	DrawScore();

//	DrawTime();


	if (g_bPause == true)
	{
		DrawPause();
	}

}

void SetPause(bool bpause)
{
	g_bPause = bpause;
}