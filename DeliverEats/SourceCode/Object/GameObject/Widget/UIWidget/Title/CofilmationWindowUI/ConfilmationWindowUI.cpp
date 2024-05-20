#include "ConfilmationWindowUI.h"
#include "..\..\UIWidget.h"
#include "..\..\..\..\..\Score\Ranking\Ranking.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"
#include "..\..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"

namespace {
	enum enSpriteList : unsigned char {
		BackGround,	// 背景.
		Yubi,		// カーソル.
		Text,		// テキスト.
		Yes,		// 「はい」.
		No,			// 「いいえ」.

		Max
	}typedef ESpriteList;

	enum enTextNo : unsigned char {
		SelectYes,
		NotYes,
		SelectNo,
		NotNo
	} typedef ETextNo;

	constexpr int	CHOICE_STARTCHOICE	= static_cast<int>( EChoice::StartChoice );		// 最初の選択肢.
	constexpr int	CHOICE_ENDCHOICE	= static_cast<int>( EChoice::EndChoice );		// 最後の選択肢.
	constexpr int	CHOICE_MAX			= static_cast<int>( EChoice::Max );				// 最大要素数.

	constexpr int	SCALING_NONE		= static_cast<int>( EScaling::None );			// 待機中.
	constexpr int	SCALING_EXPAND		= static_cast<int>( EScaling::Expand );			// 拡大中.
	constexpr int	SCALING_SHRINK		= static_cast<int>( EScaling::Shrink );			// 縮小中.

	constexpr char	OPEN_SE[]			= "Open";										// ポーズを開く時のSE.
	constexpr char	CLOSE_SE[]			= "Close";										// ポーズを閉じる時のSE.
	constexpr char	CURSOR_MOVE_SE[]	= "CursorMove";									// カーソル移動時のSE.
	constexpr char	TRASH_SE[]			= "Trash";										// ベストスコアの削除時のSE.
}

CConfilmationWindowUI::CConfilmationWindowUI()
	: m_DeltaTime				( INIT_FLOAT )
	, m_IsRankingDelete			( false )
	, m_IsShowWindow			( false )
	, m_ScalingSwitch			( INIT_INT )
	, m_ConfilmationWindowScale	( INIT_FLOAT )
	, m_Choice					( INIT_INT )
	, m_AnimCnt					( INIT_FLOAT )
	, m_pSpriteList				( Max )
	, m_SpriteStateList			( Max, SSpriteRenderState() )
{
}

CConfilmationWindowUI::~CConfilmationWindowUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CConfilmationWindowUI::Init()
{
	m_IsShowWindow				= false;
	m_ScalingSwitch				= SCALING_NONE;
	m_ConfilmationWindowScale	= INIT_FLOAT;
	m_Choice					= INIT_INT;

	// 画像の設定.
	m_pSpriteList[BackGround]	= CSpriteResource::GetSprite( "back",	&m_SpriteStateList[BackGround]	);
	m_pSpriteList[Yubi]			= CSpriteResource::GetSprite( "Yubi"									);
	m_pSpriteList[Text]			= CSpriteResource::GetSprite( "Text1",	&m_SpriteStateList[Text]		);
	m_pSpriteList[Yes]			= CSpriteResource::GetSprite( "YesNo",	&m_SpriteStateList[Yes]			);
	m_pSpriteList[No]			= CSpriteResource::GetSprite( "YesNo",	&m_SpriteStateList[No]			);

	// 親の設定.
	m_SpriteStateList[Yubi].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Text].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Yes].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[No].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );

	// UIの位置設定.
	m_SpriteStateList[BackGround].Transform.Position.x	= FWND_W / 2.0f;
	m_SpriteStateList[BackGround].Transform.Position.y	= FWND_H / 2.0f;
	m_SpriteStateList[Text].Transform.Position.x		= 0.0f;
	m_SpriteStateList[Text].Transform.Position.y		= -100.0f;
	m_SpriteStateList[Text].AnimState.IsSetAnimNumber	= true;
	m_SpriteStateList[Text].AnimState.AnimNumber		= 0;
	m_SpriteStateList[Yes].Transform.Position.x			= 0.0f;
	m_SpriteStateList[Yes].Transform.Position.y			= 192.0f;
	m_SpriteStateList[Yes].AnimState.IsSetAnimNumber	= true;
	m_SpriteStateList[Yes].AnimState.AnimNumber			= SelectYes;
	m_SpriteStateList[No].Transform.Position.x			= 0.0f;
	m_SpriteStateList[No].Transform.Position.y			= 64.0f;
	m_SpriteStateList[No].AnimState.IsSetAnimNumber		= true;
	m_SpriteStateList[No].AnimState.AnimNumber			= NotNo;

	SwitchDisp( false );
	SetConfilmationScaleAndAlpha( m_ConfilmationWindowScale );

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CConfilmationWindowUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	if (m_IsShowWindow == false) return;

	// アニメーション用のカウントの加算.
	m_AnimCnt += Math::ToRadian( 80.0f ) * DeltaTime;
	if ( m_AnimCnt >= Math::RADIAN_MID ) m_AnimCnt -= Math::RADIAN_MID;

	// 選択肢を取得する.
	const SSize Size = m_pSpriteList[Yes]->GetSpriteState().Disp;
	switch ( m_Choice ) {
	// いいえを選択したとき.
	case CHOICE_STARTCHOICE:
		m_SpriteStateList[Yubi].Transform.Position		= m_SpriteStateList[No].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x	-= Size.w / 2.0f - 10.0f * sinf( m_AnimCnt );
		m_SpriteStateList[Yubi].Transform.Position.y	-= Size.h / 2.0f;
		m_SpriteStateList[Yes].AnimState.AnimNumber		= NotYes;
		m_SpriteStateList[No ].AnimState.AnimNumber		= SelectNo;
		break;
	// はいを選択したとき.
	case CHOICE_ENDCHOICE:
		m_SpriteStateList[Yubi].Transform.Position		= m_SpriteStateList[Yes].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x	-= Size.w / 2.0f - 10.0f * sinf( m_AnimCnt );
		m_SpriteStateList[Yubi].Transform.Position.y	-= Size.h / 2.0f;
		m_SpriteStateList[Yes].AnimState.AnimNumber		= SelectYes;
		m_SpriteStateList[No].AnimState.AnimNumber		= NotNo;
		break;
	default:
		break;
	}

	switch ( m_ScalingSwitch ) {
	case SCALING_EXPAND:	Expand();					break; // 拡大中の処理.
	case SCALING_SHRINK:	Shrink();					break; // 縮小中の処理.
	case SCALING_NONE:		UpdateConfilmationWindow(); break; // 表示中の処理.
	default:											break;
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CConfilmationWindowUI::Render()
{
	for ( int i = 0; i < Max;++i ) {
		m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
	}
}

//---------------------------.
//フェードイン開始.
//---------------------------.
void CConfilmationWindowUI::StartExpand( const bool IsRankingDelete )
{
	// 確認画面を表示.
	SwitchDisp( true );
	// 確認画面の拡大開始.
	m_ScalingSwitch = SCALING_EXPAND;
	// 確認画面を表示状態にする.
	m_IsShowWindow	= true;
	// ランキングの削除か保存.
	m_IsRankingDelete = IsRankingDelete;
	// テキストの更新.
	if ( m_IsRankingDelete ) m_SpriteStateList[Text].AnimState.AnimNumber = 0;
	else					 m_SpriteStateList[Text].AnimState.AnimNumber = 1;

	// SEの再生.
	CSoundManager::PlaySE( OPEN_SE );
}

//---------------------------.
// 表示/非表示の一括切り替え.
//	(true:表示/false:非表示).
//---------------------------.
void CConfilmationWindowUI::SwitchDisp( bool flag )
{
	for ( auto& ss : m_SpriteStateList ) ss.IsDisp = flag;
}

//---------------------------.
// 確認画面の拡縮を一括管理.
//---------------------------.
void CConfilmationWindowUI::SetConfilmationScaleAndAlpha( float value )
{
	D3DXVECTOR3 Scale = { value, value, 1.0f };
	m_SpriteStateList[BackGround].Transform.Scale = Scale;

	for ( auto& ss : m_SpriteStateList ) ss.Color.w = value;
}

//---------------------------.
// 拡大処理.
//---------------------------.
void CConfilmationWindowUI::Expand()
{
	// 確認画面を拡大する.
	m_ConfilmationWindowScale += 3.0f * m_DeltaTime;
	if ( m_ConfilmationWindowScale >= 1.0f ) {
		m_ConfilmationWindowScale = 1.0f;

		// 確認画面の拡大を完了する.
		m_ScalingSwitch = SCALING_NONE;
	}
	SetConfilmationScaleAndAlpha( m_ConfilmationWindowScale );
}

//---------------------------.
// 縮小処理.
//---------------------------.
void CConfilmationWindowUI::Shrink()
{
	// 確認画面を縮小する.
	m_ConfilmationWindowScale -= 3.0f * m_DeltaTime;
	if ( m_ConfilmationWindowScale <= 0.0f ) {
		m_ConfilmationWindowScale = 0.0f;

		// 確認画面の縮小を完了する.
		m_ScalingSwitch = SCALING_NONE;
		m_Choice = 0;

		// 確認画面を非表示状態にする.
		m_IsShowWindow = false;
		SwitchDisp( false );
	}
	SetConfilmationScaleAndAlpha( m_ConfilmationWindowScale );
}

//---------------------------.
// 確認画面表示中の処理.
//---------------------------.
void CConfilmationWindowUI::UpdateConfilmationWindow()
{
	// 選択.
	if ( CInput::IsKeyDown( "Up" ) ) {
		// カーソルを上げる.
		m_Choice--;
		if ( m_Choice <= -1 ) m_Choice = CHOICE_ENDCHOICE;

		// SEの再生.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Down" ) ) {
		// カーソルを下げる.
		m_Choice++;
		if ( m_Choice >= CHOICE_MAX ) m_Choice = CHOICE_STARTCHOICE;

		// SEの再生.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}

	if ( CInput::IsKeyDown( "Decision" ) ) {
		CSoundManager::PlaySE( "Decision" );
		CSoundManager::PlaySE( CLOSE_SE );

		// 選択肢を取得する.
		switch ( m_Choice ) {
		case CHOICE_STARTCHOICE:	// いいえを選択したとき.
			// 確認画面の後ろをフェードインする.

			// 確認画面の縮小開始.
			m_ScalingSwitch = SCALING_SHRINK;
			break;
		case CHOICE_ENDCHOICE:	// はいを選択したとき.
			// ランキングの削除.
			if ( m_IsRankingDelete ) {
				CRanking::Reset();

				// 確認画面の縮小開始.
				m_ScalingSwitch = SCALING_SHRINK;

				// SEの再生.
				CSoundManager::PlaySE( TRASH_SE );
				break;
			}
			// ゲームを終了する.
			PostQuitMessage( 0 );
			break;
		default:
			break;
		}
	}
}