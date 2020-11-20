#include "main.h"
#include "input.h"
#include "player.h"
#include "block.h"
#include "enemy.h"
#include "item.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePolygon[POLYGON_TYPE] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;			//���_�o�b�t�@�ւ̃|�C���^
PLAYER g_Player;											//�v���C���[�\����
int g_nCntAnim;												//�v���C���[�̃A�j���[�V�����J�E���^
int g_nAnimSpan;
int g_nCounterAnim;											//�v���C���[�̃A�j���[�V�����J�E���^
int g_nCntReturn;											//�v���C���[�̕��A
bool g_bAnim;												//�A�j���[�V�����̐؂�ւ�
bool g_bMoveScrole;											//�X�N���[�����邽�߂̕ϐ�
float fScroll;

//����������
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//������
	g_Player.fWidth = POLYGON_WIDTH;
	g_Player.fHeight = POLYGON_HEIGHT;
	g_Player.BeforePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.BeforeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.bJump = false;
	g_Player.nCounterState = 0;
	g_Player.State = PLAYERSTATE_NOMAL;
	g_nCntAnim = 0;
	g_nCounterAnim = 0;
	g_bAnim = false;
	g_bMoveScrole = false;

	g_Player.bJump = true;
	g_Player.bMoveR = false;
	g_Player.bMoveL = false;
	g_Player.nMoveTexX = 0;
	g_Player.nMoveTexY = 0;
	g_Player.nCntTex = 0;
	g_Player.bScroll = false;
	fScroll = 0.0f;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/santa000.png",
		&g_pTexturePolygon[0]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//srand((unsigned int)time(NULL));

	BLOCK *pBlock;
	//�u���b�N�̎擾
	pBlock = GetBlock();

	g_Player.pos = D3DXVECTOR3(g_Player.fWidth / 2, SCREEN_HEIGHT - 40.0f, 0.0f);


	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//�I������
void UninitPlayer(void)
{
	int nCntPolygon;
	for (nCntPolygon = 0; nCntPolygon < POLYGON_TYPE; nCntPolygon++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePolygon[nCntPolygon] != NULL)
		{
			g_pTexturePolygon[nCntPolygon]->Release();
			g_pTexturePolygon[nCntPolygon] = NULL;
		}
	}


	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//�X�V����
void UpdatePlayer(void)
{
	BLOCK *pBlock;
	//�u���b�N�̎擾
	pBlock = GetBlock();
	static float fTexU = 0.0f;		//�e�N�X�`����U���W
	static float fTexV = 0.0f;		//�e�N�X�`����V���W
	int nAnimSpan;

	g_Player.bMoveR = false;
	g_Player.bMoveL = false;
	g_Player.bScroll = false;

	Enemy *pEnemy;
	pEnemy = GetEnemy();

	Item *pItem;
	pItem = GetItem();

	g_Player.BeforePos = g_Player.pos;
	g_Player.move.y += GRAVITY;		//�d��

									//�v���C���[�̃A�j���[�V����
	if (g_Player.move.x > 0.0f)
	{
		g_nCntAnim++;
		if (g_nCntAnim >= 8)
		{
			g_nCounterAnim++;
			if (g_nCounterAnim > 20)
			{
				g_nCounterAnim = 0;
			}
			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
									//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);


			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 0.5f);


			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffPolygon->Unlock();
			g_nCntAnim = 0;
		}
	}
	else if (g_Player.move.x < 0.0f)
	{
		//�v���C���[�̃A�j���[�V����
		g_nCntAnim++;
		if (g_nCntAnim >= 8)
		{
			g_nCounterAnim++;
			if (g_nCounterAnim > 20)
			{
				g_nCounterAnim = 0;
			}

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
									//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + 0.25f * g_nCounterAnim, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f + 0.25f * g_nCounterAnim, 1.0f);

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffPolygon->Unlock();

			g_nCntAnim = 0;
		}
	}

	if (g_Player.move.x == 0.0f || g_Player.move.y < 0.0f)
	{
		VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
								//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
		g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

		if (g_bAnim == false)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPolygon->Unlock();
	}

	//�C�ӂ̃L�[�������ꂽ���ǂ���

	switch (g_Player.State)
	{
	case PLAYERSTATE_NOMAL:
		if ((GetKeyboardPress(DIK_RIGHT) == true || GetKeyboardPress(DIK_D) == true) && g_Player.pos.y - g_Player.fHeight <= SCREEN_HEIGHT)
		{
			if (g_Player.bJump = true)
			{
				g_Player.move.x = POLYGON_SPEED;
			}
			else if (g_Player.bJump = false)
			{
				g_Player.move.x = POLYGON_SPEED * 0.8;
			}
			g_bAnim = false;


			if (g_Player.pos.x >= SCROLE_START_POS)
			{
				for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
				{
					pBlock->pos.x += -g_Player.move.x;
				}
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					pEnemy->pos.x += -g_Player.move.x;
				}
				for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
				{
					pItem->pos.x += -g_Player.move.x;
				}
			}
		}
		else if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_A) == true)
		{
			if (g_Player.bJump = true)
			{
				g_Player.move.x = -POLYGON_SPEED;
			}
			else if (g_Player.bJump = false)
			{
				g_Player.move.x = -POLYGON_SPEED * 0.8;
			}
			g_bAnim = true;
		}
		else if (GetKeyboardPress(DIK_RIGHT) == false && GetKeyboardPress(DIK_D) == false)
		{
			g_Player.move.x = 0.0f;
		}
		else if (GetKeyboardPress(DIK_LEFT) == false && GetKeyboardPress(DIK_A) == false)
		{
			g_Player.move.x = 0.0f;
		}

		break;

	case PLAYERSTATE_DAMAGE:
		g_Player.move.x = 0.0f;
		g_Player.nCounterState--;
		if (g_Player.nCounterState <= 0)
		{
			g_Player.State = PLAYERSTATE_NOMAL;

			VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
			//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
			g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffPolygon->Unlock();
		}

		break;
	}


	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		g_nCntAnim = 29;
	}
	else if (GetKeyboardPress(DIK_LEFT) == true)
	{
		g_nCntAnim = 29;
	}


	g_Player.pos.y += g_Player.move.y;
	g_Player.pos.x += g_Player.move.x;

	//�v���C���[�ƃu���b�N�̓����蔻��

	CollisionBlock(&g_Player.pos, &g_Player.BeforePos, &g_Player.move, g_Player.fWidth, g_Player.fHeight);

	if (CollisionBlock(&g_Player.pos, &g_Player.BeforePos, &g_Player.move, g_Player.fWidth, g_Player.fHeight) == true)	//���n���Ă�����
	{
		g_Player.bJump = false;
	}

	//�u���b�N�̏�ɏ��ƃv���C���[����������
	BLOCK *pBlock2;
	//�u���b�N�̎擾
	pBlock2 = GetBlock2();
	pEnemy = GetEnemy();

	g_Player.pos += pBlock2->SaveMove;
	if (g_Player.pos.x >= SCROLE_START_POS)
	{
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			pEnemy->pos += -pBlock2->SaveMove;
		}
	}

	//�����u���b�N�ɏ���Ă�Ƃ��̃X�N���[������
	pBlock = GetBlock();
	pEnemy = GetEnemy();
	pItem = GetItem();
	if (g_Player.pos.x >= SCROLE_START_POS)
	{
		for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
		{
			pBlock->pos.x += -pBlock2->move.x;
		}
		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			pEnemy->pos.x += -pBlock2->move.x;
		}
		for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
		{
			pItem->pos.x += -pBlock2->move.x;
		}
		//CollisionBlock(&g_Player.pos, &g_Player.BeforePos, &g_Player.move, g_Player.fWidth, g_Player.fHeight);

	}

	//�X�y�[�X�L�[���������Ƃ��̏���
	if (g_Player.State == PLAYERSTATE_NOMAL)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_Player.bJump == false)
			{
				g_Player.move.y = POLYGON_JUMP;
				g_Player.bJump = true;
			}
		}
	}

	//�v���C���[�̈ړ�����
	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Player.pos.x <= 0.0f + POLYGON_WIDTH / 2.0f)
	{
		g_Player.pos.x = 0.0f + POLYGON_WIDTH / 2.0f;		//�ʒu�̍X�V
	}
	else if (g_Player.pos.x >= SCROLE_START_POS)
	{
		g_Player.pos.x = SCROLE_START_POS;
	}
	if (g_Player.pos.y - g_Player.fHeight >= SCREEN_HEIGHT)
	{
		g_nCntReturn++;
		if (g_nCntReturn >= 50)
		{
			g_Player.pos = D3DXVECTOR3(g_Player.fWidth / 2, 0.0f, 0.0f);
			g_Player.move.y = 0.1f;
			g_Player.bJump = true;
			g_nCntReturn = 0;
		}
	}

	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y - POLYGON_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + POLYGON_WIDTH / 2, g_Player.pos.y, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();


}

//�`�揈��
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;					//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();						//�f�o�C�X���擾����
												//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);			//���_�t�H�[�}�b�g�̐ݒ�

	pDevice->SetTexture(0, g_pTexturePolygon[0]);	//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,					//�J�n���钸�_�̃C���f�b�N�X
		2);					//�`�悷��v���~�e�B�u��
}

//�_���[�W����
void HitPlayer(void)
{
	g_Player.State = PLAYERSTATE_DAMAGE;
	g_Player.nCounterState = 50;

	VERTEX_2D *pVtx;		//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N���A���_�f�[�^�̃|�C���^���擾
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPolygon->Unlock();
}

//�擾����
PLAYER *GetPlayer(void)
{
	return &g_Player;
}

