//============================
//
// �}�b�v�`�b�v�ݒ�
// Author:hamada ryuuga
//
//============================
#include "Pallet.h"
#include "input.h"
#include "enemy.h"
#include <stdio.h>
#include "map.h"

#include <crtdbg.h>
#ifdef _DEBUG
//#define DEBUG_PRINT(...) _RPT_BASE(_CRT_WARN, __FILE__, __LINE__, NULL, __VA_ARGS__)
#define DEBUG_PRINT(...) _RPT_BASE(_CRT_WARN, nullptr, 0, NULL, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

// �}�N����`

#define NUM_PALLET (X_MAP*Y_MAP)

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTexturePallet = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPallet = NULL; //���_�o�b�t�@�̐ݒ�
static Pallet s_aPallet[NUM_PALLET];
static D3DXVECTOR3 s_Move(0.0f, 5.0f, 0.0f);
static D3DXVECTOR3 s_PosOffset;
static float s_fPalletScale;
static int stage = 0;


//================
//����������
//================
void InitPallet(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/mapchip_field2.png",
		&s_pTexturePallet);

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * NUM_PALLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //�������_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuffPallet,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPallet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_PALLET; i++, pVtx += 4)
	{
		s_aPallet[i].bUse = false;

		s_aPallet[i].tex = 0;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}
	int count = 0;
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPallet->Unlock();
	for (int nCntY = 0; nCntY < Y_MAP-4; nCntY++)
	{
		for (int nCntX = 0; nCntX < X_MAP; nCntX++)
		{//Pallet�̏�������
			
			SetPalletMap(D3DXVECTOR3(BLOCKSIZEX*nCntX, ((BLOCKSIZEY*nCntY)), 0.0f), 1, count);
			
			count++;
		}
	}
	s_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_fPalletScale = 1.0f;
}

//�j��
void UninitPallet(void)
{
	stage = 0;
	//�e�N�X�`���̔j��
	if (s_pTexturePallet != NULL)
	{
		s_pTexturePallet->Release();
		s_pTexturePallet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffPallet != NULL)
	{
		s_pVtxBuffPallet->Release();
		s_pVtxBuffPallet = NULL;
	}
}

//==================
//�X�V����
//==================
void UpdatePallet(void)
{

	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffPallet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_PALLET; i++, pVtx += 4)
	{
		if (!s_aPallet[i].bUse)
		{
			continue;
		}

		if (GetKeyboardPress(DIK_LCONTROL))
		{

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		else
		{

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);


		}

		SetNormalpos2d(pVtx,
			s_aPallet[i].pos.x,
			s_aPallet[i].pos.x + BLOCKSIZEX,
			s_aPallet[i].pos.y,
			s_aPallet[i].pos.y + BLOCKSIZEX);
	
	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPallet->Unlock();
}

//==================
//�`�揈��
//==================
void DrawPallet(void)
{
	for (int i = 0; i < NUM_PALLET; i++)
	{
		if (!s_aPallet[i].bUse)
		{
			continue;
		}

		//�f�o�C�X�̃|�C���^
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffPallet, 0, sizeof(VERTEX_2D));
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexturePallet);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			4 * i,
			2);
	}
}

//==================
//�}�b�v�f�[�^�̐ݒ�
//==================
void SetPalletMap(D3DXVECTOR3 pos, int nType, int tex)
{
	VERTEX_2D *pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffPallet->Lock(0, 0, (void**)&pVtx, 0);

	Pallet* pPallet = s_aPallet;
	for (int i = 0; i < NUM_PALLET; i++, pVtx += 4, pPallet++)
	{
		if (pPallet->bUse)
		{
			continue;
		}

		pPallet->bUse = true;

		//�}�b�v���g�p����ĂȂ��ꍇ
		pPallet->pos = pos;
		pPallet->nType = nType;
		pPallet->tex = tex;
		
		int X = (pPallet->tex % X_MAP);
		int Y = (pPallet->tex / X_MAP);
		float XSIZE = (1.0f / X_MAP);
		float YSIZE = (1.0f / Y_MAP);
		//�e�N�X�`���̍��W�ݒ�
		Settex2d(pVtx, XSIZE * X, XSIZE * (X + 1), YSIZE * Y, YSIZE * (Y + 1));

		DEBUG_PRINT("l:%f, r:%f, t:%f, b:%f\n", XSIZE * X, XSIZE * (X + 1), YSIZE * Y, YSIZE * (Y + 1));

		break;
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPallet->Unlock();
}

//==================
//MAP�X�|�C�g
//==================
int CollisionPallet(D3DXVECTOR3 pos)
{
	int Hit = 0;//�`�b�v�ƃ}�E�X�̓����蔻�肪�������Ă鎞
	D3DXVECTOR3 mapPos;

	for (int i = 0; i < NUM_PALLET; i++)
	{
		if (!s_aPallet[i].bUse)
		{
			continue;
		}


		if (((s_aPallet[i].pos.x < pos.x) && (s_aPallet[i].pos.x + BLOCKSIZEX > pos.x)) &&
			((s_aPallet[i].pos.y < pos.y) && (s_aPallet[i].pos.y + BLOCKSIZEX > pos.y)))
		{

			Hit = s_aPallet[i].tex;
			
		}
	}
	return Hit;
}

void PalletMoveMap(bool Set)
{
	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffPallet->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_PALLET; i++, pVtx += 4)
	{
		if (!s_aPallet[i].bUse)
		{
			continue;
		}

		if (GetKeyboardPress(DIK_LCONTROL))
		{

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		}
		else
		{

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);


		}

		SetNormalpos2d(pVtx,
			s_aPallet[i].pos.x,
			s_aPallet[i].pos.x + BLOCKSIZEX,
			s_aPallet[i].pos.y,
			s_aPallet[i].pos.y + BLOCKSIZEX);

	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPallet->Unlock();
}