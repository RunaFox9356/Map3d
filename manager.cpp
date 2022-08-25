//=============================================================================
//
// マネージャー
// Author : 浜田琉雅
//
//=============================================================================

#include "manager.h"
#include "main.h"

#include "renderer.h"
#include "input.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "nemeset.h"

#include "texture.h"

#include "ranking.h"
//-----------------------------------------------------------------------------
// 静的メンバー変数の初期化
//-----------------------------------------------------------------------------
CRenderer * CManager::m_cRenderer = nullptr; 
CTexture * CManager::m_pTexture = nullptr;
CFade*  CManager::m_Fade = nullptr;
CObject*CManager::m_Game = nullptr;
const D3DXVECTOR3 CManager::Pos = D3DXVECTOR3(1280.0f * 0.5f, 720.0f * 0.5f, 0.0f);
//=============================================================================
// コンストラクト関数
//=============================================================================
CManager::CManager()
{

}
//=============================================================================
// デストラクト関数
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CManager::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	

	m_cRenderer = new CRenderer;

	m_Input = CInput::Create();

	// 初期化処理
	if (FAILED(m_cRenderer->Init(hWnd, TRUE)))	//画面サイズ
	{//初期化処理が失敗した場合
		return -1;
	}
	//入力処理の初期化処理
	if (FAILED(m_Input->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}


	m_pTexture = nullptr;
	m_pTexture = new CTexture;
	
	m_mode = CManager::MODE_GAME;	//現在のモード

	//モードの設定
	SetMode(m_mode);

	m_Fade = CFade::Create();

	
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CManager::Uninit()
{
	CObject::AllUninit();

	if (m_pTexture != nullptr)
	{// 終了処理

		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}
	if (m_cRenderer != nullptr)
	{// 終了処理

		m_cRenderer->Uninit();
		delete m_cRenderer;
		m_cRenderer = nullptr;
	}



	//入力処理の終了処理
	m_Input->Uninit();

}

//=============================================================================
// 更新
//=============================================================================
void CManager::Update()
{
	//入力処理の更新処理
	m_Input->Update();
	m_cRenderer->Update();
}

//=============================================================================
// 描画
//=============================================================================
void CManager::Draw()
{
	// 描画処理	
	m_cRenderer->Draw();
}


//=============================================================================
// GetRenderer
//=============================================================================
CRenderer *CManager::GetRenderer()
{
	return m_cRenderer;
}


//=============================================================================
// GetTexture
//=============================================================================
CTexture *CManager::GetTexture()
{
	return m_pTexture;
}

CFade * CManager::GetFade()
{
	return m_Fade;
}


//========================
// モードの設定
//========================
void CManager::SetMode(MODE mode)
{
	
	if (m_Game != nullptr)
	{
		m_Game->Release();
	}

	// ポリゴンの終了処理
	CObject::ModeNotUninit();
	
	switch (mode)
	{
	case CManager::MODE_TITLE:
		m_Game = new CTitle;
		break;
	case CManager::MODE_GAME:
		m_Game = new CGame;
		break;
	case CManager::MODE_RESULT:
		m_Game = new CResult;
		break;
	case CManager::MODE_RANKING:
		m_Game = new CRanking;
		break;
	case CManager::MODE_NAMESET:
		m_Game = new CNemeSet;
		break;
	default:
		break;
	}

	// 初期化処理
	if (FAILED(m_Game->Init()))	//画面サイズ
	{//初期化処理が失敗した場合
		return ;
	}
	m_Game->SetUp(CObject::MODE);
}

