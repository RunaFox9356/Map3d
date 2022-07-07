//=================================================
//	��������
//
// �}�b�v�G�f�B�^
// Auther�F Hamada Ryuuga
//
//=================================================
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "input.h"
#include "comn.h"
#include "bg.h"

#include "enemy.h"
#include "map.h"
#include "Pallet.h"
#include "Select.h"
#include "PalletEnemy.h"
#include "resource1.h"
#include "process.h"
#include "debug.h"
#include "camera.h" 
#include "player.h"
#include "light.h"
#include "comn.h"

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <d3d9.h>
#include <tchar.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>

#define MAX_NAME (7)



//=================================================
// �ÓI�ϐ�
//=================================================
static LPDIRECT3D9 s_pD3D = NULL;				// Direct3d�I�u�W�F�N�g�ւ̃|�C���^
static LPDIRECT3DDEVICE9 s_pD3DDevice = NULL;	// Direct3d�f�o�C�X�ւ̂ۂ���
static int s_nCountFPS;							// FPS�̃J�E���^�[
static bool bPress = false;						// ���{���o�[�̃g���K�[�����̂��߂ɕK�v�ȕϐ�
D3DPRESENT_PARAMETERS    g_d3dpp = {};
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
bool show_demo_window = true;//��{�̌Ăяo��
bool show_another_window = false;//������Ăяo��
static char Txet[8] = "";

int camera = 0;//������Ăяo��


void ResetDevice();
bool ImGuiTxet(bool show_demo_window, bool show_another_window);
int Button(int nSize);
// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

CCamera* pCamera;

//===================
//���C���֐�
//===================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hlnstacePrev, LPSTR ipCmdLine, int nCmdShow)
{
	HWND hWnd;	//Window�n���h�����ʎq
	MSG msg;	//���b�Z�[�W���i�[����ϐ�
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),											// WNDCLASSEX�̃������T�C�Y
		CS_HREDRAW | CS_VREDRAW,									// �E�C���h�E�̃X�^�C��
		WindowProc,													// Window�v���V�[�W��  
		0,															// �[���ɂ���
		0,															// �[���ɂ���
		hInstance,													// �C���X�^���X�n���h��
		LoadIcon(hInstance, IDI_APPLICATION),		// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL, IDC_ARROW),								// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),									// �N���C�A���g�̗̈�w�i�F
		MAKEINTRESOURCE(IDR_MENU1) ,								// ���j���[�o�[
		CLASS_NAME,													// Window�N���X�̖��O
		LoadIcon(wcex.hInstance, IDI_APPLICATION)	// �t�@�C���A�C�R��
	};

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(
		0,							// �g��Windows�X�^�C��
		CLASS_NAME,					// Window�N���X�̖��O
		WINDOW_NAME,				// Window�̖��O
		WS_OVERLAPPEDWINDOW,		// Window�X�^�C��
		CW_USEDEFAULT,				// Window�̍���X���W
		CW_USEDEFAULT,				// Window����Y���W
		(rect.right - rect.left),	// Window��
		(rect.bottom - rect.top),	// Window����
		NULL,						// �eWindow�̃n���h��
		NULL,						// ���j���[�n���h���܂��͌qWindowID
		hInstance,					// �C���X�^���X�n���h��
		IDI_APPLICATION);			// Window�쐬�f�[�^

	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExedastTime;	// �Ō�X�V����
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS

	if (FAILED(Init(hInstance, hWnd, TRUE)))	// ������false�ɂ���Ƒ��ʂɂȂ�
	{// �����������s�����ꍇ
		return -1;
	}

	// Show the window
	::ShowWindow(hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(hWnd);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontConfig config;
config.MergeMode = true;
io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(s_pD3DDevice);

	//����\�̐ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExedastTime = timeGetTime();

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ���b�Z�[�W���[�v
	while (1)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯂Ƃ����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���

			dwCurrentTime = timeGetTime();//���ݎ������擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{ // 0.5�b�o��
				// FPS�v��
				s_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExedastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExedastTime = dwCurrentTime;	// �����J�n�̎���[���ݎ���]��ۑ�

				// �X�V����
				show_another_window = ImGuiTxet(show_demo_window, show_another_window);

				Update(hWnd);

				// �`�揈��
				Draw();

				dwFrameCount++;
			}
		}

	}


	//�I������
	Uninit();


	::DestroyWindow(hWnd);
	::UnregisterClass(wcex.lpszClassName, wcex.hInstance);

	//���@�\��߂�
	timeEndPeriod(1);
	// Window�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//========================
//�E�B���h�E�����Ă����
//========================
static void funcFileSave(HWND hWnd, bool nMap)
{
	static OPENFILENAME     ofn;
	static TCHAR            szPath[MAX_PATH];
	static TCHAR            szFile[MAX_PATH];

	if (szPath[0] == TEXT('\0')) {
		GetCurrentDirectory(MAX_PATH, szPath);
	}
	if (ofn.lStructSize == 0) {
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrInitialDir = szPath;	// �����t�H���_�ʒu
		ofn.lpstrFile = szFile;			// �I���t�@�C���i�[
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrDefExt = TEXT(".json");
		ofn.lpstrFilter = TEXT("json�t�@�C��(*.json)\0*.json\0");
		ofn.lpstrTitle = TEXT("�e�L�X�g�t�@�C����ۑ����܂��B");
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
	}
	if (GetSaveFileName(&ofn)) {
		MessageBox(hWnd, szFile, TEXT("�t�@�C������t���ĕۑ�"), MB_OK);
	}

	if (szFile[0] != '\0')
	{
		if (nMap)
		{
			OutputMap(szFile);
		}
		if (!nMap)
		{
			GetEnemy()->OutputEnemy(szFile);
		}
	}
	bPress = true;
}

//---------------------------------------
//�E�B���h�E�v���V�[�W��
//---------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//�|�C���g�\����
	//	POINT    pt;

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	int nID;//�Ԃ�l���i�[
	static HWND hWndEditlnput1;		//���̓E�B���h�E�n���h��(���ʎq)

	switch (uMsg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //	�L�[�̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE: //�G�X�P�[�v�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂����H","�I���̃R�}���h", MB_YESNO | MB_TOPMOST);
			// ��������NULL�ɂ���ƃ��b�Z�[�WBOX�A�N�e�B�u�E�B���h�E�ɂȂ�Ȃ�
			// ��������hWnd�ɂ���Ƃ��̃E�B���h�E���e(�I�[�i�[)�ɂȂ�A
			// ���̃��b�Z�[�WBOX���������Ȃ�����ق��̏������ł��Ȃ�
			// ���̓E�B���h�E�̐���

			if (nID == IDYES)
			{// Window��j�󂷂�(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
				break;
			}
			break;
		}
		break;

		// case WM_RBUTTONDOWN:
		// pt.x = LOWORD(lParam);
		// pt.y = HIWORD(lParam);
		// //�N���C�A���g���W���X�N���[�����W�֕ϊ�
		// ClientToScreen(hWnd, &pt);
		// //�|�b�v�A�b�v���j���[��\��
		// TrackPopupMenu(GetSubMenu(GetMenu(hWnd), 0), TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
		// break;
	case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case ID_40001:
				// �}�b�v�ۑ�
				funcFileSave(hWnd, true);

				break;
			case ID_40002:
				// �G�l�~�[�ۑ�
				funcFileSave(hWnd, false);

				break;
			case ID_40003:
				//�f�o�b�NMAPON
				if (!IsDebug())
				{
					ChangeDebug(true);
					ConteSet(0);
				}
				ChangeSetMode(false);
				break;
			case ID_40004:
				// �f�o�b�NEnemyON
				if (!IsDebug())
				{
					ChangeDebug(true);
					ConteSet(0);
				}

				ChangeSetMode(true);
				break;
			case ID_40005:
				setDebugNumber(32);
				// �f�o�b�NEnemyON
				break;
			case ID_40006:
				// �o�[�W����(A)
				MessageBox(hWnd, ("�X�V��������\n�G�����܂�\n�G�������܂�"), ("�}�b�v�G�f�B�^�[Ver1.1"), MB_OK);
				break;
			case ID_40007:
				// �o�[�W����(A)
				MessageBox(hWnd, ("�͈͑I��"), ("�}�b�v�G�f�B�^�[Ver1.2"), MB_OK);
				break;
			default:
				bPress = false;
				break;
			}
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//---------------------------------------
//�f�o�C�X�擾
//---------------------------------------
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return s_pD3DDevice;
}

//---------------------------------------
// ������
//---------------------------------------
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)//TRUE�F�E�C���h�E/FALSE:�t���X�N���[��
{
	// ���[�J���ϐ��錾
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3d�I�u�W�F�N�g�̐���
	s_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (s_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(s_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �|���S���̏���������
	ZeroMemory(&d3dpp, sizeof(d3dpp));	// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`����
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ��i�f���M���̓����j
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ��ĂP�U��it���g��
	d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if ((FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))) &&
		(FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))) &&
		(FAILED(s_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &s_pD3DDevice))))
	{
		return E_FAIL;
	}

	//���W�_�[�X�e�[�g�̐ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O�̐ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h�ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �A���t�@�u�����h�ݒ�
	s_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �A���t�@�u�����h�ݒ�

	//�T���v���[�X�e�[�g�̐ݒ�
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �������̊g��
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �傫���̏k��
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	s_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�̐ݒ�
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �|���S���ƃe�N�X�`���̐F��������
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	s_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//���͏����̏���������
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	pCamera = new CCamera;
	pCamera->Init();

	//�����̏�����
	srand((unsigned int)time(0));

	// ����
	InitProcess();

	// �f�o�b�O
	InitDebug();

	return S_OK;
}

//---------------------------------------
// �I��
//---------------------------------------
void Uninit(void)
{
	//�I������
	UninitInput();
	UninitProcess();
	UninitDebug();


	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (s_pD3D != NULL)
	{
		s_pD3D->Release();
		s_pD3D = NULL;
	}
	if (s_pD3DDevice != NULL)
	{
		s_pD3DDevice->Release();
		s_pD3DDevice = NULL;
	}
	// �J�����I������
	if (pCamera != nullptr)
	{
		pCamera->Uninit();
		delete pCamera;
		pCamera = nullptr;
	}
}

//---------------------------------------
// �X�V
//---------------------------------------
void Update(HWND hWnd)
{
	//�X�V����
	UpdateInput();
	UpdateProcess();
	pCamera->Update();
}

//---------------------------------------
// �`��
//---------------------------------------
void Draw(void)
{
	//��ʃN���A
	s_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	if (SUCCEEDED(s_pD3DDevice->BeginScene()))
	{//���������Ƃ�
	 //D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x*clear_color.w*0.0f), (int)(clear_color.y*clear_color.w*255.0f), (int)(clear_color.z*clear_color.w*255.0f), (int)(clear_color.w*255.0f));
	 //g_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

	 //g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	 //g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	 //g_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	 //HRESULT result = g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

		pCamera->Set();
		DrawProcess();

		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

#ifdef _DEBUG

#endif // DEBUG

		s_pD3DDevice->EndScene();	//�`��I��
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	s_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//---------------------------------------
// FPS�̎擾
//---------------------------------------
int GetFPS()
{
	return s_nCountFPS;
}


//---------------------------------------
//�Z�b�g�e�N�X�`��(3d)
//---------------------------------------
void Settex(VERTEX_3D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);
}

//---------------------------------------
//�Z�b�g�e�N�X�`��(2d)
//---------------------------------------
void SetUp(VERTEX_2D *pVtx,
	float posx,		//���S�_��
	float posy,		//���S�_��
	float rotPlayer,		//�v���C���[��]
	float fAngleAttack 		//�Ίp���p�x
	, float fLengthAttack)	//�Ίp���m����
{
	//��]
	pVtx[0].pos.x = posx + sinf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.y = posy + cosf(rotPlayer - D3DX_PI + fAngleAttack)*fLengthAttack;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = posx + sinf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.y = posy + cosf(rotPlayer + D3DX_PI - fAngleAttack)*fLengthAttack;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = posx + sinf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.y = posy + cosf(rotPlayer + -fAngleAttack)*fLengthAttack;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = posx + sinf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.y = posy + cosf(rotPlayer + fAngleAttack)*fLengthAttack;
	pVtx[3].pos.z = 0.0f;
}

//---------------------------------------
//�Z�b�g�e�N�X�`��(2d)
//---------------------------------------
void Settex2d(VERTEX_2D *pVtx, float left, float right, float top, float down)
{
	//�e�N�X�`���̍��W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(left, top);
	pVtx[1].tex = D3DXVECTOR2(right, top);
	pVtx[2].tex = D3DXVECTOR2(left, down);
	pVtx[3].tex = D3DXVECTOR2(right, down);

}

//---------------------------------------
//�Z�b�g�|�X(3d)
//---------------------------------------
void SetNormalpos(VERTEX_3D *pVtx, float XUP, float XDW, float YUP, float YDW, float ZUP, float ZDW)
{
	pVtx[0].pos = D3DXVECTOR3(XDW, YUP, ZUP);
	pVtx[1].pos = D3DXVECTOR3(XUP, YUP, ZUP);
	pVtx[2].pos = D3DXVECTOR3(XDW, YDW, ZDW);
	pVtx[3].pos = D3DXVECTOR3(XUP, YDW, ZDW);
}

//---------------------------------------
//�Z�b�g�|�X(2d)
//---------------------------------------
void SetNormalpos2d(VERTEX_2D *pVtx, float XUP, float XDW, float YUP, float YDW)
{
	pVtx[0].pos = D3DXVECTOR3(XUP, YUP, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(XDW, YUP, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(XUP, YDW, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(XDW, YDW, 0.0f);
}

//---------------------------------------
//�f�t�H���g
//---------------------------------------
void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = s_pD3DDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}

//---------------------------------------
//�T�C���J�[�u�̂���
//---------------------------------------
// ImGui Bezier widget. @r-lyeh, public domain
// v1.02: add BezierValue(); comments; usage
// v1.01: out-of-bounds coord snapping; custom border width; spacing; cosmetics
// v1.00: initial version
//
// [ref] http://robnapier.net/faster-bezier
// [ref] http://easings.net/es#easeInSine
//
// Usage:
// {  static float v[] = { 0.390f, 0.575f, 0.565f, 1.000f }; 
//    ImGui::Bezier( "easeOutSine", v );       // draw
//    float y = ImGui::BezierValue( 0.5f, v ); // x delta in [0..1] range
// }



namespace ImGui
{
	template<int steps>
	void bezier_table(ImVec2 P[4], ImVec2 results[steps + 1]) {
		static float C[(steps + 1) * 4], *K = 0;
		if (!K) {
			K = C;
			for (unsigned step = 0; step <= steps; ++step) {
				float t = (float)step / (float)steps;
				C[step * 4 + 0] = (1 - t)*(1 - t)*(1 - t);   // * P0
				C[step * 4 + 1] = 3 * (1 - t)*(1 - t) * t; // * P1
				C[step * 4 + 2] = 3 * (1 - t) * t*t;     // * P2
				C[step * 4 + 3] = t*t*t;               // * P3
			}
		}
		for (unsigned step = 0; step <= steps; ++step) {
			ImVec2 point = {
				K[step * 4 + 0] * P[0].x + K[step * 4 + 1] * P[1].x + K[step * 4 + 2] * P[2].x + K[step * 4 + 3] * P[3].x,
				K[step * 4 + 0] * P[0].y + K[step * 4 + 1] * P[1].y + K[step * 4 + 2] * P[2].y + K[step * 4 + 3] * P[3].y
			};
			results[step] = point;
		}
	}

	float BezierValue(float dt01, float P[4]) {
		enum { STEPS = 256 };
		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
		ImVec2 results[STEPS + 1];
		bezier_table<STEPS>(Q, results);
		return results[(int)((dt01 < 0 ? 0 : dt01 > 1 ? 1 : dt01) * STEPS)].y;
	}

	int Bezier(const char *label, float P[4]) {
		// visuals
		enum { SMOOTHNESS = 64 }; // curve smoothness: the higher number of segments, the smoother curve
		enum { CURVE_WIDTH = 4 }; // main curved line width
		enum { LINE_WIDTH = 1 }; // handlers: small lines width
		enum { GRAB_RADIUS = 6 }; // handlers: circle radius
		enum { GRAB_BORDER = 2 }; // handlers: circle border width

		const ImGuiStyle& Style = GetStyle();
		const ImGuiIO& IO = GetIO();
		ImDrawList* DrawList = GetWindowDrawList();
		ImGuiWindow* Window = GetCurrentWindow();
		if (Window->SkipItems)
			return false;

		// header and spacing
		int changed = SliderFloat4(label, P, 0, 1, "%.3f", 1.0f);
		int hovered = IsItemActive() || IsItemHovered(); // IsItemDragged() ?
		Dummy(ImVec2(0, 3));

		// prepare canvas
		const float avail = GetContentRegionAvail().x;
		const float dim = ImMin(avail, 128.f);
		ImVec2 Canvas(dim, dim);

		ImRect bb(Window->DC.CursorPos, Window->DC.CursorPos + Canvas);
		ItemSize(bb);
		if (!ItemAdd(bb, NULL))
			return changed;

		const ImGuiID id = Window->GetID(label);
		// hovered |= 0 != IsItemHovered(ImRect(bb.Min, bb.Min + ImVec2(avail,dim)), id);

		RenderFrame(bb.Min, bb.Max, GetColorU32(ImGuiCol_FrameBg, 1), true, Style.FrameRounding);

		// background grid
		for (int i = 0; i <= Canvas.x; i += (Canvas.x * 0.25f)) {
			DrawList->AddLine(
				ImVec2(bb.Min.x + i, bb.Min.y),
				ImVec2(bb.Min.x + i, bb.Max.y),
				GetColorU32(ImGuiCol_TextDisabled));
		}
		for (int i = 0; i <= Canvas.y; i += (Canvas.y * 0.25f)) {
			DrawList->AddLine(
				ImVec2(bb.Min.x, bb.Min.y + i),
				ImVec2(bb.Max.x, bb.Min.y + i),
				GetColorU32(ImGuiCol_TextDisabled));
		}

		// eval curve
		ImVec2 Q[4] = { { 0, 0 },{ P[0], P[1] },{ P[2], P[3] },{ 1, 1 } };
		ImVec2 results[SMOOTHNESS + 1];
		bezier_table<SMOOTHNESS>(Q, results);

		// control points: 2 lines and 2 circles
		{
			char buf[128];
			sprintf(buf, "0##%s", label);

			// handle grabbers
			for (int i = 0; i < 2; ++i)
			{
				ImVec2 pos = ImVec2(P[i * 2 + 0], 1 - P[i * 2 + 1]) * (bb.Max - bb.Min) + bb.Min;
				SetCursorScreenPos(pos - ImVec2(GRAB_RADIUS, GRAB_RADIUS));
				InvisibleButton((buf[0]++, buf), ImVec2(2 * GRAB_RADIUS, 2 * GRAB_RADIUS));
				if (IsItemActive() || IsItemHovered())
				{
					SetTooltip("(%4.3f, %4.3f)", P[i * 2 + 0], P[i * 2 + 1]);
				}
				if (IsItemActive() && IsMouseDragging(0))
				{
					P[i * 2 + 0] += GetIO().MouseDelta.x / Canvas.x;
					P[i * 2 + 1] -= GetIO().MouseDelta.y / Canvas.y;
					changed = true;
				}
			}

			if (hovered || changed) DrawList->PushClipRectFullScreen();

			// draw curve
			{
				ImColor color(GetStyle().Colors[ImGuiCol_PlotLines]);
				for (int i = 0; i < SMOOTHNESS; ++i) {
					ImVec2 p = { results[i + 0].x, 1 - results[i + 0].y };
					ImVec2 q = { results[i + 1].x, 1 - results[i + 1].y };
					ImVec2 r(p.x * (bb.Max.x - bb.Min.x) + bb.Min.x, p.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
					ImVec2 s(q.x * (bb.Max.x - bb.Min.x) + bb.Min.x, q.y * (bb.Max.y - bb.Min.y) + bb.Min.y);
					DrawList->AddLine(r, s, color, CURVE_WIDTH);
				}
			}

			// draw lines and grabbers
			float luma = IsItemActive() || IsItemHovered() ? 0.5f : 1.0f;
			ImVec4 pink(1.00f, 0.00f, 0.75f, luma), cyan(0.00f, 0.75f, 1.00f, luma);
			ImVec4 white(GetStyle().Colors[ImGuiCol_Text]);
			ImVec2 p1 = ImVec2(P[0], 1 - P[1]) * (bb.Max - bb.Min) + bb.Min;
			ImVec2 p2 = ImVec2(P[2], 1 - P[3]) * (bb.Max - bb.Min) + bb.Min;
			DrawList->AddLine(ImVec2(bb.Min.x, bb.Max.y), p1, ImColor(white), LINE_WIDTH);
			DrawList->AddLine(ImVec2(bb.Max.x, bb.Min.y), p2, ImColor(white), LINE_WIDTH);
			DrawList->AddCircleFilled(p1, GRAB_RADIUS, ImColor(white));
			DrawList->AddCircleFilled(p1, GRAB_RADIUS - GRAB_BORDER, ImColor(pink));
			DrawList->AddCircleFilled(p2, GRAB_RADIUS, ImColor(white));
			DrawList->AddCircleFilled(p2, GRAB_RADIUS - GRAB_BORDER, ImColor(cyan));

			if (hovered || changed) DrawList->PopClipRect();

			// restore cursor pos
			SetCursorScreenPos(ImVec2(bb.Min.x, bb.Max.y + GRAB_RADIUS)); // :P
		}

		return changed;
	}

	void ShowBezierDemo()
	{
		{ static float v[] = { 0.000f, 0.000f, 1.000f, 1.000f }; Bezier("easeLinear", v); }
		{ static float v[] = { 0.470f, 0.000f, 0.745f, 0.715f }; Bezier("easeInSine", v); }
		{ static float v[] = { 0.390f, 0.575f, 0.565f, 1.000f }; Bezier("easeOutSine", v); }
		{ static float v[] = { 0.445f, 0.050f, 0.550f, 0.950f }; Bezier("easeInOutSine", v); }
		{ static float v[] = { 0.550f, 0.085f, 0.680f, 0.530f }; Bezier("easeInQuad", v); }
		{ static float v[] = { 0.250f, 0.460f, 0.450f, 0.940f }; Bezier("easeOutQuad", v); }
		{ static float v[] = { 0.455f, 0.030f, 0.515f, 0.955f }; Bezier("easeInOutQuad", v); }
		{ static float v[] = { 0.550f, 0.055f, 0.675f, 0.190f }; Bezier("easeInCubic", v); }
		{ static float v[] = { 0.215f, 0.610f, 0.355f, 1.000f }; Bezier("easeOutCubic", v); }
		{ static float v[] = { 0.645f, 0.045f, 0.355f, 1.000f }; Bezier("easeInOutCubic", v); }
		{ static float v[] = { 0.895f, 0.030f, 0.685f, 0.220f }; Bezier("easeInQuart", v); }
		{ static float v[] = { 0.165f, 0.840f, 0.440f, 1.000f }; Bezier("easeOutQuart", v); }
		{ static float v[] = { 0.770f, 0.000f, 0.175f, 1.000f }; Bezier("easeInOutQuart", v); }
		{ static float v[] = { 0.755f, 0.050f, 0.855f, 0.060f }; Bezier("easeInQuint", v); }
		{ static float v[] = { 0.230f, 1.000f, 0.320f, 1.000f }; Bezier("easeOutQuint", v); }
		{ static float v[] = { 0.860f, 0.000f, 0.070f, 1.000f }; Bezier("easeInOutQuint", v); }
		{ static float v[] = { 0.950f, 0.050f, 0.795f, 0.035f }; Bezier("easeInExpo", v); }
		{ static float v[] = { 0.190f, 1.000f, 0.220f, 1.000f }; Bezier("easeOutExpo", v); }
		{ static float v[] = { 1.000f, 0.000f, 0.000f, 1.000f }; Bezier("easeInOutExpo", v); }
		{ static float v[] = { 0.600f, 0.040f, 0.980f, 0.335f }; Bezier("easeInCirc", v); }
		{ static float v[] = { 0.075f, 0.820f, 0.165f, 1.000f }; Bezier("easeOutCirc", v); }
		{ static float v[] = { 0.785f, 0.135f, 0.150f, 0.860f }; Bezier("easeInOutCirc", v); }
		{ static float v[] = { 0.600f, -0.28f, 0.735f, 0.045f }; Bezier("easeInBack", v); }
		{ static float v[] = { 0.175f, 0.885f, 0.320f, 1.275f }; Bezier("easeOutBack", v); }
		{ static float v[] = { 0.680f, -0.55f, 0.265f, 1.550f }; Bezier("easeInOutBack", v); }
		// easeInElastic: not a bezier
		// easeOutElastic: not a bezier
		// easeInOutElastic: not a bezier
		// easeInBounce: not a bezier
		// easeOutBounce: not a bezier
		// easeInOutBounce: not a bezier
	}
}

bool ImGuiTxet(bool show_demo_window, bool show_another_window)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	bool SetMode = GetSetMode();				// �擾



	ImGui::Begin("CameraSystem");                          // Create a window called "Hello, world!" and append into it.


	ImGui::SliderInt("Number", &camera, 0, 10);

	ImGui::End();
	if (!SetMode)
	{
		// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		if (show_demo_window)
		{
			int MapSet = GetDebugNumber();
			CAMERA *pCamera = GetCamera()->Get();

			ImGui::Begin("MapMode");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("(%.1f PosV)(%.1f PosR)", pCamera->posV.x, pCamera->posR.x);
			ImGui::Text("Y debug |A ModeChange |LCONTROL PALLET | MOUSE_INPUT_LEFT Change");               // Display some text (you can use a format strings too
			ImGui::InputText("textbox 1", Txet, sizeof(Txet));
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderInt("Type", &MapSet, 0, MAX_MAP/2);

			//ImGui::SliderFloat("float", &f, 0.0f, 93.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


			ImGui::SameLine();
			setDebugNumber(MapSet);
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}
	}
	else
	{
		// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		if (show_demo_window)
		{
			int EnemySet = GetDebugNumberEnemy();

			ImGui::Text("");               // Display some text (you can use a format strings too

			ImGui::Begin("EnemyMode");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("Y debug |A ModeChange |LCONTROL PALLET | MOUSE_INPUT_LEFT Change");               // Display some text (you can use a format strings too            // Display some text (you can use a format strings too
			ImGui::Text("LCONTROL & Z Select|LCONTROL& D delete");               // Display some text (you can use a format strings too            // Display some text (you can use a format strings too
			ImGui::InputText("textbox 1", Txet, sizeof(Txet));

			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderInt("Type", &EnemySet, 0, 10);

			
			//EnemySet = Button(EnemySet);

			//ImGui::SliderFloat("float", &f, 0.0f, 93.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			ImGui::SameLine();
			setEnemyNumber(EnemySet);
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}	
	}
	return show_another_window;
}


int Button(int nSize)
{
	if (ImGui::Button("1++"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nSize++;
	}
	if (ImGui::Button("1--"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nSize--;
	}
	if (ImGui::Button("5++"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nSize += 5;
	}
	if (ImGui::Button("5--"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
	{
		nSize -= 5;
	}
	return nSize;
}
//=============================================================================
// GetCamera�֐�
//=============================================================================
CCamera *GetCamera()
{
	return pCamera;
}

//=============================================================================
// GetCameraSystem�֐�
//=============================================================================
int GetCameraSystem()
{
	return camera;
}