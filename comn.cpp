//============================
//
// �v�Z�n��
// Author:hamada ryuuga
//
//============================
#include "comn.h"

//�C�[�W���O�J�[�u
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
// 2D�x�N�g���̊O��
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->z - v1->z * v2->x;
}

// 2D�x�N�g���̊O��
float Vec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->x + v1->z * v2->z;
}

//---------------------------------------------------------------------------
// ���[���h���W�ւ̃L���X�g����
//---------------------------------------------------------------------------
D3DXVECTOR3	WorldCastVtx(D3DXVECTOR3 vtx, D3DXVECTOR3 FormerPos, D3DXVECTOR3 FormerRot)
{
	// �ϐ��錾
	D3DXMATRIX		mtxWorldVtx;				// ���[���h�}�g���b�N�X
	D3DXMATRIX		mtxRot, mtxTrans;			// �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	// �s�񏉊����֐�(��������[�s��]��[�P�ʍs��]�ɏ�����)
	D3DXMatrixIdentity(&mtxWorldVtx);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, vtx.x, vtx.y, vtx.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// �s��|���Z�֐�

	// �����̔��f
	// �s���]�֐� (��������[���[(y)�s�b�`(x)���[��(z)]�����̉�]�s����쐬)
	D3DXMatrixRotationYawPitchRoll(&mtxRot, FormerRot.y, FormerRot.x, FormerRot.z);

	// �s��|���Z�֐� (������ * ��O������������Ɋi�[)
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxRot);

	// �ʒu�𔽉f
	// �s��ړ��֐� (��������X,Y,Z�����̈ړ��s����쐬)
	D3DXMatrixTranslation(&mtxTrans, FormerPos.x, FormerPos.y, FormerPos.z);
	D3DXMatrixMultiply(&mtxWorldVtx, &mtxWorldVtx, &mtxTrans);		// �s��|���Z�֐�

	return D3DXVECTOR3(mtxWorldVtx._41, mtxWorldVtx._42, mtxWorldVtx._43);
}


//if (s_Enemy[nCnt].bUse)
//{
///*	float vecA = ;
//	float vecB;*/

//	if (pPos->y + Siz.y / 2 > s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMin.y,
//		pPos->y - Siz.y / 2 < s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y)
//	{
//		//���E�̕�
//		if (pPosOld->z + Siz.z / 2 > s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z
//			&& pPosOld->z - Siz.z / 2 < s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z)
//		{
//			if (pPos->x + Siz.x / 2 > s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x
//				&& pPosOld->x + Siz.x / 2 <= s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x)
//			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//��
//				pPos->x = s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x - Siz.x / 2.0f;
//			}
//			if (pPos->x - Siz.x / 2 < s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x
//				&& pPosOld->x - Siz.x / 2 >= s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x)
//			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//�E
//				pPos->x = s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x + Siz.x / 2.0f + 0.01f;
//			}
//		}
//		//�㉺�̕�
//		if (pPosOld->x + Siz.x / 4 > s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x
//			&& pPosOld->x - Siz.x /4 < s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x)
//		{
//			if (pPos->z + Siz.z / 2 > s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z
//				&& pPosOld->z + Siz.z / 2 <= s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z)
//			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//��
//				pPos->z = s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z - Siz.z / 2.0f;
//			}
//			if (pPos->z - Siz.z / 2 < s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z
//				&& pPosOld->z - Siz.z / 2 >= s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z)
//			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//��
//				pPos->z = s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z + Siz.z / 2.0f + 0.01f;
//			}

//		}
//	}

//	//���E�̕�
//	if (pPosOld->z + Siz.z / 2 > s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMin.z
//		&& pPosOld->z - Siz.z / 2 < s_Enemy[nCnt].pos.z + s_Enemy[nCnt].ModelMax.z)
//	{
//		//�㉺�̕�
//		if (pPosOld->x + Siz.x / 2 > s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMin.x
//			&& pPosOld->x - Siz.x / 2 < s_Enemy[nCnt].pos.x + s_Enemy[nCnt].ModelMax.x)
//		{
//			if (pPos->y < s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y
//				&& pPosOld->y >= s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y)
//			{//�u���b�N�̍��W�ƍ��W���d�Ȃ荇������//�ʏ탂�[�h//����
//				pPos->y = s_Enemy[nCnt].pos.y + s_Enemy[nCnt].ModelMax.y;
//				bIsLanding = true;
//			}
//		}
//	}

//}