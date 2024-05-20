#pragma once
#include "..\..\Global.h"
#include "..\..\Utility\Constant\Constant.h"

/************************************************
*	�Q�[�����̎��ԃN���X.
*		���c�F�P.
**/
class CTime final
{
public:
	using Time			= std::pair<int, int>;
	using Time_String	= std::pair<std::string, std::string>;

public:
	CTime();
	~CTime();

	// �C���X�^���X�̎擾.
	static CTime* GetInstance();

	// ������.
	static void Init();

	// �X�V.
	static void Update( const float& DeltaTime );
	// �f�o�b�N�̍X�V.
	static void DebugUpdate( const float& DeltaTime );


	// ���݂̎������擾<��, ��>.
	static Time GetTime();
	// ���݂̎����𕶎���Ŏ擾<��, ��>.
	static Time_String GetTimeString();

	// �o�ߎ��Ԃ̎擾.
	static float GetElapsedTime() { return GetInstance()->m_ElapsedTime; }
	// ���݂̎��Ԃ��擾.
	static int GetHour() { return GetInstance()->m_Hour; }
	// ���݂̕����擾.
	static int GetMinute() { return GetInstance()->m_Minute; }

	// �w�肵�����Ԃ���o�߂������̎擾.
	static int GetElapsedMinute( const float ElapsedTime );
	// �w�肵�����Ԃ���o�߂������Ԃ̎擾.
	static int GetElapsedHour( const float ElapsedTime );

	// ���Ԃ̐i�݋���擾( 0.0f ~ 1.0f ).
	static float GetTimeRatio();

	// �S�[���f���^�C�����擾.
	static bool GetIsGoldenTime() { return GetInstance()->m_IsGoldenTime; }
	// �Q�[�����I���������擾.
	static bool GetIsGameEnd() { return GetInstance()->m_IsEnd; }
	// �Q�[���N���A�Ɉړ������邩�擾.
	static bool GetIsClearSceneMove() { return GetInstance()->m_IsClearSceneMove; }

private:
	float				m_RealElapsedTime;			// �����̌o�ߎ���(�b).
	float				m_ElapsedTime;				// �o�ߎ���(�b).
	int					m_Minute;					// ���݂̕�(��).
	int					m_Hour;						// ���݂̎���(��).
	bool				m_IsGoldenTime;				// �S�[���f���^�C����.
	bool				m_IsEnd;					// �Q�[�����I��������.
	bool				m_IsClearSceneMove;			// �Q�[���N���A�Ɉړ������邩.
	std::vector<bool>	m_IsPushTimeNotification;	// ���Ԃɂ��ʒm�𑗂�����.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};