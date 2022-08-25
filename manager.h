//=============================================================================
//
// レンダラー
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _MANEAGER_H_			// このマクロ定義がされてなかったら
#define _MANEAGER_H_			// 二重インクルード防止のマクロ定義

#include "renderer.h"

class CRenderer;
class CInput;
class CTexture;
class CObject;
class CResult;
class CTitle;
class CFade;

class CManager
{
public:

	static const D3DXVECTOR3 Pos;

	//画面(モード)の種類
	enum MODE
	{
		MODE_TITLE = 0,		//タイトル画面
		MODE_GAME,			//ゲーム画面
		MODE_RESULT,		//リザルト画面
		MODE_FADE,			//フェード画面
		MODE_RANKING,		//ランキング画面
		MODE_GAMEOVER,
		MODE_TUTORIAL,
		MODE_NAMESET,
		MODE_MAX
	};

	CManager();
	~CManager();
	HRESULT Init(HWND hWnd, bool bWindow, HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();

	static CRenderer *GetRenderer();
	static CTexture* GetTexture();	// テクスチャの情報の取得
	static CFade* GetFade();	// テクスチャの情報の取得
	static void SetMode(CManager::MODE mode);
	
//	static CManager* GetInstance();
private:
	static CTexture *m_pTexture;
	static CRenderer * m_cRenderer;
	static CFade* m_Fade;
	CInput *m_Input;
	static CObject*m_Game;
	MODE m_mode;

};



#endif
