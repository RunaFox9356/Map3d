
//============================
//
// �}�b�v�`�b�v�ݒ�
// Author:hamada ryuuga
//
//============================
#include "PalletEnemy.h"
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

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTexturePalletE[ENEMY_TYPE_MAX] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffPalletE = NULL; //���_�o�b�t�@�̐ݒ�
static PalletE s_aPalletE[ENEMY_TYPE_MAX];
static D3DXVECTOR3 s_Move(0.0f, 5.0f, 0.0f);
static D3DXVECTOR3 s_PosOffset;
static float s_fPalletEScale;
static int stage = 0;


//================
//����������
//================
void InitPalletE(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();



	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * ENEMY_TYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, //�������_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&s_pVtxBuffPalletE,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPalletE->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < ENEMY_TYPE_MAX; i++, pVtx += 4)
	{
		s_aPalletE[i].bUse = false;

		s_aPalletE[i].tex = 0;

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
	s_pVtxBuffPalletE->Unlock();

	for (int nCntX = 0; nCntX < ENEMY_TYPE_MAX; nCntX++)
	{//PalletE�̏�������

		SetPalletE(D3DXVECTOR3(BLOCKSIZEX*nCntX, (150), 0.0f), 1, count);

		count++;
	}

	s_PosOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	s_fPalletEScale = 1.0f;
	//------------------------
	//�e�N�X�`���ݒ�
	//------------------------
	PasSetTex("Data/txt/enemy.txt");
}

//================
//�j������
//================
void UninitPalletE(void)
{
	stage = 0;
	//�e�N�X�`���̔j��
	for (int i = 0; i < (int)ENEMY_TYPE_MAX; i++)
	{
		if (s_pTexturePalletE[i] != NULL)
		{
			s_pTexturePalletE[i]->Release();
			s_pTexturePalletE[i] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffPalletE != NULL)
	{
		s_pVtxBuffPalletE->Release();
		s_pVtxBuffPalletE = NULL;
	}
}

//==================
//�X�V����
//==================
void UpdatePalletE(void)
{

	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffPalletE->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < ENEMY_TYPE_MAX; i++, pVtx += 4)
	{
		if (!s_aPalletE[i].bUse)
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
			s_aPalletE[i].pos.x,
			s_aPalletE[i].pos.x + BLOCKSIZEX,
			s_aPalletE[i].pos.y,
			s_aPalletE[i].pos.y + BLOCKSIZEX);

	}

	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPalletE->Unlock();
}

//==================
//�`�揈��
//==================
void DrawPalletE(void)
{
	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		if (!s_aPalletE[i].bUse)
		{
			continue;
		}

		//�f�o�C�X�̃|�C���^
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuffPalletE, 0, sizeof(VERTEX_2D));
		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, s_pTexturePalletE[s_aPalletE[i].tex]);

		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			4 * i,
			2);
	}
}

//==================
//�}�b�v�f�[�^�̐ݒ�
//==================
void SetPalletE(D3DXVECTOR3 pos, int nType, int tex)
{
	VERTEX_2D *pVtx; //���_�ւ̃|�C���^
	s_pVtxBuffPalletE->Lock(0, 0, (void**)&pVtx, 0);

	PalletE* pPalletE = s_aPalletE;
	for (int i = 0; i < ENEMY_TYPE_MAX; i++, pVtx += 4, pPalletE++)
	{
		if (pPalletE->bUse)
		{
			continue;
		}

		pPalletE->bUse = true;

		//�}�b�v���g�p����ĂȂ��ꍇ
		pPalletE->pos = pos;
		pPalletE->nType = nType;
		pPalletE->tex = tex;

		//�e�N�X�`���̍��W�ݒ�
		Settex2d(pVtx, 0.0f, 1.0f, 0.0f, 1.0f);

		break;
	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPalletE->Unlock();
}

//==================
//MAP�X�|�C�g
//==================
int CollisionPalletE(D3DXVECTOR3 pos)
{
	int Hit = 0;//�`�b�v�ƃ}�E�X�̓����蔻�肪�������Ă鎞
	D3DXVECTOR3 mapPos;

	for (int i = 0; i < ENEMY_TYPE_MAX; i++)
	{
		if (!s_aPalletE[i].bUse)
		{
			continue;
		}


		if (((s_aPalletE[i].pos.x < pos.x) && (s_aPalletE[i].pos.x + BLOCKSIZEX > pos.x)) &&
			((s_aPalletE[i].pos.y < pos.y) && (s_aPalletE[i].pos.y + BLOCKSIZEX > pos.y)))
		{

			Hit = s_aPalletE[i].tex;

		}
	}
	return Hit;
}

//================
//�Ȃ񂾂������ꏈ��
//================
void PalletMove(bool Set)
{
	//���_�ւ̃|�C���^
	VERTEX_2D*pVtx;
	//���_�o�b�t�@�����b�N
	s_pVtxBuffPalletE->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < ENEMY_TYPE_MAX; i++, pVtx += 4)
	{
		if (!s_aPalletE[i].bUse)
		{
			continue;
		}

		if (Set)
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
			s_aPalletE[i].pos.x,
			s_aPalletE[i].pos.x + BLOCKSIZEX,
			s_aPalletE[i].pos.y,
			s_aPalletE[i].pos.y + BLOCKSIZEX);

	}
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffPalletE->Unlock();
}

//==================
//file�p�X�ǂݍ���
//==================
void PasSetTex(char *Filename)
{
	//�t�@�C�����J��
	Enemy *Enemy = GetEnemy();
	FILE *pFile = fopen(&Filename[0], "r");
	char	s_aString[256];//�t�@�C���̕��������
	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		fscanf(pFile, "%s", &s_aString);

		while (strncmp(&s_aString[0], "SCRIPT", 6) != 0)
		{//�X�^�[�g����܂ŋ󔒓ǂݍ���
			s_aString[0] = {};
			fscanf(pFile, "%s", &s_aString[0]);
		}
		int nNumber = 0;
		int nEnemt = 0;
		while (strncmp(&s_aString[0], "END_SCRIPT", 10) != 0)
		{// ������̏������Ɠǂݍ���// ������̏������Ɠǂݍ���
			fscanf(pFile, "%s", &s_aString[0]);
			if (strcmp(&s_aString[0], "TEXTURE_FILENAME") == 0)
			{
				LPDIRECT3DDEVICE9  pDevice;
				//�f�o�C�X�̎擾
				pDevice = GetDevice();
				fscanf(pFile, "%s", &s_aString[0]);//���ǂݍ��ނ��
				fscanf(pFile, "%s", &s_aPalletE[nEnemt].filename[0]);
				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					&s_aPalletE[nEnemt].filename[0],
					&s_pTexturePalletE[nEnemt]);
				nEnemt++;
			}
		}
		//�t�@�C�������
		fclose(pFile);
	}

}

//==================
//�Q�b�g
//==================
PalletE *GetPalltE(void)
{
	return &s_aPalletE[0];
}