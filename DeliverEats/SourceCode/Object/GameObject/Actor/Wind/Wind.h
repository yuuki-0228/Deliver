#pragma once
#include "..\..\..\..\Global.h"
#include "..\..\..\..\Utility\Constant\Constant.h"

class CPlayer;

/************************************************
*	���N���X.
*		���c�F�P.
**/
class CWind final
{
public:
	CWind();
	~CWind();

	// �C���X�^���X�̎擾.
	static CWind* GetInstance();

	// ������.
	static void Init();

	// �X�V.
	static void Update( const float& DeltaTime );
	// �f�o�b�N�̍X�V.
	static void DebugUpdate( const float& DeltaTime );
	// SE�̍Đ�.
	static void SEUpdate( const float& DeltaTime );

	// ���̒�~.
	static void StopWind();

	// ���x�N�g���̎擾.
	static D3DXVECTOR3 GetWindVector( const float& Y );
	// ���̋������擾.
	static float GetWindPower();
	// ���̕������擾.
	static std::string GetWindDirection() { return GetInstance()->m_NowWindDire; }
	// ���������Ă��邩�擾.
	static bool GetIsWind() { return GetInstance()->m_IsWind.get(); }

	// �v���C���[�̐ݒ�.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

private:
	CPlayer*					m_pPlayer;			// �v���C���[.
	CBool						m_IsWind;			// ���������Ă��邩.
	std::string					m_NowWindDire;		// ���݂̕��̌���.
	int							m_OldHour;			// �O�̎���.
	float						m_Power;			// ���̋���.
	float						m_SEVolume;			// SE�̉���.
	bool						m_IsWindStop;		// ���𐁂�����/�~�߂�����~������.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SWind& CONSTANT = CConstant::GetWind();
};