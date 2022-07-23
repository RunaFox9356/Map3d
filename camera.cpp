//============================
//
// カメラ設定
// Author:hamada ryuuga
//
//============================
#include "camera.h"
#include "input.h"

#include "file.h"
#include "letter.h"


//Particle DataEffect;
namespace nl = nlohmann;

static nl::json Camera;//リストの生成

//----------------------------
//コンストラクト
//----------------------------
CCamera::CCamera()
{



}

//----------------------------
//デストラクト
//----------------------------
CCamera::~CCamera()
{


}
//----------------------------
//初期化処理
//----------------------------
void CCamera::Init(void)
{
	m_aCamera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//視点　注視点　上方向　設定
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
//終了処理
//----------------------------
void CCamera::Uninit(void)
{
}

//----------------------------
//更新処理
//----------------------------
void CCamera::Update(void)
{
	// マップのスクロール
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
		//視点　注視点　上方向　設定
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
		int Number = GetCameraSystem();
		posVlog[Number] = m_aCamera.posV;
		posRlog[Number] = m_aCamera.posR;
	}
	if (GetKeyboardTrigger(DIK_P))
	{
		Savemove();
	}
	if (GetKeyboardTrigger(DIK_M))
	{
		Loadmove();
	}
	//変数宣言
	if (m_aCamera.bStartBool)
	{
		if (m_moveCount == 0)
		{// フレームカウントが0の時
		 // 目的の位置と向きの算出
			posVDest = ((posVlog[m_nCntFrame]) - m_aCamera.posV);
			posRDest = ((posRlog[m_nCntFrame]) - m_aCamera.posR);
		}

		// 変数宣言
		D3DXVECTOR3 addposV = posVDest / (float)keySet[m_nCntFrame];
		D3DXVECTOR3 addposR = posRDest / (float)keySet[m_nCntFrame];

		m_aCamera.posV += addposV;
		//	向きの加算
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
//描画処理
//----------------------------
void CCamera::Set(void)
{
	LPDIRECT3DDEVICE9  pDevice = GetDevice();//デバイスのポインタ

	//ビューマトリックスを初期化
	D3DXMatrixIdentity(&m_aCamera.MtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_aCamera.MtxView,
		&m_aCamera.posV,
		&m_aCamera.posR,
		&m_aCamera.vecU);

	//適用
	pDevice->SetTransform(D3DTS_VIEW, &m_aCamera.MtxView);

	//プロジェクションマトリックスを初期化
	D3DXMatrixIdentity(&m_aCamera.MtxProje);

	////プロジェクションマトリックス作成
	//D3DXMatrixPerspectiveFovLH(&m_aCamera.MtxProje,
	//	D3DXToRadian(90.0f),
	//	(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
	//	10.0f,
	//	10000.0f);

	// プロジェクションマトリックスの作成(平行投影)
	D3DXMatrixOrthoLH(&m_aCamera.MtxProje,					// プロジェクションマトリックス
		(float)SCREEN_WIDTH,								// 幅
		(float)SCREEN_HEIGHT,								// 高さ
		10.0f,												// ニア
		1000.0f);											// ファー

															//適用
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


void CCamera::Savemove()
{
	int nIndex = 0;

	for (int nCnt = 0; nCnt < MAX_CAMERAKEY; nCnt++)
	{

		    std::string name = "CAMERA";
			std::string Number = std::to_string(nIndex);
			name += Number;
			Camera[name] = 
			{
				{ "Vlog",{
					{ "X", posVlog[nCnt].x } ,
					{ "Y", posVlog[nCnt].y } ,
					{ "Z",posVlog[nCnt].z } } },
				{ "Rlog",{
					{ "X", posRlog[nCnt].x } ,
					{ "Y", posRlog[nCnt].y } ,
					{ "Z", posRlog[nCnt].z } } }
			};
			nIndex++;
	}
	Camera["INDEX"] = nIndex;

	auto jobj = Camera.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = "data//Out.json";
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}

void CCamera::Loadmove()
{
	std::ifstream ifs("data//Out.json");

	int nIndex = 0;

	if (ifs)
	{
		ifs >> Camera;
		nIndex = Camera["INDEX"];
		D3DXVECTOR3 pos;
		D3DXVECTOR3 size;
		D3DXVECTOR3 rot;
		int Life;
		for (int nCnt = 0; nCnt < nIndex; nCnt++)
		{
			std::string name = "CAMERA";
			std::string Number = std::to_string(nCnt);
			name += Number;

			posVlog[nCnt] = D3DXVECTOR3(Camera[name]["Vlog"]["X"], Camera[name]["Vlog"]["Y"], Camera[name]["Vlog"]["Z"]);
			posRlog[nCnt] = D3DXVECTOR3(Camera[name]["Rlog"]["X"], Camera[name]["Rlog"]["Y"], Camera[name]["Rlog"]["Z"]);

		}
	}
}