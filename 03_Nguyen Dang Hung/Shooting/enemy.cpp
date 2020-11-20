#include "enemy.h"
#include "player.h"
#include "explosion.h"
#include "score.h"
#include "sound.h"
#include "bullet.h"
#include <stdlib.h>
#include <time.h>
#include "fade.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_apTextureEnemy[MAX_ENEMY_TYPE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;
ENEMY g_aEnemy[MAX_ENEMY];								//�G�̏��
int g_nCounterAnimEnemy;								//�A�j���[�V�����J�E���^�[
int g_nPatternAnimEnemy;								//�A�j���[�V�����p�^�[��No.
int g_Bulletfiring;
int g_Bulletfiring1;
int g_Bulletfiring2;
bool g_bFiring;
float g_fLength;
float g_fAngle;
D3DXVECTOR3 g_pos;
int nCount;
int g_FadeCount;
int g_EnemyCounter;

// ����������
HRESULT InitEnemy(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/zerg000.png", &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/deadSpace000.png", &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/deadSpace001.png", &g_apTextureEnemy[2]);

	//�\���̏�����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;					//���
		g_aEnemy[nCntEnemy].nCounterState = 0;							//��ԃJ�E���^�[
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�̗�
		g_aEnemy[nCntEnemy].bUse = false;
	}

	nCount = 0;
	g_FadeCount = 0;
	g_EnemyCounter = 0;

	srand((unsigned int)time(NULL));

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		//���_���W
		SetVertexEnemy(nCntEnemy);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�̃J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_�̏��̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 0.0f);

	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	return S_OK;
}

//�@�I������
void UninitEnemy(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
	//�e�N�X�`���̊J��
	for (int nCntEnemyType = 0; nCntEnemyType < MAX_ENEMY_TYPE; nCntEnemyType++)
	{
		if (g_apTextureEnemy[nCntEnemyType] != NULL)
		{
			g_apTextureEnemy[nCntEnemyType]->Release();
			g_apTextureEnemy[nCntEnemyType] = NULL;
		}
	}
}

// �X�V����
void UpdateEnemy(void)
{

	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	g_nCounterAnimEnemy++;

	g_Bulletfiring++;

	rand();

	g_EnemyCounter++;

	//�A�j���[�V����
	if ((g_nCounterAnimEnemy % 12) == 0)
	{
		g_nPatternAnimEnemy = (g_nPatternAnimEnemy + 1) % 2;		//�A�j���[�V�����p�^�[��No.�X�V
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{

			g_fLength = (pEnemy->pos.x * pPlayer->pos.x + pEnemy->pos.y * pPlayer->pos.y);

			//�e�̔���
			if (pEnemy->nType == 0)
			{
				if ((g_Bulletfiring % 120) == 0)
				{
					SetBullet(pEnemy->pos, D3DXVECTOR3(0.0f, 8.24f, 0.0f), ENEMY_BULLET, BULLETTYPE_ENEMY);
				}
			}

			if (pEnemy->nType == 1)
			{
				if ((g_Bulletfiring % 180) == 0)
				{
					SetBullet(pEnemy->pos, D3DXVECTOR3(0.0f, 8.24f, 0.0f), ENEMY_BULLET, BULLETTYPE_ENEMY);
				}
			}

			if (pEnemy->nType == 2)
			{
				if ((g_Bulletfiring % 150) == 0)
				{
					SetBullet(pEnemy->pos, D3DXVECTOR3(0.0f, 8.24f, 0.0f), ENEMY_BULLET, BULLETTYPE_ENEMY);
				}
			}

			//���_�̏��̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (0.5f * g_nPatternAnimEnemy), 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f + (0.5f * g_nPatternAnimEnemy), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.5f + (0.5f * g_nPatternAnimEnemy), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f + (0.5f * g_nPatternAnimEnemy), 0.0f);

			//�ʒu�̍X�V
			pEnemy->pos.x += pEnemy->move.x;
			pEnemy->pos.y += pEnemy->move.y;

			/*if (pEnemy->pos.x > SCREEN_WIDTH - ENEMY_SIZE)
			{
			pEnemy->move.x *= -1;
			}*/

			if (pEnemy->pos.y > SCREEN_HEIGHT + 200)
			{
				pEnemy->bUse = false;
			}

			/*if (pEnemy->pos.x < 0 + ENEMY_SIZE)
			{
			pEnemy->move.x *= -1;
			}*/

			//���_���W
			pVtx[0].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_SIZE, pEnemy->pos.y + ENEMY_SIZE, 0);
			pVtx[1].pos = D3DXVECTOR3(pEnemy->pos.x - ENEMY_SIZE, pEnemy->pos.y - ENEMY_SIZE, 0);
			pVtx[2].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_SIZE, pEnemy->pos.y + ENEMY_SIZE, 0);
			pVtx[3].pos = D3DXVECTOR3(pEnemy->pos.x + ENEMY_SIZE, pEnemy->pos.y - ENEMY_SIZE, 0);

			switch (g_aEnemy[nCntEnemy].State)
			{
			case ENEMYSTATE_DAMAGE:

				g_aEnemy[nCntEnemy].nCounterState--;

				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{
					g_aEnemy[nCntEnemy].State = ENEMYSTATE_NORMAL;

					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}

				break;
			}

			if (pPlayer->State == PLAYERSTATE_NORMAL)
			{
				if (pPlayer->pos.y + PLAYER_HITAREA > pEnemy->pos.y - ENEMY_SIZE && pPlayer->pos.y - PLAYER_HITAREA < pEnemy->pos.y + ENEMY_SIZE &&
					pPlayer->pos.x + PLAYER_HITAREA > pEnemy->pos.x - ENEMY_SIZE && pPlayer->pos.x - PLAYER_HITAREA < pEnemy->pos.x + ENEMY_SIZE)
				{
					HitPlayer(pPlayer->nLife);
				}
			}

		}

		pVtx += 4;
	}

	if ((g_EnemyCounter % 124) == 0)
	{
		SetEnemy(D3DXVECTOR3(ENEMY_SIZE + rand() % 700, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), rand() % 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}

	if ((g_EnemyCounter % 96) == 0)
	{
		SetEnemy(D3DXVECTOR3(ENEMY_SIZE + 700.0f + rand() % 700, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), rand() % 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}

	if ((g_EnemyCounter % 153) == 0)
	{
		SetEnemy(D3DXVECTOR3(ENEMY_SIZE + 700.0f + rand() % 1800, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f), rand() % 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	}
	g_pVtxBuffEnemy->Unlock(); //�A�����b�N
}

// �`�揈��
void DrawEnemy(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�|���S���̕`��
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureEnemy[pEnemy->nType]);

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

// �G�̐ݒ�
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, D3DXCOLOR col)
{
	ENEMY *pEnemy;
	pEnemy = &g_aEnemy[0];

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == false)
		{
			//�G�̐��J�E���g
			//nCount++;

			//�ړ���
			pEnemy->move.x = move.x;
			pEnemy->move.y = move.y;

			//�ʒu
			pEnemy->pos = pos;

			//���_���W
			SetVertexEnemy(nCntEnemy);

			//�^�C�v
			pEnemy->nType = nType;

			//�G�̗̑�
			pEnemy->nLife = pEnemy->nType + 3;

			//�F
			pEnemy->col = col;
			pVtx[0].col = pEnemy->col;
			pVtx[1].col = pEnemy->col;
			pVtx[2].col = pEnemy->col;
			pVtx[3].col = pEnemy->col;

			//�g���Ă����Ԃ�
			pEnemy->bUse = true;

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffEnemy->Unlock(); //�A�����b�N
}

// �e������������
bool HitEnemy(int nIdx, int nDamage)
{
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	//�_���[�W�v�Z
	g_aEnemy[nIdx].nLife -= nDamage;

	//�̗͂�0�̎�
	if (g_aEnemy[nIdx].nLife <= 0)
	{

		//nCount--;

		SetExplosion(g_aEnemy[nIdx].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		g_aEnemy[nIdx].bUse = false;
		AddScore(200);
		return true;
	}

	else
	{
		PlaySound(SOUND_LABEL_SE_HIT);
		g_aEnemy[nIdx].State = ENEMYSTATE_DAMAGE;
		g_aEnemy[nIdx].nCounterState = 10;

		pVtx += nIdx * 4;

		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		return false;
	}
	g_pVtxBuffEnemy->Unlock(); //�A�����b�N
}

// �G�̒��_���W�̐ݒ�
void SetVertexEnemy(int nIdx)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_SIZE, g_aEnemy[nIdx].pos.y + ENEMY_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x - ENEMY_SIZE, g_aEnemy[nIdx].pos.y - ENEMY_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_SIZE, g_aEnemy[nIdx].pos.y + ENEMY_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nIdx].pos.x + ENEMY_SIZE, g_aEnemy[nIdx].pos.y - ENEMY_SIZE, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
// �G�̏K��
ENEMY*GetEnemy(void)
{
	return &g_aEnemy[0];
}

