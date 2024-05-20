#include "MiniGame.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\Random\Random.h"
#include "..\..\Utility\FileManager\FileManager.h"

namespace{
	constexpr int	PAUSE_TEXT_NUM				= 4;											// ポーズテキストの表示数.
	constexpr int	TEXT_NUM					= 8;											// テキストの表示数.
	constexpr int	TEXT_MAX					= 7;											// テキストの上限.
	constexpr float	TEXT_GAP					= 20.0f;										// テキストの隙間.
	constexpr float	TEXT_JUMP_Y					= 20.0f;										// テキストのジャンプさせる高さ.
	constexpr float	TEXT_ANIM_SPEED				= 5.0f;											// テキストのアニメーションの速度.
	constexpr int	CLOUD_SPOWN_TIME			= 50;											// 雲を召喚していく時間.
	constexpr float	CLOUD_SPOWN_POS_X			= 20.0f;										// 雲を召喚するX座標.
	constexpr float	CLOUD_DELETE_POS_X			= FWND_W - CLOUD_SPOWN_POS_X;					// 雲を削除するX座標.
	constexpr float	CLOUD_SPOWN_POS_Y_MIN		= 50.0f;										// 雲を召喚するY座標の最小値.
	constexpr float	CLOUD_SPOWN_POS_Y_MAX		= 620.0f;										// 雲を召喚するY座標の最大値.
	constexpr float	CLOUD_START_MOVE_SPPED		= 5.0f;											// 雲の開始時の移動速度.
	constexpr float	CLOUD_CENTER_MOVE_SPPED		= 15.0f;										// 雲の中間の移動速度.
	constexpr float	CLOUD_END_MOVE_SPPED		= 18.0f;										// 雲の最終の移動速度.
	constexpr float	CLOUD_RADIUS				= 8.0f;											// 雲の当たり判定の半径.
	constexpr float	CLOUD_AVERT_RADIUS			= 23.0f;										// 雲のギリギリを避けたか調べる当たり判定の半径.
	constexpr float GRAVITY_POWER				= 0.4f;											// 重力の強さ.
	constexpr float	PLAYER_POS_X				= 1000.0f;										// プレイヤーのX座標.
	constexpr float	PLAYER_FIRE_POS_X			= 37.0f;										// プレイヤーの炎のX座標.
	constexpr float	PLAYER_FIRE_POS_Y			= 11.0f;										// プレイヤーの炎のY座標.
	constexpr float	PLAYER_RADIUS				= 23.0f;										// プレイヤーの当たり判定の半径.
	constexpr float	PLAYER_AVERT_RADIUS			= 38.0f;										// プレイヤーのギリギリを避けたか調べる当たり判定の半径.
	constexpr float	PLAYER_ROT_SPEED			= 6.0f;											// プレイヤーの雲に当たった時のアニメーションの回転速度.
	constexpr int	COOL_TIME					= 10;											// 当たり判定のクールタイム.
	constexpr float	CEILING_POS_Y				= 100.0f;										// 天井のY座標.
	constexpr float	GROUND_POS_Y				= 550.0f;										// 地面のY座標.
	constexpr float	FLIGHT_ANIM_SIZE			= 20.0f;										// 飛行アニメーション用のサイズ.
	constexpr int	AVERT_ADD_SCORE				= 15;											// ギリギリを避けた時に追加するスコア.
	constexpr int	CHOICE_STARTCHOICE			= 0;											// 最初の選択肢.
	constexpr int	CHOICE_CENTERCHOICE			= 1;											// 真ん中の選択肢.
	constexpr int	CHOICE_ENDCHOICE			= 2;											// 最後の選択肢.
	constexpr char	HIT_SE[]					= "MiniGameHit";								// 雲に当たった時のSE.
	constexpr char	AVERT_SE[]					= "MiniGameAvert";								// ギリギリを避けた時のSE.
	constexpr char	SCORE_1000M_SE[]			= "MiniGameScore1000";							// 1000mごとに鳴るのSE.
	constexpr char	SCORE_5000M_SE[]			= "MiniGameScore5000";							// 5000mごとに鳴るのSE.
	constexpr char	SCORE_10000M_SE[]			= "MiniGameScore10000";							// 10000mごとに鳴るのSE.
	constexpr char	BESTSCORE_UPDATE_SE[]		= "MiniGameBestScoreUpdate";					// ベストスコアを更新した瞬間に鳴るSE.
	constexpr char	END_BESTSCORE_UPDATE_SE[]	= "MiniGameBestScoreUpdateEnd";					// ぶつかった時にベストスコアを更新していた時に鳴るSE.
	constexpr char	DATA_FILE_PATH[]			= "Data\\Parameter\\MiniGame\\MiniGame.bin";	// スコアデータのファイルパス.
	constexpr char	SCORE_LIST_FILE_PATH[]		= "Data\\Parameter\\MiniGame\\PlayData.csv";	// スコアリストデータのファイルパス.
	constexpr char	OPEN_SE[]					= "Open";										// ポーズを開く時のSE.
	constexpr char	CLOSE_SE[]					= "Close";										// ポーズを閉じる時のSE.
	constexpr char	CURSOR_MOVE_SE[]			= "CursorMove";									// カーソル移動時のSE.
	constexpr char	TRASH_SE[]					= "Trash";										// ベストスコアの削除時のSE.

	// スコアNo.
	enum enScoreNo : unsigned char {
		BestScore,		// ベストスコア.
		PreviousScore,	// 前回のスコア.

		Score_Max
	} typedef ScoreNo;

	// 自動操作の行動No
	enum enAutoPlayAction : unsigned char {
		MoveUp,			// 上昇.
		MoveUpSmall,	// 少し上昇.
		MoveWait,		// その場で待機.
		MoveDownSmall,	// 少し下降.
		MoveDown		// 下降.
	} typedef EAutoPlayAction;
}

CMiniGame::CMiniGame()
	: m_pText				( nullptr )
	, m_pPlayer				( nullptr )
	, m_pPlayerFire			( nullptr )
	, m_pPlayerIcon			( nullptr )
	, m_pCloud				( nullptr )
	, m_pLRTrigger			( nullptr )
	, m_pPauseButton		( nullptr )
	, m_pPauseBack			( nullptr )
	, m_pPauseText			( nullptr )
	, m_pPauseCursor		( nullptr )
	, m_pFont				( nullptr )
	, m_FontStateList		( Score_Max )
	, m_TextStateList		( TEXT_NUM )
	, m_CloudStateList		()
	, m_PauseTextStateList	( PAUSE_TEXT_NUM )
	, m_PauseTextStateBase	()
	, m_CloudStateBase		()
	, m_PlayerState			()
	, m_PlayerFireState		()
	, m_PlayerIconState		()
	, m_LRTriggerState		()
	, m_PauseButtonState	()
	, m_PauseBackState		()
	, m_PauseCursorState	()
	, m_SortCloudList		()
	, m_PlayData			()
	, m_DeltaTime			( INIT_FLOAT )
	, m_AnimTimeCnt			( INIT_FLOAT )
	, m_Power				( INIT_FLOAT )
	, m_CloudMoveSpeed		( CLOUD_START_MOVE_SPPED )
	, m_Score				( INIT_FLOAT )
	, m_PlayerFireCnt		( INIT_FLOAT )
	, m_AutoPlay			( INIT_FLOAT )
	, m_PreviousScore		( INIT_INT )
	, m_BestScore			( INIT_INT )
	, m_CloudTimeCnt		( INIT_INT )
	, m_TextAnimNo			( INIT_INT )
	, m_CoolTime			( INIT_INT )
	, m_Choice				( INIT_INT )
	, m_AutoPlayAction		( MoveWait )
	, m_IsPlay				( false )
	, m_IsCloudHit			( false )
	, m_LeftRotation		( false )
	, m_IsPause				( false )
	, m_IsPauseAnim			( false )
{
}

CMiniGame::~CMiniGame()
{
	// スコアの保存.
	int Data[3];
	Data[0] = m_BestScore;
	Data[1] = CRandom::GetRand( 0, 360 );
	Data[2] = static_cast<int>( m_BestScore * 2.319845f - 194350.726071f * sinf( static_cast<float>( Data[1] ) ) );
	FileManager::BinarySave( DATA_FILE_PATH, Data );

	// 背景の色の初期化.
	CDirectX11::InitBackColor();

	// プレイデータの保存.
	std::string ScoreText = "プレイ回数,=COUNT(A2:A100000),最大スコア,=MAX(A2:A100000),平均,=AVERAGE(A2:A100000)";
	for ( auto& d : m_PlayData ) ScoreText += "\n" + std::to_string( d );
	FileManager::TextSave( SCORE_LIST_FILE_PATH, ScoreText );
}

//---------------------------.
// 初期化.
//---------------------------.
bool CMiniGame::Init()
{
	// 画像の取得.
	m_pText			= CSpriteResource::GetSprite( "LoadText"	);
	m_pPlayer		= CSpriteResource::GetSprite( "LoadPlayer",			&m_PlayerState			);
	m_pPlayerFire	= CSpriteResource::GetSprite( "LoadPlayerFire",		&m_PlayerFireState		);
	m_pPlayerIcon	= CSpriteResource::GetSprite( "LoadPlayerIcon",		&m_PlayerIconState		);
	m_pCloud		= CSpriteResource::GetSprite( "LoadCloud",			&m_CloudStateBase		);
	m_pLRTrigger	= CSpriteResource::GetSprite( "LoadLRTrigger",		&m_LRTriggerState		);
	m_pPauseButton	= CSpriteResource::GetSprite( "LoadPauseButton",	&m_PauseButtonState		);
	m_pPauseBack	= CSpriteResource::GetSprite( "back",				&m_PauseBackState		);
	m_pPauseText	= CSpriteResource::GetSprite( "LoadPauseText",		&m_PauseTextStateBase	);
	m_pPauseCursor	= CSpriteResource::GetSprite( "Yubi",				&m_PauseCursorState		);
	m_pFont			= CFontResource::GetFont(	  "NasuM",				&m_FontStateList		);

	// ポーズ画面の設定.
	m_PauseTextStateBase.Transform.AttachParent( &m_PauseBackState.Transform );
	m_PauseCursorState.Transform.AttachParent(	 &m_PauseBackState.Transform );
	m_PauseBackState.Transform.Position.x				= FWND_W / 2.0f;
	m_PauseBackState.Transform.Position.y				= FWND_H / 2.0f;
	m_PauseBackState.Transform.Scale					= { 0.0f, 0.0f, 0.0f };
	m_PauseBackState.Color.w							= Color::ALPHA_MIN;
	m_PauseTextStateBase.AnimState.IsSetAnimNumber		= true;
	m_PauseTextStateBase.Transform.Position.y			= -166.0f;
	m_PauseTextStateBase.Color.w						= Color::ALPHA_MIN;
	for ( int i = 0; i < PAUSE_TEXT_NUM; ++i ) {
		m_PauseTextStateList[i]							= m_PauseTextStateBase;
		m_PauseTextStateList[i].Transform.Position.y	+= 110 * i;
		m_PauseTextStateList[i].AnimState.AnimNumber	= 2 * i;
	}

	// ポーズのカーソルの設定.
	m_PauseCursorState.SceneName = "Menu";
	m_PauseCursorState.UIAnimState.StartTransform.Position		= m_PauseTextStateList[1].Transform.Position;
	m_PauseCursorState.UIAnimState.StartTransform.Position.x	= -250.0f;
	m_PauseCursorState.Transform.Rotation.z						= 0.0f;

	// スコアの取得.
	int Out[3];
	FileManager::BinaryLoad( DATA_FILE_PATH, Out );
	const int c = static_cast<int>( Out[0] * 2.319845f - 194350.726071f * sinf( static_cast<float>( Out[1] ) ) );
	if ( c == Out[2] ) m_BestScore = Out[0];

	// フォントの設定.
	m_FontStateList[BestScore].TextAlign				= ETextAlign::Left;
	m_FontStateList[BestScore].Transform.Position.x		= TEXT_GAP;
	m_FontStateList[BestScore].Transform.Position.y		= TEXT_GAP;
	m_FontStateList[BestScore].IsOutLine				= true;
	m_FontStateList[BestScore].OutLineColor				= Color4::Black;
	m_FontStateList[BestScore].OutLineSize				= 2.5f;
	m_FontStateList[PreviousScore].TextAlign			= ETextAlign::Right;
	m_FontStateList[PreviousScore].Transform.Position.x = FWND_W - TEXT_GAP;
	m_FontStateList[PreviousScore].Transform.Position.y = TEXT_GAP;
	m_FontStateList[PreviousScore].IsOutLine			= true;
	m_FontStateList[PreviousScore].OutLineColor			= Color4::Black;
	m_FontStateList[PreviousScore].OutLineSize			= 2.5f;

	// 文字の設定.
	const SSize&				TextSize	= m_pText->GetSpriteState().Disp;
	const SSpriteRenderState&	TextState	= m_pText->GetRenderState();
	for ( int i = 0; i < TEXT_NUM; ++i ) {
		const int No = 11 + ( i > TEXT_MAX ? TEXT_MAX : i );
		m_TextStateList[i] = TextState;
		m_TextStateList[i].AnimState.IsSetAnimNumber	= true;
		m_TextStateList[i].AnimState.AnimNumber			= No;
		m_TextStateList[i].Transform.Position.x			= FWND_W - TextSize.w * ( TEXT_NUM - i ) - TEXT_GAP;
		m_TextStateList[i].Transform.Position.y			= FWND_H - TextSize.h - TEXT_GAP;
	}

	// 操作説明のテキストの位置の設定.
	const SSize LRTriggerSize = m_pLRTrigger->GetSpriteState().Disp;
	m_LRTriggerState.Transform.Position.x			= TEXT_GAP;
	m_LRTriggerState.Transform.Position.y			= FWND_H - TEXT_GAP;
	m_PauseButtonState.Transform.Position.x			= m_LRTriggerState.Transform.Position.x + LRTriggerSize.w + 40.0f;
	m_PauseButtonState.Transform.Position.y			= FWND_H - TEXT_GAP;
	m_PauseButtonState.AnimState.IsSetAnimNumber	= true;

	// 雲の設定.
	const SSize& CloudSize = m_pCloud->GetSpriteState().Disp;
	m_CloudStateBase.AnimState.IsSetAnimNumber = true;
	m_CloudStateBase.RenderArea = {
		CLOUD_SPOWN_POS_X + CloudSize.w / 2.0f,
		0.0f,
		CLOUD_DELETE_POS_X - CLOUD_SPOWN_POS_X - CloudSize.w,
		FWND_H
	};

	// プレイヤーの設定.
	m_PlayerState.Transform.Position.x		= PLAYER_POS_X;
	m_PlayerState.Transform.Position.y		= GROUND_POS_Y;
	m_PlayerFireState.Transform.AddParent( &m_PlayerState.Transform );
	m_PlayerFireState.Transform.Position.x	= PLAYER_FIRE_POS_X;
	m_PlayerFireState.Transform.Position.y	= PLAYER_FIRE_POS_Y;
	
	// プレイヤーのアイコンの設定.
	m_PlayerIconState.Transform.Position.x = TEXT_GAP;
	m_PlayerIconState.Transform.Position.y = m_PlayerState.Transform.Position.y;

	// BGM名の設定.
	m_BGMName = "MiniGame";

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// 背景の色の設定.
	CDirectX11::SetBackColor( Color4::Black );

	// プレイデータの取得.
	std::vector<std::string> Data = FileManager::TextLoad( SCORE_LIST_FILE_PATH );
	const int Size = static_cast<int>( Data.size() ) - 6;
	m_PlayData.resize( Size );
	for ( int i = 0; i < Size; ++i ) m_PlayData[i] = std::stoi( Data[i + 6] );
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CMiniGame::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	if ( m_IsPause ) {
		PauseOpen();				// ポーズ画面を開く.
		PauseUpdate();				// ポーズ中の更新.
	}
	else {
		CloudSpown();				// 雲の召喚.
		CloudMove();				// 雲の移動.
		CloudDelete();				// 使用していない雲の削除.
		TextAnimation();			// テキストのアニメーション.
		PlayerJump();				// プレイヤーのジャンプ.
		PlayerFall();				// プレイヤーの落下.
		GroundCollision();			// 地面の当たり判定.
		CeilingCollision();			// 天井の当たり判定.
		CloudCollision();			// 雲の当たり判定.
		PlayerCloudHitAnimation();	// プレイヤーの雲に当たった時のアニメーション.
		PlayerPositionUpdate();		// プレイヤーの座標の更新.
		PlayerFireUpdate();			// プレイヤーの炎の更新.
		ScoreUpdate();				// スコアの更新.
		TextUpdate();				// テキストの更新.
		PauseButtonUpdate();		// ポーズのボタンの更新.
		SoundUpdate();				// サウンドの更新.
		CoolTimeUpdate();			// クールタイムの更新.
		AutoPlayUpdate();			// 自動操作の更新.

		PauseClose();				// ポーズ画面を閉じる.
		MovePause();				// ポーズに移動.
	}
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CMiniGame::SpriteUIRender()
{
	for ( auto& t : m_TextStateList		) m_pText->RenderUI( &t );
	m_pLRTrigger->RenderUI( &m_LRTriggerState );
	m_pPauseButton->RenderUI( &m_PauseButtonState );
	m_pPlayer->RenderUI( &m_PlayerState );
	m_pPlayerFire->RenderUI( &m_PlayerFireState );
	m_pPlayerIcon->RenderUI( &m_PlayerIconState );
	for ( auto& c : m_CloudStateList	) m_pCloud->RenderUI( &c );
	for ( auto& t : m_FontStateList		) m_pFont->RenderUI( &t );

	// ポーズの描画.
	if ( m_IsPauseAnim || m_IsPause ) {
		m_pPauseBack->RenderUI( &m_PauseBackState );
		for ( auto& t : m_PauseTextStateList ) m_pPauseText->RenderUI( &t );
		m_pPauseCursor->RenderUI( &m_PauseCursorState );
	}
}

//---------------------------.
// 雲の召喚.
//---------------------------.
void CMiniGame::CloudSpown()
{
	// 雲を召喚するか.
	m_CloudTimeCnt++;
	if ( m_CloudTimeCnt <= CLOUD_SPOWN_TIME / ( m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED ) ) return;
	m_CloudTimeCnt = 0;

	// 雲を召喚する.
	const int NoMax		= m_CloudStateBase.AnimState.AnimNumberMax - 1;
	const int SpownMax	= CRandom::Probability( 1, 2 ) ? 2 : CRandom::GetRand( 3, 4 );
	const int SpownNum	= CRandom::GetRand( 2, SpownMax );
	for ( int i = 0; i < SpownNum; ++i ) {
		// 使用していないのがあれば使用する.
		for ( auto& c : m_CloudStateList ) {
			if ( c.IsDisp ) continue;

			// 雲を召喚する.
			c.IsDisp				= true;
			c.Color					= Color4::White;
			c.Transform.Position.x	= CLOUD_SPOWN_POS_X;
			c.Transform.Position.y	= CRandom::GetRand( CLOUD_SPOWN_POS_Y_MIN, CLOUD_SPOWN_POS_Y_MAX );
			c.AnimState.AnimNumber	= CRandom::GetRand( 0, NoMax );
			return;
		}

		// 新しく作成.
		m_CloudStateList.emplace_back( m_CloudStateBase );
		m_CloudStateList.back().Transform.Position.x	= CLOUD_SPOWN_POS_X;
		m_CloudStateList.back().Transform.Position.y	= CRandom::GetRand( CLOUD_SPOWN_POS_Y_MIN, CLOUD_SPOWN_POS_Y_MAX );
		m_CloudStateList.back().AnimState.AnimNumber	= CRandom::GetRand( 0, NoMax );
	}
}

//---------------------------.
// 雲の移動.
//---------------------------.
void CMiniGame::CloudMove()
{
	// 雲の移動.
	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// 雲の移動.
		c.Transform.Position.x += m_CloudMoveSpeed;
		c.Color					= Color4::White;

		// 移動が終了したか.
		if ( c.Transform.Position.x >= CLOUD_DELETE_POS_X ) {
			c.IsDisp = false;
		}
	}

	// 雲の移動速度を上げる.
	if ( m_IsPlay == false ) return;
	if ( m_CloudMoveSpeed < CLOUD_CENTER_MOVE_SPPED ) {
		m_CloudMoveSpeed += 0.01f;
		if ( m_CloudMoveSpeed >= CLOUD_CENTER_MOVE_SPPED ) 
			m_CloudMoveSpeed = CLOUD_CENTER_MOVE_SPPED;
	}
	else if ( m_CloudMoveSpeed < CLOUD_END_MOVE_SPPED ) {
		if ( static_cast<int>( m_Score ) % 100 != 0 ) return;
		m_CloudMoveSpeed += 0.025f;
		if ( m_CloudMoveSpeed >= CLOUD_END_MOVE_SPPED ) m_CloudMoveSpeed = CLOUD_END_MOVE_SPPED;
	}
}

//---------------------------.
// 使用していない雲の削除.
//---------------------------.
void CMiniGame::CloudDelete()
{
	const int Size = static_cast<int>( m_CloudStateList.size() );
	for ( int i = Size - 1; i >= 0; i-- ) {
		if ( m_CloudStateList[i].IsDisp ) break;
		m_CloudStateList.pop_back();
	}
}

//---------------------------.
// テキストのアニメーション.
//---------------------------.
void CMiniGame::TextAnimation()
{
	// 文字のアニメーションの更新.
	m_AnimTimeCnt += TEXT_ANIM_SPEED * ( m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED );

	// 位置の更新.
	const SSize Size = m_pText->GetSpriteState().Disp;
	m_TextStateList[m_TextAnimNo].Transform.Position.y = 
		FWND_H - Size.h - TEXT_GAP - TEXT_JUMP_Y * sinf( Math::ToRadian( m_AnimTimeCnt ) );
	 
	// 次の文字に移動.
	if ( m_AnimTimeCnt >= Math::DEGREE_MID ) {
		m_AnimTimeCnt = 0.0f;
		m_TextAnimNo++;
		if ( m_TextAnimNo >= TEXT_NUM ) m_TextAnimNo = 0;
	}
}

//---------------------------.
// プレイヤーのジャンプ.
//---------------------------.
void CMiniGame::PlayerJump()
{
	if ( m_IsCloudHit ) return;

	// プレイヤーのジャンプ.
	if ( m_PlayerState.Transform.Position.y <= CEILING_POS_Y + FLIGHT_ANIM_SIZE ) return;

	// トリガーの押し具合の取得.
	const float LTrigger = CXInput::GetLTrigger();
	const float RTrigger = CXInput::GetRTrigger();

	// プレイヤーのパワーの更新.
	m_Power -= ( LTrigger + RTrigger + m_AutoPlay + 1.1f ) / 5.0f;

	// 操作中に変更.
	if ( m_IsPlay ) return;
	if ( LTrigger > 0.0f || RTrigger > 0.0f ) {
		m_IsPlay	= true;
		m_AutoPlay	= INIT_FLOAT;

		// スコアの初期化.
		m_PreviousScore = static_cast<int>( std::ceil( m_Score ) );
		m_Score			= INIT_FLOAT;
	}
}

//---------------------------.
// プレイヤーの落下.
//---------------------------.
void CMiniGame::PlayerFall()
{
	// プレイヤーの重力.
	if ( m_PlayerState.Transform.Position.y >= GROUND_POS_Y - FLIGHT_ANIM_SIZE ) return;
	
	// プレイヤーを落下させていく.
	m_Power += GRAVITY_POWER;
}

//---------------------------.
// プレイヤーの雲に当たった時のアニメーション.
//---------------------------.
void CMiniGame::PlayerCloudHitAnimation()
{
	if ( m_IsCloudHit == false ) return;

	// プレイヤーを回転させる.
	const float RotSpeed = m_LeftRotation ? -PLAYER_ROT_SPEED : PLAYER_ROT_SPEED;
	m_PlayerState.Transform.Rotation.z += Math::ToRadian( RotSpeed );

	// アニメーション終了.
	if ( ( m_PlayerState.Transform.Rotation.z <= -Math::RADIAN_MAX ) || 
		 ( m_PlayerState.Transform.Rotation.z >=  Math::RADIAN_MAX ) )
	{
		m_PlayerState.Transform.Rotation.z	= 0.0f;
		m_IsCloudHit						= false;
		m_LeftRotation						= false;
		m_CoolTime							= COOL_TIME;
	}
}

//---------------------------.
// プレイヤーの座標の更新.
//---------------------------.
void CMiniGame::PlayerPositionUpdate()
{
	m_PlayerState.Transform.Position.y		+= m_Power;
	m_PlayerIconState.Transform.Position.y	 = m_PlayerState.Transform.Position.y;
}

//---------------------------.
// プレイヤーの炎の更新.
//---------------------------.
void CMiniGame::PlayerFireUpdate()
{
	// アニメーション用のカウントの加算.
	m_PlayerFireCnt += Math::ToRadian( 3.0f );
	if ( m_PlayerFireCnt >= Math::RADIAN_MID ) m_PlayerFireCnt = 0.0f;

	// 炎の拡縮.
	const float Scale = STransform::NORMAL_SCALE - 0.3f * sinf( m_PlayerFireCnt );
	m_PlayerFireState.Transform.Scale = { Scale, Scale, Scale };

	// 色の設定.
	if ( m_IsPlay ) {
		m_PlayerState.Color		= Color4::White;
		m_PlayerFireState.Color = Color4::White;
	}
	else {
		m_PlayerState.Color		= Color4::Gray;
		m_PlayerFireState.Color = Color4::Darkgray;
	}
}

//---------------------------.
// 地面の当たり判定.
//---------------------------.
void CMiniGame::GroundCollision()
{
	if ( m_PlayerState.Transform.Position.y < GROUND_POS_Y ) return;
	
	// 地面の位置に止める.
	m_PlayerState.Transform.Position.y	= GROUND_POS_Y;
	if ( m_Power > 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// 天井の当たり判定.
//---------------------------.
void CMiniGame::CeilingCollision()
{
	if ( m_PlayerState.Transform.Position.y > CEILING_POS_Y ) return;

	// 天井の位置で止める.
	m_PlayerState.Transform.Position.y = CEILING_POS_Y;
	if ( m_Power < 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// 雲の当たり判定.
//---------------------------.
void CMiniGame::CloudCollision()
{
	if ( m_IsCloudHit		) return;
	if ( m_CoolTime > 0		) return;

	// プレイヤーの座標の取得.
	const D3DXPOSITION3 PPos = m_PlayerState.Transform.Position;

	// 雲と当たっているか調べる.
	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// 雲の座標の取得.
		const D3DXPOSITION3& CPos = c.Transform.Position;

		// 円通しが当たっているか調べる.
		const float dx	= PPos.x - CPos.x;
		const float dy	= PPos.y - CPos.y;
		const float dr	= dx * dx + dy * dy;
		const float ar	= PLAYER_RADIUS			+ CLOUD_RADIUS;
		const float br	= PLAYER_AVERT_RADIUS	+ CLOUD_AVERT_RADIUS;
		const float dla = ar * ar;
		const float dlb = br * br;

		// 当たっているか.
		if ( dr < dla ) {
			m_IsCloudHit = true;
			if ( CPos.y >= PPos.y ) m_LeftRotation = true;

			// プレイデータの追加.
			const int CheckScore = static_cast<int>( std::ceil( m_Score ) );
			if ( m_IsPlay ) m_PlayData.emplace_back( CheckScore );

			// 操作中では無くす.
			m_IsPlay	= false;
			m_AutoPlay	= 0.0f;

			// 雲の速度を遅くする.
			m_CloudMoveSpeed	= CLOUD_START_MOVE_SPPED;

			// ベストスコアを更新したか.
			if ( CheckScore > m_BestScore ) {
				m_BestScore = CheckScore;

				// スコアの保存.
				int Data[1];
				Data[0] = m_BestScore;
				FileManager::BinarySave( DATA_FILE_PATH, Data );

				// SEの再生.
				CSoundManager::PlaySE( END_BESTSCORE_UPDATE_SE );
			}

			// SEの再生.
			CSoundManager::PlaySE( HIT_SE );
			return;
		}

		// ギリギリを避けたか.
		if ( dr < dlb && m_IsPlay ) {
			// スコアを少し増やす.
			m_Score += AVERT_ADD_SCORE;

			// SEの再生.
			CSoundManager::PlaySE( AVERT_SE, 0.05f );
		}
	}
}

//---------------------------.
// ポーズのボタンのテキストの更新.
//---------------------------.
void CMiniGame::PauseButtonUpdate()
{
	// テキストの見た目の変更.
	m_PauseButtonState.AnimState.AnimNumber = m_IsPlay ? 1 : 0;
}

//---------------------------.
// サウンドの更新.
//---------------------------.
void CMiniGame::SoundUpdate()
{
	// BGMの再生.
	CSoundManager::PlayBGM( m_BGMName );
}

//---------------------------.
// スコアの更新.
//---------------------------.
void CMiniGame::ScoreUpdate()
{
	if ( m_IsPlay		== false) return;
	if ( m_IsCloudHit	== true ) return;
	if ( m_CoolTime		> 0		) return;

	// スコアの加算.
	const int	OldCheckScore		= static_cast<int>( std::ceil( m_Score ) );
	const int	OldScore10000Cnt	= static_cast<int>( OldCheckScore / 10000.0f );
	const int	OldScore5000Cnt		= static_cast<int>( OldCheckScore / 5000.0f  );
	const int	OldScore1000Cnt		= static_cast<int>( OldCheckScore / 1000.0f  );
	const bool	OldBestscoreUpdate	= OldCheckScore > m_BestScore;
	m_Score += m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED;

	// ベストスコアを更新したか.
	const int	NewCheckScore		= static_cast<int>( std::ceil( m_Score ) );
	const bool	NewBestscoreUpdate	= NewCheckScore > m_BestScore;
	if ( OldBestscoreUpdate == false && NewBestscoreUpdate ) {
		CSoundManager::PlaySE( BESTSCORE_UPDATE_SE );
	}

	// SEの再生.
	const int NewScore10000Cnt	= static_cast<int>( NewCheckScore / 10000.0f );
	const int NewScore5000Cnt	= static_cast<int>( NewCheckScore / 5000.0f  );
	const int NewScore1000Cnt	= static_cast<int>( NewCheckScore / 1000.0f  );
	if ( OldScore10000Cnt != NewScore10000Cnt ) {
		CSoundManager::PlaySE( SCORE_10000M_SE );
		return;
	}
	if ( OldScore5000Cnt != NewScore5000Cnt ) {
		CSoundManager::PlaySE( SCORE_5000M_SE );
		return;
	}
	if ( OldScore1000Cnt != NewScore1000Cnt ) {
		CSoundManager::PlaySE( SCORE_1000M_SE );
		return;
	}
}

//---------------------------.
// テキストの更新.
//---------------------------.
void CMiniGame::TextUpdate()
{
	// スコアのテキストの作成.
	std::string BestScoreText		= std::to_string( m_BestScore );
	std::string PreviousScoreText	= std::to_string( m_PreviousScore );
	std::string NowScoreText		= std::to_string( static_cast<int>( std::ceil( m_Score ) ) );

	// テキストの長さの取得.
	const int PTextLength	= static_cast<int>( PreviousScoreText.length()	);
	const int NTextLength	= static_cast<int>( NowScoreText.length()		);

	// テキストの調整.
	const size_t TextLength = PTextLength > NTextLength ? PTextLength : NTextLength;
	PreviousScoreText		= StringConversion::to_LeftPaddingString( PreviousScoreText, static_cast<int>( TextLength ) );
	NowScoreText			= StringConversion::to_LeftPaddingString( NowScoreText,		 static_cast<int>( TextLength ) );

	// テキストの更新.
	m_FontStateList[BestScore].Text =
		"ベストスコア:" + BestScoreText + "m";
	m_FontStateList[PreviousScore].Text =
		"前回のスコア:" + PreviousScoreText + "m\n"
		"現在のスコア:" + NowScoreText + "m";

	// 自動操作の情報をテキストに追加.
#ifdef _DEBUG
	m_FontStateList[BestScore].Text +=
		"\n-------------\n"
		"パワー:" + std::to_string( m_Power ) + "\n"
		"操　作:" + std::to_string( m_AutoPlay ) + "\n"
		"行　動:" + std::to_string( m_AutoPlayAction );
#endif // #ifdef _DEBUG.
}

//---------------------------.
// クールタイムの更新.
//---------------------------.
void CMiniGame::CoolTimeUpdate()
{
	if ( m_CoolTime <= 0 ) return;

	// クールタイムを減らす.
	m_CoolTime--;
	if ( m_CoolTime <= 0 ) m_CoolTime = 0;
}

//---------------------------.
// 自動操作の更新.
//---------------------------.
void CMiniGame::AutoPlayUpdate()
{
	if ( m_IsPlay					) return;
	if ( m_IsCloudHit				) return;
	if ( m_CloudStateList.empty()	) return;
	
	CreateSortCloudList();		// 雲のソートリストの作成.
	AutoPlayActionUpdate();		// 自動操作の行動の更新.
	AutoPlayOutUpdate();		// 自動操作の出力の更新.
}

//---------------------------.
// 雲のソートリストの作成.
//---------------------------.
void CMiniGame::CreateSortCloudList()
{
	// プレイヤーの座標の取得.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// 雲をプレイヤーに近い順に並べる.
	m_SortCloudList.clear();
	for ( auto& c : m_CloudStateList ) {
		if ( c.Transform.Position.x >= PlayerPos.x - PLAYER_RADIUS * 1.5f ) continue;
		const D3DXVECTOR3&	Vector		= c.Transform.Position - PlayerPos;
		const float			Distance	= D3DXVec3Length( &Vector );
		m_SortCloudList.emplace_back( std::make_pair( Distance, &c ) );
	}
	std::sort( m_SortCloudList.begin(), m_SortCloudList.end() );
}

//---------------------------.
// 自動操作の行動の更新.
//---------------------------.
void CMiniGame::AutoPlayActionUpdate()
{
	// プレイヤーの座標の取得.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// 評価する雲の情報の取得.
	const D3DXPOSITION3& Pos		= m_SortCloudList[0].second->Transform.Position;
	const float			 CheckUY	= Pos.y - CLOUD_RADIUS - PLAYER_RADIUS * 1.5f;
	const float			 CheckDY	= Pos.y + CLOUD_RADIUS + PLAYER_RADIUS * 1.5f;
	bool				 IsUp		= PlayerPos.y <= CheckUY ? true : false;

	// 評価する雲を赤色にする.
#ifdef _DEBUG
	m_SortCloudList[0].second->Color = Color4::Red;
#endif	// #ifdef _DEBUG.

	// 上下に分けて評価する.
	bool IsMoveEnd = false;
	for ( int i = 0; i < 2; ++i ) {
		// 上に移動出来そうか.
		if ( IsUp ) {
			const bool IsMoveOK = UpMoveCheck( 0 );

			// 上に移動.
			if ( IsMoveOK ) {
				if (	  PlayerPos.y > CheckUY - PLAYER_RADIUS			) m_AutoPlayAction = MoveUp;
				else if ( PlayerPos.y < CheckUY - PLAYER_RADIUS * 2.0f	) m_AutoPlayAction = MoveWait;
				else													  m_AutoPlayAction = MoveUpSmall;
				IsMoveEnd = true;
			}
		}

		// 下に移動出来そうか.
		if ( IsUp == false ) {
			const bool IsMoveOK = DownMoveCheck( 0 );

			// 下に移動.
			if ( IsMoveOK ) {
				if ( PlayerPos.y < CheckDY + PLAYER_RADIUS * 2.0f ) m_AutoPlayAction = MoveDown;
				else												m_AutoPlayAction = MoveDownSmall;
				IsMoveEnd = true;;
			}
		}

		// 移動方向を反転させて再評価.
		if ( IsMoveEnd ) break;
		IsUp = !IsUp;
	}
}

//---------------------------.
// 上に移動できるか判断.
//---------------------------.
bool CMiniGame::UpMoveCheck( const int CloudNo )
{
	// 位置の取得.
	const D3DXPOSITION3& Pos	= m_SortCloudList[CloudNo].second->Transform.Position;
	const float			 CheckY	= Pos.y - CLOUD_RADIUS - PLAYER_RADIUS * 1.5f;

	// プレイヤーの座標の取得.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// 上に移動できるか.
	const float DeadCeiling = CEILING_POS_Y + PLAYER_RADIUS;
	if ( CheckY <= DeadCeiling ) {
#ifdef _DEBUG
		m_SortCloudList[CloudNo].second->Color.w = 0.5f;
#endif	// #ifdef _DEBUG.
		return false;
	}

	// 他の雲に近すぎないか確認.
	const int ListSize	= static_cast<int>( m_SortCloudList.size() );
	for ( int CheckNo = 0; CheckNo < ListSize; ++CheckNo ) {
		if ( CloudNo == CheckNo												) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.x != Pos.x	) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.y  > Pos.y	) continue;

		// 距離を求める.
		const float Distance = std::abs( m_SortCloudList[CheckNo].second->Transform.Position.y - Pos.y );
		if ( Distance > ( CLOUD_RADIUS + PLAYER_RADIUS * 2.5f ) * 2.0f ) continue;

		// 評価する雲を黄色にする.
#ifdef _DEBUG
		m_SortCloudList[CheckNo].second->Color = Color4::Yellow;
#endif	// #ifdef _DEBUG.

		// 上の雲の上は移動できるか.
		return UpMoveCheck( CheckNo );
	}
	return true;
}

//---------------------------.
// 下に移動できるか判断.
//---------------------------.
bool CMiniGame::DownMoveCheck( const int CloudNo )
{
	// 位置の取得.
	const D3DXPOSITION3& Pos	= m_SortCloudList[CloudNo].second->Transform.Position;
	const float			 CheckY	= Pos.y + CLOUD_RADIUS + PLAYER_RADIUS * 1.5f;

	// プレイヤーの座標の取得.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// 下に移動できるか.
	const float DeadGround = GROUND_POS_Y - PLAYER_RADIUS * 2.0f;
	if ( CheckY >= DeadGround ) {
#ifdef _DEBUG
		m_SortCloudList[CloudNo].second->Color.w = 0.5f;
#endif	// #ifdef _DEBUG.
	return false;
}
	// 他の雲に近すぎないか確認.
	const int ListSize	= static_cast<int>( m_SortCloudList.size() );
	for ( int CheckNo = 0; CheckNo < ListSize; ++CheckNo ) {
		if ( CloudNo == CheckNo												) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.x != Pos.x	) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.y <= Pos.y	) continue;

		// 距離を求める.
		const float Distance = std::abs( m_SortCloudList[CheckNo].second->Transform.Position.y - Pos.y );
		if ( Distance > ( CLOUD_RADIUS + PLAYER_RADIUS * 2.5f ) * 2.0f ) continue;

		// 評価する雲を黄色にする.
#ifdef _DEBUG
		m_SortCloudList[CheckNo].second->Color = Color4::Yellow;
#endif	// #ifdef _DEBUG.

		// 下の雲の下は移動できるか.
		return DownMoveCheck( CheckNo );
	}
	return true;
}

//---------------------------.
// 自動操作の出力の更新.
//---------------------------.
void CMiniGame::AutoPlayOutUpdate()
{
	switch ( m_AutoPlayAction ) {
	case MoveUp:
		// パワーが -6 になるように調整する.
		if ( m_Power == -6.0f ) break;
		if ( m_Power > -6.0f )	m_AutoPlay = m_Power > -4.0f ? 2.0f : m_Power + 6.0f;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveUpSmall:
		// パワーが -2 になるように調整する.
		if ( m_Power == -2.0f ) break;
		if ( m_Power > -2.0f )	m_AutoPlay = m_Power > 0.0f ? 2.0f : m_Power + 2.0f;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveWait:
		// パワーが0になるように調整する.
		if ( m_Power == 0.0f ) break;
		if ( m_Power > 0.0f )	m_AutoPlay = m_Power > 2.0f ? 2.0f : m_Power;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveDownSmall:
		// パワーが 2 になるように調整する.
		if ( m_Power == 2.0f ) break;
		if ( m_Power > 2.0f )	m_AutoPlay = m_Power > 4.0f ? 2.0f : m_Power - 2.0f;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveDown:
		// パワーが 6 になるように調整する.
		if ( m_Power == 6.0f ) break;
		if ( m_Power > 6.0f )	m_AutoPlay = m_Power > 8.0f ? 2.0f : m_Power - 6.0f;
		else					m_AutoPlay = 0.0f;
		break;
	default:
		break;
	}
}

//---------------------------.
// ポーズに移動.
//---------------------------.
void CMiniGame::MovePause()
{
	if ( m_IsPlay ) return;

	// ポーズを開く.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_START ) ) {
		CSoundManager::PlaySE( OPEN_SE );
		m_IsPause		= true;
		m_IsPauseAnim	= true;
	}
}

//---------------------------.
// ポーズ画面を開く.
//---------------------------.
void CMiniGame::PauseOpen()
{
	if ( m_IsPauseAnim == false ) return;

	// ポーズ画面を拡大しながらフェードインさせる.
	float Scale = m_PauseBackState.Transform.Scale.x;
	Scale += 3.0f * m_DeltaTime;
	if ( Scale >= STransform::NORMAL_SCALE ) {
		Scale			= STransform::NORMAL_SCALE;
		m_IsPauseAnim	= false;
	}

	// 設定.
	m_PauseBackState.Transform.Scale	= { Scale, Scale, Scale };
	m_PauseBackState.Color.w			= Scale;
	m_PauseCursorState.Color.w			= Scale;
	for ( auto& t : m_PauseTextStateList ) t.Color.w = Scale;
}

//---------------------------.
// ポーズ画面を閉じる.
//---------------------------.
void CMiniGame::PauseClose()
{
	if ( m_IsPauseAnim == false ) return;

	// ポーズ画面を縮小しながらフェードアウトさせる.
	float Scale = m_PauseBackState.Transform.Scale.x;
	Scale -= 3.0f * m_DeltaTime;
	if ( Scale <= 0.0f ) {
		m_Choice		= 0;
		Scale			= 0.0f;
		m_IsPauseAnim	= false;
	}

	// 設定.
	m_PauseBackState.Transform.Scale	= { Scale, Scale, Scale };
	m_PauseBackState.Color.w			= Scale;
	m_PauseCursorState.Color.w			= Scale;
	for ( auto& t : m_PauseTextStateList ) t.Color.w = Scale;
}

//---------------------------.
// ポーズ中の更新.
//---------------------------.
void CMiniGame::PauseUpdate()
{
	// ポーズを閉じる.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_START ) ) {
		CSoundManager::PlaySE( CLOSE_SE );
		m_IsPause		= false;
		m_IsPauseAnim	= true;
	}

	// 選択箇所の移動.
	else if ( CInput::IsKeyDown( "Up" ) ) {
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

	// 選択肢の決定.
	else if ( CInput::IsKeyDown( "Decision" ) ) {
		switch ( m_Choice ) {
		// 「ゲームに戻る」を選択したとき.
		case CHOICE_STARTCHOICE:
			CSoundManager::PlaySE( CLOSE_SE );
			m_IsPause		= false;
			m_IsPauseAnim	= true;
			break;
		// 「ベストスコアを削除する」を選択したとき.
		case CHOICE_CENTERCHOICE:
			FileManager::BinarySave( DATA_FILE_PATH, 0 );
			m_BestScore		= 0;
			m_IsPause		= false;
			m_IsPauseAnim	= true;

			// SEの再生.
			CSoundManager::PlaySE( TRASH_SE );
			CSoundManager::PlaySE( CLOSE_SE );
			break;
		// 「ジェットラベルを終了する」を選択したとき.
		case CHOICE_ENDCHOICE:
			CSoundManager::PlaySE( "Decision" );
			CSceneManager::SceneChange( ESceneList::Title, EFadeType::MaskFade );
			break;
		}
	}

	// カーソルの位置の設定.
	m_PauseCursorState.Transform.Position.y = m_PauseTextStateList[m_Choice + 1].Transform.Position.y;

	// 見た目の変更.
	switch ( m_Choice ) {
	case 0:
		m_PauseTextStateList[1].AnimState.AnimNumber = 2;
		m_PauseTextStateList[2].AnimState.AnimNumber = 3;
		m_PauseTextStateList[3].AnimState.AnimNumber = 5;
		break;
	case 1:
		m_PauseTextStateList[1].AnimState.AnimNumber = 1;
		m_PauseTextStateList[2].AnimState.AnimNumber = 4;
		m_PauseTextStateList[3].AnimState.AnimNumber = 5;
		break;
	case 2:
		m_PauseTextStateList[1].AnimState.AnimNumber = 1;
		m_PauseTextStateList[2].AnimState.AnimNumber = 3;
		m_PauseTextStateList[3].AnimState.AnimNumber = 6;
		break;
	default:
		break;
	};
}
