//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _PALLET_H_
#define _PALLET_H_

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
}Pallet;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitPallet(void);
void UninitPallet(void);
void UpdatePallet(void);
void DrawPallet(void);
void SetPalletMap(D3DXVECTOR3 pos, int nType, int tex);
int CollisionPallet(D3DXVECTOR3 pos);
void PalletMoveMap(bool Set);

#endif // !_Pallet_H_

