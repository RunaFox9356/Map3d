//=============================================================================
//
// 3dpolygon
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _3DPOLYGON_H_			// このマクロ定義がされてなかったら
#define _3DPOLYGON_H_			// 二重インクルード防止のマクロ定義

#include "renderer.h"
#include "texture.h"
#include "object2d.h"

#define SIZEX (45.0f)
#define SIZEY (90.0f)
#define TIMER (m_nTimer + 90)

class  C3dpolygon : public CObject
{
protected:
	//polygonの基準サイズ
	static const D3DXVECTOR3 m_Vtx[4];

public:
	C3dpolygon(int list = 0);
	~C3dpolygon() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	virtual const D3DXVECTOR3 *GetPos() const;
	virtual void SetPos(const D3DXVECTOR3 &pos);
	virtual void SetMove(const D3DXVECTOR3 &move) = 0;
	
	void SetTexture(CTexture::TEXTURE texture);
	void SetTex(PositionVec4 Tex);
	void SetSize(const D3DXVECTOR3 &size);
	void SetCollar(PositionVec4 Collar);
	LPDIRECT3DVERTEXBUFFER9 &GetVtx();

protected:
	float m_nScale;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_pos; //polygonの位置
	D3DXVECTOR3 m_Size;
	D3DXMATRIX m_mtxWorld;					// マトリックス
	int  m_nTimer;
private:

	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff = nullptr;
	CTexture::TEXTURE m_texture;	// テクスチャの列挙型

};

#endif
