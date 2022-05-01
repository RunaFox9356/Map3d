//============================
//
// �}�b�v�`�b�v�ݒ�w�b�^�[
// Author:hamada ryuuga
//
//============================
#ifndef _MAP_H_
#define _MAP_H_

#include"main.h"

// �t�@�C����
#define MAP_FILE0	"Data/txt/map.txt"
#define MAP_FILE1	"Data/txt/map1.txt"
#define MAP_FILE2	"Data/txt/map2.txt"
#define MAP_FILE3	"Data/txt/map3.txt"
#define MAP_FILE4	"Data/txt/map4.txt"
#define MAP_FILE5	"Data/txt/map5.txt"
#define MAP_FILE6	"Data/txt/map6.txt"

// �}�N����`
#define NUM_MAP	    (2048)		//�w�i����
#define X_MAP		(8)		//�}�b�v�`�b�v�T�C�YX
#define Y_MAP		(8)			//�}�b�v�`�b�v�T�C�YY
#define MAP_SIZEX	(100)		//�}�b�v�T�C�YX
#define MAP_SIZEY	(10)		//�}�b�v�T�C�YY
#define MAPYLENGTH	(10)		//�}�b�v�̒���
#define BLOCKSIZEX	(BLOCKSIZEY)
#define BLOCKSIZEY	((float)SCREEN_HEIGHT/MAP_SIZEY)//
#define MAPMOVE (5.0f)

//-------------
//�\����
//-------------
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�J���[
	int nType;			//�e�N�X�`��
	int tex;			//�e�N�X�`���^�C�v
	bool bUse;			//
	char filename[256];
}Map;

//-----------------
//�v���g�^�C�v�錾
//-----------------
void InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);
void SetMap(D3DXVECTOR3 pos, int nType, int tex);
void InitMapSet(char *Filename);
void ConversionMap (D3DXVECTOR3 pos,int tex);
void OutputMap(char *Filename);
void SizeMap(float fSize);
Map *GetMap(void);
bool CollisionMap(D3DXVECTOR3 pos);//�����蔻��
void ConteSet(int nStage);			//�R���e
void PasSetMap(char *Filename);//�}�b�v�p�X���Z�b�g����
int SopiteMap(D3DXVECTOR3 pos);
void FalseSetMap(void);
D3DXVECTOR3 EnemyMap(D3DXVECTOR3 pos);
#endif // !_Map_H_
