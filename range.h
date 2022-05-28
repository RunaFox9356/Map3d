//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _RANGE_H_
#define _RANGE_H_

#include"main.h"

//-------------
//�\����
//-------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posSet;	//�ʒu
	D3DXCOLOR col; //�J���[
	bool bUse;
}MoveRange;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitRange(void);
void UninitRange(void);
void UpdateRange(void);
void DrawRange(void);
bool CollisionRange(D3DXVECTOR3 pos);

#endif 


