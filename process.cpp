//=========================================
// 
// �����S��
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "process.h"
#include "bg.h"
#include "enemy.h"
#include "map.h"
#include "Pallet.h"
#include "Select.h"
#include "PalletEnemy.h"
#include "camera.h" 
#include "player.h"
#include "light.h"
#include "comn.h"
#include "input.h"
#include "range.h"
//=========================================
// �ÓI�ϐ�
//=========================================
static int s_DebugNumberEnemy;
static int DebugNumber;
static bool EnemyAlignment = false;				// �}�b�v�̃}�X�ڊ�ɂ��邩���Ȃ���
static float Size = 1.0f;
static bool EnemyMode = false;					// Map�����邩Enemy�����邩
static bool s_bDebug;							// �f�o�b�O�����ۂ�
static CPlayer player;
static CEnemy enemy;
//=========================================
// �v���g�^�C�v�錾
//=========================================
static void EnemySetSystem(D3DXVECTOR3 Mouse);	// Enemy�̋���
static void MapSetSystem(D3DXVECTOR3 Mouse);	// �}�b�v�̋���

//-----------------------------------------
// ������
//-----------------------------------------
void InitProcess()
{
	InitLight();

	InitBG();
	enemy.Init();
	InitMap();
	InitPallet();
	InitPalletE();
	InitSelect();
	InitRange();
	player.Init();
}

//-----------------------------------------
// �I��
//-----------------------------------------
void UninitProcess()
{
	
	UninitLight();
	UninitBG();
	enemy.Uninit();
	UninitMap();
	UninitPallet();
	player.Uninit();
	UninitPalletE();
	UninitSelect();
	UninitRange();
}

//-----------------------------------------
// �X�V
//-----------------------------------------
void UpdateProcess()
{

	UpdateBG();
	enemy.Update();
	player.Update();
	UpdateMap();
	UpdateRange();
	UpdateSelect(DebugNumber, s_DebugNumberEnemy);

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_U))
	{//�G�̍��W�����[�h����i���f���r���A�[���j
		enemy.LoadSetFile("Data\\hoge2.txt");
	}

	if (s_bDebug)
	{
		//------------------------
		//�}�b�v�`�b�vdebug�I�t
		//------------------------
		if (GetKeyboardTrigger(DIK_Y))
		{
			s_bDebug = false;
		}

		D3DXVECTOR3 Mouse = GetMouse();

		if (EnemyMode)
		{//Enemy�ݒ�
			UpdatePalletE();
			EnemySetSystem(Mouse);

		}
		else
		{//MAP�ݒ�
			UpdatePallet();
			MapSetSystem(Mouse);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_Y))
		{//�}�b�v�`�b�vdebug�I�t
			s_bDebug = true;
			ConteSet(0);
		}
	}
	//------------------------
	//���[�h�ύX
	//------------------------
	if (GetKeyboardTrigger(DIK_A))
	{
		EnemyMode = !EnemyMode;
	}
#endif // DEBUG
}

//-----------------------------------------
// �`��
//-----------------------------------------
void DrawProcess()
{

	DrawBG();
	DrawMap();
	DrawPallet();
	DrawSelect();
	DrawPallet();
	DrawPalletE();
	DrawRange();
	// 2D�̑O��3D��u��
	GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	enemy.Draw();
	player.Draw();
}

//------------------------
//�G�f�o�b�N���̐ݒ�
//------------------------
void EnemySetSystem(D3DXVECTOR3 Mouse)
{

	//------------------------
	//�}�b�v�̃}�X�ڊ�ɂ��邩���Ȃ���
	//------------------------
	if (GetKeyboardTrigger(DIK_S))
	{//
		EnemyAlignment = !EnemyAlignment;
	}

	//------------------------
	//�X�|�C�g
	//------------------------
	if (GetMouseTrigger(MOUSE_INPUT_RIGHT))
	{//�}�E�X�|�C���^�[�̈ʒu
		s_DebugNumberEnemy++;
		s_DebugNumberEnemy %= (int)2;
		if (GetKeyboardPress(DIK_LCONTROL))
		{
			s_DebugNumberEnemy = CollisionPalletE(Mouse);
		}
	}

	//------------------------
	//�G�l�~�̃Z�b�g
	//------------------------
	if (GetMouseTrigger(MOUSE_INPUT_LEFT)&& !GetKeyboardPress(DIK_LSHIFT))
	{//
		bool Hit = CollisionMap(Mouse);
		if (Hit)
		{
			if (EnemyAlignment)
			{
				D3DXVECTOR3 BLOCK = EnemyMap(Mouse*Size);
				CAMERA *pCamera = GetCamera()->Get();
				BLOCK = WorldCastScreen(&BLOCK,								// �X�N���[�����W
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// �X�N���[���T�C�Y
					&pCamera->MtxView,										// �r���[�}�g���b�N�X
					&pCamera->MtxProje);

				enemy.SetEnemy(D3DXVECTOR3(BLOCK.x, BLOCK.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (ENEMY_TYPE)s_DebugNumberEnemy);
			}
			else
			{
				CAMERA *pCamera = GetCamera()->Get();
				Mouse = WorldCastScreen(&Mouse,								// �X�N���[�����W
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// �X�N���[���T�C�Y
					&pCamera->MtxView,										// �r���[�}�g���b�N�X
					&pCamera->MtxProje);								// �v���W�F�N�V�����}�g���b�N�X

				enemy.SetEnemy(D3DXVECTOR3(Mouse.x, Mouse.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (ENEMY_TYPE)s_DebugNumberEnemy);
			}
		}
	}
	enemy.SelectDes(Mouse);
}

//------------------------
//�}�b�v�f�o�b�N���̐ݒ�
//------------------------
void MapSetSystem(D3DXVECTOR3 Mouse)
{
	if (GetMousePress(MOUSE_INPUT_LEFT))
	{//�}�E�X�|�C���^�[�̈ʒu
		ConversionMap(Mouse, DebugNumber);
	}

	if (GetMouseTrigger(MOUSE_INPUT_RIGHT))
	{//�}�E�X�|�C���^�[�̈ʒu
		DebugNumber++;
		DebugNumber %= X_MAP * (Y_MAP - 4) + 1;
		if (GetKeyboardPress(DIK_LCONTROL))
		{
			DebugNumber = CollisionPallet(Mouse);
		}

	}
	if (GetKeyboardPress(DIK_LCONTROL))
	{
		PalletMoveMap(true);
	}
	else
	{
		PalletMoveMap(false);
	}
}

//---------------------------------------
// ���݂̐ݒu�\���[�h�̎擾
//---------------------------------------
bool GetSetMode()
{
	return EnemyMode;
}

//---------------------------------------
// ���݂̐ݒu�\���[�h�̐ݒ�
//---------------------------------------
void ChangeSetMode(bool hasChange)
{
	EnemyMode = hasChange;
}

//---------------------------------------
// ��ύX
//---------------------------------------
int GetDebugNumber()
{
	return DebugNumber;
}

//---------------------------------------
// ��ύX
//---------------------------------------
void setDebugNumber(int inDebugNumber)
{
	DebugNumber = inDebugNumber;
}

//---------------------------------------
// ��ύX
//---------------------------------------
void setEnemyNumber(int inDebugNumber)
{
	s_DebugNumberEnemy = inDebugNumber;
}
//---------------------------------------
// �f�o�b�N���Ă邩���Ȃ���
//---------------------------------------
bool IsDebug(void)//�擾
{
	return s_bDebug;
}

//---------------------------------------
// �f�o�C�X��Ԃ̐؂�ւ�
//---------------------------------------
void ChangeDebug(bool inDebug)
{
	s_bDebug = inDebug;
}

//---------------------------------------
// �}�X�ڊ���ۂ�
//---------------------------------------
bool GetEnemyAlignment()
{
	return EnemyAlignment;
}

int GetDebugNumberEnemy()
{
	return s_DebugNumberEnemy;
}

CEnemy *GetEnemy()
{
	return &enemy;
}