#include "RadioWavesUI.h"
#include "..\..\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr int RADIOWAVES_MAX = 3;	// 電波の最大.
	constexpr int RADIOWAVES_MIN = 0;	// 電波の最小.
}

CRadioWavesUI::CRadioWavesUI()
	: m_pRadioWaves			( nullptr )
	, m_RadioWavesState		()
{
}

CRadioWavesUI::~CRadioWavesUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CRadioWavesUI::Init()
{
	// 電波の画像の取得.
	m_pRadioWaves = CSpriteResource::GetSprite( "RadioWaves", &m_RadioWavesState );

	// スマホの画面の位置を取得.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize	= CMiniMapUI::GetScreenSize();

	// 表示位置の設定.
	m_RadioWavesState.Transform.Position.x = ScreenPos.x + ScreenSize.w / 2.0f - 34.0f;
	m_RadioWavesState.Transform.Position.y = ScreenPos.y - ScreenSize.h / 2.0f - 12.0f;

	// アニメーション番号を設定できるように変更.
	m_RadioWavesState.AnimState.IsSetAnimNumber = true;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CRadioWavesUI::Update( const float& DeltaTime )
{
	if ( CRandom::Probability( 1, 100 ) == false ) return;

	// 電波の強さを上げる.
	if ( m_RadioWavesState.AnimState.AnimNumber == RADIOWAVES_MIN ) {
		m_RadioWavesState.AnimState.AnimNumber++;
		return;
	}

	// 電波の強さを下げる.
	if ( m_RadioWavesState.AnimState.AnimNumber == RADIOWAVES_MAX ) {
		m_RadioWavesState.AnimState.AnimNumber--;
		return;
	}

	// 電波の強さをランダムで変更する.
	if ( CRandom::Probability( 1, 3 ) ) {
		m_RadioWavesState.AnimState.AnimNumber--;
	}
	else {
		m_RadioWavesState.AnimState.AnimNumber++;
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CRadioWavesUI::Render()
{
	m_pRadioWaves->RenderUI( &m_RadioWavesState );
}
