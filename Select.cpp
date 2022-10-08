//============================
//
// ���g�p���Ă�̂�\���ݒ�
// Author:hamada ryuuga
//
//============================
#include "Select.h"
#include "input.h"
#include "enemy.h"
#include <stdio.h>
#include "map.h"
#include "PalletEnemy.h"
#define MAP_SIZE (50)
//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureSelect = NULL; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffSelect = NULL; //���_�o�b�t�@�̐ݒ�

static LPDIRECT3DTEXTURE9 s_pTextureEnemy[ENEMY_TYPE_MAX] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffEnemy = NULL; //���_�o�b�t�@�̐ݒ�

static Select s_aSelect;
static D3DXVECTOR3 s_Move(0.0f, 5.0f, 0.0f);
static D3DXVECTOR3 s_PosOffset;
static float s_fSelectScale;
static int stage = 0;


//================
//����������
//================
void InitSelect(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/mapchip_field2.png",
		&s_pTextureSelect);

	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //�������_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuffSelect,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);


	s_aSelect.bUse = true;
	s_aSelect.pos = D3DXVECTOR3(1230.0f, 30.0f, 0.0f);
	s_aSelect.tex = 0;

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;


	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	int X = (s_aSelect.tex % X_MAP);
	int Y = (s_aSelect.tex / X_MAP);

	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, (1.0f / X_MAP)*(X), (1.0f / X_MAP) * (1.0f + X), (1.0f / Y_MAP)*(Y), (1.0f / Y_MAP) *(Y + 1.0f));

	int count = 0;


	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffSelect->Unlock();

	PalletE* nType = GetPalltE();

	for (int i = 0; i < (int)ENEMY_TYPE_MAX; i++, nType++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			nType->filename,
			&s_pTextureEnemy[i]);
	}
	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //�������_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuffEnemy,
		NULL);

	//���_�o�b�t�@�����b�N
	s_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	s_aSelect.nType = 0;

	SetNormalpos2d(pVtx,
		s_aSelect.pos.x - MAP_SIZE,
		s_aSelect.pos.x,
		s_aSelect.pos.y,
		s_aSelect.pos.y + MAP_SIZE);


	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);


	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffEnemy->Unlock();

	s_fSelectScale = 1.0f;
}

//================
//����������
//================
void UninitSelect(void)
{
	stage = 0;
	//�e�N�X�`���̔j��
	if (s_pTextureSelect != NULL)
	{
		s_pTextureSelect->Release();
		s_pTextureSelect = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffSelect != NULL)
	{
		s_pVtxBuffSelect->Release();
		s_pVtxBuffSelect = NULL;
	}
	for (int i = 0; i < (int)ENEMY_TYPE_MAX; i++)
	{
		if (s_pTextureEnemy[i] != NULL)
		{
			s_pTextureEnemy[i]->Release();
			s_pTextureEnemy[i] = NULL;
		}
	}
	if (s_pVtxBuffEnemy != NULL)
	{
		s_pVtxBuffEnemy->Release();
		s_pVtxBuffEnemy = NULL;
	}
}

//==================
//�X�V����
//==================
void UpdateSelect(int Set, int type)
{

	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos;

	s_aSelect.tex = Set;

	SetNormalpos2d(pVtx,
		s_aSelect.pos.x,
		s_aSelect.pos.x + MAP_SIZE,
		s_aSelect.pos.y,
		s_aSelect.pos.y + MAP_SIZE);

	int X = (s_aSelect.tex % X_MAP);
	int Y = (s_aSelect.tex / X_MAP);

	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, (1.0f / X_MAP)*(X), (1.0f / X_MAP) * (1.0f + X), (1.0f / Y_MAP)*(Y), (1.0f / Y_MAP) *(Y + 1.0f));



	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffEnemy->Unlock();


	//���_�o�b�t�@�����b�N
	s_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);



	s_aSelect.nType = type;

	SetNormalpos2d(pVtx,
		s_aSelect.pos.x - MAP_SIZE,
		s_aSelect.pos.x,
		s_aSelect.pos.y,
		s_aSelect.pos.y + MAP_SIZE);


	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffEnemy->Unlock();
}

//==================
//�`�揈��
//==================
void DrawSelect(void)
{

	//�f�o�C�X�̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffSelect, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureSelect);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetTexture(0, s_pTextureEnemy[s_aSelect.nType]);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}



