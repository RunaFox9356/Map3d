//============================
//
// ���C���ݒ�̃w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "xaudio2.h"
#include "xinput.h"//�W���C�p�b�g���͂ɕK�v

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")//�W���C�p�b�g���͂ɕK�v

//�}�N����`
#define SCREEN_WIDTH	(1280)			// ���T�C�Y
#define SCREEN_HEIGHT	(720)			// �����T�C�Y
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define CLASS_NAME		"WindowClass"	// �N���X�̖��O
#define WINDOW_NAME		"���s���e�ɂ�����}�b�v�G�f�B�^"		// ���O���߂�Ƃ�
#define ID_BUTTON_FINISH (101)			// �I���{�^����ID
#define ID_EDIT_INPUT1 (111)			// ���̓E�C���h�E1ID
#define ID_MODE	()
#define WORLDSIZ (100.0f)
#define ZERO_VECTOR		(D3DXVECTOR3(0.0f, 0.0f, 0.0f))		// �[���x�N�g��
#define ZERO_QUATERNION	(D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f))
class CCamera;

//=========================================
//���_���u�Q���v�̍\���̂��`
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;

}VERTEX_2D;

//=========================================
//���_���u3���v�̍\���̂��`
//=========================================
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �x�N�g��
	D3DCOLOR col;		// �J���[
	D3DXVECTOR2 tex;	// �e�N�X�`��
}VERTEX_3D;

//=========================================
//���[�h�̎��
//=========================================
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_GAMEOVER,
	MODE_RANKING,
	MODE_MAX
}MODE;

//=========================================
// �v���g�^�C�v�錾
//=========================================
LPDIRECT3DDEVICE9 GetDevice(void);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// �E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hlnstance, HWND hWnd, BOOL bWindow);	// ������
void Uninit(void);				// �j��
void Update(HWND hWnd);			// �X�V
void Draw(void);				// �`��
int GetFPS();					// FPS�̎擾

CCamera *GetCamera();
int GetCameraSystem();
int GetLife();
//=========================================
//�Z�b�g�̐ݒ�܂Ƃ�
//=========================================
void Settex(VERTEX_3D *pVtx, float left, float right, float top, float down);
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down);
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW);
void SetNormalpos(VERTEX_3D *pVtx, float XUP, float XDW, float YUP, float YDW, float ZUP, float ZDW);
void SetUp(VERTEX_2D *pVtx, float posx, float posy, float rotPlayer, float fAngleAttack, float fLengthAttack);

#endif
