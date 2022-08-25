//=============================================================================
//
// 2Dオブジェクト
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _OBJECT2D_H_			// このマクロ定義がされてなかったら
#define _OBJECT2D_H_			// 二重インクルード防止のマクロ定義

#include "renderer.h"
#include "object.h"
#include "texture.h"

struct PositionVec4
{
	float     P0, P1, P2, P3;
	constexpr PositionVec4() : P0(0.0f), P1(0.0f), P2(0.0f), P3(0.0f) { }
	constexpr PositionVec4(float _P0, float _P1, float _P2, float _P3) : P0(_P0), P1(_P1), P2(_P2), P3(_P3) { }
};

class CObject2d : public CObject
{
private:
	//polygonの拡大サイズ
	static const D3DXVECTOR3 m_Vtx[4];

public:
	static CObject2d *CObject2d::Create();

	CObject2d(int list = 0);
	~CObject2d() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	virtual const D3DXVECTOR3 *GetPos() const;

	virtual void SetPos(const D3DXVECTOR3 &pos);
	virtual void SetMove(const D3DXVECTOR3 &move);

	void SetTexture(CTexture::TEXTURE texture);
	void SetTex(PositionVec4 Tex);
	CTexture::TEXTURE GetTexture(); 
	void SetCollar(PositionVec4 Collar);

	D3DXCOLOR GetCollar() { return D3DXCOLOR(m_col.P0, m_col.P1, m_col.P2, m_col.P3); };
	D3DXVECTOR3 GetSize() { return m_Size; };

	void SetSize(D3DXVECTOR3 Size) { m_Size = Size; };
	D3DXVECTOR3 &GetMove() { return m_move; };
	LPDIRECT3DVERTEXBUFFER9 &GetVtx() { return m_pVtxBuff; };
protected:
	int  m_nTimer;
	float m_nScale;
	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_move; //polygonの位置
	D3DXVECTOR3 m_pos; //polygonの位置
	PositionVec4 m_col;	// 色
private:
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff = NULL;
	CTexture::TEXTURE m_texture;	// テクスチャの列挙型
};

#endif
