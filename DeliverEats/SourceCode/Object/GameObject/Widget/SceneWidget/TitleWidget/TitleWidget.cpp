#include "TitleWidget.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Utility\Math\Math.h"
#include "..\..\..\..\..\Utility\Input\Input.h"
#include "..\..\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\UIWidget\Title\CofilmationWindowUI\ConfilmationWindowUI.h"
#include "..\..\UIWidget\GameMain\StatusBarUI\StatusBarUI.h"

namespace {
	constexpr int	CHOICE_GAMESTART 		= static_cast<int>( ETitleChoice::GameStart		);	// ゲーム開始.
	constexpr int	CHOICE_MINIGAME			= static_cast<int>( ETitleChoice::MiniGame		);	// ミニゲーム.
	constexpr int	CHOICE_PIZZAEDIT		= static_cast<int>( ETitleChoice::PizzaEdit		);	// ピザエディタ.
	constexpr int	CHOICE_RANKINGDELETE	= static_cast<int>( ETitleChoice::RankingDelete );	// ランキングの削除.
	constexpr int	CHOICE_GAMEEND			= static_cast<int>( ETitleChoice::GameEnd		);	// ゲーム終了.

	constexpr int	CHOICE_MAX				= static_cast<int>( ETitleChoice::Max			);	// 最大要素数.
	constexpr int	CHOICE_START			= static_cast<int>( ETitleChoice::Start			);	// 最大要素数.
	constexpr int	CHOICE_END				= static_cast<int>( ETitleChoice::End			);	// 最大要素数.

	constexpr char	CURSOR_MOVE_SE[]		= "CursorMove";										// カーソル移動時のSE.
}

CTitleWidget::CTitleWidget()
	: m_DeltaTime				( INIT_FLOAT )
	, m_Choice					( INIT_INT )
	, m_pText					( nullptr )
	, m_pConfilmationWindowUI	( nullptr )
	, m_pStatusBarUI			( nullptr )
	, m_TextState				()
{
}

CTitleWidget::~CTitleWidget()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CTitleWidget::Init()
{
	SetSceneName( ESceneList::Title );
	GetSpriteList();

	// ミニマップの初期化.
	CMiniMapUI::Init();

	m_pStatusBarUI = std::make_unique<CStatusBarUI>();
	m_pStatusBarUI->SetSpriteState( m_SpriteState );
	m_pStatusBarUI->SetIsCharging( true );
	if ( m_pStatusBarUI->Init() == false ) return false;

	m_pText		= CSpriteResource::GetSprite( "Text2",	&m_TextState	);
	m_TextState.AnimState.IsSetAnimNumber = true;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CTitleWidget::Update( const float & DeltaTime )
{
	if ( m_pConfilmationWindowUI->GetIsShowWindow() ) return;

	m_DeltaTime = DeltaTime;

	// ステータスバーの更新.
	m_pStatusBarUI->Update( DeltaTime );

	// 選択.
	if ( CInput::IsKeyDown( "Right" ) ) {
		m_Choice--;
		if ( m_Choice <= -1 ) m_Choice = CHOICE_END;

		// SEの再生.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Left" ) ) {
		m_Choice++;
		if ( m_Choice >= CHOICE_MAX ) m_Choice = CHOICE_START;

		// SEの再生.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}

	// テキストの変更.
	m_TextState.AnimState.AnimNumber = m_Choice;

	// 選択肢を取得する.
	switch ( m_Choice ) {
	// 「出勤する」が選択されたら.
	case CHOICE_GAMESTART:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 923.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 292.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 105.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 105.0f;
		break;
	// 「ジェットラベルを遊ぶ」が選択されたら.
	case CHOICE_MINIGAME:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 587.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 361.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 190.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 190.0f;
		break;
	// 「ピザを作る」が選択されたら.
	case CHOICE_PIZZAEDIT:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 514.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 456.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 155.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 155.0f;
		break;
	//「ランキングの削除」が選択されたら.
	case CHOICE_RANKINGDELETE:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 408.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 450.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 190.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 190.0f;
		break;
	//「寝る」が選択されたら.
	case CHOICE_GAMEEND:
		m_SpriteState["Yubi"][0].Transform.Position.x			= 210.0f;
		m_SpriteState["Yubi"][0].Transform.Position.y			= 440.0f;
		m_SpriteState["RightCursor"][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["LeftCursor" ][0].Transform.Position		= m_SpriteState["Yubi"][0].Transform.Position;
		m_SpriteState["RightCursor"][0].Transform.Position.x	+= 65.0f;
		m_SpriteState["LeftCursor" ][0].Transform.Position.x	-= 65.0f;
		break;
	default:
		break;
	}
	const SSize Size = m_SpriteList["Yubi"][0]->GetSpriteState().Disp;
	m_TextState.Transform.Position								= m_SpriteState["Yubi"][0].Transform.Position;
	m_TextState.Transform.Position.y							-= Size.h;
	m_SpriteState["RightCursor"][0].Transform.Position.y		= m_TextState.Transform.Position.y;
	m_SpriteState["LeftCursor" ][0].Transform.Position.y		= m_TextState.Transform.Position.y;
	m_SpriteState["RightCursor"][0].UIAnimState.StartTransform	= m_SpriteState["RightCursor"][0].Transform;
	m_SpriteState["LeftCursor" ][0].UIAnimState.StartTransform	= m_SpriteState["LeftCursor" ][0].Transform;
	m_SpriteState["Yubi"][0].UIAnimState.StartTransform			= m_SpriteState["Yubi"][0].Transform;

	// シーン遷移.
	if ( CInput::IsKeyDown( "Decision" ) ) {
		CSoundManager::PlaySE( "Decision" );

		// 選択肢を取得する.
		switch ( m_Choice ) {
		// 「出勤する」が選択されたら.
		case CHOICE_GAMESTART:
			// メインシーンに遷移.
			CSceneManager::SceneChange( ESceneList::GameMain, EFadeType::MaskFade );
			break;
		// 「ジェットラベルを遊ぶ」が選択されたら.
		case CHOICE_MINIGAME:
			// ミニゲームに遷移.
			CSceneManager::SceneChange( ESceneList::MiniGame, EFadeType::MaskFade );
			break;
		// 「ピザを作る」が選択されたら.
		case CHOICE_PIZZAEDIT:
			// ピザエディタに遷移.
			CSceneManager::SceneChange( ESceneList::PizzaEdit, EFadeType::MaskFade );
			break;
		//「ランキングの削除」が選択されたら.
		case CHOICE_RANKINGDELETE:
			// 確認画面の拡大を始める.
			if ( m_pConfilmationWindowUI == nullptr )return;
			m_pConfilmationWindowUI->StartExpand( true );
			break;
		//「寝る」が選択されたら.
		case CHOICE_GAMEEND:
			// 確認画面の拡大を始める.
			if ( m_pConfilmationWindowUI == nullptr )return;
			m_pConfilmationWindowUI->StartExpand();
			break;
		default:
			break;
		}
	}
}

//---------------------------.
// 追加で行う描画.
//---------------------------.
void CTitleWidget::LateAddRender()
{
	// テキストの描画.
	m_pText->RenderUI( &m_TextState );

	// ステータスバーの描画.
	m_pStatusBarUI->Render();
}
