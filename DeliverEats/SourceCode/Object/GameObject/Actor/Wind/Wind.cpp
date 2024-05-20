#include "Wind.h"
#include "..\Character\Player\Player.h"
#include "..\..\Widget\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\..\..\Time\Time.h"
#include "..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\..\..\Utility\Input\Input.h"

namespace {
	// ���̌������X�g.
	const std::vector<std::string> WIND_DIRE_LIST = {
		"Left", "Right"
	};
	const int WIND_DIRE_MAX = static_cast<int>( WIND_DIRE_LIST.size() );

	// ����SE.
	constexpr char WIND_SE[] = "Wind";
}

CWind::CWind()
	: m_pPlayer			( nullptr )
	, m_IsWind			( false, u8"���������Ă��邩" )
	, m_NowWindDire		( "Left" )
	, m_OldHour			( INIT_INT )
	, m_Power			( INIT_FLOAT )
	, m_SEVolume		( INIT_FLOAT )
	, m_IsWindStop		( false )
{
}

CWind::~CWind()
{
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CWind* CWind::GetInstance()
{
	static std::unique_ptr<CWind> pInstance = std::make_unique<CWind>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
void CWind::Init()
{
	CWind* pI = GetInstance();

	pI->m_IsWind		= false;
	pI->m_IsWindStop	= false;
	pI->m_OldHour		= CConstant::GetTime().START_HOUR;
	pI->m_Power			= INIT_FLOAT;
	pI->m_NowWindDire	= "Left";
}

//---------------------------.
// �X�V.
//---------------------------.
void CWind::Update( const float& DeltaTime )
{
	CWind* pI = GetInstance();

	// ��莞�ԑ҂����ꍇ�A���𐁂�����~�߂��肷��.
	const int NowHour = CTime::GetHour();
	if ( pI->m_IsWindStop == false && pI->m_OldHour != NowHour ) {
		pI->m_OldHour = NowHour;
		pI->m_IsWind.Inversion();

		// �����������ꍇ�����������_���Ō��߂�.
		if ( pI->m_IsWind.get() ) {
			pI->m_NowWindDire = WIND_DIRE_LIST[CRandom::GetRand( 0, WIND_DIRE_MAX - 1 )];
			CNotificationUI::PushMessage( EMessageType::WindIsGenerated, pI->CONSTANT.WIND_MESSAGE );

			// �����~������.
			if ( CTime::GetIsGameEnd() ) pI->m_IsWindStop = true;
		}
		else {
			CNotificationUI::PushMessage( EMessageType::WindIsGenerated, pI->CONSTANT.WIND_STOP_MESSAGE );
		}
	}

	// ���������Ă�ꍇ.
	const bool IsLTrigger	= CXInput::GetLTrigger()		!= 0.0f;
	const bool IsRTrigger	= CXInput::GetRTrigger()		!= 0.0f;
	const bool IsNotIsland	= pI->m_pPlayer->GetIsIsland()	== false;
	if ( pI->m_IsWind.get() && ( IsLTrigger || IsRTrigger ) && IsNotIsland ) {
		// ���̋��������߂Ă���.
		if ( pI->m_Power < 1.0f ) {
			pI->m_Power += DeltaTime / pI->CONSTANT.WIND_STRONG_SPEED;
			if ( pI->m_Power > 1.0f ) pI->m_Power = 1.0f;
		}
	}
	// ���������Ă��Ȃ��ꍇ.
	else {
		// ���̋�������߂Ă���.
		if ( pI->m_Power > 0.0f ) {
			pI->m_Power -= DeltaTime / pI->CONSTANT.WIND_WEAK_SPEED;
			if ( pI->m_Power < 0.0f ) pI->m_Power = 0.0f;
		}
	}

	// �����v���C���[�ɒǉ�����.
	pI->m_pPlayer->SetWind( GetWindVector( pI->m_pPlayer->GetPosition().y ) );
}

//---------------------------.
// �f�o�b�N�̍X�V.
//---------------------------.
void CWind::DebugUpdate( const float& DeltaTime )
{
	CDebugWindow::PushProc( "Wind", [&] () {
		CWind* pI = GetInstance();

		ImGui::Text( "OldHour     : %f", pI->m_OldHour );
		ImGui::Text( "Power       : %f", pI->m_Power );
		ImGui::Text( "NowDire     : %s", pI->m_NowWindDire.c_str() );
	} );
}

//---------------------------.
// SE�̍Đ�.
//---------------------------.
void CWind::SEUpdate( const float& DeltaTime )
{
	CWind* pI = GetInstance();

	// SE�̍Đ�.
	if ( pI->m_IsWind.get() ) {
		if ( pI->m_SEVolume < 1.0f ) {
			pI->m_SEVolume += pI->CONSTANT.ADD_SE_VOLUME * DeltaTime;
			if ( pI->m_SEVolume >= 1.0f ) pI->m_SEVolume = 1.0f;
			CSoundManager::SetSEVolume( WIND_SE, pI->m_SEVolume );
		}
		CSoundManager::PlaySELoop( WIND_SE );
	}
	else {
		if ( pI->m_SEVolume > 0.0f ) {
			pI->m_SEVolume -= pI->CONSTANT.ADD_SE_VOLUME * DeltaTime;
			if ( pI->m_SEVolume <= 0.0f ) {
				pI->m_SEVolume = 0.0f;
				CSoundManager::StopSE( WIND_SE );
			}
			CSoundManager::SetSEVolume( WIND_SE, pI->m_SEVolume );
		}
	}
}

//---------------------------.
// ���̒�~.
//---------------------------.
void CWind::StopWind()
{
	GetInstance()->m_IsWind = false;
	CSoundManager::StopSE( WIND_SE );
}

//---------------------------.
// ���x�N�g���̎擾.
//---------------------------.
D3DXVECTOR3 CWind::GetWindVector( const float& Y )
{
	CWind* pI = GetInstance();
	if ( pI->m_pPlayer == nullptr ) return INIT_FLOAT3;

	// �����ɉ����ĕ��̋�����ύX.
	float HeightPower = Y * pI->CONSTANT.WIND_HEIGHT_POWER;
	if ( HeightPower < 0.0f ) HeightPower = 0.0f;

	// �v���C���[�̌����x�N�g�����擾.
	const D3DXVECTOR3& PlayerDireVec = pI->m_pPlayer->GetDireVector();

	// ���x�N�g���̍쐬.
	D3DXVECTOR3 Out;
	if ( pI->m_NowWindDire == "Left" ) {
		Out.x =  PlayerDireVec.z * ( HeightPower * pI->m_Power );
		Out.y =  PlayerDireVec.y * ( HeightPower * pI->m_Power );
		Out.z = -PlayerDireVec.x * ( HeightPower * pI->m_Power );
	}
	else {
		Out.x = -PlayerDireVec.z * ( HeightPower * pI->m_Power );
		Out.y =  PlayerDireVec.y * ( HeightPower * pI->m_Power );
		Out.z =  PlayerDireVec.x * ( HeightPower * pI->m_Power );
	}
	return Out;
}

//---------------------------.
// ���̋������擾.
//---------------------------.
float CWind::GetWindPower()
{
	CWind* pI = GetInstance();
	if ( pI->m_pPlayer == nullptr ) return INIT_FLOAT;

	// �����ɉ��������̋������v�Z.
	float HeightPower = pI->m_pPlayer->GetPosition().y * pI->CONSTANT.WIND_HEIGHT_POWER;
	if ( HeightPower < 0.0f ) HeightPower = 0.0f;
	return HeightPower * pI->m_Power;
}
