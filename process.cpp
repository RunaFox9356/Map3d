//=========================================
// 
// 処理全体
// Author YudaKaito
// 
//=========================================
//=========================================
// include
//=========================================
#include "process.h"
#include "bg.h"
#include "enemy.h"
#include "map.h"
#include "Pallet.h"
#include "Select.h"
#include "PalletEnemy.h"
#include "camera.h" 
#include "player.h"
#include "light.h"
#include "comn.h"
#include "input.h"
#include "range.h"
//=========================================
// 静的変数
//=========================================
static int s_DebugNumberEnemy;
static int DebugNumber;
static bool EnemyAlignment = false;				// マップのマス目基準にするかしないか
static float Size = 1.0f;
static bool EnemyMode = false;					// MapいじるかEnemyいじるか
static bool s_bDebug;							// デバッグ中か否か
static CPlayer player;
static CEnemy enemy;
//=========================================
// プロトタイプ宣言
//=========================================
static void EnemySetSystem(D3DXVECTOR3 Mouse);	// Enemyの挙動
static void MapSetSystem(D3DXVECTOR3 Mouse);	// マップの挙動

//-----------------------------------------
// 初期化
//-----------------------------------------
void InitProcess()
{
	InitLight();

	InitBG();
	enemy.Init();
	InitMap();
	InitPallet();
	InitPalletE();
	InitSelect();
	InitRange();
	player.Init();
}

//-----------------------------------------
// 終了
//-----------------------------------------
void UninitProcess()
{
	
	UninitLight();
	UninitBG();
	enemy.Uninit();
	UninitMap();
	UninitPallet();
	player.Uninit();
	UninitPalletE();
	UninitSelect();
	UninitRange();
}

//-----------------------------------------
// 更新
//-----------------------------------------
void UpdateProcess()
{

	UpdateBG();
	enemy.Update();
	player.Update();
	UpdateMap();
	UpdateRange();
	UpdateSelect(DebugNumber, s_DebugNumberEnemy);

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_U))
	{//敵の座標をロードする（モデルビュアー式）
		enemy.LoadSetFile("Data\\hoge2.txt");
	}

	if (s_bDebug)
	{
		//------------------------
		//マップチップdebugオフ
		//------------------------
		if (GetKeyboardTrigger(DIK_Y))
		{
			s_bDebug = false;
		}

		D3DXVECTOR3 Mouse = GetMouse();

		if (EnemyMode)
		{//Enemy設定
			UpdatePalletE();
			EnemySetSystem(Mouse);

		}
		else
		{//MAP設定
			UpdatePallet();
			MapSetSystem(Mouse);
		}
	}
	else
	{
		if (GetKeyboardTrigger(DIK_Y))
		{//マップチップdebugオフ
			s_bDebug = true;
			ConteSet(0);
		}
	}
	//------------------------
	//モード変更
	//------------------------
	if (GetKeyboardTrigger(DIK_A))
	{
		EnemyMode = !EnemyMode;
	}
#endif // DEBUG
}

//-----------------------------------------
// 描画
//-----------------------------------------
void DrawProcess()
{

	DrawBG();
	DrawMap();
	DrawPallet();
	DrawSelect();
	DrawPallet();
	DrawPalletE();
	DrawRange();
	// 2Dの前に3Dを置く
	GetDevice()->Clear(0, NULL, (D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
	enemy.Draw();
	player.Draw();
}

//------------------------
//敵デバック時の設定
//------------------------
void EnemySetSystem(D3DXVECTOR3 Mouse)
{

	//------------------------
	//マップのマス目基準にするかしないか
	//------------------------
	if (GetKeyboardTrigger(DIK_S))
	{//
		EnemyAlignment = !EnemyAlignment;
	}

	//------------------------
	//スポイト
	//------------------------
	if (GetMouseTrigger(MOUSE_INPUT_RIGHT))
	{//マウスポインターの位置
		s_DebugNumberEnemy++;
		s_DebugNumberEnemy %= (int)2;
		if (GetKeyboardPress(DIK_LCONTROL))
		{
			s_DebugNumberEnemy = CollisionPalletE(Mouse);
		}
	}

	//------------------------
	//エネミのセット
	//------------------------
	if (GetMouseTrigger(MOUSE_INPUT_LEFT)&& !GetKeyboardPress(DIK_LSHIFT))
	{//
		bool Hit = CollisionMap(Mouse);
		if (Hit)
		{
			if (EnemyAlignment)
			{
				D3DXVECTOR3 BLOCK = EnemyMap(Mouse*Size);
				CAMERA *pCamera = GetCamera()->Get();
				BLOCK = WorldCastScreen(&BLOCK,								// スクリーン座標
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// スクリーンサイズ
					&pCamera->MtxView,										// ビューマトリックス
					&pCamera->MtxProje);

				enemy.SetEnemy(D3DXVECTOR3(BLOCK.x, BLOCK.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (ENEMY_TYPE)s_DebugNumberEnemy);
			}
			else
			{
				CAMERA *pCamera = GetCamera()->Get();
				Mouse = WorldCastScreen(&Mouse,								// スクリーン座標
					D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// スクリーンサイズ
					&pCamera->MtxView,										// ビューマトリックス
					&pCamera->MtxProje);								// プロジェクションマトリックス

				enemy.SetEnemy(D3DXVECTOR3(Mouse.x, Mouse.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), (ENEMY_TYPE)s_DebugNumberEnemy);
			}
		}
	}
	enemy.SelectDes(Mouse);
}

//------------------------
//マップデバック時の設定
//------------------------
void MapSetSystem(D3DXVECTOR3 Mouse)
{
	if (GetMousePress(MOUSE_INPUT_LEFT))
	{//マウスポインターの位置
		ConversionMap(Mouse, DebugNumber);
	}

	if (GetMouseTrigger(MOUSE_INPUT_RIGHT))
	{//マウスポインターの位置
		DebugNumber++;
		DebugNumber %= X_MAP * (Y_MAP - 4) + 1;
		if (GetKeyboardPress(DIK_LCONTROL))
		{
			DebugNumber = CollisionPallet(Mouse);
		}

	}
	if (GetKeyboardPress(DIK_LCONTROL))
	{
		PalletMoveMap(true);
	}
	else
	{
		PalletMoveMap(false);
	}
}

//---------------------------------------
// 現在の設置可能モードの取得
//---------------------------------------
bool GetSetMode()
{
	return EnemyMode;
}

//---------------------------------------
// 現在の設置可能モードの設定
//---------------------------------------
void ChangeSetMode(bool hasChange)
{
	EnemyMode = hasChange;
}

//---------------------------------------
// 後変更
//---------------------------------------
int GetDebugNumber()
{
	return DebugNumber;
}

//---------------------------------------
// 後変更
//---------------------------------------
void setDebugNumber(int inDebugNumber)
{
	DebugNumber = inDebugNumber;
}

//---------------------------------------
// 後変更
//---------------------------------------
void setEnemyNumber(int inDebugNumber)
{
	s_DebugNumberEnemy = inDebugNumber;
}
//---------------------------------------
// デバックしてるかしないか
//---------------------------------------
bool IsDebug(void)//取得
{
	return s_bDebug;
}

//---------------------------------------
// デバイス状態の切り替え
//---------------------------------------
void ChangeDebug(bool inDebug)
{
	s_bDebug = inDebug;
}

//---------------------------------------
// マス目基準か否か
//---------------------------------------
bool GetEnemyAlignment()
{
	return EnemyAlignment;
}

int GetDebugNumberEnemy()
{
	return s_DebugNumberEnemy;
}

CEnemy *GetEnemy()
{
	return &enemy;
}