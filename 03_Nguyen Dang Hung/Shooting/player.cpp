#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "fade.h"
#include "sound.h"
#include "effect.h"
#include "life.h"
#include "score.h"

// �O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	//���_���
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;			//�e�N�X�`��
int g_nCounterAnim;									//�A�j���[�V�����J�E���^�[
int g_nPatternAnim;									//�A�j���[�V�����p�^�[��No.
D3DXVECTOR3 g_rotPlayer;							//
float g_fLengthPlayer;								//�Ίp���̒���
float g_fAnglePlayer;								//�Ίp���̊p�x
PLAYER g_Player;
int g_nCounterPlayer;
int g_nCounterWait;
int g_nCounterDeath;

// �v���C���[�̏���������
HRESULT InitPlayer(void)
{
	//���[�J���ϐ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/spaceship002.png", &g_pTexturePlayer);

	//�\���̏�����
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.State = PLAYERSTATE_APPEAR;				//���
	g_Player.nCounterState = 0;							//��ԃJ�E���^�[
	g_Player.nLife = 4;									//�̗�
	g_Player.nRemaining = 3;							//�c�@
	g_Player.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
	g_Player.bDisp = true;								//�\�����
	g_nCounterPlayer = 0;
	g_nCounterDeath = 0;


	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	SetVertexPlayer();

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	return S_OK;
}

// �v���C���[�̏I������
void UninitPlayer(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}

	//�e�N�X�`���̊J��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
}

// �v���C���[�̍X�V����
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//A���������Ƃ�
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += -MOVE_NUMBER;
		g_Player.pos.y += 0;
		g_Player.pos.z += 0.0f;

		//������
		g_Player.move.x += -sinf(g_fAnglePlayer) * 2;
	}

	//A��W�𓯎��ɉ������Ƃ�
	if (GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += -sinf(D3DX_PI / 4) * 0.2f;
		g_Player.pos.y += -cosf(D3DX_PI / 2) * 0.2f;
	}

	//A��S�𓯎��ɉ������Ƃ�
	if (GetKeyboardPress(DIK_S) == true && GetKeyboardPress(DIK_A) == true)
	{
		g_Player.pos.x += -sinf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
		g_Player.pos.y += cosf(D3DX_PI / 2) * MOVE_NUMBER * 0.2f;
	}

	//D���������Ƃ�
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += MOVE_NUMBER;
		g_Player.pos.y += 0;
		g_Player.pos.z += 0.0f;

		//������
		g_Player.move.x += sinf(g_fAnglePlayer) * 2;
	}

	//D��W�𓯎��ɉ������Ƃ�
	if (GetKeyboardPress(DIK_W) == true && GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += sinf(D3DX_PI / 2) * MOVE_NUMBER * 0.2f;
		g_Player.pos.y += -cosf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
	}

	//D��S�𓯎��ɉ������Ƃ�
	if (GetKeyboardPress(DIK_S) == true && GetKeyboardPress(DIK_D) == true)
	{
		g_Player.pos.x += sinf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
		g_Player.pos.y += cosf(D3DX_PI / 4) * MOVE_NUMBER * 0.2f;
	}

	//W���������Ƃ�
	if (GetKeyboardPress(DIK_W) == true)
	{
		if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_Player.pos.x += 0;
			g_Player.pos.y += -MOVE_NUMBER;
			g_Player.pos.z += 0.0f;
		}

		//������
		g_Player.move.y -= cosf(g_fAnglePlayer) * 2;
	}

	//S���������Ƃ�
	if (GetKeyboardPress(DIK_S) == true)
	{
		if (GetKeyboardPress(DIK_A) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_Player.pos.x += 0;
			g_Player.pos.y += MOVE_NUMBER;
			g_Player.pos.z += 0.0f;
		}
		//������
		g_Player.move.y += cosf(g_fAnglePlayer) * 2;
	}

	switch (g_Player.State)
	{
	case PLAYERSTATE_APPEAR:

		g_Player.nCounterDisp++;

		g_Player.bDisp = true;

		g_Player.nLife = 4;

		g_Player.nCounterState++;

		if ((g_Player.nCounterDisp % 5) == 0)
		{
			g_Player.bDisp = g_Player.bDisp ? false : true;
		}
		if (g_Player.nCounterState >= 100)
		{
			g_Player.State = PLAYERSTATE_NORMAL;
		}

		//�e�̔���
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -11.5f, 0.0f), PLAYER_BULLET, BULLETTYPE_PLAYER);
			//SetBullet(D3DXVECTOR3(g_Player.pos.x - 10.0f, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -15.0f, 0.0f), 50, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_SHOT);
		}

		break;

	case PLAYERSTATE_NORMAL:

		//�e�̔���
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			SetBullet(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -11.5f, 0.0f), PLAYER_BULLET, BULLETTYPE_PLAYER);

			//SetBullet(D3DXVECTOR3(g_Player.pos.x - 10.0f, g_Player.pos.y, g_Player.pos.z), D3DXVECTOR3(0.0f, -15.0f, 0.0f), 50, BULLETTYPE_PLAYER);
			PlaySound(SOUND_LABEL_SE_SHOT);
		}

		g_Player.bDisp = true;

		break;

	case PLAYERSTATE_DAMAGE:

		g_Player.nCounterState--;

		if (g_Player.nCounterState <= 0)
		{//�v���C���[���m�[�}���ɖ߂�
			g_Player.State = PLAYERSTATE_NORMAL;
			//�F�����ɖ߂�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		break;

		//�҂�����
	case PLAYERSTATE_WAIT:

		//�o���ʒu
		g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 1000, 0.0f);

		g_Player.nCounterState = 0;

		g_nCounterWait++;

		//�ʒu�X�V�����Ȃ����߂̏���
		g_Player.pos.x += 0;
		g_Player.pos.y += 0;
		g_Player.pos.z += 0;

		//�o���܂ł̎���
		if (g_nCounterWait >= 60)
		{
			g_Player.State = PLAYERSTATE_APPEAR;
		}

		//�F�̍X�V
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		break;

		//�v���C���[���S���
	case PLAYERSTATE_DEATH:

		g_nCounterDeath++;

		//���U���g�ւ̑ҋ@����
		if (g_nCounterDeath == 60)
		{
			//���U���g��
			SetFade(FADE_OUT, MODE_RESULT);
		}

		break;
	}

	//�ʒu�X�V
	g_Player.pos.x += g_Player.move.x;
	g_Player.pos.y += g_Player.move.y;
	g_Player.pos.z += g_Player.move.z;

	//�ړ��ʌ���
	g_Player.move.x += (0 - g_Player.move.x) * 0.2f;
	g_Player.move.y += (0 - g_Player.move.y) * 0.2f;

	//��ʊO�ɏo�Ȃ��悤�ɂ���ݒ�
	if (g_Player.pos.x > SCREEN_WIDTH - PLAYER_SIZEX)
	{
		g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZEX;
	}

	if (g_Player.pos.y > SCREEN_HEIGHT - PLAYER_SIZEY)
	{
		g_Player.pos.y = SCREEN_HEIGHT - PLAYER_SIZEY;
	}

	if (g_Player.pos.x < 0 + PLAYER_SIZEX)
	{
		g_Player.pos.x = 0 + PLAYER_SIZEX;
	}

	if (g_Player.pos.y < 0 + PLAYER_SIZEY)
	{
		g_Player.pos.y = 0 + PLAYER_SIZEY;
	}

	//���_���W
	pVtx[0].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[1].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[2].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[3].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.z = (g_Player.pos.z = 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

}

// �v���C���[�̕`�揈��
void DrawPlayer(void)
{
	//���[�J���ϐ�
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_Player.bDisp == true)
	{
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//�e������������
bool HitPlayer(int nDamage)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�̗̓}�C�i�X
	g_Player.nLife -= nDamage;

	//�̗͂����邩�ǂ����̔���
	if (g_Player.nLife <= 0)
	{
		g_nCounterWait = 0;

		if (g_Player.bDisp == true)
		{
			g_Player.nLife = 0;

			//�c�@�}�C�i�X
			g_Player.nRemaining--;

			//�������@
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			//�����A�j���[�V����
			SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			if (g_Player.nRemaining > 0)
			{//�c�@�c���Ă���

				g_Player.State = PLAYERSTATE_WAIT;
			}
			else if (g_Player.nRemaining <= 0)
			{//�c�@�Ȃ�
				g_Player.State = PLAYERSTATE_DEATH;
			}
			g_Player.bDisp = false;	//�v���C���[��\��

		}

		else
		{

			//�q�b�g��
			PlaySound(SOUND_LABEL_SE_HIT);
			if (g_Player.State == PLAYERSTATE_NORMAL)
			{
				//�v���C���[�̏�Ԃ��_���[�W��
				g_Player.State = PLAYERSTATE_DAMAGE;
			}

			//�F�ς�鎞��
			g_Player.nCounterState = 5;

			//�F�̍X�V
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

			return false;

		}
		g_pVtxBuffPlayer->Unlock(); //�A�����b�N
	}
}

// �v���C���[�̒��_���W�ݒ�
void SetVertexPlayer(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//�|���S���̈ʒu
	g_Player.pos = D3DXVECTOR3(1000, 1000, 0.0f);

	//�Ίp���̒���
	g_fLengthPlayer = sqrtf(PLAYER_SIZEX * PLAYER_SIZEX + PLAYER_SIZEY * PLAYER_SIZEY);

	//�Ίp���̊p�x
	g_fAnglePlayer = atan2f(PLAYER_SIZEX, PLAYER_SIZEY);

	//���_���W
	pVtx[0].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[0].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[1].pos.x = (g_Player.pos.x - sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[1].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[2].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.y = (g_Player.pos.y + cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[2].pos.z = (g_Player.pos.z = 0.0f);

	pVtx[3].pos.x = (g_Player.pos.x + sinf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.y = (g_Player.pos.y - cosf(g_fAnglePlayer) * g_fLengthPlayer);
	pVtx[3].pos.z = (g_Player.pos.z = 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//�v���C���[���
PLAYER *GetPlayer(void)
{
	return &g_Player;
}
