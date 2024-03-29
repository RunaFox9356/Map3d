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
#include "range.h"

#include "file.h"
#include "letter.h"


//Particle DataEffect;
namespace nl = nlohmann;

static nl::json j;//リストの生成

CEnemy::CEnemy(void)
{
	

}

CEnemy::~CEnemy(void)
{
}

//==================================================
// 初期化処理
//==================================================
void CEnemy::Init(void)
{
	// カメラのデータ取得
	CAMERA *pCamera = GetCamera()->Get();

	s_pow = 0;
	
	// 初期化処理
	memset(&s_Enemy, NULL, sizeof(s_Enemy));

	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		pEnemy->rot.y = D3DX_PI*0.5f;

	}
	s_fLog = 0.0f;
	s_fMapScale = 1.0f;
}

//=========================================
// 終了処理
//=========================================
void  CEnemy::Uninit(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		for (int j = 0; j < pEnemy->nParts; j++)
		{
			// 頂点バッファーの解放
			if (pEnemy->parts[j].pBuffer != NULL)
			{
				pEnemy->parts[j].pBuffer->Release();
				pEnemy->parts[j].pBuffer = NULL;
			}

			if (pEnemy->parts[j].pMesh != NULL)
			{
			}
 		}
	}
}

//=========================================
// 更新処理
//=========================================
void  CEnemy::Update(void)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{
		Enemy* pEnemy = &s_Enemy[i];

		if (!pEnemy->isUse)
		{
			continue;
		}

		D3DXVECTOR3 Mouse = GetMouse();
		if (s_Enemy[i].bRange)
		{
			//pEnemy->scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);

			s_Enemy[i].pos = s_Enemy[i].posOld + Mouse;

		}
		else
		{
			s_Enemy[i].posOld = s_Enemy[i].pos - Mouse;
		//	pEnemy->scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			//s_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}


		if (GetKeyboardTrigger(DIK_V))
		{//Vおすと場所が変わる
			s_Enemy[i].bRange = false;
			s_Enemy[i].fLog = s_fLog;
		}


		bool Hit = CollisionRange(s_Enemy[i].pos);
		if (Hit)
		{
			if (GetKeyboardTrigger(DIK_V))
			{
				s_Enemy[i].bRange = true;

			}
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
				//pEnemy->pos += s_Move * MAPMOVE / s_fMapScale;				
			}
			else if (wheel < 0)
			{
				//pEnemy->pos -= s_Move * MAPMOVE / s_fMapScale;
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
			//s_fLog += s_Move.x * MAPMOVE / s_fMapScale;

		}
		else if (wheel < 0)
		{
			//s_fLog -= s_Move.x * MAPMOVE / s_fMapScale;
		}
	}
	
}

//=========================================
// 描画処理
//=========================================
void  CEnemy::Draw(void)
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
void  CEnemy::SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type)
{
	for (int i = 0; i < MAX_ENEMY; i++)
	{

		//s_Enemy[i] = s_EnemyType[type];
		Enemy* pEnemy = &s_Enemy[i];

		if (pEnemy->isUse)
		{
			continue;
		}


		*pEnemy = s_EnemyType[type];
		LoadEnemy(pEnemy);
		// プレイヤー情報の初期化
		pEnemy->pos = pos;									// 位置の初期化
		pEnemy->posOld = pEnemy->pos;						// 過去位置の初期化
		//pEnemy->rot = rot;									// 向きの初期化
		pEnemy->fLog = s_fLog;
		pEnemy->mtxWorld = {};								// ワールドマトリックス
		pEnemy->motionType = CPlayer::ANIME_NORMAL;					// ニュートラルモーション
		pEnemy->motionTypeOld = pEnemy->motionType;			// 前回のモーション
		pEnemy->move = D3DXVECTOR3(GetMove().x, GetMove().y,GetMove().z);		// 移動量
		pEnemy->bMotionBlend = false;						// モーションブレンドの使用状況
		pEnemy->nLife = GetLife();
		//pEnemy->isUse = true;								// 使用状況
		pEnemy->type = type;
		break;
	}
}

//--------------------------
//当たり判定のサイズせってい
//--------------------------
void  CEnemy::SizeSet(void)
{

}

//------------------------------
//アニメーションセット
//-------------------------------
void  CEnemy::AnimationSet(int animation)
{

}

//------------------------------
//動きセット
//-------------------------------
void  CEnemy::MoveSet(void)
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
void  CEnemy::Collision(void)
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
void   CEnemy::Loadmotion(CPlayer::MODELDATAPLAYER* set, int Setnumber)
{
	s_ModelData[s_nMotion] = *set;
	s_nMotion++;
}

//----------------------
// ゲット(構造体)
//----------------------
Enemy * CEnemy::GetEnemy(void)
{
	return s_Enemy;
}

//----------------------
// 読込
//----------------------
void  CEnemy::LoadSetFile(char *Filename)
{

}

//----------------------
// 読込
//----------------------
void  CEnemy::LoadEnemy(Enemy * pEnemy)
{
	FILE *pFile = fopen("Data/system/enemy/enemy.txt", "r");

	if (pFile == NULL)
	{//ファイルが開いた場合
		//assert(false);
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

			pEnemy = &s_EnemyType[fileCnt];
			
			if (pEnemy->isUse)
			{
				continue;
			}

			// プレイヤー情報の初期化
			pEnemy->scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
			pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置の初期化
			pEnemy->posOld = pEnemy->pos;					// 過去位置の初期化
			pEnemy->fLog = s_fLog;
			pEnemy->rot = D3DXVECTOR3(0.0f, D3DX_PI*0.5f, 0.0f);;	// 向きの初期化
			pEnemy->modelMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);		// 頂点座標の最小値
			pEnemy->modelMax = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);	// 頂点座標の最大値
			pEnemy->mtxWorld = {};								// ワールドマトリックス
			pEnemy->motionType = CPlayer::ANIME_NORMAL;					// ニュートラルモーション
			pEnemy->motionTypeOld = pEnemy->motionType;			// ニュートラルモーション
			pEnemy->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 移動量
			pEnemy->bMotionBlend = false;						// モーションブレンドの使用状況
			pEnemy->isUse = true;								// 使用状況
			pEnemy->bMotionBlend = false;						// プレイヤーがディスクを持っていない
			pEnemy->nLife = GetLife();
	
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
				pEnemy->nParts++;
			}
			fileCnt++;

		}
	}
	fclose(pFile);
}


//----------------------------
//ファイルの入力マップ情報
//----------------------------
void  CEnemy::OutputEnemy(char *Filename)
{

	int nIndex = 0;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		
		if (s_Enemy[nCntEnemy].isUse)
		{
			std::string name = "ENEMY";
			std::string Number = std::to_string(nIndex);
			name += Number;
			j[name] = {
			{"POS",{
				{ "X", s_Enemy[nCntEnemy].pos.x - s_fLog } ,
				{ "Y", s_Enemy[nCntEnemy].pos.y } ,
				{ "Z", s_Enemy[nCntEnemy].pos.z }}},
			{"SIZE",{
				{ "X", s_Enemy[nCntEnemy].scale.x } ,
				{ "Y", s_Enemy[nCntEnemy].scale.y } ,
				{ "Z", s_Enemy[nCntEnemy].scale.z }}},
			{"ROT", {
				{ "X", s_Enemy[nCntEnemy].rot.x } ,
				{ "Y", s_Enemy[nCntEnemy].rot.y } ,
				{ "Z", s_Enemy[nCntEnemy].rot.z }}} ,
			{ "MOVE",{
				{ "X", s_Enemy[nCntEnemy].rot.x } ,
				{ "Y", s_Enemy[nCntEnemy].rot.y } ,
				{ "Z", s_Enemy[nCntEnemy].rot.z } } } ,
			{"TYPE",  (int)s_Enemy[nCntEnemy].type}, 
			{"LIFE",  s_Enemy[nCntEnemy].nLife }};
			nIndex++;
			
		}
	}
	j["INDEX"] = nIndex;

	auto jobj = j.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = Filename;
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();


}
//----------------------------
//エネミ-のサイズと座標とタイプを変更する
//----------------------------
void  CEnemy::SelectDes(D3DXVECTOR3 pos)
{
	CAMERA *pCamera = GetCamera()->Get();
	pos = WorldCastScreen(&pos,								// スクリーン座標
		D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),			// スクリーンサイズ
		&pCamera->MtxView,										// ビューマトリックス
		&pCamera->MtxProje);								// プロジェクションマトリックス

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
				//s_Enemy[nCntEnemy].scale = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
				if (GetKeyboardPress(DIK_LCONTROL))
				{//CONTROL押しながら
					int wheel = GetMouseWheel();
					if (wheel > 0 && !s_Enemy[nCntEnemy].bSelect)
					{//マウスホイール回すとサイズ変更
						s_Enemy[nCntEnemy].scale += D3DXVECTOR3(0.1f, 0.1f, 0.1f);
					}
					else if (wheel < 0 && !s_Enemy[nCntEnemy].bSelect)
					{
						s_Enemy[nCntEnemy].scale -= D3DXVECTOR3(0.1f, 0.1f, 0.1f);
					}
					
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
				//s_Enemy[nCntEnemy].scale = D3DXVECTOR3(1.8f, 1.8f, 1.8f);
			}

		}
	}
}
