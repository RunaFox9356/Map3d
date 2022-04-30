#pragma once
//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _SELECT_H_
#define _SELECT_H_

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
}Select;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitSelect(void);
void UninitSelect(void);
void UpdateSelect(int Set, int type);
void DrawSelect(void);

#endif // !_Select_H_

#pragma once
