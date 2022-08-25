//=============================================================================
//
// Object
// Author : 浜田琉雅
//
//=============================================================================

#include "object.h"
#include "3dpolygon.h"
#include "manager.h"
#include "input.h"
#include "utility.h"
#include "camera.h"
#include "hamada.h"


const D3DXVECTOR3 C3dpolygon::m_Vtx[4] =
{
	D3DXVECTOR3(-1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, +1.0f, 0.0f),
	D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
	D3DXVECTOR3(+1.0f, -1.0f, 0.0f),
};

//=============================================================================
// コンストラクト関数
//=============================================================================
C3dpolygon::C3dpolygon(int list):CObject(list)
{
	
}

//=============================================================================
// デストラクト関数
//=============================================================================
C3dpolygon::~C3dpolygon()
{
}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT C3dpolygon::Init()
{
	m_Size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
	m_nScale = 10.0f;
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスの取得

	m_texture = CTexture::TEXTURE_NONE;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,		//頂点フォーマット
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);


	VERTEX_3D*pVtx;		//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//rhwの設定
	pVtx[0].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

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
void C3dpolygon::Uninit()
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
void C3dpolygon::Update()
{
	m_nTimer++;
	m_rot.z = -D3DXToRadian(TIMER);
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void C3dpolygon::Draw()
{
	//デバイスへのポインタ
	//デバイスの取得
 	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールド座標行列の設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, CManager::GetTexture()->GetTexture(m_texture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,
		2);						//プリミティブ(ポリゴン)数

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetTexture(0, NULL);
}


//=============================================================================
// GetPos関数
//=============================================================================
const D3DXVECTOR3 *C3dpolygon::GetPos() const
{
	return &m_pos;
}

//=============================================================================
// SetPos関数
//=============================================================================
void C3dpolygon::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//--------------------------------------------------
// テクスチャの設定
//--------------------------------------------------
void C3dpolygon::SetTexture(CTexture::TEXTURE texture)
{
	m_texture = texture;
}

//---------------------------------------
//セットテクスチャ(2d)
//---------------------------------------
void C3dpolygon::SetTex(PositionVec4 Tex)
{
	VERTEX_3D *pVtx; //頂点へのポインタ

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

//---------------------------------------
//セットサイズ
//---------------------------------------
void  C3dpolygon::SetSize(const D3DXVECTOR3 &size)
{
	m_Size = size;

	VERTEX_3D*pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//------------------------
	// 頂点情報の設定
	//------------------------
	//頂点座標
	for (int i = 0; i < 4; ++i)
	{
		pVtx[i].pos.x = m_Vtx[i].x * m_Size.x; // TODO: これなおす。
		pVtx[i].pos.y = m_Vtx[i].y * m_Size.y; // TODO: これなおす。
		pVtx[i].pos.z = 0.0f;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

}
//---------------------------------------
//頂点Collarの設定
//---------------------------------------
void C3dpolygon::SetCollar(PositionVec4 Collar)
{
	VERTEX_3D *pVtx; //頂点へのポインタ

					 //頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標設定
	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);
	pVtx[1].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);
	pVtx[2].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);
	pVtx[3].col = D3DXCOLOR(Collar.P0, Collar.P1, Collar.P2, Collar.P3);


	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//---------------------------------------
//Vtxの取得
//---------------------------------------
LPDIRECT3DVERTEXBUFFER9 &C3dpolygon::GetVtx()
{
	return m_pVtxBuff;
}
