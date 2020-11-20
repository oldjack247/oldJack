#include "game.h"
#include "Player.h"
#include "background.h"
#include "Bullet.h"
#include "Explosion.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
#include "fade.h"
#include "input.h"
#include "Effect.h"
#include "life.h"
#include "pause.h"
#include <stdlib.h>
#include <time.h>

//�O���[�o���ϐ�
bool g_bPause = false;

// ����������
HRESULT InitGame(void)
{

	//�w�i�̏�����
	InitBG();

	//�|���S���̏���������
	InitPlayer();

	//�e�̏���������
	InitBullet();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//�G�̏���������
	InitEnemy();

	//�����̏�����
	InitExplosion();

	//�X�R�A�̏�����
	InitScore();

	//���C�t�e�N�X�`���̏�����
	InitLife();

	//�|�[�Y�̏���������
	InitPause();

	//�T�E���h�X�^�[�g
	PlaySound(SOUND_LABEL_BGM003);

	return S_OK;
}

// �I������
void UninitGame(void)
{
	//�T�E���h�X�g�b�v
	StopSound();

	//�|�[�Y�̏I������
	UninitPause();

	//���C�t�e�N�X�`���̏I������
	//UninitLife();

	//�X�R�A�̏I������
	UninitScore();

	//���j�̏I������
	UninitExplosion();

	//�G�̏I������
	UninitEnemy();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//�e�̏I������
	UninitBullet();

	//�v���C���[�̏I������
	UninitPlayer();

	//�̗͕\���̏I������
	UninitLife();

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

		//�e�̍X�V����
		UpdateBullet();

		//�G�t�F�N�g�̍X�V����
		UpdateEffect();

		//�G�̍X�V����
		UpdateEnemy();

		//�G�̍X�V����
		UpdateExplosion();

		//���C�t�e�N�X�`���̍X�V����		
		UpdateLife();

		//�X�R�A�̍X�V����
		UpdateScore();
	}

}

// �`�揈��
void DrawGame(void)
{
	//�w�i�̕`��
	DrawBG();

	//�|���S���̕`�揈��
	DrawPlayer();

	//�e�̕`�揈��
	DrawBullet();

	//�G�t�F�N�g�̕`�揈��
	DrawEffect();

	//�G�̕`�揈��
	DrawEnemy();

	//�����̕`�揈��
	DrawExplosion();

	//���C�t�e�N�X�`���̕`��
	DrawLife();

	//�X�R�A�̕`�揈��
	DrawScore();


	if (g_bPause == true)
	{
		DrawPause();
	}

}

void SetPause(bool bpause)
{
	g_bPause = bpause;
}