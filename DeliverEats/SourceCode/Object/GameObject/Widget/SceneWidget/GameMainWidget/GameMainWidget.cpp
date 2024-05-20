#include "GameMainWidget.h"
#include "..\..\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\UIWidget\GameMain\HeightGaugeUI\HeightGaugeUI.h"
#include "..\..\UIWidget\GameMain\RucksackUI\RucksackUI.h"
#include "..\..\UIWidget\GameMain\FoodGaugeUI\FoodGaugeUI.h"
#include "..\..\UIWidget\GameMain\WarningUI\WarningUI.h"
#include "..\..\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\..\UIWidget\GameMain\StatusBarUI\StatusBarUI.h"
#include "..\..\UIWidget\GameMain\WindUI\WindUI.h"
#include "..\..\UIWidget\GameMain\NightUI\NightUI.h"
#include "..\..\UIWidget\GameMain\ShopInfoManagerUI\ShopInfoManagerUI.h"
#include "..\..\UIWidget\GameMain\TimeGaugeUI\TimeGaugeUI.h"
#include "..\..\UIWidget\GameMain\GoldenTimeUI\GoldenTimeUI.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Object\Score\Score.h"
#include "..\..\..\..\..\Object\Score\Ranking\Ranking.h"
#include "..\..\..\..\..\Object\GameObject\Actor\Wind\Wind.h"
#include "..\..\..\..\..\Utility\Input\Input.h"

namespace {
	enum enSpriteList : unsigned char {
		BackGround,	// 背景.
		Yubi,		// カーソル.
		Text,		// テキスト.
		Choice_1,	// 選択肢1.
		Choice_2,	// 選択肢2.

		Max
	}typedef ESpriteList;

	enum enTextNo : unsigned char {
		PauseText,
		SelectToBack,
		NotToBack,
		SelectToClear,
		NotToClear
	} typedef ETextNo;

	constexpr int	CHOICE_STARTCHOICE	= static_cast<int>( EChoice::StartChoice );	// 最初の選択肢.
	constexpr int	CHOICE_ENDCHOICE	= static_cast<int>( EChoice::EndChoice );	// 最後の選択肢.
	constexpr int	CHOICE_MAX			= static_cast<int>( EChoice::Max );			// 最大要素数.

	constexpr int	SCALING_NONE		= static_cast<int>( EScaling::None );		// 待機中.
	constexpr int	SCALING_EXPAND		= static_cast<int>( EScaling::Expand );		// 拡大中.
	constexpr int	SCALING_SHRINK		= static_cast<int>( EScaling::Shrink );		// 縮小中.

	constexpr char	OPEN_SE[]			= "Open";									// ポーズを開く時のSE.
	constexpr char	CLOSE_SE[]			= "Close";									// ポーズを閉じる時のSE.
	constexpr char	CURSOR_MOVE_SE[]	= "CursorMove";								// カーソル移動時のSE.
}

CGameMainWidget::CGameMainWidget()
	: m_DeltaTime			( INIT_FLOAT )
	, m_pHeightGaugeUI		( nullptr )
	, m_pRucksackUI			( nullptr )
	, m_pFoodGaugeUI		( nullptr )
	, m_pWarningUI			( nullptr )
	, m_pStatusBarUI		( nullptr )
	, m_pWindUI				( nullptr )
	, m_pNightUI			( nullptr )
	, m_pShopInfoUI			( nullptr )
	, m_pTimeGaugeUI		( nullptr )
	, m_pGoldenTimeUI		( nullptr )

	, m_Choice				( CHOICE_STARTCHOICE )

	, m_pSpriteList			( Max )
	, m_SpriteStateList		( Max )
	, m_ScalingSwitch		( INIT_INT )
	, m_PauseWindowScale	( INIT_FLOAT )
	, m_AnimCnt				( INIT_FLOAT )
{
}

CGameMainWidget::~CGameMainWidget()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CGameMainWidget::Init()
{
	SetSceneName( ESceneList::GameMain );
	GetSpriteList();

	// 通知の初期化.
	CNotificationUI::Init();

	m_pHeightGaugeUI	= std::make_unique<CHeightGaugeUI>();
	if ( m_pHeightGaugeUI->Init() == false ) return false;

	m_pRucksackUI		= std::make_unique<CRucksackUI>();
	if ( m_pRucksackUI->Init() == false ) return false;

	m_pFoodGaugeUI		= std::make_unique<CFoodGaugeUI>();
	m_pFoodGaugeUI->SetSpriteState( m_SpriteState );
	m_pFoodGaugeUI->SetSpriteList( m_SpriteList );
	if ( m_pFoodGaugeUI->Init() == false ) return false;

	m_pWarningUI		= std::make_unique<CWarningUI>();
	m_pWarningUI->SetSpriteState( m_SpriteState );
	if ( m_pWarningUI->Init() == false ) return false;

	m_pStatusBarUI		= std::make_unique<CStatusBarUI>();
	m_pStatusBarUI->SetSpriteState( m_SpriteState );
	if ( m_pStatusBarUI->Init() == false ) return false;

	m_pWindUI			= std::make_unique<CWindUI>();
	if ( m_pWindUI->Init()		== false ) return false;

	m_pNightUI			= std::make_unique<CNightUI>();
	if ( m_pNightUI->Init()		== false ) return false;

	m_pShopInfoUI		= std::make_unique<CShopInfoManagerUI>();
	if ( m_pShopInfoUI->Init()	== false ) return false;

	m_pTimeGaugeUI		= std::make_unique<CTimeGaugeUI>();
	if ( m_pTimeGaugeUI->Init()	== false ) return false;

	m_pGoldenTimeUI		= std::make_unique<CGoldenTimeUI>();
	if ( m_pGoldenTimeUI->Init()	== false ) return false;

	m_pSpriteList[BackGround	] = CSpriteResource::GetSprite( "back",			&m_SpriteStateList[BackGround]	);
	m_pSpriteList[Yubi			] =	CSpriteResource::GetSprite( "Yubi",			&m_SpriteStateList[Yubi]		);
	m_pSpriteList[Text			] =	CSpriteResource::GetSprite( "PauseText",	&m_SpriteStateList[Text]		);
	m_pSpriteList[Choice_1		] =	CSpriteResource::GetSprite( "PauseText",	&m_SpriteStateList[Choice_1]	);
	m_pSpriteList[Choice_2		] =	CSpriteResource::GetSprite( "PauseText",	&m_SpriteStateList[Choice_2]	);

	m_SpriteStateList[Yubi		].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Text		].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Choice_1	].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Choice_2	].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Yubi		].Transform.Scale			= { 1.0f, 1.0f, 1.0f };
	m_SpriteStateList[Text		].AnimState.AnimNumber		= PauseText;
	m_SpriteStateList[Choice_1	].AnimState.IsSetAnimNumber	= true;
	m_SpriteStateList[Choice_1	].AnimState.AnimNumber		= SelectToBack;
	m_SpriteStateList[Choice_2	].AnimState.IsSetAnimNumber = true;
	m_SpriteStateList[Choice_2	].AnimState.AnimNumber		= NotToClear;

	m_SpriteStateList[BackGround].Transform.Position.x = FWND_W / 2.0f;
	m_SpriteStateList[BackGround].Transform.Position.y = FWND_H / 2.0f;
	m_SpriteStateList[Text		].Transform.Position.y = -100.0f;
	m_SpriteStateList[Choice_1	].Transform.Position.y =   64.0f;
	m_SpriteStateList[Choice_2	].Transform.Position.y =  192.0f;

	m_Choice			= CHOICE_STARTCHOICE;
	m_ScalingSwitch		= SCALING_NONE;
	m_PauseWindowScale	= 0.0f;
	SetPauseScaleAndAlpha( m_PauseWindowScale );
	SwitchDisp( false );
	CSceneManager::SetIsPause( false );

	CScore::Init();

	CMiniMapUI::Init();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameMainWidget::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;
	const bool IsPause = CSceneManager::GetIsPause();

	PauseWindowUpdate();
	switch ( static_cast<EScaling>( m_ScalingSwitch ) ) {
	// 縮小中なら.
	case EScaling::Shrink:
		// フェードアウトの処理を行う.
		UpdateFadeOut();
		break;
	// 拡大中なら.
	case EScaling::Expand:
		// フェードインの処理を行う.
		UpdateFadeIn();
		break;
	// 拡縮が完了したなら.
	case EScaling::None:
		// ポーズ中なら.
		if ( IsPause ) UpdatePauseWindow();
		// ポーズ中でないなら.
		else UpdatePushPause();
		break;
	default:
		break;
	}

	// ポーズ中はアニメーションを止める.
	m_pRucksackUI->SetIsAnimPlay( !IsPause );
	// もしポーズ中なら以後の処理を行わない.
	if ( IsPause ) return;
	// 高度計の更新.
	m_pHeightGaugeUI->Update( DeltaTime );
	// リュックの更新
	m_pRucksackUI->Update( DeltaTime );
	// 受け渡しゲージの更新.
	m_pFoodGaugeUI->Update( DeltaTime );
	// 警告の更新.
	m_pWarningUI->Update( DeltaTime );
	// ステータスバーの更新.
	m_pStatusBarUI->Update( DeltaTime );
	// 風UIの更新.
	m_pWindUI->Update( DeltaTime );
	// 夜の更新.
	m_pNightUI->Update( DeltaTime );
	// 日給の更新.
	CScore::Update( DeltaTime );
	// 通知の更新.
	CNotificationUI::Update( DeltaTime );
	// お店の詳細情報の更新.
	m_pShopInfoUI->Update( DeltaTime );
	// 時間ゲージの更新.
	m_pTimeGaugeUI->Update( DeltaTime );
	// ゴールデンタイムの更新.
	m_pGoldenTimeUI->Update( DeltaTime );
}

//---------------------------.
// 表示/非表示の一括切替.
//---------------------------.
void CGameMainWidget::SwitchDisp( bool flag )
{
	// UIを表示/非表示する.
	m_SpriteStateList[BackGround].IsDisp = flag;
	m_SpriteStateList[Yubi		].IsDisp = flag;
	m_SpriteStateList[Text		].IsDisp = flag;
	m_SpriteStateList[Choice_1	].IsDisp = flag;
	m_SpriteStateList[Choice_2	].IsDisp = flag;
}

//---------------------------.
// ポーズ画面の拡縮を一括管理.
//---------------------------.
void CGameMainWidget::SetPauseScaleAndAlpha( float value )
{
	const D3DXVECTOR3 Scale = { value, value, 1.0f };

	m_SpriteStateList[BackGround].Transform.Scale	= Scale;
	m_SpriteStateList[Yubi		].Transform.Scale	= Scale;
	m_SpriteStateList[Text		].Transform.Scale	= Scale;
	m_SpriteStateList[Choice_1	].Transform.Scale	= Scale;
	m_SpriteStateList[Choice_2	].Transform.Scale	= Scale;

	m_SpriteStateList[BackGround].Color.w			= value;
	m_SpriteStateList[Yubi		].Color.w			= value;
	m_SpriteStateList[Text		].Color.w			= value;
	m_SpriteStateList[Choice_1	].Color.w			= value;
	m_SpriteStateList[Choice_2	].Color.w			= value;
}

//---------------------------.
// プレイヤーを設定.
//---------------------------.
void CGameMainWidget::SetPlayer( CPlayer* pPlayer )
{
	m_pHeightGaugeUI->SetPlayer( pPlayer );
	m_pRucksackUI->SetPlayer( pPlayer );
	m_pWarningUI->SetPlayer( pPlayer );
	m_pShopInfoUI->SetPlayer( pPlayer );
}

//---------------------------.
// UIエディタの前に追加で行う描画.
//---------------------------.
void CGameMainWidget::FastAddRender()
{
	// 夜の描画.
	m_pNightUI->Render();

	// 風UIの描画.
	m_pWindUI->Render();

	// ゴールデンタイムの描画.
	m_pGoldenTimeUI->Render();
}

//---------------------------.
// UIエディタの後に追加で行う描画.
//---------------------------.
void CGameMainWidget::LateAddRender()
{
	// 高度ゲージの描画.
	m_pHeightGaugeUI->Render();

	// リュックの描画.
	m_pRucksackUI->Render();

	// 日給の描画.
	CScore::Render();

	// 時間ゲージの描画.
	m_pTimeGaugeUI->Render();

	// ポーズ中か.
	if ( CSceneManager::GetIsPause() ) {
		// ランキングの描画.
		CRanking::RankingRender();

		// ステータスバーの描画.
		m_pStatusBarUI->Render();

		// ポーズの描画.
		for ( int i = 0; i < Max; ++i ) m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
		return;
	}
	
	// ミニマップの描画.
	CMiniMapUI::Render();

	// 通知の描画.
	CNotificationUI::Render();

	// お店の詳細情報の描画.
	m_pShopInfoUI->Render();

	// ステータスバーの描画.
	m_pStatusBarUI->Render();
	
	// ポーズの描画.
	for ( int i = 0; i < Max; ++i ) m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
}

//---------------------------.
// ポーズUIの更新.
//---------------------------.
void CGameMainWidget::PauseWindowUpdate()
{
	if ( m_SpriteStateList[BackGround].IsDisp == false ) return;

	// アニメーション用のカウントの加算.
	m_AnimCnt += Math::ToRadian( 80.0f ) * m_DeltaTime;
	if ( m_AnimCnt >= Math::RADIAN_MID ) m_AnimCnt -= Math::RADIAN_MID;

	// 選択肢の画像の変更.
	const SSize Size = m_pSpriteList[Choice_1]->GetSpriteState().Disp;
	switch ( m_Choice ) {
	case CHOICE_STARTCHOICE:	// 「戻る」を選択したとき.
		m_SpriteStateList[Choice_1].AnimState.AnimNumber	= SelectToBack;
		m_SpriteStateList[Choice_2].AnimState.AnimNumber	= NotToClear;
		m_SpriteStateList[Yubi].Transform.Position			= m_SpriteStateList[Choice_1].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x		-= Size.w / 2.0f + 32.0f - 16.0f * sinf( m_AnimCnt );
		break;
	case CHOICE_ENDCHOICE:		// 「帰宅する」を選択したとき.
		m_SpriteStateList[Choice_1].AnimState.AnimNumber	= NotToBack;
		m_SpriteStateList[Choice_2].AnimState.AnimNumber	= SelectToClear;
		m_SpriteStateList[Yubi].Transform.Position			= m_SpriteStateList[Choice_2].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x		-= Size.w / 2.0f + 32.0f - 16.0f * sinf( m_AnimCnt );
		break;
	}
}

//---------------------------.
// フェードインの処理.
//---------------------------.
void CGameMainWidget::UpdateFadeIn()
{
	// ポーズ画面を拡大しながらフェードインさせる.
	m_PauseWindowScale += 3.0f * m_DeltaTime;
	// ポーズ画面がフェードインしきったなら.
	if ( m_PauseWindowScale >= Color::ALPHA_MAX ) {
		m_PauseWindowScale	= Color::ALPHA_MAX;
		m_ScalingSwitch		= SCALING_NONE;
	}

	SetPauseScaleAndAlpha( m_PauseWindowScale );
}

//---------------------------.
// フェードアウトの処理.
//---------------------------.
void CGameMainWidget::UpdateFadeOut()
{
	// ポーズ画面を縮小しながらフェードアウトさせる.
	m_PauseWindowScale -= 3.0f * m_DeltaTime;
	// ポーズ画面がフェードアウトしきったなら.
	if ( m_PauseWindowScale <= Color::ALPHA_MIN ) {
		m_PauseWindowScale	= Color::ALPHA_MIN;
		m_ScalingSwitch		= SCALING_NONE;

		// 選択肢を初期化する.
		m_Choice = CHOICE_STARTCHOICE;

		// ポーズ画面を非表示にする.
		SwitchDisp( false );
	}

	SetPauseScaleAndAlpha( m_PauseWindowScale );
}

//---------------------------.
// ポーズ画面の処理.
//---------------------------.
void CGameMainWidget::UpdatePauseWindow()
{
	if ( CInput::IsKeyDown( "Start" ) ) {
		// 効果音を鳴らす.
		CSoundManager::PlaySE( CLOSE_SE );

		// ポーズ解除の準備.
		CSceneManager::SetIsPause( false );
		m_ScalingSwitch = SCALING_SHRINK;
	}

	// 選択.
	if ( CInput::IsKeyDown( "Up" ) ) {
		// カーソルを上げる.
		m_Choice--;
		if ( m_Choice < CHOICE_STARTCHOICE ) m_Choice = CHOICE_ENDCHOICE;

		// SEの再生.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Down" ) ) {
		// カーソルを下げる.
		m_Choice++;
		if ( m_Choice > CHOICE_ENDCHOICE ) m_Choice = CHOICE_STARTCHOICE;

		// SEの再生.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Decision" ) ) {
		CSoundManager::PlaySE( "Decision" );

		switch ( m_Choice ) {
		case CHOICE_STARTCHOICE:	// 「戻る」を選択したとき.
			// ポーズ解除する.
			CSceneManager::SetIsPause( false );
			m_ScalingSwitch = SCALING_SHRINK;

			// SEの再生.
			CSoundManager::PlaySE( CLOSE_SE );
			break;
		case CHOICE_ENDCHOICE:		// 「帰宅する」を選択したとき.
			// リザルトシーンに遷移する.
			CWind::StopWind();
			CScore::SetHomeEnd();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::MaskFade );
			break;
		}
	}
}

//---------------------------.
// ポーズが押されたときの処理.
//---------------------------.
void CGameMainWidget::UpdatePushPause()
{
	if ( CInput::IsKeyDown( "Start" ) == false ) return;

	// 効果音を鳴らす.
	CSoundManager::PlaySE( "Decision" );
	CSoundManager::PlaySE( OPEN_SE );

	// ポーズの準備.
	CSceneManager::SetIsPause( true );
	m_ScalingSwitch = SCALING_EXPAND;
	SwitchDisp( true );

	// 振動を停止させる.
	CXInput::SetVibration( 0.0f, 0.0f );
}
