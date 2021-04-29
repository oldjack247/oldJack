#include "enemy.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_ENEMY	(8)
#define MOVE_ENEMY (1.5f)
#define MAX_TEXTURE (10)
#define SHADOW_SIZE_ENEMY (30.0f)

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
Enemy g_aEnemy[MAX_ENEMY];							// ���f�����
LPDIRECT3DTEXTURE9 g_aTextureEnemy[MAX_TEXTURE] = {};	//�e�N�X�`���ւ̃|�C���^
D3DXVECTOR3 g_vtxMinEnemy, g_vtxMaxEnemy;				// �e���_���W�̍ŏ��l�A�ő�l
const char *g_aTextureNameEnemy[MAX_TEXTURE];
int g_nCntEffectGetE = 0;

//=============================================================================
// �G�̏���������
//=============================================================================
HRESULT InitEnemy(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//�e�N�X�`���̏�����
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++) {
		g_aTextureNameEnemy[nCntTex] = "";
	}
	//�ǂݍ��ރe�N�X�`���̐ݒ�
	g_aTextureNameEnemy[0] = "data/TEXTURE/Joker_map_D.png";	//�Œ�

																//�e�N�X�`���̓ǂݍ���
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++) {
		if (g_aTextureNameEnemy[nCntTex] != NULL) {
			D3DXCreateTextureFromFile(pDevice,
				g_aTextureNameEnemy[nCntTex],
				&g_aTextureEnemy[nCntTex]);
		}
	}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//������
		g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		g_aEnemy[nCntEnemy].nNumMatEnemy = NULL;
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(50.0f, 10.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rotDest = g_aEnemy[nCntEnemy].rot;
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].moverot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nLife = 50;
		g_aEnemy[nCntEnemy].nIdx = -1;
		g_aEnemy[nCntEnemy].bUse = false;
		for (int nCntTex = 0; nCntTex < 24; nCntTex++) {
			g_aEnemy[nCntEnemy].aTexType[nCntTex] = NULL;
		}

		//X�t�@�C���̓ǂݍ���
		if (FAILED(D3DXLoadMeshFromX("data/MODEL/JokerStatue.x",
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aEnemy[nCntEnemy].pBuffMatEnemy,
			NULL,
			&g_aEnemy[nCntEnemy].nNumMatEnemy,
			&g_aEnemy[nCntEnemy].pMeshEnemy)))
		{
			return E_FAIL;
		}
	}
	//�G�̐ݒu
	SetEnemy(D3DXVECTOR3(0.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(200.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	///SetEnemy(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	//SetEnemy(D3DXVECTOR3(200.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	//SetEnemy(D3DXVECTOR3(-200.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 100.0f, 0.0f));

	return S_OK;
}

//=============================================================================
// �G�̏I������
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//���b�V���̔j��
		if (g_aEnemy[nCntEnemy].pBuffMatEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pBuffMatEnemy->Release();
			g_aEnemy[nCntEnemy].pBuffMatEnemy = NULL;
		}

		//�}�e���A���̔j��
		if (g_aEnemy[nCntEnemy].pMeshEnemy != NULL)
		{
			g_aEnemy[nCntEnemy].pMeshEnemy->Release();
			g_aEnemy[nCntEnemy].pMeshEnemy = NULL;
		}
	}
}

//=============================================================================
// �G�̍X�V����
//=============================================================================
void UpdateEnemy(void)
{
	Player *pPlayer = GetPlayer();
	Enemy *pEnemy = &g_aEnemy[0];
	BULLET *pBullet = GetBullet();

	// �ϐ��錾
	int nCntTrue = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, nCntEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			MoveEnemy(nCntEnemy);

			//�v���C���[���������������
			if ((pPlayer->pos.x - pPlayer->vtxMinPlayer.x) < (pEnemy->pos.x + pEnemy->vtxMaxEnemy.x) &&
				(pPlayer->pos.x + pPlayer->vtxMaxPlayer.x) > (pEnemy->pos.x - pEnemy->vtxMaxEnemy.x) &&
				(pPlayer->pos.z - pPlayer->vtxMinPlayer.z) < (pEnemy->pos.z + pEnemy->vtxMaxEnemy.z) &&
				(pPlayer->pos.z + pPlayer->vtxMaxPlayer.z) > (pEnemy->pos.z - pEnemy->vtxMaxEnemy.z))
			{
				if (pPlayer->nLife <= 100)
				{
					//�v���C���[����
					HitPlayer(100);
				}
			}

			//�e������������G�Ƀ_���[�W
			if ((pBullet->pos.x) < (pEnemy->pos.x + pEnemy->vtxMaxEnemy.x) &&
				(pBullet->pos.x) > (pEnemy->pos.x - pEnemy->vtxMaxEnemy.x) &&
				(pBullet->pos.z) < (pEnemy->pos.z + pEnemy->vtxMaxEnemy.z) &&
				(pBullet->pos.z) > (pEnemy->pos.z - pEnemy->vtxMaxEnemy.z))
			{
				if (pBullet->type == BULLETTYPE_PLAYER)
				{
					HitEnemy(5);
				}
			}

			if (pEnemy->nLife <= 0)
			{
				//�g��������
				pEnemy->bUse = false;

				//�e������
				//DeleteShadow(pEnemy->nIdx);
			}

			//�g���Ă���J�E���g
			nCntTrue++;
		}
	}
}

//=============================================================================
//�G�̕`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	pDevice = GetDevice();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatEnemy; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aTextureEnemy[g_aEnemy[nCntEnemy].aTexType[nCntMat]]);
				//���f��(�p�[�c)�̕`��
				g_aEnemy[nCntEnemy].pMeshEnemy->DrawSubset(nCntMat);
			}
			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
// �G�̈ړ�����
//=============================================================================
void MoveEnemy(int nCntEnemy)
{
	//�ϐ��錾
	Player *pPlayer = GetPlayer();
	D3DXVECTOR3 EnemyV;		//�v���C���[�Ɍ���

							//�ړI�̉�]�p�𐧌�
	if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rotDest.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rotDest.y -= D3DX_PI * 2.0f;
	}

	//�����̍X�V
	g_aEnemy[nCntEnemy].rot.y += (g_aEnemy[nCntEnemy].rotDest.y - g_aEnemy[nCntEnemy].rot.y) * -0.05f;

	//���݂̉�]�p�𐧌�
	if (g_aEnemy[nCntEnemy].rot.y < -D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rot.y += D3DX_PI * 2.0f;
	}
	else if (g_aEnemy[nCntEnemy].rot.y > D3DX_PI)
	{
		//3.14f���傫���Ȃ�����-3.14f�ɂ���
		g_aEnemy[nCntEnemy].rot.y -= D3DX_PI * 2.0f;
	}

	//�G���v���C���[������悤�ɂ���
	EnemyV = pPlayer->pos - g_aEnemy[nCntEnemy].pos;

	//�p�x�����߂�
	float fAngle = (float)atan2(EnemyV.x, EnemyV.z);

	//���@�ƒ��S�_�̑Ίp���̒���
	float fLength = sqrtf((EnemyV.x * EnemyV.x) + (EnemyV.z * EnemyV.z));

	//�v���C���[�̒ǔ�
	g_aEnemy[nCntEnemy].move.x = MOVE_ENEMY * sinf(fAngle);
	g_aEnemy[nCntEnemy].move.z = MOVE_ENEMY * cosf(fAngle);

	//�����̍X�V
	g_aEnemy[nCntEnemy].rotDest.y = fAngle;

	//�ʒu�̍X�V
	g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
	g_aEnemy[nCntEnemy].pos.z += g_aEnemy[nCntEnemy].move.z;

	//�e�̈ړ�
	SetPositionShadow(g_aEnemy[nCntEnemy].nIdx, g_aEnemy[nCntEnemy].pos);
}

//=============================================================================
// �G�̐ݒu
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATERIAL *pMat;	//�}�e���A���ւ̃|�C���^
	const char *Xfilename = NULL;//�ǂݍ��ރ��f����

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++) {
		if (g_aEnemy[nCntEnemy].bUse == false) {

			//���f���̏��̐ݒ�
			//g_aAIEnemy[nCntEnemy].pBuffMat = g_aEnemyData[ntype].pBuffMat;
			//g_aAIEnemy[nCntEnemy].nNumMat = g_aEnemyData[ntype].nNumMat;
			//g_aAIEnemy[nCntEnemy].pMesh = g_aAIEnemyData[ntype].pMesh;

			g_aEnemy[nCntEnemy].bUse = true;
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;

			//�}�e���A�����ɑ΂���|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].pBuffMatEnemy->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].nNumMatEnemy; nCntMat++) {
				if (pMat[nCntMat].pTextureFilename != NULL) {
					for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++) {
						//�����񂪈�v�Ńe�N�X�`���̔ԍ������߂�
						if (strcmp(pMat[nCntMat].pTextureFilename, g_aTextureNameEnemy[nCntTex]) == 0) {
							g_aEnemy[nCntEnemy].aTexType[nCntMat] = nCntTex;
							break;
						}
					}
				}
			}

			//���_�����擾
			nNumVtx = g_aEnemy[nCntEnemy].pMeshEnemy->GetNumVertices();
			//���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMeshEnemy->GetFVF());
			//���_�o�b�t�@�����b�N
			g_aEnemy[nCntEnemy].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++) {
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; 	//���_���W�̑��
															//X���W
				if (g_aEnemy[nCntEnemy].vtxMinEnemy.x > vtx.x || g_aEnemy[nCntEnemy].vtxMinEnemy.x == NULL) {
					g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
				}
				if (g_aEnemy[nCntEnemy].vtxMaxEnemy.x < vtx.x || g_aEnemy[nCntEnemy].vtxMaxEnemy.x == NULL) {
					g_aEnemy[nCntEnemy].vtxMaxEnemy.x = vtx.x;
				}
				//Y���W
				if (g_aEnemy[nCntEnemy].vtxMinEnemy.y > vtx.y || g_aEnemy[nCntEnemy].vtxMinEnemy.y == NULL) {
					g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
				}
				if (g_aEnemy[nCntEnemy].vtxMaxEnemy.y < vtx.y || g_aEnemy[nCntEnemy].vtxMaxEnemy.y == NULL) {
					g_aEnemy[nCntEnemy].vtxMaxEnemy.y = vtx.y;
				}
				//Z���W
				if (g_aEnemy[nCntEnemy].vtxMinEnemy.z > vtx.z || g_aEnemy[nCntEnemy].vtxMinEnemy.z == NULL) {
					g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
				}
				if (g_aEnemy[nCntEnemy].vtxMaxEnemy.z < vtx.z || g_aEnemy[nCntEnemy].vtxMaxEnemy.z == NULL) {
					g_aEnemy[nCntEnemy].vtxMaxEnemy.z = vtx.z;
				}
				pVtxBuff += sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			}
			//���_�o�b�t�@���A�����b�N
			g_aEnemy[nCntEnemy].pMeshEnemy->UnlockVertexBuffer();



			break;
		}
	}
}

//=============================================================================
// �G�̒��_���W
//=============================================================================
void VecEnemy(int nCntEnemy)
{
	//�ϐ��錾
	int nNumVtx;			//���_��
	DWORD sizeFVF;			//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�ւ̃|�C���^

							//���_���̎擾
	nNumVtx = g_aEnemy[nCntEnemy].pMeshEnemy->GetNumVertices();

	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(g_aEnemy[nCntEnemy].pMeshEnemy->GetFVF());

	//���_�o�b�t�@�̃��b�N
	g_aEnemy[nCntEnemy].pMeshEnemy->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		//���_���W�̑��
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//�S�Ă̒��_���r�����f���̍ŏ��l�A�ő�l�𔲂��o��
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.x > vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < vtx.x)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.x = vtx.x;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y > vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < vtx.y)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.y = vtx.y;
		}

		if (g_aEnemy[nCntEnemy].vtxMinEnemy.z > vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}
		else if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < vtx.z)
		{
			g_aEnemy[nCntEnemy].vtxMinEnemy.z = vtx.z;
		}

		pVtxBuff += sizeFVF;
	}
	//���_�o�b�t�@�̃A�����b�N
	g_aEnemy[0].pMeshEnemy->UnlockVertexBuffer();

	//0.0f��菬�����Ƃ�-1.0�{����
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
		{
		}
		if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
			g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.x < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.x *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.y < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.y *= -1.0f;
	}
	if (g_aEnemy[nCntEnemy].vtxMinEnemy.z < 0.0f)
	{
		g_aEnemy[nCntEnemy].vtxMinEnemy.z *= -1.0f;
	}
}

//=============================================================================
// �G�̓����蔻��
//=============================================================================
bool HitEnemy(int nDamage)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 50)
		{
			//�v���C���[�_���[�W�̃}�C�i�X
			g_aEnemy[nCntEnemy].nLife -= nDamage;
			return true;
		}
	}

	return false;
}

//=============================================================================
// ���f���̈ʒu���̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}