#include "Time.h"
#include "..\Light\Light.h"
#include "..\GameObject\Widget\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\Scene\SceneManager\SceneManager.h"

namespace {
	constexpr int	CONVERT_MINUTE		= 60;		// 秒から分に変換する用.
	constexpr int	CONVERT_HOUR		= 3600;		// 秒から時に変換する用.
	constexpr int	CONVERT_DAY			= 86400;	// 秒から日に変換する用.
	constexpr int	MAX_MINUTE			= 60;		// 分の上限.
	constexpr int	MAX_HOUR			= 24;		// 時の上限.
}

CTime::CTime()
	: m_RealElapsedTime			( INIT_FLOAT )
	, m_ElapsedTime				( INIT_FLOAT )
	, m_Minute					( INIT_INT )
	, m_Hour					( INIT_INT )
	, m_IsGoldenTime			( false )
	, m_IsEnd					( false )
	, m_IsClearSceneMove		( false )
	, m_IsPushTimeNotification	()
{
}

CTime::~CTime()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CTime* CTime::GetInstance()
{
	static std::unique_ptr<CTime> pInstance = std::make_unique<CTime>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
void CTime::Init()
{
	CTime* pI = GetInstance();

	pI->m_RealElapsedTime	= INIT_FLOAT;
	pI->m_ElapsedTime		= INIT_FLOAT;
	pI->m_Minute			= INIT_INT;
	pI->m_Hour				= INIT_INT;
	pI->m_IsGoldenTime		= false;
	pI->m_IsEnd				= false;
	pI->m_IsClearSceneMove	= false;

	pI->m_IsPushTimeNotification.clear();
	pI->m_IsPushTimeNotification.resize( pI->CONSTANT.TIME_NOTIFICATION_MAX, false );
}

//---------------------------.
// 更新.
//---------------------------.
void CTime::Update( const float& DeltaTime )
{
	CTime* pI = GetInstance();

	// 経過時間の加算.
	pI->m_RealElapsedTime	+= DeltaTime;
	pI->m_ElapsedTime		+= DeltaTime * pI->CONSTANT.ONE_SECOND_TIME;
	const int ElapsedTime	= static_cast<int>( pI->m_ElapsedTime );

	// 現在の分を更新.
	pI->m_Minute = ElapsedTime % CONVERT_HOUR / CONVERT_MINUTE;
	if ( pI->m_Minute >= MAX_MINUTE ) pI->m_Minute -= MAX_MINUTE;

	// 現在の時の更新.
	pI->m_Hour = pI->CONSTANT.START_HOUR + ElapsedTime % CONVERT_DAY / CONVERT_HOUR;
	if ( pI->m_Hour >= pI->CONSTANT.CLEAR_SCENE_MOVE_HOUR	) pI->m_IsClearSceneMove = true;
	if ( pI->m_Hour >= pI->CONSTANT.GAME_END_HOUR			) pI->m_IsEnd			 = true;
	if ( pI->m_Hour >= MAX_HOUR								) pI->m_Hour			-= MAX_HOUR;

	// 通知を送る.
	for ( int i = 0; i < pI->CONSTANT.TIME_NOTIFICATION_MAX; ++i ) {
		if ( pI->m_IsPushTimeNotification[i] ) continue;
		const int Hour	 = pI->CONSTANT.NOTIFICATION_TIMING[i].first;
		const int Minute = pI->CONSTANT.NOTIFICATION_TIMING[i].second;
		if ( pI->m_Hour >= Hour && pI->m_Minute >= Minute ) {
			pI->m_IsPushTimeNotification[i] = true;
			CNotificationUI::PushMessage( EMessageType::TimeLeft, pI->CONSTANT.NOTIFICATION_MESSAGE[i] );
		}
	}

	// ライトの強さの更新.
	if ( pI->m_IsEnd == false ) {
		CLight::SetIntensity( pI->CONSTANT.LIGHT_INTENSITY_MAX - pI->CONSTANT.LIGHT_INTENSITY_MIN * ( pI->m_ElapsedTime / pI->CONSTANT.TIME_MAX ) );
	}

	// ゴールデンタイムか設定.
	const bool IsOldGoldenTime = pI->m_IsGoldenTime;
	if ( (	pI->CONSTANT.NORMAL_TIME_HOUR[0].first <= pI->m_Hour &&
			pI->m_Hour < pI->CONSTANT.NORMAL_TIME_HOUR[0].second ) ||
		 (	pI->CONSTANT.NORMAL_TIME_HOUR[1].first <= pI->m_Hour &&
			pI->m_Hour < pI->CONSTANT.NORMAL_TIME_HOUR[1].second ) ||
		 pI->m_IsEnd )
	{
		// 通常の時間に戻す.
		pI->m_IsGoldenTime = false;
	}
	else if ( ( pI->CONSTANT.GOLDEN_TIME_HOUR[0].first <= pI->m_Hour &&
				pI->m_Hour < pI->CONSTANT.GOLDEN_TIME_HOUR[0].second ) ||
			  ( pI->CONSTANT.GOLDEN_TIME_HOUR[1].first <= pI->m_Hour &&
				pI->m_Hour < pI->CONSTANT.GOLDEN_TIME_HOUR[1].second ) )
	{
		// ゴールデンタイムにする.
		pI->m_IsGoldenTime = true;
	}

	// 通知を送る.
	if ( IsOldGoldenTime == false && pI->m_IsGoldenTime ) {
		CNotificationUI::PushMessage( EMessageType::GoldenTime, pI->CONSTANT.GOLDEN_TIME_MESSAGE );
	}
	else if ( IsOldGoldenTime && pI->m_IsGoldenTime == false ) {
		CNotificationUI::PushMessage( EMessageType::GoldenTime, pI->CONSTANT.NORMAL_TIME_MESSAGE );
	}
}

//---------------------------.
// デバックの更新.
//---------------------------.
void CTime::DebugUpdate( const float& DeltaTime )
{
	CDebugWindow::PushProc( "Time",
	[&](){
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"経過時間(秒)      : %5f",		GetInstance()->m_ElapsedTime );
		ImGui::Text( u8"現在時間          : %2d:%2d",	GetInstance()->m_Hour, GetInstance()->m_Minute );
		ImGui::Text( u8"------------------------------" );
		ImGui::Text( u8"現実の経過時間(秒) : %5f",		GetInstance()->m_RealElapsedTime );
		ImGui::Text( u8"現実の経過時間(分) : %d",			static_cast<int>( GetInstance()->m_RealElapsedTime / CONVERT_MINUTE ) );
		ImGui::Text( u8"------------------------------" );
	} );
}

//---------------------------.
// 現在の時刻を取得<時, 分>.
//---------------------------.
CTime::Time CTime::GetTime()
{
	CTime* pI = GetInstance();

	return std::make_pair( 
		pI->m_Hour,
		pI->m_Minute
	);
}

//---------------------------.
// 現在の時刻を文字列で取得<時, 分>.
//---------------------------.
CTime::Time_String CTime::GetTimeString()
{
	CTime* pI = GetInstance();

	return std::make_pair(
		StringConversion::to_LeftPaddingString( std::to_string( pI->m_Hour		), 2, '0' ),
		StringConversion::to_LeftPaddingString( std::to_string( pI->m_Minute	), 2, '0' )
	);
}

//---------------------------.
// 指定した時間から経過した分の取得.
//---------------------------.
int CTime::GetElapsedMinute( const float ElapsedTime )
{
	CTime* pI = GetInstance();

	const int Elapsed = static_cast<int>( pI->m_ElapsedTime - ElapsedTime );
	return Elapsed / CONVERT_MINUTE;
}

//---------------------------.
// 指定した時間から経過した時間の取得.
//---------------------------.
int CTime::GetElapsedHour( const float ElapsedTime )
{
	CTime* pI = GetInstance();

	const int Elapsed = static_cast<int>( pI->m_ElapsedTime - ElapsedTime );
	return pI->CONSTANT.START_HOUR + Elapsed / CONVERT_HOUR;
}

//---------------------------.
// 時間の進み具合を取得( 0.0f ~ 1.0f ).
//---------------------------.
float CTime::GetTimeRatio()
{
	CTime* pI = GetInstance();
	return pI->m_ElapsedTime / pI->CONSTANT.TIME_MAX;
}
