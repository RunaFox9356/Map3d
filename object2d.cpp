//=============================================================================
//
// Object
// Author : 浜田琉雅
//
//=============================================================================

#include "object.h"
#include "object2d.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"

 const D3DXVECTOR3 CObject2d::m_Vtx[4] =
{
	D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(-1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, +1.0f, 0.0f),
};
//=============================================================================
// コンストラクト関数
//=============================================================================
CObject2d::CObject2d(int list):CObject(list)
{
}

//=============================================================================
// デストラクト関数
//=============================================================================
CObject2d::~CObject2d()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CObject2d::Init()
{

	m_nScale = 10.0f;

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得

	m_texture = CTexture::TEXTURE_NONE;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------ 
	// 頂点情報の設定
	//------------------------
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);


	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CObject2d::Uninit()
{
	// 破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CObject2d::Update()
{
	
	VERTEX_2D *pVtx; //頂点へのポインタ

	 //頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 addPos[4];
	D3DXMATRIX mtx;	// 計算用マトリックス

	//マトリックス作成
	D3DXMatrixIdentity(&mtx);

	//頂点座標
	for (int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&addPos[i], &m_Vtx[i], &mtx);
		pVtx[i].pos.x = m_pos.x + (addPos[i].x * m_Size.x);//<-サイズ変更
		pVtx[i].pos.y = m_pos.y + (addPos[i].y * m_Size.y);//<-サイズ変更
		pVtx[i].pos.z = 0.0f;
	}

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CObject2d::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;        //デバイスへのポインタ

	 //デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	CTexture* pTexture = CManager::GetTexture();

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));
	
	//ポリゴンの描画

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,
		2);
	//プリミティブ(ポリゴン)数
	pDevice->SetTexture(0, NULL);
}

//=============================================================================
// create関数
//=============================================================================
CObject2d *CObject2d::Create()
{
	CObject2d * pObject = nullptr;
	pObject = new CObject2d;

	if (pObject != nullptr)
	{
		pObject->Init();
	}

	return pObject;
}

//=============================================================================
// GetPos関数
//=============================================================================
const D3DXVECTOR3 * CObject2d::GetPos() const
{
	return &m_pos;
}

//=============================================================================
// SetPos関数
//=============================================================================
void CObject2d::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}
//=============================================================================
// SetPos関数
//=============================================================================
void CObject2d::SetMove(const D3DXVECTOR3 &move)
{
	m_move = move;
}

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
void CObject2d::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
} 

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
CTexture::TEXTURE CObject2d::GetTexture()
{
	return m_texture;
}

//---------------------------------------
//セットテクスチャ(2d)
//---------------------------------------
void CObject2d::SetTex(PositionVec4 Tex)
{
	VERTEX_2D *pVtx; //頂点へのポインタ

	 //頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	pVtx[0].tex = D3DXVECTOR2(Tex.P0, Tex.P2);
	pVtx[1].tex = D3DXVECTOR2(Tex.P1, Tex.P2);
	pVtx[2].tex = D3DXVECTOR2(Tex.P0, Tex.P3);
	pVtx[3].tex = D3DXVECTOR2(Tex.P1, Tex.P3);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

void CObject2d::SetCollar(PositionVec4 Collar)
{
	VERTEX_2D *pVtx; //頂点へのポインタ

					 //頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);
	pVtx[1].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);
	pVtx[2].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);
	pVtx[3].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);

	m_col = Collar;
	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}