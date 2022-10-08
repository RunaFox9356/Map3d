//=============================================================================
//
// �����_���[
// Author : �l�c����
//
//=============================================================================


#ifndef _MANEAGER_H_			// ���̃}�N����`������ĂȂ�������
#define _MANEAGER_H_			// ��d�C���N���[�h�h�~�̃}�N����`

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

	//���(���[�h)�̎��
	enum MODE
	{
		MODE_TITLE = 0,		//�^�C�g�����
		MODE_GAME,			//�Q�[�����
		MODE_RESULT,		//���U���g���
		MODE_FADE,			//�t�F�[�h���
		MODE_RANKING,		//�����L���O���
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
	static CTexture* GetTexture();	// �e�N�X�`���̏��̎擾
	static CFade* GetFade();	// �e�N�X�`���̏��̎擾
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