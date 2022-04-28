//============================
//
// �v�Z�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _COMN_H_
#define _COMN_H_

#include "main.h"

#define Vtx (4)		//���_��
#define RET (D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f))
#define WHITE (D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f))

D3DXVECTOR3	WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXVECTOR3 FormerRot);
float SeteaseIn(float X);//�ꏊ.��]
float easeInOutBack(float x);
float easeOutCirc(float x);
float easeOutElastic(float x);
float Vec2Cross( D3DXVECTOR3* v1, D3DXVECTOR3* v2);
float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2);
D3DXVECTOR3 WorldCastScreen(D3DXVECTOR3 *screenPos,			// �X�N���[�����W
	D3DXVECTOR3 screenSize,									// �X�N���[���T�C�Y
	D3DXMATRIX* mtxView,									// �r���[�}�g���b�N�X
	D3DXMATRIX* mtxProjection);								// �v���W�F�N�V�����}�g���b�N�X

#endif