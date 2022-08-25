//=============================================================================
//
// レンダラー
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _RENDERER_H_			// このマクロ定義がされてなかったら
#define _RENDERER_H_			// 二重インクルード防止のマクロ定義


//*****************************************************************************
// ライブラリーリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")

//*****************************************************************************
// インクルード
//*****************************************************************************
#include <Windows.h>
#include <tchar.h> // _T
#include <d3dx9.h>
#include <string>


//*****************************************************************************
// 定数定義
//*****************************************************************************
namespace
{
	// ウインドウのクラス名
	LPCTSTR CLASS_NAME = _T("AppClass");
	// ウインドウのキャプション名
	LPCTSTR WINDOW_NAME = _T("Fox");
}

// スクリーンの幅
const int SCREEN_WIDTH = 1280;
// スクリーンの高さ
const int SCREEN_HEIGHT = 720;
// 頂点フォーマット
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE| D3DFVF_TEX1);

const DWORD FVF_VERTEX_3D(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);


//頂点情報「3ｄ」の構造体を定義
struct VERTEX_3D
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//ベクトル
	D3DCOLOR col;//カラー
	D3DXVECTOR2 tex;//テクスチャ

};

int Gettime(void);

class CCamera;
class CLight;
class CParticle;


//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点データ
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;	// テクスチャ座標(レベル1)
};

class CRenderer
{
public:
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9	GetDevice(void)
	{
		return m_pD3DDevice;
	};

	static CCamera *GetCamera();

private:

#ifdef _DEBUG
	void DrawFPS();
#endif // _DEBUG


	// Direct3Dオブジェクト
	LPDIRECT3D9 m_pD3D = nullptr;
	// Deviceオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice = nullptr;

	
#ifdef _DEBUG
	// フォント
	LPD3DXFONT m_pFont = nullptr;
#endif // _DEBUG

	
	static CCamera* pCamera[2];
	static CLight* pLight;
	static CParticle* particle;


};


#endif