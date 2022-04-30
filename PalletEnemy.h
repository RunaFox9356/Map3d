
//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _PALLETENEMY_H_
#define _PALLETENEMY_H_

#include"main.h"

//-------------
//�\����
//-------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col; //�J���[
	int nType;//�e�N�X�`��
	int tex;
	bool bUse;
	char filename[256];
}PalletE;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitPalletE(void);
void UninitPalletE(void);
void UpdatePalletE(void);
void DrawPalletE(void);
void SetPalletE(D3DXVECTOR3 pos, int nType, int tex);//�������W�@�^�C�v�@�e�N�X�`���̔ԍ�
int CollisionPalletE(D3DXVECTOR3 pos);	//�������W
void PalletMove(bool Set);				//�����H�H
void PasSetTex(char *Filename);			//�����t�@�C���p�X
PalletE *GetPalltE(void);
#endif // !_PalletE_H_

