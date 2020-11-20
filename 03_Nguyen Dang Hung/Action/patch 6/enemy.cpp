#include "main.h"
#include "enemy.h"
#include "block.h"
#include "player.h"
#include "score.h"
#include "sound.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureEnemy[ENEMY_TYPE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;			//���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];								//�G
int g_nCntEnemy;										//�G�̐ݒu�̂��߂̕ϐ�
bool g_bKnockBack;										//�v���C���[���m�b�N�o�b�N������
int g_fSaveMove1;										//�X�N���[��������̂ɕK�v

//����������
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�ŃA�C�X�̃|�C���^
	int nCntEnemy;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�G�̏�����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].BeforePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].fEnemyWidth = 20.0f;
		g_aEnemy[nCntEnemy].fEnemyHeight = 20.0f;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
	}
	g_nCntEnemy = 0;
	g_fSaveMove1 = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/grinch001.png",
		&g_apTextureEnemy[0]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffEnemy->Unlock();
}

//�I������
void UninitEnemy(void)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < ENEMY_TYPE; nCntEnemy++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureEnemy[nCntEnemy] != NULL)
		{
			g_apTextureEnemy[nCntEnemy]->Release();
			g_apTextureEnemy[nCntEnemy] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//�X�V����
void UpdateEnemy(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	if (pPlayer->pos.x >= SCROLE_START_POS)
	{
		if (pPlayer->move.x > 0.0f)
		{
			g_fSaveMove1 = pPlayer->move.x;
		}
		else if (pPlayer->move.x = 0.0f)
		{
			g_fSaveMove1 = 0.0f;

		}
	}

	BLOCK *pBlock2;
	//�u���b�N�̎擾
	pBlock2 = GetBlock2();


	BLOCK *pBlock;
	//�u���b�N�̎擾
	pBlock = GetBlock();

	int nCntEnemy;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			g_aEnemy[nCntEnemy].BeforePos = g_aEnemy[nCntEnemy].pos;
			g_aEnemy[nCntEnemy].move.y += GRAVITY;		//�d��
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

			//�G�ƃu���b�N�̓����蔻��
			CollisionBlock(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].BeforePos, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fEnemyWidth, g_aEnemy[nCntEnemy].fEnemyHeight);

			pBlock = GetBlock();
			for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
			{
				if (g_aEnemy[nCntEnemy].pos.y > pBlock->pos.y &&
					g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight < pBlock->pos.y + pBlock->fBlockHeight &&
					g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2 >= pBlock->pos.x &&
					g_aEnemy[nCntEnemy].BeforePos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2 <= pBlock->pos.x + g_fSaveMove1)
				{
					if (g_aEnemy[nCntEnemy].nType == 0 || pBlock->nType == 1)
					{
						g_aEnemy[nCntEnemy].move.x *= -1.0f;
					}
				}
				else if (g_aEnemy[nCntEnemy].pos.y > pBlock->pos.y &&
					g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight < pBlock->pos.y + pBlock->fBlockHeight &&
					g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2 <= pBlock->pos.x + pBlock->fBlockWidth &&
					g_aEnemy[nCntEnemy].BeforePos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2 >= pBlock->pos.x + pBlock->fBlockWidth - g_fSaveMove1)
				{
					if (g_aEnemy[nCntEnemy].nType == 0 || pBlock->nType == 1)
					{
						g_aEnemy[nCntEnemy].move.x *= -1.0f;
					}
				}
			}

			//�G�ƃv���C���[�̓����蔻��
			if (g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f <= pPlayer->pos.x + pPlayer->fWidth / 2.0f &&
				g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f >= pPlayer->pos.x - pPlayer->fWidth / 2.0f &&
				g_aEnemy[nCntEnemy].pos.y >= pPlayer->pos.y - pPlayer->fHeight &&
				g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight <= pPlayer->pos.y)
			{
				if (g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight >= pPlayer->BeforePos.y)
				{
					//�v���C���[���ォ�炠��������
					pPlayer->move.y = POLYGON_JUMP * 0.8f;
					SetScore(300);
					//PlaySound(SOUND_LABEL_ENEMY_KILL);
				}
				else
				{
					//SetExplosion(g_posMother, (1.0f, 0.0f, 0.0f, 0.0f));
					//PlaySound(SOUND_LABEL_PLAYER_DAMAGE);
					//g_aEnemy[nCntEnemy].bUse = false;
					HitPlayer();
				}
				g_aEnemy[nCntEnemy].bUse = false;
			}


			//�v���C���[�̃A�j���[�V����
			if (g_aEnemy[nCntEnemy].move.x != 0.0f)
			{
				g_aEnemy[nCntEnemy].nCounterAnim++;
				if (g_aEnemy[nCntEnemy].nCounterAnim++ >= 16)
				{
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim > 20)
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}


					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f *g_aEnemy[nCntEnemy].nPatternAnim, 0.5f);
					pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_aEnemy[nCntEnemy].nPatternAnim, 0.5f);

					g_aEnemy[nCntEnemy].nCounterAnim = 0;
				}
			}
			

			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f);


		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//�`�揈��
void DrawEnemy(void)
{
	int nCntEnemy;

	LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();						//�f�o�C�X���擾����
												//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//�e�N�X�`��
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntEnemy * 4,							//�`����J�n���钸�_�̃C���f�b�N�X
				2);										//�`�悷��v���~�e�B�u��
		}
	}
}

//�ݒ菈��
void SetEnemy(D3DXVECTOR3 pos, int nType, D3DXVECTOR3 move, int nLife, float fEnemyWidth, float fEnemyHeight)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].fEnemyWidth = 65.0f;
			g_aEnemy[nCntEnemy].fEnemyHeight = 65.0f;
			//���_���W�̐ݒ�

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
			//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += (nCntEnemy * 4);

			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y - g_aEnemy[nCntEnemy].fEnemyHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fEnemyWidth / 2.0f, g_aEnemy[nCntEnemy].pos.y, 0.0f);

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffEnemy->Unlock();

			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].move = move;
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}
}

//�擾����
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

