#include "WindUI.h"
#include "..\..\..\..\Actor\Wind\Wind.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

CWindUI::CWindUI()
	: m_pWindLine				( nullptr )
	, m_WindLineStateBase		()
	, m_WindLineStateList		()
	, m_DispWindLineTimeCnt		( INIT_FLOAT )
{
}

CWindUI::~CWindUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CWindUI::Init()
{
	// 風の線の取得.
	m_pWindLine = CSpriteResource::GetSprite( "WindLine", &m_WindLineStateBase );
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CWindUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	WindLineMove();		// 風の線の移動.
	ListDelete();		// リストの削除.
	SpownWindLine();	// 風の線を召喚する.
}

//---------------------------.
// 描画.
//---------------------------.
void CWindUI::Render()
{
	for ( auto& w : m_WindLineStateList ) m_pWindLine->RenderUI( &w );
}

//---------------------------.
// 風の線の移動.
//---------------------------.
void CWindUI::WindLineMove()
{
	// 風の情報の取得.
	const bool  IsWindLeft	= CWind::GetWindDirection() == "Left";
	const float WindSpeed	= CRandom::GetRand( CONSTANT.WIND_LINE_SPPED_MIN, CONSTANT.WIND_LINE_SPPED_MAX ) + 10.0f * CWind::GetWindPower();
	const float AddSpeed	= CONSTANT.WIND_LINE_WIND_POWER_ADD_SPPED * CWind::GetWindPower();

	// 雲の移動.
	for ( auto& w : m_WindLineStateList ) {
		if ( w.IsDisp == false ) continue;
		if( IsWindLeft ) w.Transform.Position.x -= WindSpeed + AddSpeed;
		else			 w.Transform.Position.x += WindSpeed + AddSpeed;

		// 画面外に移動したか.
		if ( w.Transform.Position.x <= -w.Transform.Scale.x || w.Transform.Position.x >= FWND_W ) {
			w.IsDisp = false;
		}
	}
}

//---------------------------.
// 風の線を召喚.
//---------------------------.
void CWindUI::SpownWindLine()
{
	if ( CWind::GetIsWind() == false ) return;

	// 雲を召喚するか.
	m_DispWindLineTimeCnt += m_DeltaTime;
	if ( m_DispWindLineTimeCnt < CONSTANT.WIND_LINE_DISP_SPEED ) return;
	m_DispWindLineTimeCnt = INIT_FLOAT;

	// サイズのランダムで変更.
	const float ScaleX = CRandom::GetRand( CONSTANT.WIND_LINE_SCALE_MIN, CONSTANT.WIND_LINE_SCALE_MAX );

	// 表示位置の取得.
	D3DXPOSITION3	SpownPos	= INIT_FLOAT3;
	if ( CWind::GetWindDirection() == "Left" )	SpownPos.x =  FWND_W;
	else										SpownPos.x = -ScaleX;
	SpownPos.y = CRandom::GetRand( 0.0f, FWND_H );

	// アルファ値をランダムで選ぶ.
	const float Alpha = CRandom::GetRand( CONSTANT.WIND_LINE_ALPHA_MIN, CONSTANT.WIND_LINE_ALPHA_MAX );

	// 使用していないのがあれば使用する.
	for ( auto& w : m_WindLineStateList ) {
		if ( w.IsDisp ) continue;

		// アニメーションを開始させる.
		w = m_WindLineStateBase;
		w.Transform.Position	= SpownPos;
		w.Transform.Scale.x		= ScaleX;
		w.Color.w				= Alpha;
		return;
	}

	// 新しく作成.
	m_WindLineStateList.emplace_back( m_WindLineStateBase );
	m_WindLineStateList.back().Transform.Position	= SpownPos;
	m_WindLineStateList.back().Transform.Scale.x	= ScaleX;
	m_WindLineStateList.back().Color.w				= Alpha;
}

//---------------------------.
// 雲リストの削除.
//---------------------------.
void CWindUI::ListDelete()
{
	// 風の線の削除.
	for ( int i = static_cast<int>( m_WindLineStateList.size() ) - 1; i >= 0; i-- ) {
		if ( m_WindLineStateList[i].IsDisp ) break;
		m_WindLineStateList.pop_back();
	}
}