//============================
//
// object3d�ݒ�
// Author:hamada ryuuga
//
//============================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "object3d.h"
#include "input.h"
#include "camera.h"
#include "motion.h"
#include "manager.h"
#include "motion.h"
#include "crystal.h"
#include "score.h"
#include "hit.h"
#include "multiply.h"
#include "bell.h"
#include "player.h"

int CObject3d::m_drop = 0;
//------------------------------------
// �R���X�g���N�^
//------------------------------------
CObject3d::CObject3d() :
	m_pos(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rot(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_modelMin(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_modelMax(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_motionType(ANIME_NORMAL),
	m_motionTypeOld(ANIME_NORMAL),
	m_nMaxModelType(0),
	m_nMaxModelParts(0),
	m_nMaxMotion(0),
	m_type(0),
	m_shadow(0),
	m_invincible(0),
	m_consumption(0.0f),
	m_bMotionBlend(false),
	m_bMotion(false),
	m_isUse(false),
	m_isLoop(false),
	m_time(0),
	m_nparts(0),
	m_pow(0),
	m_nMotion(0),
	m_Damegeis(DAMEGE_NORMAL)
{
	//memset(&s_Player, NULL, sizeof(s_Player));
	/*memset(&m_motion, 0, sizeof(m_motion));*/

}

//------------------------------------
// �f�X�g���N�^
//------------------------------------
CObject3d::~CObject3d()
{
	assert(m_pMotion == nullptr);
}

//------------------------------------
// ������
//------------------------------------
HRESULT CObject3d::Init(void)
{
	D3DXVECTOR3 *Camerarot = CRenderer::GetCamera()->GetRot();

	//�J�����̃f�[�^�擾
	m_rotMove = D3DXVECTOR3(D3DX_PI + Camerarot->y, D3DX_PI * 0.5f + Camerarot->y, 0.0f);

	m_modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	return S_OK;
}

//------------------------------------
// �I��
//------------------------------------
void CObject3d::Uninit(void)
{
	m_pMotion->Uninit();
	if (m_pMotion != nullptr)
	{
		delete m_pMotion;
		m_pMotion = nullptr;
	}
}

//------------------------------------
// �X�V
//------------------------------------
void CObject3d::Update(void)
{

	// ���݂̃��[�V�����ԍ��̕ۊ�
	m_motionTypeOld = m_motionType;

	if (m_motionTypeOld != m_motionType)
	{// ���[�V�����^�C�v���ύX���ꂽ��
		m_pMotion->CntReset((int)(m_motionTypeOld));
		m_bMotionBlend = true;
	}

	if (m_bMotionBlend)
	{// ���[�V�����u�����h���g�p���Ă�
		m_bMotionBlend = m_pMotion->MotionBlend((int)(m_motionType));
	}
	else if (!m_bMotionBlend)
	{// ���[�V�����u�����h���g�p���ĂȂ��ꍇ
		m_bMotion = m_pMotion->PlayMotion((int)(m_motionType));
	}

	if (!m_bMotion)
	{// �g�p���Ă郂�[�V�������Ȃ��ꍇ
		m_motionType = ANIME_NORMAL;
		m_isLoop = false;
	}

	//�A�j���[�V�����⑫���̐ݒ�
	if (!m_isLoop)
	{
		m_motionType = ANIME_NORMAL;
	}

	m_Invincible--;
	if (m_Invincible <= 0)
	{
		m_Damegeis = DAMEGE_NORMAL;
	}
}

//------------------------------------
// �`��
//------------------------------------
void CObject3d::Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 marDef;
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	if (m_isUse)//�g���Ă��o��
	{
		// ���[���h�}�g���b�N�X�̏�����
		// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)
		D3DXMatrixIdentity(&m_mtxWorld);
		
		// �s��g�k�֐�
		D3DXMatrixScaling(&mtxScale, m_nScale.x, m_nScale.y, m_nScale.z);
	
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

		// �����𔽉f
		// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_pos.x <=SCREEN_WIDTH / 2.0f+100.0f)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			if (m_pMotion)
			{
				// �p�[�c�̕`��ݒ�
				m_pMotion->SetParts(m_mtxWorld, m_Damegeis);
			}
		}
		// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		pDevice->SetMaterial(&marDef);
	}
}
//------------------------------------
// POS�����Z�b�g
//------------------------------------
void CObject3d::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//------------------------------------
// SetMove�֐�
//------------------------------------
void CObject3d::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//------------------------------------
// �ݒ�
//------------------------------------
void CObject3d::Set(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot,  char * pFileName)
{
	// ���̏�����
	m_pos = pos;											// �ʒu�̏�����
	m_posOld = m_pos;										// �ߋ��ʒu�̏�����
	m_posOld = m_pos;										// �ߋ��ʒu�̏�����
	m_rot = rot;											// �����̏�����
	m_modelMin = D3DXVECTOR3(100.0f, 100.0f, 100.0f);		// ���_���W�̍ŏ��l
	m_modelMax = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);	// ���_���W�̍ő�l
	m_mtxWorld = {};										// ���[���h�}�g���b�N�X
	//m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړI�̌���
	m_motionType = ANIME_NORMAL;							// �j���[�g�������[�V����
	m_motionTypeOld = m_motionType;							// �j���[�g�������[�V����
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// �ړ���
	m_bMotionBlend = false;									// ���[�V�����u�����h�̎g�p��
	m_isUse = true;											// �g�p��

	// ���[�V�������
	m_pMotion = new CMotion(pFileName);
	assert(m_pMotion != nullptr);
}

//------------------------------------
// GetPos
//------------------------------------
const D3DXVECTOR3 * CObject3d::GetPos() const
{
	return &m_pos;
}
//------------------------------------
// GetSize
//------------------------------------
const D3DXVECTOR3 * CObject3d::GetSize() const
{
	return &m_nScale;
}
//------------------------------------
//GetRot
//------------------------------------
const D3DXVECTOR3 * CObject3d::GetRot() const
{
	return nullptr;
}

//------------------------------------
// SetSize
//------------------------------------
void CObject3d::SetSize(D3DXVECTOR3 &Size)
{
	m_nScale = Size;
}

//------------------------------------
// SetLife
//------------------------------------
void CObject3d::SetLife(int Life)
{
	m_Life = Life;
}

//------------------------------------
// HitLife
//------------------------------------
void CObject3d::HitLife(int Damage)
{
	m_Life -= Damage;
	
	m_Damegeis = DAMEGE_DAMAGE;

	m_Invincible = INVINCIBLE;
	EObjectType Type =  GetType();

	if (Type != PLAYER)
	{
		if (m_Life <= 0)
		{

			CMultiply::SetRate((1 + *CMultiply::GetRate()));
			CMultiply::list(*CMultiply::GetRate(), m_pos, true);

			for (int i = 0; i < 5; i++)
			{
				GetScore()->Add(50);
				D3DXVECTOR3 scale(3.8f, 3.8f, 3.8f);
				CBell * Bell = CBell::Create();
				Bell->SetUp(BELL);

				Bell->SetMove(D3DXVECTOR3((float)-(rand() % 10 + 1), (float)(rand() % 10 + 5), 0.0f));
				Bell->SetPos(m_pos);
				Bell->SetSize(scale);

				Bell->SetLife(10);
			}
			m_drop++;
			if (m_drop >= 5)
			{
				m_drop = 0;
				CCrystal::Create(m_pos, D3DXVECTOR3(0.0f, 2.0f, 0.0f));
			}
			// ���
			CObject::Release();
		}
	}
	else
	{
		if (m_Life <= 0)
		{
			CManager::SetMode(CManager::MODE_RESULT);
		}
	}
}
//------------------------------------
// GetDamegeData
//------------------------------------
CObject3d::DAMEGE &CObject3d::GetDamegeData()
{
	return m_Damegeis;
}

//------------------------------------
// SetRot
//------------------------------------
void CObject3d::SetRot(D3DXVECTOR3 &Rot)
{
	m_rot = Rot;
}