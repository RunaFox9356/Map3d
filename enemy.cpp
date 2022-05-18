//==================================================
//
// プレイヤー設定
// Author:hamada ryuuga
//
//==================================================
//--------------------------------------------------
// include
//--------------------------------------------------
#include <stdio.h>
#include <assert.h>
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "utility.h"
#include "motion.h"
#include "map.h"
#include "comn.h"

//--------------------------------------------------
// マクロ定義
#define MAX_ENEMY	(100)	// 最大エネミー数
#define SIZE_ENEMY (D3DXVECTOR3(50.0f,50.0f,0.0f))
//#define ATTENUATION	(0.5f)		//減衰係数
//#define SPEED	(1.0f)			//スピード
#define WIDTH (10.0f)			//モデルの半径
#define MAX_PRAYER (16)			//最大数
#define MAX_MOVE (9)			//アニメーションの最大数
#define INVINCIBLE (300)		//無敵時間

//--------------------------------------------------
// 静的変数
//--------------------------------------------------
static Enemy s_Enemy[MAX_ENEMY];		// エネミーの構造体
static Enemy s_EnemyType[ENEMY_TYPE_MAX];	// エネミー種別の構造体
static CPlayer::MODELDATAPLAYER s_ModelData[MAX_MOVE];

static float s_fMapScale;
static float s_fLog;
static D3DXVECTOR3 s_Move(5.0f, 0.0f, 0.0f);

static int s_parts;	// パーツの最大数
static int s_pow;	// ジャンプパワー
static int s_nMotion; //

//--------------------------------------------------
// プロトタイプ宣言
//--------------------------------------------------
static void Loadmotion(CPlayer::MODELDATAPLAYER* set, int Setnumber);	// モーションの読込
static void AnimationSet(int animation);						// アニメーションの計算
static void MoveSet(void);										// ムーブセット
static void Collision(void);									// 当たり判定まとめ
static void SetCopy(void);

//==================================================
// 初期化処理
//==================================================
void InitEnemy(void)
{
	// カメラのデータ取得
	Camera *pCamera = GetCamera();

	s_pow = 0;
	
	// 初期化処理
	memset(&s_Enemy, NULL, sizeof(s_Enemy));

	// 読込
	LoadEnemy();

	s_fLog = 0.0f;
	s_fMapScale = 1.0f;
}

//=========================================
// 終了処理
//=========================================
void UninitEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		for (int i = 0; i <= s_parts; i++)
		{
			// 頂点バッファーの解放
			if (pEnemy->parts[i].pBuffer != NULL)
			{
				pEnemy->parts[i].pBuffer->Release();
				pEnemy->parts[i].pBuffer = NULL;
			}

			if (pEnemy->parts[i].pMesh != NULL)
			{
				pEnemy->parts[i].pMesh->Release();
				pEnemy->parts[i].pMesh = NULL;
			}
		}
	}
}

//=========================================
// 更新処理
//=========================================
void UpdateEnemy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		// 現在のモーション番号の保管
		pEnemy->motionTypeOld = pEnemy->motionType;

		if (!pEnemy->bMotion)
		{// 使用してるモーションがない場合
			pEnemy->motionType = CPlayer::ANIME_NORMAL;
		}

		// マップのスクロール
		if (!GetKeyboardPress(DIK_LCONTROL) && !GetKeyboardPress(DIK_LSHIFT))
		{
			int wheel = GetMouseWheel();
			if (wheel > 0)
			{
				pEnemy->pos += s_Move * MAPMOVE / s_fMapScale;				
			}
			else if (wheel < 0)
			{
				pEnemy->pos -= s_Move * MAPMOVE / s_fMapScale;
			}
		}

		//Collision();	// 床

		// アニメーションや足音の設定
		if (!pEnemy->notLoop)
		{
			pEnemy->motionType = CPlayer::ANIME_NORMAL;
		}

		if (s_pow >= 1 && s_pow <= 10)
		{// ジャンプシステム
			s_pow++;
			pEnemy->move.y = 1.00f * s_pow;
		}

		pEnemy->move.y -= 1.0f;
		if (GetKeyboardPress(DIK_B))
		{
			pEnemy->motionType = CPlayer::ANIME_ATTACK;//攻撃

			pEnemy->notLoop = true;
		}
		if (GetKeyboardPress(DIK_N))
		{
			pEnemy->notLoop = true;
		}

		if (pEnemy->motionTypeOld != pEnemy->motionType)
		{// モーションタイプが変更された時
			pEnemy->motion[pEnemy->motionTypeOld].nCntFrame = 0;
			pEnemy->motion[pEnemy->motionTypeOld].nCntKeySet = 0;
			pEnemy->bMotionBlend = true;
		}

		if (pEnemy->bMotionBlend)
		{// モーションブレンドを使用してる
			pEnemy->bMotionBlend = MotionBlend((int)(pEnemy->motionType),	// モーションの配列番号
				&pEnemy->parts[0],											// パーツ情報
				pEnemy->nMaxModelParts,										// パーツ数
				&pEnemy->motion[0]);										// モーション情報	
		}
		else if (!pEnemy->bMotionBlend)
		{// モーションブレンドを使用してない場合
			pEnemy->bMotion = PlayMotion(pEnemy->nMaxModelParts,			// パーツ数
				&pEnemy->parts[0],											// パーツ情報
				&pEnemy->motion[(int)(pEnemy->motionType)]);				// モーション情報
		}
	}
	//移動記録を保存
	if (!GetKeyboardPress(DIK_LCONTROL) && !GetKeyboardPress(DIK_LSHIFT))
	{
		int wheel = GetMouseWheel();
		if (wheel > 0)
		{	
			s_fLog += s_Move.x * MAPMOVE / s_fMapScale;

		}
		else if (wheel < 0)
		{
			s_fLog -= s_Move.x * MAPMOVE / s_fMapScale;
		}
	}
	
}

//=========================================
// 描画処理
//=========================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScale, mtxTrans, mtxRot;	// 計算用マトリックス
	D3DMATERIAL9 marDef;
	D3DXMATERIAL *pMat = {};


	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		D3DXVECTOR3 scale(pEnemy->scale.x, pEnemy->scale.y, pEnemy->scale.z);

		if (!pEnemy->isUse)//使ってるやつ出す
		{
			continue;
		}

		// ワールドマトリックスの初期化
		// 行列初期化関数(第1引数の行列を単位行列に初期化)
		D3DXMatrixIdentity(&pEnemy->mtxWorld);

		// 拡縮を反映
		// 行列拡縮関数
		D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxScale);

		// 向きを反映
		// 行列回転関数(第1引数にヨー(y)ピッチ(x)ロール(z)方向の回転行列を作成)
		D3DXMatrixRotationYawPitchRoll(&mtxRot, pEnemy->rot.y, pEnemy->rot.x, pEnemy->rot.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxRot);

		// 位置を反映
		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixTranslation(&mtxTrans, pEnemy->pos.x, pEnemy->pos.y, pEnemy->pos.z);
		// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
		D3DXMatrixMultiply(&pEnemy->mtxWorld, &pEnemy->mtxWorld, &mtxTrans);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
		// パーツの描画設定
		SetParts(pEnemy->nMaxModelParts,	// パーツ数
			&pEnemy->parts[0],				// パーツ情報
			pEnemy->mtxWorld,				// ワールドマトリックス
			mtxRot,							// 計算用マトリックス
			mtxTrans,						// 計算用マトリックス
			&marDef,						// マテリアル保存変数
			pMat);							// マテリアルデータ

		//現在のマテリアルを元に戻す
		GetDevice()->SetMaterial(&marDef);
	}
}

//=========================================
// 設定
//=========================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->isUse)
		{
			continue;
		}

		LoadEnemy();

		*pEnemy = s_EnemyType[type];
		// プレイヤー情報の初期化
		pEnemy->pos = pos;									// 位置の初期化
		pEnemy->posOld = pEnemy->pos;						// 過去位置の初期化
		pEnemy->rot = rot;									// 向きの初期化
		pEnemy->fLog = s_fLog;
		pEnemy->mtxWorld = {};								// ワールドマトリックス
		pEnemy->motionType = CPlayer::ANIME_NORMAL;					// ニュートラルモーション
		pEnemy->motionTypeOld = pEnemy->motionType;			// 前回のモーション
		pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
		pEnemy->bMotionBlend = false;						// モーションブレンドの使用状況
		pEnemy->isUse = true;								// 使用状況

		break;
	}
}

//--------------------------
//当たり判定のサイズせってい
//--------------------------
void SizeSet(void)
{

}

//------------------------------
//アニメーションセット
//-------------------------------
void AnimationSet(int animation)
{

}

//------------------------------
//動きセット
//-------------------------------
void MoveSet(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}
	}
}

//-------------------------------
//当たり判定まとめ
//-------------------------------
void Collision(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		if (pEnemy->pos.y <= 0.0f)
		{
			pEnemy->pos.y = 0.0f;
		}
	}
}

//-------------------------------
//モーションをロードする処理
//-------------------------------
void  Loadmotion(CPlayer::MODELDATAPLAYER* set, int Setnumber)
{
	s_ModelData[s_nMotion] = *set;
	s_nMotion++;
}

//-------------------------------
//コピーを処理
//-------------------------------
void SetCopy(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		s_nMotion = 0;
		if (s_parts >= 8)
		{
			s_parts = 7;
		}

		switch (pEnemy->copy)
		{
		case CPlayer::COPY_SWORD:
			break;
		case CPlayer::COPY_FIRE:
			break;
		case CPlayer::COPY_LASER:
			break;
		case CPlayer::COPY_CUTTER:
			break;
		default:
			break;
		}
	}
}

//----------------------
// ゲット(構造体)
//----------------------
Enemy *GetEnemy(void)
{
	return s_Enemy;
}

//----------------------
// 読込
//----------------------
void LoadSetFile(char *Filename)
{
	FILE *pFile = fopen(Filename, "r");
	if (pFile == NULL)
	{//ファイルが開いた場合
		assert(false);
		return;
	}
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
	D3DXVECTOR3 size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// サイズ
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ロット
	char aString[128] = {};			// 文字列比較用の変数
	char aEqual[128] = {};		// ＝読み込み用変数
	int nType = 0;
	fscanf(pFile, "%s", &aString);

	while (strncmp(&aString[0], "SCRIPT", 6) != 0)
	{// 文字列が一致した場合
		aString[0] = {};
		// 文字列の読み込み
		fscanf(pFile, "%s", &aString[0]);
	}
	while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
	{
		fscanf(pFile, "%s", &aString[0]);

		if (strncmp(&aString[0], "#", 1) == 0)
		{// 一列読み込むコメント
			fgets(&aString[0], sizeof(aString), pFile);
		}
		if (strcmp(&aString[0], "SET_ENEMY") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aString[0]);

				if (strncmp(&aString[0], "#", 1) == 0)
				{// 一列読み込むコメント
					fgets(&aString[0], sizeof(aString), pFile);
				}
				if (strcmp(&aString[0], "POS") == 0)
				{// POSの読み込み
					fscanf(pFile, "%s", &aEqual[0]);

					fscanf(pFile, "%f", &pos.x);
					fscanf(pFile, "%f", &pos.y);
					fscanf(pFile, "%f", &pos.z);
				}
				if (strcmp(&aString[0], "SIZE") == 0)
				{// SIZEの読み込み
					fscanf(pFile, "%s", &aEqual[0]);

					fscanf(pFile, "%f", &size.x);
					fscanf(pFile, "%f", &size.y);
					fscanf(pFile, "%f", &size.z);
				}
				if (strcmp(&aString[0], "ROT") == 0)
				{// ROTの読み込み
					fscanf(pFile, "%s", &aEqual[0]);

					fscanf(pFile, "%f", &rot.x);
					fscanf(pFile, "%f", &rot.y);
					fscanf(pFile, "%f", &rot.z);
				}
				if (strcmp(&aString[0], "TYPE") == 0)
				{
					fscanf(pFile, "%s", &aEqual[0]);

					fscanf(pFile, "%d", &nType);
				}
				if (strcmp(&aString[0], "END_SET") == 0)
				{
					SetEnemy(pos, rot, (ENEMY_TYPE)nType);
					break;
				}
			}
		}
	}
}

//----------------------
// 読込
//----------------------
void LoadEnemy(void)
{
	FILE *pFile = fopen("Data/system/enemy/enemy.txt", "r");

	if (pFile == NULL)
	{//ファイルが開いた場合
		assert(false);
		return;
	}

	char aString[128] = {};			// 文字列比較用の変数
	char aEqual[128] = {};		// ＝読み込み用変数
	char fileName[ENEMY_TYPE_MAX][128];
	int fileCnt = 0;

	fscanf(pFile, "%s", &aString);

	while (strncmp(&aString[0], "SCRIPT", 6) != 0)
	{// 文字列が一致した場合
		aString[0] = {};
		// 文字列の読み込み
		fscanf(pFile, "%s", &aString[0]);
	}
	while (strncmp(&aString[0], "END_SCRIPT", 10) != 0)
	{
		fscanf(pFile, "%s", &aString[0]);

		if (strncmp(&aString[0], "#", 1) == 0)
		{// 一列読み込む
			fgets(&aString[0], sizeof(aString), pFile);
		}

		if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
		{// ファイル名の読み込み
			fscanf(pFile, "%s", &aEqual[0]);
			fscanf(pFile, "%s", fileName[fileCnt]);

			Enemy* pEnemy = &s_EnemyType[fileCnt];
			if (pEnemy->isUse)
			{
				continue;
			}

			// プレイヤー情報の初期化
			pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
			pEnemy->posOld = pEnemy->pos;					// 過去位置の初期化
			pEnemy->fLog = s_fLog;
			pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);;	// 向きの初期化
			pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// 頂点座標の最小値
			pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// 頂点座標の最大値
			pEnemy->mtxWorld = {};								// ワールドマトリックス
			pEnemy->motionType = CPlayer::ANIME_NORMAL;					// ニュートラルモーション
			pEnemy->motionTypeOld = pEnemy->motionType;			// ニュートラルモーション
			pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
			pEnemy->bMotionBlend = false;						// モーションブレンドの使用状況
			pEnemy->isUse = true;								// 使用状況
			pEnemy->bMotionBlend = false;						// プレイヤーがディスクを持っていない

			// ファイルの読み込み
			LoodSetMotion(fileName[fileCnt], pEnemy->partsFile, pEnemy->parts, pEnemy->motion, &pEnemy->nMaxModelParts);

			for (int i = 0; i < pEnemy->nMaxModelParts; i++)
			{
				Parts* pParts = &pEnemy->parts[i];

				// 位置と向きの初期値を保存
				pParts->posOrigin = pParts->pos;
				pParts->rotOrigin = pParts->rot;

				// パーツ情報の初期化
				pParts->mtxWorld = {};	// ワールドマトリックス
				pParts->vtxMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);	// 頂点座標の最小値
				pParts->vtxMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// 頂点座標の最大値

				// Xファイルの読み込み
				D3DXLoadMeshFromX(pEnemy->partsFile[pParts->nType].aName,
					D3DXMESH_SYSTEMMEM,
					GetDevice(),
					NULL,
					&pParts->pBuffer,
					NULL,
					&pParts->nNumMat,
					&pParts->pMesh);

				// 頂点座標の最小値・最大値の算出
				int nNumVtx;	// 頂点数
				DWORD sizeFVF;	// 頂点フォーマットのサイズ
				BYTE *pVtxBuff;	// 頂点バッファへのポインタ

				// 頂点数の取得
				nNumVtx = pParts->pMesh->GetNumVertices();

				// 頂点フォーマットのサイズの取得
				sizeFVF = D3DXGetFVFVertexSize(pParts->pMesh->GetFVF());

				// 頂点バッファのロック
				pParts->pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{
					// 頂点座標の代入
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

					if (vtx.x < pParts->vtxMin.x)
					{// 比較対象が現在の頂点座標(X)の最小値より小さい
						pParts->vtxMin.x = vtx.x;
					}
					if (vtx.y < pParts->vtxMin.y)
					{// 比較対象が現在の頂点座標(Y)の最小値より小さい
						pParts->vtxMin.y = vtx.y;
					}
					if (vtx.z < pParts->vtxMin.z)
					{// 比較対象が現在の頂点座標(Z)の最小値より小さい
						pParts->vtxMin.z = vtx.z;
					}

					if (vtx.x > pParts->vtxMax.x)
					{// 比較対象が現在の頂点座標(X)の最大値より大きい
						pParts->vtxMax.x = vtx.x;
					}
					if (vtx.y > pParts->vtxMax.y)
					{// 比較対象が現在の頂点座標(Y)の最大値より大きい
						pParts->vtxMax.y = vtx.y;
					}
					if (vtx.z > pParts->vtxMax.z)
					{// 比較対象が現在の頂点座標(Z)の最大値より大きい
						pParts->vtxMax.z = vtx.z;
					}

					// 頂点フォーマットのサイズ分ポインタを進める
					pVtxBuff += sizeFVF;
				}

				if (pParts->vtxMin.x < pEnemy->modelMin.x)
				{// 比較対象が現在の頂点座標(X)の最小値より小さい
					pEnemy->modelMin.x = pParts->vtxMin.x;
				}
				if (pParts->vtxMin.y < pEnemy->modelMin.y)
				{// 比較対象が現在の頂点座標(Y)の最小値より小さい
					pEnemy->modelMin.y = pParts->vtxMin.y;
				}
				if (pParts->vtxMin.z < pEnemy->modelMin.z)
				{// 比較対象が現在の頂点座標(Z)の最小値より小さい
					pEnemy->modelMin.z = pParts->vtxMin.z;
				}

				if (pParts->vtxMax.x > pEnemy->modelMax.x)
				{// 比較対象が現在の頂点座標(X)の最大値より大きい
					pEnemy->modelMax.x = pParts->vtxMax.x;
				}
				if (pParts->vtxMax.y > pEnemy->modelMax.y)
				{// 比較対象が現在の頂点座標(Y)の最大値より大きい
					pEnemy->modelMax.y = pParts->vtxMax.y;
				}
				if (pParts->vtxMax.z > pEnemy->modelMax.z)
				{// 比較対象が現在の頂点座標(Z)の最大値より大きい
					pEnemy->modelMax.z = pParts->vtxMax.z;
				}

				// 頂点バッファのアンロック
				pParts->pMesh->UnlockVertexBuffer();
			}
			fileCnt++;
		}
	}
}
//----------------------------
//ファイルの入力マップ情報
//----------------------------
void OutputEnemy(char *Filename)
{

	D3DXVECTOR3 pos;
	//ファイル開け
	FILE *pFile = fopen(Filename, "w");
	if (pFile == NULL)
	{
		assert(false);
	}
	fprintf(pFile, "#-----------------------\n");
	fprintf(pFile, "#エネミーの設定スクリプト\n");
	fprintf(pFile, "#Author:hamada ryuuga\n");
	fprintf(pFile, "#-----------------------\n");

	fprintf(pFile, "SCRIPT #ここ消さない\n\n");

	fprintf(pFile, "\n# ここから敵の座標などの設定\n\n");

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		
		if (s_Enemy[nCntEnemy].isUse)
		{
			//s_Enemy[nCntEnemy].pos.y += Log;
			fprintf(pFile, "SET_ENEMY\n");
			fprintf(pFile, "TYPE = %d\n", s_Enemy[nCntEnemy].type);
			fprintf(pFile, "POS = %.4f %.4f %.4f\n", s_Enemy[nCntEnemy].pos.x - s_fLog, s_Enemy[nCntEnemy].pos.y, s_Enemy[nCntEnemy].pos.z);
			fprintf(pFile, "SIZE = %.4f %.4f %.4f\n", s_Enemy[nCntEnemy].scale.x, s_Enemy[nCntEnemy].scale.y, s_Enemy[nCntEnemy].scale.z);
			fprintf(pFile, "ROT = %.4f %.4f %.4f\n", s_Enemy[nCntEnemy].rot.x, s_Enemy[nCntEnemy].rot.y, s_Enemy[nCntEnemy].rot.z);
			fprintf(pFile, "END_SET\n");
			fprintf(pFile, "\n");
		}
	}

	fprintf(pFile, "END_SCRIPT #ここ消さない");
	fclose(pFile);
}
//----------------------------
//エネミ-のサイズと座標とタイプを変更する
//----------------------------
void SelectDes(D3DXVECTOR3 pos)
{
	Camera *pCamera = GetCamera();
	pos = WorldCastScreen(&pos,								// スクリーン座標
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// スクリーンサイズ
		&pCamera->mtxView,										// ビューマトリックス
		&pCamera->mtxProjection);								// プロジェクションマトリックス

	D3DXVECTOR3 enemyPos;
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (s_Enemy[nCntEnemy].isUse)
		{
			if (s_Enemy[nCntEnemy].bSelect)
			{
				s_Enemy[nCntEnemy].pos = D3DXVECTOR3(pos.x, pos.y, 0.0f);
			}
			
			D3DXVECTOR3 Size = s_Enemy[nCntEnemy].modelMax - s_Enemy[nCntEnemy].modelMin;

			if (((s_Enemy[nCntEnemy].pos.x - Size.x* s_fMapScale < pos.x) && (s_Enemy[nCntEnemy].pos.x + Size.x* s_fMapScale > pos.x)) &&
				((s_Enemy[nCntEnemy].pos.y - Size.y*2* s_fMapScale < pos.y) && (s_Enemy[nCntEnemy].pos.y + Size.y * 2 * s_fMapScale > pos.y)))
			{
				//マウスカーソル合わさってたらサイズ変更がかかる
				s_Enemy[nCntEnemy].scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
				if (GetKeyboardPress(DIK_LCONTROL))
				{//CONTROL押しながら
					
				}
				if (GetKeyboardTrigger(DIK_Z))
				{
					s_Enemy[nCntEnemy].bSelect = !s_Enemy[nCntEnemy].bSelect;
				}
				if (GetKeyboardTrigger(DIK_D))
				{
					s_Enemy[nCntEnemy].isUse = false;
					s_Enemy[nCntEnemy].pos = D3DXVECTOR3(NULL, NULL, NULL);
				}
			}
			else
			{
				s_Enemy[nCntEnemy].scale = D3DXVECTOR3(1.8f, 1.8f, 1.8f);
			}

		}
	}
}
