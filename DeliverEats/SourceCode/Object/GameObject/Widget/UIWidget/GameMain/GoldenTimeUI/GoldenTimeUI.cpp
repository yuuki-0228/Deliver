#include "GoldenTimeUI.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Utility\Easing\Easing.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr float EASING_MAX			= 2.0f;		// イージングの最大値.
	constexpr float EFFECT_CNT_MAX		= 180.0f;	// エフェクトのアニメーション用カウントの最大値.
	constexpr float EFFECT_ADD_SCALE	= 0.5f;		// エフェクトの追加する拡縮.
}

CGoldenTimeUI::CGoldenTimeUI()
	: m_pNotice				( nullptr )
	, m_pEffect				( nullptr )
	, m_NoticeState			()
	, m_EffectStateBase		()
	, m_EffectStateList		()
	, m_AnimTimeCnt			( INIT_FLOAT )
	, m_EffectPlayTimeCnt	( INIT_FLOAT )
	, m_IsOpen				( false )
	, m_IsClose				( false )
{
}

CGoldenTimeUI::~CGoldenTimeUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CGoldenTimeUI::Init()
{
	m_pNotice	= CSpriteResource::GetSprite( "GoldenTimeNotice", &m_NoticeState );
	m_pEffect	= CSpriteResource::GetSprite( "GoldenTimeEffect", &m_EffectStateBase );
	m_NoticeState.IsDisp						= false;
	m_EffectStateBase.AnimState.IsSetAnimNumber = true;

	// 位置の設定.
	m_NoticeState.Transform.Position.x = FWND_W / 2.0f;
	m_NoticeState.Transform.Position.y = 0.0f;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGoldenTimeUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	NoticeOpen();			// 看板の表示の設定.
	NoticeClose();			// 看板の非表示の設定.
	NoticeOpenUpdate();		// 看板の表示の更新.
	NoticeCloseUpdate();	// 看板の非表示の更新.
	EffectPlay();			// エフェクトの再生.
	EffectUpdate();			// エフェクトの更新.
}

//---------------------------.
// 描画.
//---------------------------.
void CGoldenTimeUI::Render()
{
	for ( auto& e : m_EffectStateList ) m_pEffect->RenderUI( &e );
	m_pNotice->RenderUI( &m_NoticeState );
}

//---------------------------.
// 看板の表示の設定.
//---------------------------.
void CGoldenTimeUI::NoticeOpen()
{
	if ( m_NoticeState.IsDisp				) return;
	if ( CTime::GetIsGoldenTime() == false	) return;

	// 看板を表示させる.
	m_NoticeState.IsDisp	= true;
	m_IsOpen				= true;
	m_IsClose				= false;
}

//---------------------------.
// 看板を非表示の設定.
//---------------------------.
void CGoldenTimeUI::NoticeClose()
{
	if ( m_NoticeState.IsDisp == false	) return;
	if ( CTime::GetIsGoldenTime()		) return;

	// 看板を非表示にさせる.
	m_IsOpen	= false;
	m_IsClose	= true;
}

//---------------------------.
// 看板の表示の更新.
//---------------------------.
void CGoldenTimeUI::NoticeOpenUpdate()
{
	if ( m_NoticeState.IsDisp	== false ) return;
	if ( m_IsOpen				== false ) return;

	// サイズの取得.
	const SSize Size = m_pNotice->GetSpriteState().Disp;

	// 位置を更新.
	m_AnimTimeCnt += m_DeltaTime;
	const float NowPoint = CEasing::OutBounce( m_AnimTimeCnt, EASING_MAX );
	m_NoticeState.Transform.Position.y = Size.h * NowPoint;

	if ( m_AnimTimeCnt > EASING_MAX ) {
		m_AnimTimeCnt	= 0.0f;
		m_IsOpen		= false;
	}
}

//---------------------------.
// 看板の非表示の更新.
//---------------------------.
void CGoldenTimeUI::NoticeCloseUpdate()
{
	if ( m_NoticeState.IsDisp	== false ) return;
	if ( m_IsClose				== false ) return;

	// サイズの取得.
	const SSize Size = m_pNotice->GetSpriteState().Disp;

	// 位置を更新.
	m_AnimTimeCnt += m_DeltaTime;
	const float NowPoint = 1.0f - CEasing::InQuart( m_AnimTimeCnt, EASING_MAX );
	m_NoticeState.Transform.Position.y = Size.h * NowPoint;

	if ( m_AnimTimeCnt > EASING_MAX ) {
		m_AnimTimeCnt			= 0.0f;
		m_IsOpen				= false;
		m_NoticeState.IsDisp	= false;
	}
}

//---------------------------.
// エフェクトの再生.
//---------------------------.
void CGoldenTimeUI::EffectPlay()
{
	if ( m_NoticeState.IsDisp == false ) return;

	m_EffectPlayTimeCnt += m_DeltaTime;
	if ( m_EffectPlayTimeCnt < 0.0f ) return;
	m_EffectPlayTimeCnt = INIT_FLOAT;

	// 表示位置の設定.
	D3DXPOSITION3 DispPos = INIT_FLOAT3;
	DispPos.x = CRandom::GetRand( 0.0f, FWND_W );
	DispPos.y = CRandom::GetRand( 0.0f, 128.0f );
	if ( CRandom::Probability( 1, 2 ) ) DispPos.y = FWND_H - DispPos.y;

	// アニメーションNoの設定.
	const int No = CRandom::GetRand( 0, m_EffectStateBase.AnimState.AnimNumber );

	// 使用していないのがあれば使用する.
	for ( auto& s : m_EffectStateList ) {
		if ( s.IsDisp ) continue;

		// アニメーションを開始させる.
		s = m_EffectStateBase;
		s.Transform.Position	= DispPos;
		s.AnimState.AnimNumber	= No;
		return;
	}

	// 新しく作成.
	m_EffectStateList.emplace_back( m_EffectStateBase );
	m_EffectStateList.back().Transform.Position		= DispPos;
	m_EffectStateList.back().AnimState.AnimNumber	= No;
}

//---------------------------.
// エフェクトの更新.
//---------------------------.
void CGoldenTimeUI::EffectUpdate()
{
	// アニメーションの更新.
	for ( auto& e : m_EffectStateList ) {
		if ( e.IsDisp == false ) continue;

		// カウントの追加.
		e.UIAnimState.AnimCnt += EFFECT_CNT_MAX * m_DeltaTime;
		if ( e.UIAnimState.AnimCnt >= EFFECT_CNT_MAX ) {
			e.UIAnimState.AnimCnt	= EFFECT_CNT_MAX;
			e.IsDisp				= false;
			continue;
		}

		// アルファ値の設定.
		e.Color.w = sinf( Math::ToRadian( e.UIAnimState.AnimCnt ) );

		// 拡縮の設定.
		const float Scale = STransform::NORMAL_SCALE + EFFECT_ADD_SCALE * sinf( Math::ToRadian( e.UIAnimState.AnimCnt ) );
		e.Transform.Scale = { Scale, Scale, Scale };
	}

	// 使用していないエフェクトの削除.
	const int Size = static_cast<int>( m_EffectStateList.size() );
	for ( int i = Size - 1; i >= 0; --i ) {
		if ( m_EffectStateList[i].IsDisp ) return;
		m_EffectStateList.pop_back();
	}
}
