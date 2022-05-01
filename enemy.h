//============================
//
// プレイヤー設定ヘッター
// Author:hamada ryuuga
//
//============================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "motion.h"
#include "player.h"

//------------------------------------
// マクロ
//------------------------------------
#define ATTENUATION	(0.5f)		//減衰係数
#define SPEED	(1.0f)			//スピード
#define WIDTH (10.0f)			//モデルの半径
#define MAX_PRAYER (16)			//最大数
#define MAX_MOVE (9)			//アニメーションの最大数
#define INVINCIBLE (300)		//無敵時間
#define MAX_MODELPARTS (9)
#define MAX_COPY  (4)

//------------------------------------
// 種別の列挙型
//------------------------------------
enum ENEMY_TYPE
{
	ENEMY_TYPE_HUMANSOUL = 0,	// 人魂
	ENEMY_TYPE_SKELETON,		// がいこつ
	ENEMY_TYPE_UNGAIKYO,		// 雲外鏡
	ENEMY_TYPE_KAMAITACHI,		// かまいたち
	ENEMY_TYPE_BUDDHA,			// ブツー
	ENEMY_TYPE_NULL1,		// がいこつ
	ENEMY_TYPE_NULL2,		// 雲外鏡
	ENEMY_TYPE_NULL3,		// かまいたち
	ENEMY_TYPE_NULL4,			// ブツー
	ENEMY_TYPE_NULL5,
	ENEMY_TYPE_NULL6,
	ENEMY_TYPE_NULL7,
	ENEMY_TYPE_NULL8,
	ENEMY_TYPE_NULL9,
	ENEMY_TYPE_NULL10,
	ENEMY_TYPE_NULL11,
	ENEMY_TYPE_NULL12,
	ENEMY_TYPE_NULL13,
	ENEMY_TYPE_NULL14,
	ENEMY_TYPE_MAX
};

typedef struct
{
	D3DXVECTOR3	pos;						// 位置
	D3DXVECTOR3	posOld;						// 位置過去
	D3DXVECTOR3	move;						// ムーブ
	D3DXVECTOR3	rot;						// 回転	
	D3DXVECTOR3	rotMove;					// 回転ムーブ
	D3DXVECTOR3	modelMin;					// サイズ最小
	D3DXVECTOR3	modelMax;					// サイズ最大

	D3DXVECTOR3 scale;
	D3DXMATRIX	mtxWorld;					// マトリックス//ポリゴンの位置や回転行列すべてをつめてるナニカ

	STATUS		status;						// 今のステータス
	DAMEGE		damege;						// ダメージくらってるかくらってないか
	COPY		copy;						// コピー
	ENEMY_TYPE	type;						// エネミーのタイプ

	Parts		parts[MAX_MODELPARTS];		// モデルパーツ
	PartsFile	partsFile[MAX_MODELPARTS];	// パーツファイル
	MyMotion	motion[ANIME_MAX];			// モーション
	ANIME		motionType;					// モーションタイプ(現在)
	ANIME		motionTypeOld;				// モーションタイプ(過去)
	int			nMaxModelType;				// モデルのタイプ数
	int			nMaxModelParts;				// 扱うモデルパーツ数
	int			nMaxMotion;					// モーション数

	int			nInvincible;				// 無敵時間
	float		fConsumption;				// 計算用
	float		fLog;
	
	bool		bMotionBlend;				// モーションブレンド
	bool		bMotion;					// モーションを使用状況

	bool        bSelect;
	bool		isUse;						// 使ってるか使ってないか
	bool		notLoop;					// ループするかしないか
}Enemy;

//プロトタイプ宣言
void InitEnemy(void);	// 初期化
void UninitEnemy(void);	// 破棄
void UpdateEnemy(void);	// 更新
void DrawEnemy(void);	// 描画

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, ENEMY_TYPE type);//セット引数座標と読み込むファイル名
void SizeSet(void);//当たり判定取得
Enemy *GetEnemy(void);//ゲット
MODELDATAPLAYER *GetModelDataEnemy(void);//motionデータのゲット
void LoadEnemy(void);	// 読込
void LoadSetFile(char *Filename);
void OutputEnemy(char *Filename);
void ConversionDes(D3DXVECTOR3 Mouse);
void SelectDes(D3DXVECTOR3 pos);
//void falseSetEnemy(void);
//void selectDes(D3DXVECTOR3 Mouse, D3DXVECTOR3 Size);
#endif
