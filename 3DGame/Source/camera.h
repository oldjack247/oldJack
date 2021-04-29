#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

//==============================================================================
// �J�����̍\����
//==============================================================================
typedef struct
{
	D3DXVECTOR3 posV;			// ���݂̎��_
	D3DXVECTOR3 posVDest;		// �ړI�̎��_
	D3DXVECTOR3 posR;			// ���݂̒����_
	D3DXVECTOR3 posRDest;		// �ړI�̒����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3 move;			// �ړ���

	//D3DXVECTOR3 moveV;			// ���_�̈ړ���
	//D3DXVECTOR3 moveR;			// �����_�̈ړ���
	//D3DXVECTOR3 moveXVR;		// ���_�E�����_����������X���ړ���
	//D3DXVECTOR3 moveZVR;		// ���_�E�����_����������Z���ړ���
	D3DXVECTOR3 rot;			// �J�����̊p�x
	float fLength;				// ���_�E�����_�Ԃ̋���
	float fHeightV;				// ���_�̍���
	float fHeightR;				// �����_�̍���
	float fFront;				// ���f���̑O���̋���
	bool bAutoTurn;				// �J�����̉�荞��ON�EOFF
} Camera;

//==============================================================================
// �v���g�^�C�v�錾
//==============================================================================
void InitCamera(void);		// �J�����̏���������
void UninitCamera(void);	// �J�����̏I������
void UpdateCamera(void);	// �J�����̍X�V����
void SetCamera(void);		// �J�����̐ݒ�
Camera GetCamera(void);		// �J�����̎擾

#endif