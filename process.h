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
int GetDebugNumber();
void setDebugNumber(int inDebugNumber);
bool IsDebug(void);				// �擾
void ChangeDebug(bool inDebug);				// �擾
bool GetEnemyAlignment();
int GetDebugNumberEnemy();

#endif // !_DEBUG_H_
