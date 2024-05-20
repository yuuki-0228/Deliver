#include "StatusBarUI.h"
#include "RadioWavesUI\RadioWavesUI.h"
#include "NowTimeUI\NowTimeUI.h"
#include "BatteryUI\BatteryUI.h"
#include "..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

CStatusBarUI::CStatusBarUI()
	: m_pRadioWaves		( nullptr )
	, m_pNowTime		( nullptr )
	, m_pBattery		( nullptr )
	, m_IsCharging		( false )
{
}

CStatusBarUI::~CStatusBarUI()
{
}

//---------------------------.
// ‰Šú‰».
//---------------------------.
bool CStatusBarUI::Init()
{
	// “d”g‚Ì‰Šú‰».
	m_pRadioWaves	= std::make_unique<CRadioWavesUI>();
	if ( m_pRadioWaves->Init()	== false ) return false;

	// Œ»İ‚ÌŠÔ‚Ì‰Šú‰».
	m_pNowTime		= std::make_unique<CNowTimeUI>();
	if ( m_pNowTime->Init()		== false ) return false;

	// [“d‚Ì‰Šú‰».
	m_pBattery		= std::make_unique<CBatteryUI>();
	m_pBattery->SetIsCharging( m_IsCharging );
	if ( m_pBattery	->Init()	== false ) return false;
	return true;
}

//---------------------------.
// XV.
//---------------------------.
void CStatusBarUI::Update( const float& DeltaTime )
{
	m_pRadioWaves->Update( DeltaTime );

	if ( m_IsCharging ) return;
	m_pNowTime->Update( DeltaTime );
	m_pBattery->Update( DeltaTime );
}

//---------------------------.
// •`‰æ.
//---------------------------.
void CStatusBarUI::Render()
{
	m_pRadioWaves->Render();
	m_pNowTime->Render();
	m_pBattery->Render();
}
