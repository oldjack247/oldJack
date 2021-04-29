#ifndef _PLAYER_H_
#define _PLAYER_H_

//�}�N����`
#define POLYGON_WIDTH (50.0f)		//�v���C���[�̕�
#define POLYGON_HEIGHT (50.0f)		//�v���C���[�̍���
#define POLYGON_SPEED (5.0f)			//�v���C���[�̑���
#define POLYGON_JUMP_SPEED (1.5f)		//�W�����v���v���C���[�̑���
#define GRAVITY (0.5f)					//�d��
#define POLYGON_JUMP (-14.0f)			//�W�����v�̍���
#define POLYGON_TYPE (1)				//�v���C���[�̎��
#define SCROLE_START_POS (SCREEN_WIDTH / 2.0f)		//�X�N���[�����n�܂�ʒu

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NOMAL = 0,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 BeforePos;	//�O�̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 BeforeMove;	//�O�̈ړ���
	float fWidth;			//�u���b�N�̉���
	float fHeight;			//�u���b�N�̏c��
	bool bUse;				//�g�����ǂ���
	bool bJump;				//�W�����v����
	int nCntAnime;			// �A�j���[�V�����J�E���g
	int nAnimSpan;
	bool bRight;			// ���E�̌�������
	float fAnime;			// �v���C���[�̃A�j���[�V�����J��
	PLAYERSTATE State;		//�v���C���[�̏��
	int nRemaining;			//�c�@
	int nCounterState;		//��ԊǗ��̃J�E���^�[
	bool bMoveR;			//�E����
	bool bMoveL;			//������
	int nMoveTexX;			//���Ɉړ�
	int nMoveTexY;			//���E�U�����
	int nCntTex;			//���ړ��̃J�E���^�[
	bool bScroll;			//�X�N���[��
} PLAYER;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(void);
void PlayerAnim(void);					
PLAYER *GetPlayer(void);

#endif // !_PLAYER_H_