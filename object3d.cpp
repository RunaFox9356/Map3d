//============================
//
// object3d設定
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
// コンストラクタ
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
// デストラクタ
//------------------------------------
CObject3d::~CObject3d()
{
	assert(m_pMotion == nullptr);
}

//------------------------------------
// 初期化
//------------------------------------
HRESULT CObject3d::Init(void)
{
	D3DXVECTOR3 *Camerarot = CRenderer::GetCamera()->GetRot();

	//カメラのデータ取得
	m_rotMove = D3DXVECTOR3(D3DX_PI + Camerarot->y, D3DX_PI * 0.5f + Camerarot->y, 0.0f);

	m_modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	return S_OK;
}

//------------------------------------
// 終了
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
// 更新
//------------------------------------
void CObject3d::Update(void)
{

	// 現在のモーション番号の保管
	m_motionTypeOld = m_motionType;

	if (m_motionTypeOld != m_motionType)
	{// モーションタイプが変更された時
		m_pMotion->CntReset((int)(m_motionTypeOld));
		m_bMotionBlend = true;
	}

	if (m_bMotionBlend)
	{// モーションブレンドを使用してる
		m_bMotionBlend = m_pMotion->MotionBlend((int)(m_motionType));
	}
	else if (!m_bMotionBlend)
	{// モーションブレンドを使用してない場合
		m_bMotion = m_pMotion->PlayMotion((int)(m_motionType));
	}

	if (!m_bMotion)
	{// 使用してるモーションがない場合
		m_motionType = ANIME_NORMAL;
		m_isLoop = false;
	}

	//アニメーションや足音の設定
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
// 描画
//------------------------------------
void CObject3d::Draw(void)
{

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// 計算用マトリックス
	D3DMATERIAL9 marDef;
	D3DXVECTOR3 scale(1.8f, 1.8f, 1.8f);

	if (m_isUse)//使ってるやつ出す
	{
		// ワールドマトリックスの初期化
		// 行列初期化関数(第1引数の行列を単位行列に初期化)
		D3DXMatrixIdentity(&m_mtxWorld);
		
		// 行列拡縮関数
		D3DXMatrixScaling(&mtxScale, m_nScale.x, m_nScale.y, m_nScale.z);
	
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

		// 向きを反映
		// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		if (m_pos.x <=SCREEN_WIDTH / 2.0f+100.0f)
		{
			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			if (m_pMotion)
			{
				// パーツの描画設定
				m_pMotion->SetParts(m_mtxWorld, m_Damegeis);
			}
		}
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		pDevice->SetMaterial(&marDef);
	}
}
//------------------------------------
// POSだけセット
//------------------------------------
void CObject3d::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//------------------------------------
// SetMove関数
//------------------------------------
void CObject3d::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//------------------------------------
// 設定
//------------------------------------
void CObject3d::Set(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot,  char * pFileName)
{
	// 情報の初期化
	m_pos = pos;											// 位置の初期化
	m_posOld = m_pos;										// 過去位置の初期化
	m_posOld = m_pos;										// 過去位置の初期化
	m_rot = rot;											// 向きの初期化
	m_modelMin = D3DXVECTOR3(100.0f, 100.0f, 100.0f);		// 頂点座標の最小値
	m_modelMax = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);	// 頂点座標の最大値
	m_mtxWorld = {};										// ワールドマトリックス
	//m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目的の向き
	m_motionType = ANIME_NORMAL;							// ニュートラルモーション
	m_motionTypeOld = m_motionType;							// ニュートラルモーション
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 移動量
	m_bMotionBlend = false;									// モーションブレンドの使用状況
	m_isUse = true;											// 使用状況

	// モーション情報
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
			// 解放
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
