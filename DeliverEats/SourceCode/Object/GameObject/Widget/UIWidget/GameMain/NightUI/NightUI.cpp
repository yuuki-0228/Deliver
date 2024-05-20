#include "NightUI.h"
#include "..\..\..\..\..\Time\Time.h"

CNightUI::CNightUI()
	: m_pSprite		( nullptr )
	, m_SpriteState	()
{
}

CNightUI::~CNightUI()
{
}

//---------------------------.
// ‰Šú‰».
//---------------------------.
bool CNightUI::Init()
{
	m_pSprite				= CSpriteResource::GetSprite( "Night", &m_SpriteState );
	m_SpriteState.Color.w	= 0.0f;
	return true;
}

//---------------------------.
// XV.
//---------------------------.
void CNightUI::Update( const float& DeltaTime )
{
	if ( CTime::GetIsGameEnd() == false ) return;

	// •s“§–¾‚É‚µ‚Ä‚¢‚­.
	const float NewTime = CTime::GetElapsedTime();
	m_SpriteState.Color.w = ( NewTime - CONSTANT.TIME_MAX ) / ( CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX - CONSTANT.TIME_MAX );
}

//---------------------------.
// •`‰æ.
//---------------------------.
void CNightUI::Render()
{
	m_pSprite->RenderUI( &m_SpriteState );
}
