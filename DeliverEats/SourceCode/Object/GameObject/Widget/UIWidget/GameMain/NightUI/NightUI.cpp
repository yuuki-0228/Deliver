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
// 初期化.
//---------------------------.
bool CNightUI::Init()
{
	m_pSprite				= CSpriteResource::GetSprite( "Night", &m_SpriteState );
	m_SpriteState.Color.w	= 0.0f;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CNightUI::Update( const float& DeltaTime )
{
	if ( CTime::GetIsGameEnd() == false ) return;

	// 不透明にしていく.
	const float NewTime = CTime::GetElapsedTime();
	m_SpriteState.Color.w = ( NewTime - CONSTANT.TIME_MAX ) / ( CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX - CONSTANT.TIME_MAX );
}

//---------------------------.
// 描画.
//---------------------------.
void CNightUI::Render()
{
	m_pSprite->RenderUI( &m_SpriteState );
}
