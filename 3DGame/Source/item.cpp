#include "item.h"
#include "game.h"
#include "time.h"
#include "stdlib.h"

//==============================================================================
// �O���[�o���ϐ�
//==============================================================================
LPD3DXMESH g_pMeshItem = NULL;			// ���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatItem = NULL;		// �}�e���A��(�ގ����)�ւ̃|�C���^
DWORD g_nNumMatItem = 0;				// �}�e���A���̐�
ITEM g_aItem[MAX_ITEM];					// �A�C�e���̏��
//int g_nAnimCnt;						// �A�j���[�V�����J�E���g
//int g_nAnimCol;						// �F�ω��J�E���g

//==============================================================================
// �A�C�e���̏���������
//==============================================================================
HRESULT InitItem(void)
{
	// ���[�J���ϐ��錾
	VERTEX_2D *pVtx;
	LPDIRECT3DDEVICE9 pDevice;
	ITEM *pItem;
	int nRand;

	// �����������猻���������O
	srand((unsigned int)time(0));

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e���̎擾
	pItem = &g_aItem[0];

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(
		"data/MODEL/coin.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatItem,
		NULL,
		&g_nNumMatItem,
		&g_pMeshItem);

	nRand = RandItem();		// �n�C�X�R�A�A�C�e���̑I��

	// �\���̂̏�����
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItem->col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pItem->nType = NULL;
		pItem->bUse = false;
		pItem->vtxMaxObject = D3DXVECTOR3(-10000.0f,-10000.0f,-10000.0f);
		pItem->vtxMinObject = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
		//pItem->fAnim = 0.0f;
		//if (nCntItem == nRand)
		//{
		//	pItem->bHiScore = true;
		//}
		//else
		//{
		//	pItem->bHiScore = false;
		//}
	}

	//// �ϐ��̏�����
	//g_nAnimCnt = 0;
	//g_nAnimCol = 0;

	// ���[�J���ϐ��錾
	int nNumVtx;	// ���_��
	DWORD sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����擾
	nNumVtx = g_pMeshItem->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y�̎擾
	sizeFVF = D3DXGetFVFVertexSize(g_pMeshItem->GetFVF());

	// ���_�o�b�t�@�����b�N
	g_pMeshItem->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_���W�̔�r
	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// ���_���W�̑��

														// �e���W�̍ő�l�̔�r
			if (g_aItem[nCntItem].vtxMaxObject.x < vtx.x)
			{
				g_aItem[nCntItem].vtxMaxObject.x = vtx.x;
			}
			if (g_aItem[nCntItem].vtxMaxObject.y < vtx.y)
			{
				g_aItem[nCntItem].vtxMaxObject.y = vtx.y;
			}
			if (g_aItem[nCntItem].vtxMaxObject.z < vtx.z)
			{
				g_aItem[nCntItem].vtxMaxObject.z = vtx.z;
			}

			// �e���W�̍ŏ��l�̔�r
			if (g_aItem[nCntItem].vtxMinObject.x > vtx.x)
			{
				g_aItem[nCntItem].vtxMinObject.x = vtx.x;
			}
			if (g_aItem[nCntItem].vtxMinObject.y > vtx.y)
			{
				g_aItem[nCntItem].vtxMinObject.y = vtx.y;
			}
			if (g_aItem[nCntItem].vtxMinObject.z > vtx.z)
			{
				g_aItem[nCntItem].vtxMinObject.z = vtx.z;
			}

			pVtxBuff += sizeFVF;	// ���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
		}
	}


	// ���_�o�b�t�@���A�����b�N
	g_pMeshItem->UnlockVertexBuffer();

	return S_OK;
}

//==============================================================================
// �A�C�e���̏I������
//==============================================================================
void UninitItem(void)
{
	// ���b�V���̔j��
	if (g_pMeshItem != NULL)
	{
		g_pMeshItem->Release();
		g_pMeshItem = NULL;
	}

	// �}�e���A���̔j��
	if (g_pBuffMatItem != NULL)
	{
		g_pBuffMatItem->Release();
		g_pBuffMatItem = NULL;
	}
}

//==============================================================================
// �A�C�e���̍X�V����
//==============================================================================
void UpdateItem(void)
{

}

//==============================================================================
// �A�C�e���̕`�揈��
//==============================================================================
void DrawItem(void)
{
	// ���[�J���ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							// �}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aItem[nCntItem].mtxWorld);

		// �����̔��f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aItem[nCntItem].rot.y, g_aItem[nCntItem].rot.x, g_aItem[nCntItem].rot.z);
		D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, g_aItem[nCntItem].pos.z);
		D3DXMatrixMultiply(&g_aItem[nCntItem].mtxWorld, &g_aItem[nCntItem].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aItem[nCntItem].mtxWorld);

		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatItem->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMatItem; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// ���f��(�p�[�c)�̕`��
			g_pMeshItem->DrawSubset(nCntMat);
		}

		// �ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}

	//// ���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&object.mtxWorld);

	//// �����̔��f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, object.rot.y, object.rot.x, object.rot.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxRot);

	//// �ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, object.pos.x, object.pos.y, object.pos.z);
	//D3DXMatrixMultiply(&object.mtxWorld, &object.mtxWorld, &mtxTrans);

	//// ���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &object.mtxWorld);

	//// ���݂̃}�e���A�����擾
	//pDevice->GetMaterial(&matDef);

	//// �}�e���A���f�[�^�ւ̃|�C���^���擾
	//pMat = (D3DXMATERIAL*)g_pBuffMatObject->GetBufferPointer();

	//for (int nCntMat = 0; nCntMat < (int)g_nNumMatObject; nCntMat++)
	//{
	//	// �}�e���A���̐ݒ�
	//	pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

	//	// �e�N�X�`���̐ݒ�
	//	pDevice->SetTexture(0, NULL);

	//	// ���f��(�p�[�c)�̕`��
	//	g_pMeshObject->DrawSubset(nCntMat);
	//}

	//// �ۑ����Ă����}�e���A����߂�
	//pDevice->SetMaterial(&matDef);
}

//==============================================================================
// �A�C�e���̐ݒ�
//==============================================================================
void SetItem(D3DXVECTOR3 pos, int nType)
{

}

//==============================================================================
// �G�̎擾
//==============================================================================
ITEM *GetItem(void)
{
	return &g_aItem[0];
}

//==============================================================================
// �����_���Ńn�C�X�R�A�A�C�e���̑I��
//==============================================================================
int RandItem(void)
{
	//// ���[�J���ϐ��錾
	//ITEM *pItem;
	//int nStage = GetStage();
	int nHigh = 0;

	//// �A�C�e���̎擾
	//pItem = &g_aItem[0];

	//if (nStage == 0)
	//{
	//	nHigh = rand() % COIN_STAGE1;
	//}
	//else if (nStage == 1)
	//{
	//	nHigh = rand() % COIN_STAGE2;
	//}
	//else if(nStage == 2)
	//{
	//	nHigh = rand() % COIN_STAGE3;
	//}

	return nHigh;
}