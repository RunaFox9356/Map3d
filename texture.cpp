//**************************************************
// 
// texture.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "manager.h"
#include "texture.h"

#include <assert.h>

//==================================================
// 定義
//==================================================
const char* CTexture::s_FileName[] =
{// テクスチャのパス
	"data/TEXTURE/wa.jpg",			//玉画像
	"data/TEXTURE/gon.png",			//GON
	"data/TEXTURE/fire.png",		//火
	"data/TEXTURE/ice.png",			//氷
	"data/TEXTURE/storm.png",		//風
	"data/TEXTURE/sun.png",         //雷
	"data/TEXTURE/daia.png",		//CRYSTAL
	"data/TEXTURE/Score001.png",	//score
	"data/TEXTURE/impact.png",		//火
	"data/TEXTURE/Iceburst.png",	//氷	
	"data/TEXTURE/tempest.png",		//風
	"data/TEXTURE/rain.png",		//雷
	"data/TEXTURE/smoke.jpg",		//煙
	"data/TEXTURE/light.png",		//光
	"data/TEXTURE/Box.png",			//BOX背景
	"data/TEXTURE/HIT.png",			//光（クリスタルのHIT）
	"data/TEXTURE/MagicCircle1.png",//魔法陣
	"data/TEXTURE/MagicCircle3.png",
	"data/TEXTURE/MagicCircle4.png",
	"data/TEXTURE/MagicCircle5.png",
	"data/TEXTURE/MagicCircle2.png",
	"Data/TEXTURE/star.png",		//背景１
	"Data/TEXTURE/town.png",		//背景２
	"Data/TEXTURE/moon.png",		//背景３
	"Data/TEXTURE/BG.png",			//ここからTITLE
	"Data/TEXTURE/GonBG.png",
	"Data/TEXTURE/Fox.png",
	"Data/TEXTURE/gamestart.png",
	"Data/TEXTURE/Getrotation.png",	//ここからリザルト
	"Data/TEXTURE/siro.jpg",
	"Data/TEXTURE/wallpaper_00405.jpg",
	"Data/TEXTURE/ranking_rank.png",
	"Data/TEXTURE/alphabet_ucase.jpg"
};

static_assert(sizeof(CTexture::s_FileName) / sizeof(CTexture::s_FileName[0]) == CTexture::TEXTURE_MAX, "aho");

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CTexture::CTexture() :
	s_pTexture()
{
	memset(s_pTexture, 0, sizeof(s_pTexture));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CTexture::~CTexture()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CTexture::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != nullptr)
		{// テクスチャの読み込みがされている
			continue;
		}

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			s_FileName[i],
			&s_pTexture[i]);
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CTexture::Load(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		s_FileName[inTexture],
		&s_pTexture[inTexture]);
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CTexture::ReleaseAll(void)
{
	for (int i = 0; i < TEXTURE_MAX; ++i)
	{
		if (s_pTexture[i] != NULL)
		{// テクスチャの解放
			s_pTexture[i]->Release();
			s_pTexture[i] = NULL;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CTexture::Release(TEXTURE inTexture)
{
	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	if (s_pTexture[inTexture] != NULL)
	{// テクスチャの解放
		s_pTexture[inTexture]->Release();
		s_pTexture[inTexture] = NULL;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
LPDIRECT3DTEXTURE9 CTexture::GetTexture(TEXTURE inTexture)
{
	if (inTexture == TEXTURE_NONE)
	{// テクスチャを使用しない
		return nullptr;
	}

	assert(inTexture >= 0 && inTexture < TEXTURE_MAX);

	// 読み込み
	Load(inTexture);

	return s_pTexture[inTexture];
}
