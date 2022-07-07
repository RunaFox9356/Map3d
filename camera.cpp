//============================
//
// �J�����ݒ�
// Author:hamada ryuuga
//
//============================
#include "camera.h"
#include "input.h"


//----------------------------
//�R���X�g���N�g
//----------------------------
CCamera::CCamera()
{



}

//----------------------------
//�f�X�g���N�g
//----------------------------
CCamera::~CCamera()
{


}
//----------------------------
//����������
//----------------------------
void CCamera::Init(void)
{
	m_aCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//���_�@�����_�@������@�ݒ�
	m_aCamera.posV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	m_aCamera.posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_aCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_aCamera.directionR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aCamera.directionV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_aCamera.bBool = true;
	m_aCamera.bStartBool = false;
	m_aCamera.fDistance = sqrtf((m_aCamera.posR.x - m_aCamera.posV.x) *
		(m_aCamera.posR.x - m_aCamera.posV.x) +
		(m_aCamera.posR.z - m_aCamera.posV.z) *
		(m_aCamera.posR.z - m_aCamera.posV.z));

	m_aCamera.fDistance = sqrtf((m_aCamera.posR.y - m_aCamera.posV.y)*
		(m_aCamera.posR.y - m_aCamera.posV.y) +
		(m_aCamera.fDistance*m_aCamera.fDistance));

	m_aCamera.rot.x = atan2f((m_aCamera.posR.z - m_aCamera.posV.z),
		(m_aCamera.posR.y - m_aCamera.posV.y));

	posOriginV =  D3DXVECTOR3(0.0f, 0.0f, -150.0f);
	posOriginR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	m_nCntFrame = 0;
	m_moveCount = 0;
	
	posVlog[0] = D3DXVECTOR3(100.0f, 0.0f, -150.0f);
	posRlog[0] = D3DXVECTOR3(100.0f, 1.0f, 0.0f);
	keySet[0] = 40;

	posVlog[1] = D3DXVECTOR3(500.0f, 0.0f, -150.0f);
	posRlog[1] = D3DXVECTOR3(500.0f, 1.0f, 0.0f);
	keySet[1] = 40;

	posVlog[2] = D3DXVECTOR3(-300.0f, 400.0f, -150.0f);
	posRlog[2] = D3DXVECTOR3(-300.0f, 411.0f, 0.0f);
	keySet[2] = 45;
}

//----------------------------
//�I������
//----------------------------
void CCamera::Uninit(void)
{
}

//----------------------------
//�X�V����
//----------------------------
void CCamera::Update(void)
{
	// �}�b�v�̃X�N���[��
	if (!GetKeyboardPress(DIK_LCONTROL) && !GetKeyboardPress(DIK_LSHIFT))
	{
		if (m_aCamera.bBool)
		{
			int wheel = GetMouseWheel();
			if (wheel > 0)
			{
				m_aCamera.posV.x -= 15.0f;
				m_aCamera.posR.x -= 15.0f;
			}
			else if (wheel < 0)
			{
				m_aCamera.posV.x += 15.0f;
				m_aCamera.posR.x += 15.0f;
			}
		}
		else
		{
			int wheel = GetMouseWheel();
			if (wheel > 0)
			{
				m_aCamera.posV.y -= 15.0f;
				m_aCamera.posR.y -= 15.0f;
			}
			else if (wheel < 0)
			{
				m_aCamera.posV.y += 15.0f;
				m_aCamera.posR.y += 15.0f;
			}
		}

	}

	if (GetKeyboardTrigger(DIK_J))
	{
		m_aCamera.bBool = !m_aCamera.bBool;
	}
	if (GetKeyboardTrigger(DIK_L))
	{
		m_aCamera.bStartBool = !m_aCamera.bStartBool;
		m_aCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//���_�@�����_�@������@�ݒ�
		m_aCamera.posV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
		m_aCamera.posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_aCamera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		m_aCamera.directionR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera.directionV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aCamera.fDistance = sqrtf((m_aCamera.posR.x - m_aCamera.posV.x) *
			(m_aCamera.posR.x - m_aCamera.posV.x) +
			(m_aCamera.posR.z - m_aCamera.posV.z) *
			(m_aCamera.posR.z - m_aCamera.posV.z));

		m_aCamera.fDistance = sqrtf((m_aCamera.posR.y - m_aCamera.posV.y)*
			(m_aCamera.posR.y - m_aCamera.posV.y) +
			(m_aCamera.fDistance*m_aCamera.fDistance));

		m_aCamera.rot.x = atan2f((m_aCamera.posR.z - m_aCamera.posV.z),
			(m_aCamera.posR.y - m_aCamera.posV.y));

		posOriginV = D3DXVECTOR3(0.0f, 0.0f, -150.0f);
		posOriginR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		m_nCntFrame = 0;
		m_moveCount = 0;


	}
	if (GetKeyboardTrigger(DIK_K))
	{
		int Number =GetCameraSystem();
		posVlog[Number] = m_aCamera.posV;
		posRlog[Number] = m_aCamera.posR;
	}
	//�ϐ��錾
	if (m_aCamera.bStartBool)
	{
		if (m_moveCount == 0)
		{// �t���[���J�E���g��0�̎�
		 // �ړI�̈ʒu�ƌ����̎Z�o
			posVDest = ((posVlog[m_nCntFrame]) - m_aCamera.posV);
			posRDest = ((posRlog[m_nCntFrame]) - m_aCamera.posR);
		}

		// �ϐ��錾
		D3DXVECTOR3 addposV = posVDest / (float)keySet[m_nCntFrame];
		D3DXVECTOR3 addposR = posRDest / (float)keySet[m_nCntFrame];

		m_aCamera.posV += addposV;
		//	�����̉��Z
		m_aCamera.posR += addposR;

		m_moveCount++;
		if (m_moveCount >= keySet[m_nCntFrame])
		{
			m_nCntFrame++;
			m_moveCount = 0;
			if (m_nCntFrame >= MAX_CAMERAKEY)
			{
				m_nCntFrame = 0;
			}
		}
	}
}

//----------------------------
//�`�揈��
//----------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9  pDevice = GetDevice();//�f�o�C�X�̃|�C���^

	//�r���[�}�g���b�N�X��������
	D3DXMatrixIdentity(&m_aCamera.MtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_aCamera.MtxView,
		&m_aCamera.posV,
		&m_aCamera.posR,
		&m_aCamera.vecU);

	//�K�p
	pDevice->SetTransform(D3DTS_VIEW, &m_aCamera.MtxView);

	//�v���W�F�N�V�����}�g���b�N�X��������
	D3DXMatrixIdentity(&m_aCamera.MtxProje);

	////�v���W�F�N�V�����}�g���b�N�X�쐬
	//D3DXMatrixPerspectiveFovLH(&m_aCamera.MtxProje,
	//	D3DXToRadian(90.0f),
	//	(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	//	10.0f,
	//	10000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬(���s���e)
	D3DXMatrixOrthoLH(&m_aCamera.MtxProje,					// �v���W�F�N�V�����}�g���b�N�X
		(float)SCREEN_WIDTH,								// ��
		(float)SCREEN_HEIGHT,								// ����
		10.0f,												// �j�A
		1000.0f);											// �t�@�[

															//�K�p
	pDevice->SetTransform(D3DTS_PROJECTION, &m_aCamera.MtxProje);
}

//----------------------------
//Get
//----------------------------
CAMERA * CCamera::Get(void)
{
	return &m_aCamera;
}
//----------------------------
//Get
//----------------------------
D3DXMATRIX  *CCamera::GetMtxProje()
{
	return &m_aCamera.MtxProje;
}
//----------------------------
//Get
//----------------------------
D3DXMATRIX *CCamera::GetMtxView()
{
	return &m_aCamera.MtxView;
}

