//============================
//
// �͂������ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _BG_H_
#define _BG_H_

#include"main.h"

//�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR col; //�J���[
	int nType;//�e�N�X�`��
}Bg;


//�v���g�^�C�v�錾
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);
Bg *GetBg(void);

#endif // !_BG_H_
