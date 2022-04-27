//============================
//
// �͂������ݒ�
// Author:hamada ryuuga
//
//============================
#include"bg.h"
#include "input.h"


#define NUM_BG (1)//�w�i����

//�X�^�e�B�b�N�ϐ�
static LPDIRECT3DTEXTURE9 s_pTextureBG[NUM_BG] = {}; //�e�N�X�`���̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuffBG = NULL; //���_�o�b�t�@�̐ݒ�
static D3DXVECTOR3 s_moveBG;	//�ړ���
static D3DXVECTOR3 s_posBG[NUM_BG];	//���W
static Bg s_nBgnCount;
static float txeX,txeY;
//================
//����������
//================
void InitBG(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"Data/TEXTURE/map2.png",
		&s_pTextureBG[0]);
	//���_�o�b�t�@
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,//�������_�t�H�[�}�b�g
		&s_pVtxBuffBG,
		NULL);

	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	
	s_posBG[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetNormalpos2d(pVtx,
		0.0f,
		SCREEN_WIDTH,
		0.0f,
		SCREEN_HEIGHT);
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	txeX = 0.0f;
	txeY = 0.0f;
	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, 0.0f, 0.2f, 0.0f, 1.0f);
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffBG->Unlock();

}

//�j��
void UninitBG(void)
{

	for (int nCnt = 0; nCnt < NUM_BG; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (s_pTextureBG[nCnt] != NULL)
		{
			s_pTextureBG[nCnt]->Release();
			s_pTextureBG[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (s_pVtxBuffBG != NULL)
	{
		s_pVtxBuffBG->Release();
		s_pVtxBuffBG = NULL;
	}

}
//==================
//�X�V����
//==================
void UpdateBG(void)
{
	VERTEX_2D*pVtx; //���_�ւ̃|�C���^
					//���_�o�b�t�@���A�����b�N
	s_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);
	//�e�N�X�`���̍��W�ݒ�
	//s_posBG[0] -= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	SetNormalpos2d(pVtx,
		0.0f,
		SCREEN_WIDTH,
		0.0f + s_posBG[0].y,
		SCREEN_HEIGHT * 10+ s_posBG[0].y);
	txeY -= 0.0005f;
	if (txeY <= -1.0f)
	{
		txeY = 0.0f;
		txeX += 0.2f;
	}
	if (txeX >= 1.0f)
	{	
		txeX = 0.0f;
	}
	//�e�N�X�`���̍��W�ݒ�
	Settex2d(pVtx, 0.0f + txeX, 0.2f + txeX, 0.0f + txeY, 0.2f+txeY);
	//���_�o�b�t�@���A�����b�N
	s_pVtxBuffBG->Unlock();
}
//==================
//�`�揈��
//==================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;//�f�o�C�X�̃|�C���^
//	int nCntEnemy;

	pDevice = GetDevice();		//���
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, s_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, s_pTextureBG[0]);

	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);
}

Bg *GetBg(void)
{
	return &s_nBgnCount;
}