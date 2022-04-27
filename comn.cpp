//============================
//
// 計算系統
// Author:hamada ryuuga
//
//============================
#include "comn.h"

//イージングカーブ
float SeteaseIn(float x)
{
	return x * x;
}

float easeInOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

return x < 0.5f
	? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
	: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float easeOutCirc(float x) 
{
	return sqrt(1 - pow(x - 1, 2));
}

float easeOutElastic(float x)
{
	const float c4 = (2 *D3DX_PI) / 3;

return x == 0
	? 0
	: x == 1
	? 1
	: pow(2, -10 * x) * sin((x * 10 - 0.75) * c4) + 1;
}
// 2Dベクトルの外積
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->z - v1->z * v2->x;
}

// 2Dベクトルの外積
float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->x + v1->z * v2->z;
}

//---------------------------------------------------------------------------
// ワールド座標へのキャスト処理
//---------------------------------------------------------------------------
D3DXVECTOR3	WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXVECTOR3 FormerRot)
{
	// 変数宣言
	D3DXMATRIX		mtxWorldVtx;				// ワールドマトリックス
	D3DXMATRIX		mtxRot, mtxTrans;			// 計算用マトリックス

	// ワールドマトリックスの初期化
	// 行列初期化関数(第一引数の[行列]を[単位行列]に初期化)
	D3DXMatrixIdentity(&mtxWorldVtx);

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, vtx.x, vtx.y, vtx.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// 行列掛け算関数

	// 向きの反映
	// 行列回転関数 (第一引数に[ヨー(y)ピッチ(x)ロール(z)]方向の回転行列を作成)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, FormerRot.y, FormerRot.x, FormerRot.z);

	// 行列掛け算関数 (第二引数 * 第三引数を第一引数に格納)
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxRot);

	// 位置を反映
	// 行列移動関数 (第一引数にX,Y,Z方向の移動行列を作成)
	D3DXMatrixTranslation(&mtxTrans, FormerPos.x, FormerPos.y, FormerPos.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// 行列掛け算関数

	return D3DXVECTOR3(mtxWorldVtx._41, mtxWorldVtx._42, mtxWorldVtx._43);
}


//if (s_Enemy[nCnt].bUse)
//{
///*	float vecA = ;
//	float vecB;*/

//	if (pPos->y + Siz.y / 2 > s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMin.y,
//		pPos->y - Siz.y / 2 < s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y)
//	{
//		//左右の壁
//		if (pPosOld->z + Siz.z / 2 > s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z
//			&& pPosOld->z - Siz.z / 2 < s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z)
//		{
//			if (pPos->x + Siz.x / 2 > s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x
//				&& pPosOld->x + Siz.x / 2 <= s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x)
//			{//ブロックの座標と座標が重なり合ったら//通常モード//左
//				pPos->x = s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x - Siz.x / 2.0f;
//			}
//			if (pPos->x - Siz.x / 2 < s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x
//				&& pPosOld->x - Siz.x / 2 >= s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x)
//			{//ブロックの座標と座標が重なり合ったら//通常モード//右
//				pPos->x = s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x + Siz.x / 2.0f + 0.01f;
//			}
//		}
//		//上下の壁
//		if (pPosOld->x + Siz.x / 4 > s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x
//			&& pPosOld->x - Siz.x /4 < s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x)
//		{
//			if (pPos->z + Siz.z / 2 > s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z
//				&& pPosOld->z + Siz.z / 2 <= s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z)
//			{//ブロックの座標と座標が重なり合ったら//通常モード//下
//				pPos->z = s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z - Siz.z / 2.0f;
//			}
//			if (pPos->z - Siz.z / 2 < s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z
//				&& pPosOld->z - Siz.z / 2 >= s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z)
//			{//ブロックの座標と座標が重なり合ったら//通常モード//上
//				pPos->z = s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z + Siz.z / 2.0f + 0.01f;
//			}

//		}
//	}

//	//左右の壁
//	if (pPosOld->z + Siz.z / 2 > s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z
//		&& pPosOld->z - Siz.z / 2 < s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z)
//	{
//		//上下の壁
//		if (pPosOld->x + Siz.x / 2 > s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x
//			&& pPosOld->x - Siz.x / 2 < s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x)
//		{
//			if (pPos->y < s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y
//				&& pPosOld->y >= s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y)
//			{//ブロックの座標と座標が重なり合ったら//通常モード//屋根
//				pPos->y = s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y;
//				bIsLanding = true;
//			}
//		}
//	}

//}