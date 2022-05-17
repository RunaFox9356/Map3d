//=========================================
// 
// �Q�[��������
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "debug.h"
#include "process.h"
#include <stdio.h>

//-----------------------------------------
// �}�N���錾
//-----------------------------------------
#define DEBUG_NUM	(7)

//-----------------------------------------
// �ÓI�ϐ��錾
//-----------------------------------------
static LPD3DXFONT s_pFont = NULL;	// �t�H���g�ւ̃|�C���^

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------

//=========================================
// FPS�\���̏�����
//=========================================
void InitDebug(void)
{
	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(GetDevice(), 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "HG�n�p�p�޼��UB", &s_pFont);
}

//=========================================
// FPS�\���̏I������
//=========================================
void UninitDebug(void)
{
	// �f�o�b�O�\���p�t�H���g�̔j��
	if (s_pFont != NULL)
	{
		s_pFont->Release();
		s_pFont = NULL;
	}
}

//=========================================
// FPS�̕\��
//=========================================
void DrawDebug(void)
{
	char aStr[DEBUG_NUM][512];	// �\������

	//������ɑ��
	wsprintf(&aStr[0][0], "FPS%d", GetFPS());

	wsprintf(&aStr[1][0], "�X�e�[�W�ԍ�%d", GetDebugNumber());

	wsprintf(&aStr[2][0], "�G�l�~�[�^�C�v%d", GetDebugNumberEnemy());

	if (IsDebug())
	{
		wsprintf(&aStr[3][0], "debug[ON]");

		if (GetSetMode())
		{
			wsprintf(&aStr[4][0], "EnemyMode");
			if (GetEnemyAlignment())
			{
				wsprintf(&aStr[5][0], "�G���u���b�N���Ƃɔz�u���܂�");
			}
			else
			{
				wsprintf(&aStr[5][0], "�G�����R�ɂɔz�u���܂�");
			}
			wsprintf(&aStr[6][0], "�������\n�f�o�b�N�؂�ւ�Y�L�[\n���G����\n�E�ύXCTRL�����Ȃ���E�N���b�N�łς����\nCTRL+F�Őݒu����Ă�^�C�v�ύX\nV�L�[�ړ��ēx�����Ċm��\n�z�C�[���Ń}�b�v�ړ�\n�}�E�X�J�[�\�����킹���Ƃ��͑I��\nD�L�[�폜\nA�L�[MAP�z�u��Enemy�z�u�̐؂�ւ�\nL�S���폜\nS���R�z�u��BLOCK���Ƃɔz�u�̐؂�ւ�\n�V�t�g+�}�E�X�z�C�[���T�C�Y�ύX\n8�L�[�}�b�v�ۑ�\n9�L�[�G�l�~�[�ۑ�\nM&N�L�[�Ń}�b�v�؂�ւ�\n");
		}
		else
		{
			wsprintf(&aStr[4][0], "MapMode");
			wsprintf(&aStr[5][0], "�������\n�f�o�b�N�؂�ւ�Y�L�[\n���N���b�N�^�C������\n�E�N���b�N�^�C���ύX\nCTRL�����Ȃ���E�N���b�N�Ńp���b�g����X�|�C�g�ł���\n8�L�[�}�b�v�ۑ�\n9�L�[�G�l�~�[�ۑ�\nA�L�[MAP�z�u��Enemy�z�u�̐؂�ւ�");
			wsprintf(&aStr[6][0], "");
		}


	}
	else
	{
		wsprintf(&aStr[3][0], "debug[OFF]");
		wsprintf(&aStr[4][0], "�������\n�f�o�b�N�؂�ւ�Y�L�[\n");
		wsprintf(&aStr[5][0], "");
		wsprintf(&aStr[6][0], "");
	}

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	for (int i = 0; i < DEBUG_NUM; i++)
	{
		rect = { 0,i * 30,SCREEN_WIDTH,SCREEN_HEIGHT };
		//�e�L�X�g�̕`��
		s_pFont->DrawText(NULL, &aStr[i][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}