//=============================================================================
//
// OBJECT3D
// Author : 浜田琉雅
//
//=============================================================================


#ifndef _OBJECT3D_H_			// このマクロ定義がされてなかったら
#define _OBJECT3D_H_			// 二重インクルード防止のマクロ定義

#include "renderer.h"
#include "object.h"
#include "main.h"


class  CMotion;

class CObject3d : public CObject
{
private:
	//polygonの拡大サイズ
	static const D3DXVECTOR3 m_Vtx[4];
	static int m_drop;
	

public:

	static const int INVINCIBLE = 300;


	enum ANIME
	{
		ANIME_NORMAL = 0,	// ニュートラル
		ANIME_RUN,			// 歩き
		ANIME_ATTACK,		// 攻撃
		ANIME_JUMP,			// ジャンプ
		ANIME_LANDING,		// 着地
		ANIME_MAX
	};

	enum DAMEGE
	{
		DAMEGE_NORMAL = 0,	// ニュートラル
		DAMEGE_DAMAGE,			// ダメージくらってる
		DAMEGE_MAX
	};

	CObject3d();
	~CObject3d() override;
	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	virtual void SetPos(const D3DXVECTOR3 &pos) ;
	virtual void SetMove(const D3DXVECTOR3 &move);
	virtual const D3DXVECTOR3 *GetPos() const ;
	void Set(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, char *filename);
	const D3DXVECTOR3 *GetRot() const;
	void SetRot(D3DXVECTOR3 &Rot);
	void SetSize(D3DXVECTOR3 &Size);
	void SetLife(int Life);
	void HitLife(int Damage);
	DAMEGE& GetDamegeData();

	const D3DXVECTOR3 *GetSize() const;

protected:
	D3DXVECTOR3		m_move;						// ムーブ
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_rot;						// 回転	
	D3DXVECTOR3		m_rotMove;					// 回転ムーブ
	ANIME			m_motionType;					// モーションタイプ(現在)
	ANIME			m_motionTypeOld;				// モーションタイプ(過去)
	DAMEGE			m_Damegeis;
private:
	D3DXVECTOR3 m_nScale;

	LPDIRECT3DTEXTURE9	m_pTexture = NULL;
	LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff = NULL;

	CMotion			*m_pMotion;					// モーション

	// 位置
	D3DXVECTOR3		m_posOld;					// 位置過去
	//D3DXVECTOR3		m_move;						// ムーブ

	D3DXVECTOR3		m_modelMin;					// サイズ最小
	D3DXVECTOR3		m_modelMax;					// サイズ最大
	D3DXMATRIX		m_mtxWorld;					// マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ

	int				m_nMaxModelType;				// モデルのタイプ数
	int				m_nMaxModelParts;				// 扱うモデルパーツ数
	int				m_nMaxMotion;					// モーション数

	int				m_type;						// タイプ
	int				m_shadow;					// 影番号
	int				m_invincible;				// 無敵時間
	float			m_consumption;				// 計算用

	bool			m_bMotionBlend;				// モーションブレンド
	bool			m_bMotion;					// モーションを使用状況

	bool			m_isUse;					// 使ってるか使ってないか
	bool			m_isLoop;					// ループするかしないか

	int				m_time;						// タイムの最大数
	int				m_nparts;					// パーツの最大数
	int				m_pow;						// ジャンプパワー
	int				m_nMotion;					// モーション番号
	int				m_Life;

	int             m_Invincible;

		
};
#endif
