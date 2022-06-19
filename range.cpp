//============================
//
//	�͈͑I��ݒ�
// Author:hamada ryuuga
//
//============================

#include "range.h"
#include "input.h"
#include "comn.h"
#include "camera.h"

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffRange = NULL; //���_�o�b�t�@�̐ݒ�
static MoveRange s_aRange;
D3DXVECTOR3 MousePoint;

//================
//����������
//================
void InitRange(void)
{
	LPDIRECT3DDEVICE9  pDevice;
	D3DXVECTOR3 MousePoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //�������_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuffRange,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuffRange->Lock(0, 0, (void**)&pVtx, 0);

	s_aRange.bUse = false;

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;


	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);

	SetNormalpos2d(pVtx,
		s_aRange.pos.x,
		s_aRange.pos.x,
		s_aRange.pos.y,
		s_aRange.pos.y);

	int count = 0;

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffRange->Unlock();
}

//�j��
void UninitRange(void)
{
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffRange != NULL)
	{
		s_pVtxBuffRange->Release();
		s_pVtxBuffRange = NULL;
	}
}

//==================
//�X�V����
//==================
void UpdateRange(void)
{

	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffRange->Lock(0, 0, (void**)&pVtx, 0);

	s_aRange.pos = GetMouse();

	SetNormalpos2d(pVtx,
		s_aRange.pos.x,
		s_aRange.pos.x,
		s_aRange.pos.y,
		s_aRange.pos.y);

	if (GetMouseTrigger(MOUSE_INPUT_LEFT))
	{
		s_aRange.posSet = s_aRange.pos;
	}
	if (GetKeyboardPress(DIK_LSHIFT)&& GetMousePress(MOUSE_INPUT_LEFT))
	{
		
		SetNormalpos2d(pVtx,
			s_aRange.posSet.x,
			s_aRange.pos.x,
			s_aRange.posSet.y,
			s_aRange.pos.y);
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffRange->Unlock();
}

//==================
//�`�揈��
//==================
void DrawRange(void)
{
	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffRange, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

}

//==================
//�͈͑I��
//==================
bool CollisionRange(D3DXVECTOR3 pos)
{
	bool Hit = false;//�`�b�v�ƃ}�E�X�̓����蔻�肪�������Ă鎞
	D3DXVECTOR3 mapPosStart, mapPosEnd;

	if (GetKeyboardPress(DIK_LSHIFT))
	{
		
		Camera *pCamera = GetCamera();
		mapPosStart = WorldCastScreen(&s_aRange.posSet,								// �X�N���[�����W
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// �X�N���[���T�C�Y
			&pCamera->mtxView,										// �r���[�}�g���b�N�X
			&pCamera->mtxProjection);								// �v���W�F�N�V�����}�g���b�N�X

		mapPosEnd = WorldCastScreen(&s_aRange.pos,								// �X�N���[�����W
			D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// �X�N���[���T�C�Y
			&pCamera->mtxView,										// �r���[�}�g���b�N�X
			&pCamera->mtxProjection);								// �v���W�F�N�V�����}�g���b�N�X

		if (((mapPosStart.x > pos.x) && (mapPosEnd.x < pos.x)) &&
			((mapPosStart.y > pos.y) && (mapPosEnd.y < pos.y)))
		{
			Hit = true;
		}
	}
	return Hit;
}