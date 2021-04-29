#include "main.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureBlock[BLOCK_TYPE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;			//���_�o�b�t�@�ւ̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];								//�u���b�N
int g_nCntBlock;										//�u���b�N
int g_nCntBlock2;										//�u���b�N
float g_fSaveMove;										//�v���C���[�̃��[�u�ۑ�(�X�N���[���̂���)

														//����������
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�ŃA�C�X�̃|�C���^
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].SaveMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].nLife = 0;
		g_aBlock[nCntBlock].nType = 0;
		g_aBlock[nCntBlock].fBlockWidth = 0.0f;
		g_aBlock[nCntBlock].fBlockHeight = 0.0f;
		g_aBlock[nCntBlock].fTexU = 0.0f;
		g_aBlock[nCntBlock].fTexV = 0.0f;
		g_aBlock[nCntBlock].bUse = false;
	}
	g_nCntBlock = 0;
	g_nCntBlock2 = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&g_apTextureBlock[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",
		&g_apTextureBlock[1]);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);



	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
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
		pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fTexV, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].fTexU);
		pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fTexV, g_aBlock[nCntBlock].fTexU);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//�I������
void UninitBlock(void)
{
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < BLOCK_TYPE; nCntBlock++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureBlock[nCntBlock] != NULL)
		{
			g_apTextureBlock[nCntBlock]->Release();
			g_apTextureBlock[nCntBlock] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//�X�V����
void UpdateBlock(void)
{

	int nCntBlock;
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
							//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{

		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;


			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fBlockWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fBlockHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fBlockWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fBlockHeight, 0.0f);


			if (g_aBlock[nCntBlock].pos.x <= 0 - g_aBlock[nCntBlock].fBlockWidth)
			{
				g_aBlock[nCntBlock].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}
			else if (g_aBlock[nCntBlock].pos.y >= SCREEN_HEIGHT)
			{
				g_aBlock[nCntBlock].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}
			else if (g_aBlock[nCntBlock].pos.y <= 0 - g_aBlock[nCntBlock].fBlockHeight)
			{
				g_aBlock[nCntBlock].bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			}
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();

	//�����u���b�N���v���C���[����������
	BLOCK *pBlock3;
	BLOCK *pBlock;
	PLAYER *pPlayer;
	Enemy *pEnemy;
	//�u���b�N�̎擾
	pPlayer = GetPlayer();
	pEnemy = GetEnemy();
	pBlock3 = &g_aBlock[g_nCntBlock2];
	pBlock = &g_aBlock[0];

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			if (pPlayer->pos.y > pBlock->pos.y &&
				pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pPlayer->pos.x + pPlayer->fWidth / 2 >= pBlock->pos.x &&
				pPlayer->BeforePos.x + pPlayer->fWidth / 2 <= pBlock->pos.x)
			{
				pPlayer->pos.x = pBlock->pos.x - pPlayer->fWidth / 2;
				//pBlock->SaveMove = pBlock->move;

				g_nCntBlock2 = nCntBlock;
				pBlock3 = &g_aBlock[g_nCntBlock2];

				if (pPlayer->pos.x + pPlayer->fWidth / 2.0f >= pBlock3->pos.x)
				{
					if (pBlock3->move.x < 0.0f)
					{
						pPlayer->pos.x += pBlock3->move.x;
					}
				}

			}
			else if (pPlayer->pos.y > pBlock->pos.y &&
				pPlayer->pos.y - pPlayer->fHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pPlayer->pos.x - pPlayer->fWidth / 2 <= pBlock->pos.x + pBlock->fBlockWidth&&
				pPlayer->BeforePos.x - pPlayer->fWidth / 2 >= pBlock->pos.x + pBlock->fBlockWidth)
			{
				pPlayer->pos.x = pBlock->pos.x + pBlock->fBlockWidth + pPlayer->fWidth / 2;
				//pBlock->SaveMove = pBlock->move;

				g_nCntBlock2 = nCntBlock;
				pBlock3 = &g_aBlock[g_nCntBlock2];

				if (pPlayer->pos.x - pPlayer->fWidth / 2.0f <= pBlock3->pos.x + pBlock3->fBlockWidth)
				{
					if (pBlock3->move.x > 0.0f)
					{
						if (pPlayer->pos.x >= SCROLE_START_POS)
						{
							pPlayer->pos.x = SCROLE_START_POS;
							pBlock3->pos.x = pPlayer->pos.x - pPlayer->fWidth / 2.0f - pBlock3->fBlockWidth;
						}
						else
						{
							pPlayer->pos.x += pBlock3->move.x;
						}
					}
				}
			}
#if 0



			if (pEnemy->pos.y > pBlock->pos.y &&
				pEnemy->pos.y - pEnemy->fEnemyHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pEnemy->pos.x + pEnemy->fEnemyWidth / 2 >= pBlock->pos.x &&
				pEnemy->BeforePos.x + pEnemy->fEnemyWidth / 2 <= pBlock->pos.x)
			{
				pEnemy->pos.x = pBlock->pos.x - pEnemy->fEnemyWidth / 2;
				//pBlock->SaveMove = pBlock->move;

				g_nCntBlock2 = nCntBlock;
				pBlock3 = &g_aBlock[g_nCntBlock2];

				if (pEnemy->pos.x + pEnemy->fEnemyWidth / 2.0f >= pBlock3->pos.x)
				{
					if (pBlock3->move.x < 0.0f)
					{
						pEnemy->pos.x += pBlock3->move.x;
					}
				}

			}
			else if (pEnemy->pos.y > pBlock->pos.y &&
				pEnemy->pos.y - pEnemy->fEnemyHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pEnemy->pos.x - pEnemy->fEnemyWidth / 2 <= pBlock->pos.x + pBlock->fBlockWidth&&
				pEnemy->BeforePos.x - pEnemy->fEnemyWidth / 2 >= pBlock->pos.x + pBlock->fBlockWidth)
			{
				pEnemy->pos.x = pBlock->pos.x + pBlock->fBlockWidth + pEnemy->fEnemyWidth / 2;
				//pBlock->SaveMove = pBlock->move;

				g_nCntBlock2 = nCntBlock;
				pBlock3 = &g_aBlock[g_nCntBlock2];

				if (pEnemy->pos.x - pEnemy->fEnemyWidth / 2.0f <= pBlock3->pos.x + pBlock3->fBlockWidth)
				{
					if (pBlock3->move.x > 0.0f)
					{
						pEnemy->pos.x += pBlock3->move.x;
					}
				}
			}
#endif // !1
		}
	}

	if (pPlayer->pos.x >= SCROLE_START_POS)
	{
		if (pPlayer->move.x > 0.0f)
		{
			g_fSaveMove = pPlayer->move.x;
		}
		else if (pPlayer->move.x = 0.0f)
		{
			g_fSaveMove = 0.0f;

		}
	}
}

//�`�揈��
void DrawBlock(void)
{
	int nCntBlock;

	LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();						//�f�o�C�X���擾����
												//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�e�N�X�`��
			pDevice->SetTexture(0, g_apTextureBlock[g_aBlock[nCntBlock].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBlock * 4,					//�`����J�n���钸�_�̃C���f�b�N�X
				2);					//�`�悷��v���~�e�B�u��
		}
	}

}

//�����u���b�N�̐ݒ菈��
void SetMoveBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType, int nLife, float fBlockWidth, float fBlockHeight, float fTexV, float fTexU)
{
	int nCntBlock;
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].fBlockWidth = fBlockWidth;
			g_aBlock[nCntBlock].fBlockHeight = fBlockHeight;
			g_aBlock[nCntBlock].fTexU = fTexU;
			g_aBlock[nCntBlock].fTexV = fTexV;

			//���_���W�̐ݒ�

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
									//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
			g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += (nCntBlock * 4);


			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fBlockWidth, g_aBlock[nCntBlock].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fBlockHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fBlockWidth, g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fBlockHeight, 0.0f);


			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fTexV, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aBlock[nCntBlock].fTexU);
			pVtx[3].tex = D3DXVECTOR2(g_aBlock[nCntBlock].fTexV, g_aBlock[nCntBlock].fTexU);


			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBlock->Unlock();

			g_aBlock[nCntBlock].nType = nType;
			g_aBlock[nCntBlock].nLife = nLife;
			g_aBlock[nCntBlock].move = move;
			g_aBlock[nCntBlock].bUse = true;
			break;
		}
	}
}

//�u���b�N�Ƃ̓����蔻��
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pBeforePos, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	Enemy *pEnemy;
	pEnemy = GetEnemy();

	int nCntBlock;
	BLOCK *pBlock3;


	BLOCK *pBlock;
	bool bLand = false;			//���n���Ă��邩�ǂ���
	pBlock = &g_aBlock[0];
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			if (pPos->x > pBlock->pos.x - fWidth / 2 &&
				pPos->x < pBlock->pos.x + pBlock->fBlockWidth + fWidth / 2 &&
				pPos->y >= pBlock->pos.y &&
				pBeforePos->y <= pBlock->pos.y)
			{
				pPos->y = pBlock->pos.y;
				pMove->y = 0.0f;
				bLand = true;
				pBlock->SaveMove = pBlock->move;
				g_nCntBlock = nCntBlock;
			}
			else if (pPos->x > pBlock->pos.x - fWidth / 2 &&
				pPos->x < pBlock->pos.x + pBlock->fBlockWidth + fWidth / 2 &&
				pPos->y - fHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pBeforePos->y - fHeight >= pBlock->pos.y + pBlock->fBlockHeight)
			{
				pPos->y = pBlock->pos.y + pBlock->fBlockHeight + fHeight;
				pMove->y = 0.0f;
			}

			if (pPos->y > pBlock->pos.y &&
				pPos->y - fHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pPos->x + fWidth / 2 >= pBlock->pos.x &&
				pBeforePos->x + fWidth / 2 <= pBlock->pos.x + g_fSaveMove)
			{
				pPos->x = pBlock->pos.x - fWidth / 2;
				g_nCntBlock2 = nCntBlock;
				pBlock3 = &g_aBlock[g_nCntBlock2];
			}
			else if (pPos->y > pBlock->pos.y &&
				pPos->y - fHeight < pBlock->pos.y + pBlock->fBlockHeight &&
				pPos->x - fWidth / 2 <= pBlock->pos.x + pBlock->fBlockWidth &&
				pBeforePos->x - fWidth / 2 >= pBlock->pos.x + pBlock->fBlockWidth - g_fSaveMove)
			{
				pPos->x = pBlock->pos.x + pBlock->fBlockWidth + fWidth / 2;
				g_nCntBlock2 = nCntBlock;
				pBlock3 = &g_aBlock[g_nCntBlock2];
			}

		}
	}
	return bLand;
}

//�擾����
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//�擾����
BLOCK *GetBlock2(void)
{
	return &g_aBlock[g_nCntBlock];
}

//�擾����
BLOCK *GetBlock3(void)
{
	return &g_aBlock[g_nCntBlock2];
}
