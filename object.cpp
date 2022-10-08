//=============================================================================
//
// Object
// Author : �l�c����
//
//=============================================================================

#include "object.h"
#include "object2d.h"
#include "player.h"
#include "manager.h"
#include "enemy.h"
#include "gon.h"
#include "bullet.h"
#include "mesh.h"
#include "magic.h"
#include "score.h"
#include "bg.h"
#include "life.h"
#include <assert.h>


CObject *CObject::m_pObject[MAX_LIST][MAX_OBJECT] = {};
int CObject::m_AllMember = 0; 
int CObject::m_AllEnemy = 0;
CScore * CObject::pScore;
CLife* CObject::pLife;
bool CObject::notBoss = false;
//=============================================================================
// �R���X�g���N�g�֐�
//=============================================================================
CObject::CObject(int list)
{
	for (int i = 0; i < MAX_OBJECT; i++)
	{
		if (m_pObject[list][i] == nullptr)
		{
			m_Type = NONE;
			m_list = list;
			m_nID = i;
			m_pObject[list][i] = this;
			m_AllMember++;
			break;
		}
	}
}
//=============================================================================
// �f�X�g���N�g�֐�
//=============================================================================
CObject::~CObject()
{
}

//=============================================================================
// AllUpdate�֐�
//=============================================================================
void CObject::AllUpdate()
{
	for (int j = 0; j < MAX_LIST; j++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_pObject[j][i] != nullptr)
			{
				m_pObject[j][i]->Update();
			}
		}
	}
}
//=============================================================================
// AllDraw�֐�
//=============================================================================
void CObject::AllDraw()
{
	for (int j = 0; j < MAX_LIST; j++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_pObject[j][i] != nullptr)
			{
				m_pObject[j][i]->Draw();
			}
		}
	}
}

//=============================================================================
// NotBGDraw�֐�
//=============================================================================
void CObject::TypeDraw(EObjectType Type)
{
	for (int j = 0; j < MAX_LIST; j++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (Type == BG)
			{
				if (m_pObject[j][i] != nullptr)
				{
					if (m_pObject[j][i]->m_Type == BG)
					{
						m_pObject[j][i]->Draw();
					}
				}
			}
			else
			{
				if (m_pObject[j][i] != nullptr)
				{
					if (m_pObject[j][i]->m_Type != BG)
					{
						m_pObject[j][i]->Draw();
					}
				}
			}
		}
	}
}


//=============================================================================
// AllUninit�֐�
//=============================================================================
void CObject::AllUninit()
{
	for (int j = 0; j < MAX_LIST; j++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_pObject[j][i] != nullptr)
			{
				m_pObject[j][i]->Uninit();
				delete m_pObject[j][i];
				m_pObject[j][i] = nullptr;
			}

		}
	}
	m_AllMember = 0;
}

//=============================================================================
// ModeNotUninit�֐�
//=============================================================================
void CObject::ModeNotUninit()
{
	for (int j = 0; j < MAX_LIST; j++)
	{
		for (int i = 0; i < MAX_OBJECT; i++)
		{
			if (m_pObject[j][i] != nullptr)
			{
				if (m_pObject[j][i]->m_Type != MODE)
				{
					m_pObject[j][i]->Uninit();
					delete m_pObject[j][i];
					m_pObject[j][i] = nullptr;
				}
			}
		}
	}
	m_AllMember = 1;
}
//=============================================================================
// AllCreate�֐�
//=============================================================================
void CObject::AllCreate()
{
	CBg * Bg1 = CBg::Create();
	Bg1->SetMove(D3DXVECTOR3(0.0001f, 0.0f, 0.0f));
	Bg1->SetTexture(CTexture::TEXTURE_STARRY);
	CBg * Bg3 = CBg::Create();
	Bg3->SetMove(D3DXVECTOR3(0.00001f, 0.0f, 0.0f));
	Bg3->SetTexture(CTexture::TEXTURE_MOON);

	CBg * Bg2 = CBg::Create();
	Bg2->SetMove(D3DXVECTOR3(0.001f, 0.0f, 0.0f));
	Bg2->SetTexture(CTexture::TEXTURE_TOWN);


	CEnemy::LoadEnemy("Data/datatest.json");
	pScore = CScore::Create(D3DXVECTOR3(30.0f, 70.0f, 0.0f));
	pScore->Set(0);

	pLife = CLife::Create(D3DXVECTOR3(300.0f, 150.0f, 0.0f));

}

//=============================================================================
// Set�֐�
//=============================================================================
void CObject::SetUp(EObjectType Type)
{
	switch (Type)
	{
	case EObjectType::ENEMY:
	{
		m_Type = ENEMY;
		m_AllEnemy++;
		break;
	}
	case EObjectType::PLAYER:
	{
		m_Type = PLAYER;
		break;
	}
	case EObjectType::BULLET:
		m_Type = BULLET;
		break;
	case EObjectType::GON:
		m_Type = GON;
		break;
	case EObjectType::MAGIC:
		m_Type = MAGIC;
		break;
	case EObjectType::CRYSTAL:
		m_Type = CRYSTAL;
		break;
	case EObjectType::BG:
		m_Type = BG;
		break;
	case EObjectType::MODE:
		m_Type = MODE;
		break;
	default:
		break;
	}
	
}


//=============================================================================
// release�֐�
//=============================================================================
void CObject::Release()
{
	if (m_pObject[m_list][m_nID] != nullptr)
	{
		const int nID = m_nID;
		const int nlist = m_list;
		m_pObject[nlist][nID]->Uninit();
		delete m_pObject[nlist][nID];
		m_pObject[nlist][nID] = nullptr;
		m_AllMember--;
	}
}

//=============================================================================
// object�̃f�[�^���擾����֐�
//=============================================================================
CObject**CObject::GetObjectData(int nCount)
{
	return m_pObject[nCount];
}

//=============================================================================
// �X�R�A�̃f�[�^���擾����֐�
//=============================================================================
CScore*CObject::GetScore()
{
	return pScore;
}

//=============================================================================
// object��Id���擾����֐�
//=============================================================================
int * CObject::GetId()
{
	return &m_nID;
}

//=============================================================================
// object�̃^�C�v�̎擾�֐�
//=============================================================================
CObject::EObjectType CObject::GetType()
{
	return m_Type;
}

//=============================================================================
// object��Life���擾
//=============================================================================
CLife* CObject::GetLife()
{
	return pLife;
}