//=========================================
// 
// �f�o�b�O���̂ݔ����̏���(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _PROCESS_H_
#define _PROCESS_H_

//=========================================
// include
//=========================================

//=========================================
// �v���g�^�C�v�錾
//=========================================
void InitProcess();
void UninitProcess();
void UpdateProcess();
void DrawProcess();
bool GetSetMode();		// �ݒu���[�h�̎擾
void ChangeSetMode(bool hasChange);	// �ݒu���[�h�̐؂�ւ�
void setEnemyNumber(int inDebugNumber);
void setDebugNumber(int inDebugNumber);
bool IsDebug(void);				// �擾
void ChangeDebug(bool inDebug);				// �擾
bool GetEnemyAlignment();
int GetDebugNumberEnemy();			//�f�o�b�N�i���o�[�iEnemy�j
int GetDebugNumber();				//�f�o�b�N�i���o�[�i�v���C���[�j

#endif // !_DEBUG_H_
