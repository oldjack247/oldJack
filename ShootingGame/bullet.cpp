#include "Bullet.h"
#include "enemy.h"
#include "Player.h"
#include "score.h"
#include "effect.h"

//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nType;			//�e�̎��
	BULLETTYPE Type;
	bool bUse;			//�e�̎g�p��
}BULLET;


//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTextureBullet[2] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];							//�e�̏��

//-----------------------------------------------------------------------------
// �e�̒��_���W�̐ݒ�
//-----------------------------------------------------------------------------
void SetVertexBullet(int nIdx)
{
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nIdx * 4;

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0);
	pVtx[1].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x - BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0);
	pVtx[2].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y + BULLET_SIZE, 0);
	pVtx[3].pos = D3DXVECTOR3(g_aBullet[nIdx].pos.x + BULLET_SIZE, g_aBullet[nIdx].pos.y - BULLET_SIZE, 0);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//-----------------------------------------------------------------------------
// ����������
//-----------------------------------------------------------------------------
HRESULT InitBullet(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ammo000.png", &g_pTextureBullet[0]);
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ammo000.png", &g_pTextureBullet[1]);


	//�\���̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].Type = BULLETTYPE_PLAYER;
		g_aBullet[nCntBullet].nType = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W
		SetVertexBullet(nCntBullet);

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

		pVtx += 4;
	}
	g_pVtxBuffBullet->Unlock(); //�A�����b�N

	return S_OK;
}

//-----------------------------------------------------------------------------
//�@�I������
//-----------------------------------------------------------------------------
void UninitBullet(void)
{
	//�o�b�t�@�̊J��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
	//�e�N�X�`���̊J��
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET_TYPE; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// �X�V����
//-----------------------------------------------------------------------------
void UpdateBullet(void)
{
	BULLET* pBullet;
	pBullet = &g_aBullet[0];



	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{

		if (pBullet->bUse == true)
		{

			//�ʒu�̍X�V
			pBullet->pos.x += pBullet->move.x;
			pBullet->pos.y += pBullet->move.y;

			//���_���W
			SetVertexBullet(nCntBullet);

			//�����`�F�b�N
			/*pBullet->nLife--;
			if (pBullet->nLife == 0)
			{
				SetExplosion(pBullet->pos,D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
				pBullet->bUse = false;
			}*/

			//��ʊO�`�F�b�N
			if (pBullet->pos.y < 0)
			{
				pBullet->bUse = false;
			}
			if (pBullet->pos.y > SCREEN_HEIGHT)
			{
				pBullet->bUse = false;
			}

			if (pBullet->pos.x > SCREEN_WIDTH)
			{
				pBullet->bUse = false;
			}

			if (pBullet->pos.x < 0)
			{
				pBullet->bUse = false;
			}

			//�v���C���[�̒e
			if (pBullet->Type == BULLETTYPE_PLAYER)
			{

				//�G�̏��
				ENEMY* pEnemy;
				pEnemy = GetEnemy();

				//���@�̏��
				PLAYER* pPlayer;
				pPlayer = GetPlayer();

				//�G�t�F�N�g���
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.75f, 0.75f, 0.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G�ƒe�̓����蔻��
						if (pBullet->pos.y > pEnemy->pos.y - ENEMY_SIZE && pBullet->pos.y < pEnemy->pos.y + ENEMY_SIZE &&
							pBullet->pos.x > pEnemy->pos.x - ENEMY_SIZE && pBullet->pos.x < pEnemy->pos.x + ENEMY_SIZE)
						{
							HitEnemy(nCntEnemy, 1);

							pBullet->bUse = false;

						}

						if (pEnemy->nType == 2)
						{
							if (pBullet->pos.y + BULLET_SIZE > pEnemy->pos.y - BIG_ENEMY_SIZEX && pBullet->pos.y - BULLET_SIZE < pEnemy->pos.y + BIG_ENEMY_SIZEY &&
								pBullet->pos.x + BULLET_SIZE > pEnemy->pos.x - BIG_ENEMY_SIZEX && pBullet->pos.x - BULLET_SIZE < pEnemy->pos.x + BIG_ENEMY_SIZEY)
							{
								HitEnemy(nCntEnemy, 1);

								pBullet->bUse = false;
							}
						}
					}
				}
			}

			//�G�̒e
			if (pBullet->Type == BULLETTYPE_ENEMY)
			{

				//�G�̏��
				ENEMY* pEnemy;
				pEnemy = GetEnemy();

				//���@�̏��
				PLAYER* pPlayer;
				pPlayer = GetPlayer();

				//�G�t�F�N�g���
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 0.75f, 0.75f, 0.75f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				if (pPlayer->State == PLAYERSTATE_NORMAL)
				{//���@�ƒe�̓����蔻��
					if (pBullet->pos.y + BULLET_SIZE > pPlayer->pos.y - PLAYER_SIZEY && pBullet->pos.y - BULLET_SIZE < pPlayer->pos.y + PLAYER_SIZEY &&
						pBullet->pos.x + BULLET_SIZE > pPlayer->pos.x - PLAYER_SIZEX && pBullet->pos.x - BULLET_SIZE < pPlayer->pos.x + PLAYER_SIZEX)
					{
						HitPlayer(1);
						pBullet->bUse = false;
					}
				}
			}

			//�q�@�̒e
			if (pBullet->Type == BULLETTYPE_SUBPLAYER)
			{
				//�G�̏��
				ENEMY* pEnemy;
				pEnemy = GetEnemy();

				//���@�̂̏��
				PLAYER* pPlayer;
				pPlayer = GetPlayer();

				//�G�t�F�N�g���
				SetEffect(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.85f));
				SetEffect(pBullet->pos, D3DXCOLOR(0.75f, 0.75f, 0.75f, 0.5f));
				SetEffect(pBullet->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{//�G�ƒe�̓����蔻��
						if (pBullet->pos.y > pEnemy->pos.y - ENEMY_SIZE && pBullet->pos.y < pEnemy->pos.y + ENEMY_SIZE &&
							pBullet->pos.x > pEnemy->pos.x - ENEMY_SIZE && pBullet->pos.x < pEnemy->pos.x + ENEMY_SIZE)
						{

							HitEnemy(nCntEnemy, 1);

							pBullet->bUse = false;

						}

						if (pEnemy->nType == 2)
						{
							if (pBullet->pos.y + BULLET_SIZE > pEnemy->pos.y - BIG_ENEMY_SIZEX && pBullet->pos.y - BULLET_SIZE < pEnemy->pos.y + BIG_ENEMY_SIZEY &&
								pBullet->pos.x + BULLET_SIZE > pEnemy->pos.x - BIG_ENEMY_SIZEX && pBullet->pos.x - BULLET_SIZE < pEnemy->pos.x + BIG_ENEMY_SIZEY)
							{
								HitEnemy(nCntEnemy, 1);

								pBullet->bUse = false;
							}
						}
					}
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �`�揈��
//-----------------------------------------------------------------------------
void DrawBullet(void)
{
	//���[�J��
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);



	//�|���S���̕`��
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].nType]);

		if (g_aBullet[nCntBullet].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//-----------------------------------------------------------------------------
// �e�̐ݒ�
//-----------------------------------------------------------------------------
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, BULLETTYPE Type)
{
	BULLET* pBullet;
	pBullet = &g_aBullet[0];

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == false)
		{
			//�ʒu
			pBullet->pos = pos;

			//���_���W
			SetVertexBullet(nCntBullet);

			//�ړ���
			pBullet->move.x = move.x;
			pBullet->move.y = move.y;

			pBullet->Type = Type;

			//����
			pBullet->nType = nType;

			pBullet->bUse = true;
			break;
		}
	}
}
