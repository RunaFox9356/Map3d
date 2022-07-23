//============================
//
// �J�����ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

#define MAX_CAMERAKEY (3)
//�\����
typedef struct
{
	D3DXVECTOR3 posV;	//�ʒu
	D3DXVECTOR3 posR;	//�����_
	D3DXVECTOR3 vecU;	//�x�N�g��
	D3DXVECTOR3 directionV; //��]�̌����ʒu
	D3DXVECTOR3 directionR;//��]�̌��������_
	D3DXVECTOR3 rot;//�ʒu��]
	float fDistance;//����
	float fDistanceY;
	bool bBool;
	bool bStartBool;
	D3DVIEWPORT9 viewport;
	D3DXVECTOR3 posVDest, posRDest;
	D3DXMATRIX MtxProje; //�v���W�F�N�V�����}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J
	D3DXMATRIX MtxView; //�r���[�}�g���b�N�X//�|���S���̈ʒu���]�s�񂷂ׂĂ��߂Ă�i�j�J
}CAMERA;

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Init();//������
	void Uninit();//�j��
	void Update();//�X�V
	void Set();//��ʐݒ�
	CAMERA *Get(); //�Q�b�g
	D3DXMATRIX *GetMtxProje();
	D3DXMATRIX *GetMtxView();
	void Savemove();
	void Loadmove();
private:
	float m_Speed = 1.0f;

	float m_rotSpeed = 0.05f;
	float m_rotSpeed2 = D3DX_PI / 2;

	D3DXVECTOR3 posVlog[MAX_CAMERAKEY];
	D3DXVECTOR3 posRlog[MAX_CAMERAKEY];
	
	D3DXVECTOR3 posOriginV;
	D3DXVECTOR3 posOriginR;

	D3DXVECTOR3 posVDest;
	D3DXVECTOR3	posRDest;

	int keySet[MAX_CAMERAKEY];
	int m_moveCount;
	int m_nCntFrame;
	int m_next;
	//�X�^�e�B�b�N�ϐ�
	CAMERA m_aCamera;
	float m_rot;
};

#endif
