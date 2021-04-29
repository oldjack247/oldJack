#ifndef _PLAYER_H
#define _PLAYER_H
#include "main.h"

#define MODEL_PARTS (10)
#define MAX_KEY (10)	
#define MAX_MOTION (10)	

typedef struct
{
	LPD3DXMESH pMeshModel;
	LPD3DXBUFFER pBuffMatModel;
	DWORD nNumMatModel;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	int nIdxModelParent;
}Model;


//���[�V�����̎��
typedef enum {
	MOTIONTYPE_NEUTRAL = 0,	//�j���[�g�������[�V����
	MOTIONTYPE_WALK,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_MAX
} MOTIONTYPE;

//�L�[�̍\����
typedef struct
{
	float fPosX;	//�ʒuX
	float fPosY;;	//�ʒuY
	float fPosZ;	//�ʒuZ
	float fRotX;	//����X
	float fRotY;	//����Y
	float fRotZ;	//����Z
} KEY;

//�L�[���̍\����
typedef struct {
	int nFrame;	//�Đ��t���[��
	KEY aKey[MODEL_PARTS];	//�e�p�[�c�̃L�[�v�f
} KEY_INFO;

//���[�V�������̍\����
typedef struct {
	bool bLoop;	//���[�v���邩�ǂ���
	int nNumKey;	//�L�[�̑���
	KEY_INFO aKeyInfo[MAX_KEY];	//�L�[���
}MOTION_INFO;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 move;
	float rotDest;
	//float fdestRot;
	int nIdx;
	int nShadow;
	int nLife;
	bool bLand;
	Model aModel[MODEL_PARTS];
	int nNumModel = MODEL_PARTS;				//Number of body parts
	MOTION_INFO aMotionInfo[MAX_MOTION];
	int nNumMotion;
	MOTIONTYPE motionType;
	bool bLoopMotion;
	int nNumKey;
	int nKey;
	int nCounterMotion;
	bool bUse;
	bool bMove;
	bool bMoveMotion;
	bool bFirstMotion;				//���[�V�����̈ڂ�ς��̂͂���
	bool bSetLast;					//lastRot,lastPos��ݒ肵�����ǂ���
	D3DXVECTOR3 vtxMinPlayer, vtxMaxPlayer;
}Player;


HRESULT InitPlayer(void);	// 3D���f���̏���������
void UninitPlayer(void);	// 3D���f���̏I������
void UpdatePlayer(void);	// 3D���f���̍X�V����
void DrawPlayer(void);		// 3D���f���̕`�揈��
void SetMotion(MOTIONTYPE type);
D3DXVECTOR3 GetPlayerRot(void);
Player *GetPlayer(void);		// 3D���f���̎擾
bool HitPlayer(int nDamage);

#endif