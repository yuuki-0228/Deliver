#include "PizzaEditor.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Resource\PizzaResource\PizzaResource.h"
#include "..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\Random\Random.h"
#include "..\..\Utility\Easing\Easing.h"
#include "..\..\Utility\FileManager\DragAndDrop\DragAndDrop.h"

namespace {
	constexpr int	PAUSE_TEXT_NUM			= 4;													// ポーズテキストの表示数.
	constexpr int	INGREDIENTS_MAX			= 10;													// 具材の数.
	constexpr int	SAUCE_MAX				= 4;													// ソースの数.
	constexpr int	GRAB_MAYONNAISE_NO		= -3;													// 具材のマヨネーズソースを掴んでいる時のNo.
	constexpr int	GRAB_TERIYAKI_NO		= -2;													// 具材の照り焼きソースを掴んでいる時のNo.
	constexpr int	NO_GRAB_NO				= -1;													// 何も掴んでいない時のNo.
	constexpr int	NO_SAUCE_NO				= -1;													// ソースを配置していない時のNo.
	constexpr float	TEXT_CHANGE_TIME		= 3.0f;													// テキストを変更する時間.
	constexpr float	CURSOR_SLOW_SPEED		= 2.0f;													// カーソルの遅い時の時の移動速度.
	constexpr float	CURSOR_NORMAL_SPEED		= 6.0f;													// カーソルの通常時の移動速度.
	constexpr float	CURSOR_FAST_SPEED		= 10.0f;												// カーソルの速い時の移動速度.
	constexpr float	ROT_SLOW_SPEED			= 2.0f;													// 具材の回転の遅い時の時の移動速度.
	constexpr float	ROT_NORMAL_SPEED		= 5.0f;													// 具材の回転の通常時の移動速度.
	constexpr float	ROT_FAST_SPEED			= 10.0f;												// 具材の回転の速い時の移動速度.
	constexpr int	CHOICE_STARTCHOICE		= 0;													// 最初の選択肢.
	constexpr int	CHOICE_CENTERCHOICE		= 1;													// 真ん中の選択肢.
	constexpr int	CHOICE_ENDCHOICE		= 2;													// 最後の選択肢.
	constexpr char	SAVE_FILE_PATH[]		= "Data\\Parameter\\Pizza\\";							// ピザデータの保存場所.
	constexpr char	SNAME_LIST_FILE_PATH[]	= "Data\\Parameter\\PizzaEditor\\SauceName.json";		// ソースの名前リストのファイルパス.
	constexpr char	INAME_LIST_FILE_PATH[]	= "Data\\Parameter\\PizzaEditor\\IngredientName.json";	// 具材の名前リストのファイルパス.
	constexpr char	IAREA_LIST_FILE_PATH[]	= "Data\\Parameter\\PizzaEditor\\IngredientArea.json";	// 具材の範囲リストのファイルパス.
	constexpr char	NORMAL_TEXT[]			= u8"保存/読み込みファイル名を記入してください";			// 通常時のテキスト.

	constexpr char	DECISION_SE[]			= "PizzaEditorDecision";								// 決定時のSE.
	constexpr char	OPEN_SE[]				= "Open";												// ポーズを開く時のSE.
	constexpr char	CLOSE_SE[]				= "Close";												// ポーズを閉じる時のSE.
	constexpr char	CURSOR_MOVE_SE[]		= "CursorMove";											// カーソル移動時のSE.
	constexpr char	BUTTON_SE[]				= "ImguiSE";											// ボタンを押した時のSE.
	constexpr char	CHEESE_GRAB_SE[]		= "CheeseGrab";											// チーズを掴んだ時のSE.
	constexpr char	CHEESE_GRAB_END_SE[]	= "CheeseGrabEnd";										// チーズを離した時のSE.
	constexpr char	INGREDIENT_GRAB_SE[]	= "IngredientGrab";										// 具材を掴んだ時のSE.
	constexpr char	INGREDIENT_GRAB_END_SE[]= "IngredientGrabEnd";									// 具材を離した時のSE.
	constexpr char	SAUCE_SE[]				= "Sauce";												// ソースを掴んだり離した時のSE.
	constexpr char	FALL_SE[]				= "Fall";												// 具材が落下した時のSE.
	constexpr char	TRASH_SE[]				= "Trash";												// 具材などゴミ箱エリアに捨てた時のSE.
	constexpr char	SPATULA_SE[]			= "Spatula";											// ヘラの表示/非表示をする時のSE.
	constexpr char	COOK_PIZZA_SE[]			= "CookPizza";											// ピザを焼くSE.
	constexpr char	PIZZAKILN_SE[]			= "PizzaEditorKiln";									// ピザ窯のループSE.
	constexpr char	BACK_SE[]				= "PizzaEditorBack";									// 環境音のループSE.

	// カーソルNo.
	enum enCursorNo : unsigned char {
		NormaCursorl,			// 通常時.
		CanGrabCursor,			// 掴める時.
		GrabCursor,				// 掴んでる時.
		SelectCursor,			// 選べる時.
		MayonnaiseCursor,		// マヨネーズを持ってる時.
		TeriyakiCursor,			// 照り焼きソースを持ってる時.
		SauceCursor,			// ソースを持ってる時.
		CheeseCursor			// チーズを持ってるとき時.
	} typedef ECursorNo;

	// 上下ボタンの見た目No.
	enum enUpDownButtonImageNo : unsigned char {
		UpButton,				// 上ボタン.
		PushUpButton,			// 押されている上ボタン.
		DownButton,				// 下ボタン.
		PushDownButton			// 押されている下ボタン.
	} typedef EUpDownButtonImageNo;

	// 左右ボタンの見た目No.
	enum enLeftRightButtonImageNo : unsigned char {
		LeftButton,				// 左ボタン.
		PushLeftButton,			// 押されている左ボタン.
		RightButton,			// 右ボタン.
		PushRightButton			// 押されている右ボタン.
	} typedef ELeftRightButtonImageNo;

	// テキストボタンの見た目No.
	enum enTextButtonImageNo : unsigned char {
		DecisionButton,			// 決定ボタン.
		PushDecisionButton,		// 押されている決定ボタン.
		PutInKilnButton,		// 窯に入れるボタン.
		PushPutInKilnButton,	// 押されている窯に入れるボタン.
		YesButton,				// はいボタン.
		PushYesButton,			// 押されているはいボタン.
		NoButton,				// いいえボタン.
		PushNoButton,			// 押されているいいえボタン.
	} typedef ETextButtonImageNo;

	// 窯に入れるアニメーションNo.
	enum enPutInKilnAnimNo : unsigned char {
		SpatulaDispAnim,		// ヘラを表示させるアニメーション.
		SpatulaBackAnim,		// ヘラを戻すアニメーション.
		PutInKilnAnim,			// 窯に入れるアニメーション.
		CookPizzaAnim,			// ピザを焼くアニメーション.
		RemoveFromKilnAnim,		// 窯から出すアニメーション.
	} typedef EPutInKilnAnimNo;

	// 操作説明のテキストNo.
	enum enTextNo : unsigned char {
		PauseOpenText,			// ポーズを開く.
		CursorMoveText,			// カーソルの移動.
		CursorMoveChangeText,	// カーソルの移動速度の変更.
		RotationChangeText,		// 具材の回転速度の変更.
		GrabText,				// 具材を掴む.
		GrabEndText,			// 具材を離す.
		GrabSauceText,			// ソースを掴む.
		GrabEndSauceText,		// ソースを離す.
		CopyText,				// 具材のコピーを配置.
		Gray_CopyText,			// 灰色の具材のコピーを配置.
		ClockwiseText,			// 時計周りで回転.
		Gray_ClockwiseText,		// 灰色の時計周りで回転. 
		AnticlockwiseText,		// 反時計周りで回転.
		Gray_AnticlockwiseText,	// 灰色の反時計周りで回転.
	} typedef ETextNo;
}

CPizzaEditor::CPizzaEditor()
	: m_pBack						( nullptr )
	, m_pPizzaBase					( nullptr )
	, m_pIngredient					( nullptr )
	, m_pIngredientSauce			( nullptr )
	, m_pIngredientArea				( nullptr )
	, m_pSauce						( nullptr )
	, m_pSauceArea					( nullptr )
	, m_pSpatula					( nullptr )
	, m_pKiln						( nullptr )
	, m_pCursor						( nullptr )
	, m_pTrashArea					( nullptr )
	, m_pUpDownButton				( nullptr )
	, m_pLeftRightButton			( nullptr )
	, m_pTextButton					( nullptr )
	, m_pPauseBack					( nullptr )
	, m_pPauseCursor				( nullptr )
	, m_pPauseText					( nullptr )
	, m_pText						( nullptr )
	, m_pSaveArea					( nullptr )
	, m_pFont						( nullptr )
	, m_pSauceMaskList				( 8 )
	, m_pSauceMaskNoMap				()
	, m_FontState					()
	, m_PizzaBaseState				()
	, m_SamplePizzaBaseState		()
	, m_IngredientStateBase			()
	, m_IngredientSauceState		()
	, m_IngredientAreaStateBase		()
	, m_SauceStateBase				()
	, m_SauceAreaStateBase			()
	, m_AnimSpatulaState			()
	, m_SpatulaState				()
	, m_KilnState					()
	, m_CursorState					()
	, m_PauseBackState				()
	, m_PauseCursorState			()
	, m_PauseTextStateBase			()
	, m_pSelectSaveAreaState		( nullptr )
	, m_TextButtonStateList			( 2 )
	, m_PauseTextStateList			( PAUSE_TEXT_NUM )
	, m_TrashAreaStateList			( 2 )
	, m_UpDownButtonStateList		( 2 )
	, m_LeftRightButtonStateList	( 2 )
	, m_IngredientStateList			()
	, m_IngredientAreaStateList		()
	, m_SauceStateList				()
	, m_CheeseStateList				()
	, m_SauceAreaStateList			()
	, m_TextStateList				( 8 )
	, m_SaveAreaStateList			( 6 )
	, m_SampleSauceStateList		()
	, m_SampleCheeseStateList		()
	, m_SampleIngredientStateList	()
	, m_GrabPosition				( INIT_FLOAT3 )
	, m_PutInKilnAnimVector			( INIT_FLOAT3 )
	, m_SauceNameList				()
	, m_IngredientNameList			()
	, m_IngredientTypeAreaList		()
	, m_SauceList					( 8 )
	, m_SelectObject				( "" )
	, m_CursorMoveSpeedList			( 3 )
	, m_IngredientRotSpeedList		( 3 )
	, m_PutInKilnAnimCnt			( INIT_FLOAT )
	, m_ButtonAnimCnt				( INIT_FLOAT )
	, m_CursorMoveSpeedNo			( 1 )
	, m_IngredientRotSpeedNo		( 1 )
	, m_SavePizzaData				()
	, m_AreaPageNo					( INIT_INT )
	, m_GrabSauceNo					( NO_GRAB_NO )
	, m_GrabIngredientNo			( NO_GRAB_NO )
	, m_PutInKilnAnimNo				( INIT_INT )
	, m_Choice						( INIT_INT )
	, m_SavePizzaNum				( INIT_INT )
	, m_SavePizzaPageNo				( INIT_INT )
	, m_IsBButtonAction				( false )
	, m_IsDispSauce					( true )
	, m_IsPutInKiln					( false )
	, m_IsPause						( false )
	, m_IsPauseAnim					( false )
	, m_IsPizzaSave					( false )
	, m_IsPizzaLoad					( false )
	, m_IsPizzaDelete				( false )
	, m_IsPizzaDownload				( false )
{
}

CPizzaEditor::~CPizzaEditor()
{
	// ドラック&ドロップを無効にする.
	CDragAndDrop::Close();

	// サウンドの停止.
	CSoundManager::StopSE( DECISION_SE				);
	CSoundManager::StopSE( OPEN_SE					);
	CSoundManager::StopSE( CLOSE_SE					);
	CSoundManager::StopSE( CURSOR_MOVE_SE			);
	CSoundManager::StopSE( BUTTON_SE				);
	CSoundManager::StopSE( CHEESE_GRAB_SE			);
	CSoundManager::StopSE( CHEESE_GRAB_END_SE		);
	CSoundManager::StopSE( INGREDIENT_GRAB_SE		);
	CSoundManager::StopSE( INGREDIENT_GRAB_END_SE	);
	CSoundManager::StopSE( SAUCE_SE					);
	CSoundManager::StopSE( FALL_SE					);
	CSoundManager::StopSE( TRASH_SE					);
	CSoundManager::StopSE( SPATULA_SE				);
	CSoundManager::StopSE( COOK_PIZZA_SE			);
	CSoundManager::StopSE( PIZZAKILN_SE				);
	CSoundManager::StopSE( BACK_SE					);
}

//---------------------------.
// 初期化.
//---------------------------.
bool CPizzaEditor::Init()
{
	m_pBack					= CSpriteResource::GetSprite( "PizzaEditorBack" );
	m_pPizzaBase			= CSpriteResource::GetSprite( "PizzaBase",					&m_PizzaBaseState );
	m_pIngredient			= CSpriteResource::GetSprite( "PizzaIngredient",			&m_IngredientStateBase );
	m_pIngredientSauce		= CSpriteResource::GetSprite( "PizzaIngredientSauce",		&m_IngredientSauceState );
	m_pIngredientArea		= CSpriteResource::GetSprite( "PizzaEditorIngredientArea",	&m_IngredientAreaStateBase );
	m_pSauce				= CSpriteResource::GetSprite( "PizzaSauce",					&m_SauceStateBase );
	m_pSauceArea			= CSpriteResource::GetSprite( "PizzaEditorSauceArea",		&m_SauceAreaStateBase );
	m_pSpatula				= CSpriteResource::GetSprite( "PizzaEditorSpatula",			&m_SpatulaState );
	m_pKiln					= CSpriteResource::GetSprite( "PizzaEditorKiln",			&m_KilnState );
	m_pCursor				= CSpriteResource::GetSprite( "PizzaEditorCursor",			&m_CursorState );
	m_pTrashArea			= CSpriteResource::GetSprite( "PizzaEditorTrashArea",		&m_TrashAreaStateList );
	m_pUpDownButton			= CSpriteResource::GetSprite( "UpDownButton",				&m_UpDownButtonStateList );
	m_pLeftRightButton		= CSpriteResource::GetSprite( "LeftRighrButton",			&m_LeftRightButtonStateList );
	m_pTextButton			= CSpriteResource::GetSprite( "TextButton",					&m_TextButtonStateList );
	m_pPauseBack			= CSpriteResource::GetSprite( "back",						&m_PauseBackState		);
	m_pPauseText			= CSpriteResource::GetSprite( "PizzaEditorPauseText",		&m_PauseTextStateBase	);
	m_pPauseCursor			= CSpriteResource::GetSprite( "Yubi",						&m_PauseCursorState		);
	m_pText					= CSpriteResource::GetSprite( "PizzaEditorText",			&m_TextStateList );
	m_pSaveArea				= CSpriteResource::GetSprite( "PizzaEditorSaveArea",		&m_SaveAreaStateList );
	m_pFont					= CFontResource::GetFont( "NasuM", &m_FontState );

	// 操作説明の設定.
	const int TSize = static_cast<int>( m_TextStateList.size() );
	for ( int i = 0; i < TSize; ++i ) {
		m_TextStateList[i].AnimState.IsSetAnimNumber	= true;
		m_TextStateList[i].Transform.Position.x			= 936.0f;
		m_TextStateList[i].Transform.Position.y			= 467.0f + 31.0f * i;
	}
	m_TextStateList[0].AnimState.AnimNumber = PauseOpenText;
	m_TextStateList[1].AnimState.AnimNumber = CursorMoveText;
	m_TextStateList[2].AnimState.AnimNumber = CursorMoveChangeText;
	m_TextStateList[3].AnimState.AnimNumber = RotationChangeText;
	m_TextStateList[4].AnimState.AnimNumber = GrabSauceText;
	m_TextStateList[5].AnimState.AnimNumber = CopyText;
	m_TextStateList[6].AnimState.AnimNumber = ClockwiseText;
	m_TextStateList[7].AnimState.AnimNumber = AnticlockwiseText;

	// 保存エリアの設定.
	const int	ASize		 = static_cast<int>( m_SaveAreaStateList.size() );
	const SSize SaveAreaSize = m_pSaveArea->GetSpriteState().Disp;
	for ( int i = 0; i < ASize; ++i ) {
		m_SaveAreaStateList[i].AnimState.IsSetAnimNumber	= true;
		m_SaveAreaStateList[i].Transform.Position.x			= 928.0f + SaveAreaSize.w * ( i % 2 );
		m_SaveAreaStateList[i].Transform.Position.y			= 6.0f	 + SaveAreaSize.h * ( i / 2 );
	}

	// マスク画像の保存.
	for( int i = 0; i < 8; ++i ){
		m_pSauceMaskList[i].resize( 4 );
		for ( int No = 0; No < 4; ++No ) {
			m_pSauceMaskList[i][No] = CSpriteResource::GetSprite( "PizzaSauceMask_" + std::to_string( i ) + std::to_string( No ) )->GetTexture();
			m_pSauceMaskNoMap[m_pSauceMaskList[i][No]] = std::make_pair( i, No );
		}
	}

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

	// フラグの設定.
	m_SauceStateBase.AnimState.IsSetAnimNumber				= true;
	m_IngredientStateBase.AnimState.IsSetAnimNumber			= true;
	m_IngredientAreaStateBase.AnimState.IsSetAnimNumber		= true;
	m_SauceAreaStateBase.AnimState.IsSetAnimNumber			= true;
	m_PizzaBaseState.AnimState.IsSetAnimNumber				= true;
	m_CursorState.AnimState.IsSetAnimNumber					= true;

	// 親の設定.
	m_PizzaBaseState.Transform.AddParent(			&m_SpatulaState.Transform	);
	m_IngredientStateBase.Transform.AttachParent(	&m_PizzaBaseState.Transform );
	m_SauceStateBase.Transform.AttachParent(		&m_PizzaBaseState.Transform );

	// サイズの取得.
	const SSize& TrashAreaSize	= m_pTrashArea->GetSpriteState().Disp;
	const SSize& SpatulaSize	= m_pSpatula->GetSpriteState().Disp;

	// 座標の設定.
	m_SauceStateBase.Transform.Position				= INIT_FLOAT3;
	m_SpatulaState.Transform.Position.x				= FWND_W / 2.0f;
	m_SpatulaState.Transform.Position.y				= FWND_H - 64.0f;
	m_PizzaBaseState.Transform.Position.x			= 0.0f;
	m_PizzaBaseState.Transform.Position.y			= -299.0f;
	m_PizzaBaseState.Transform.Scale				= { 2.0f, 2.0f, 2.0f };
	m_KilnState.Transform.Position.x				= FWND_W / 2.0f;
	m_CursorState.Transform.Position.x				= FWND_W / 2.0f;
	m_CursorState.Transform.Position.y				= FWND_H / 2.0f;
	m_CursorState.AnimState.AnimNumber				= NormaCursorl;
	m_TrashAreaStateList[1].Transform.Position.x	= FWND_W - TrashAreaSize.w;

	// ヘラの設定.
	m_AnimSpatulaState								= m_SpatulaState;
	m_AnimSpatulaState.Transform.Position.y			= FWND_H + SpatulaSize.h;
	m_PutInKilnAnimVector							= m_SpatulaState.Transform.Position - m_AnimSpatulaState.Transform.Position;
	m_SpatulaState.IsDisp							= false;
	m_AnimSpatulaState.IsDisp						= false;

	// ゴミ箱エリアの設定.
	for ( auto& a : m_TrashAreaStateList ) {
		a.Color.w					= Color::ALPHA_MIN;
		a.AnimState.IsSetAnimNumber = true;
	}

	// ボタンの設定.
	for ( auto& b : m_UpDownButtonStateList ) b.AnimState.IsSetAnimNumber = true;
	m_UpDownButtonStateList[0].AnimState.AnimNumber		= UpButton;
	m_UpDownButtonStateList[0].Transform.Position.x		= 128.0f;
	m_UpDownButtonStateList[0].Transform.Position.y		= -4.0f;
	m_UpDownButtonStateList[1].AnimState.AnimNumber		= DownButton;
	m_UpDownButtonStateList[1].Transform.Position.x		= 128.0f;
	m_UpDownButtonStateList[1].Transform.Position.y		= 660.0f;
	for ( auto& b : m_LeftRightButtonStateList ) b.AnimState.IsSetAnimNumber = true;
	m_LeftRightButtonStateList[0].AnimState.AnimNumber	= LeftButton;
	m_LeftRightButtonStateList[0].Transform.Position.x	= 928.0f;
	m_LeftRightButtonStateList[0].Transform.Position.y	= 371.0f;
	m_LeftRightButtonStateList[1].AnimState.AnimNumber	= RightButton;
	m_LeftRightButtonStateList[1].Transform.Position.x	= 1215.0f;
	m_LeftRightButtonStateList[1].Transform.Position.y	= 370.0f;
	for( auto& t : m_TextButtonStateList ){
		t.AnimState.IsSetAnimNumber		= true;
		t.AnimState.AnimNumber			= DecisionButton;
		t.Transform.Position.x			= FWND_W / 2.0f;
		t.Transform.Position.y			= FWND_H - 39.0f;
	}
	m_TextButtonStateList[1].IsDisp		= false;

	// フォントの設定.
	m_FontState.Transform.Position.x	= FWND_W / 2.0f;
	m_FontState.Transform.Position.y	= 100.0f;
	m_FontState.TextAlign				= ETextAlign::Center;
	m_FontState.IsOutLine				= true;
	m_FontState.OutLineSize				= 2.0f;
	m_FontState.OutLineColor			= Color4::White;
	m_FontState.Color					= Color4::Black;

	// カーソルの移動速度の設定.
	m_CursorMoveSpeedList = {
		CURSOR_SLOW_SPEED,
		CURSOR_NORMAL_SPEED,
		CURSOR_FAST_SPEED
	};
	// 具材の回転速度の設定.
	m_IngredientRotSpeedList = {
		ROT_SLOW_SPEED,
		ROT_NORMAL_SPEED,
		ROT_FAST_SPEED
	};

	// ピザの画像サイズの取得.
	const SSize& BaseSpriteSize			= m_pPizzaBase->GetSpriteState().Disp;
	const SSize& IngredientSpriteSize	= m_pIngredientArea->GetSpriteState().Disp;
	const SSize& SauceSpriteSize		= m_pSauceArea->GetSpriteState().Disp;

	// 具材リストの設定.
	std::vector<std::string> INameList	= FileManager::JsonLoad( INAME_LIST_FILE_PATH )["TypeName"];
	std::vector<int>		 ISizeList	= FileManager::JsonLoad( IAREA_LIST_FILE_PATH )["TypeSize"];
	const int ISize = m_IngredientAreaStateBase.AnimState.AnimNumberMax - 1;
	m_IngredientNameList.resize( ISize );
	m_IngredientTypeAreaList.resize( ISize );
	int Start = 0;
	for ( int i = 0; i < ISize; ++i ) {
		const std::wstring& wString		= StringConversion::to_wString( INameList[i], ECodePage::UTF8 );
		m_IngredientNameList[i]			= StringConversion::to_String( wString );
		m_IngredientTypeAreaList[i]		= std::make_pair( Start, Start + ISizeList[i] - 1 );
		Start += ISizeList[i];
	}
	std::vector<std::string> SNameList	= FileManager::JsonLoad( SNAME_LIST_FILE_PATH )["TypeName"];
	const int SSize = m_SauceAreaStateBase.AnimState.AnimNumberMax - 1;
	m_SauceNameList.resize( SSize );
	for ( int i = 0; i < SSize; ++i ) {
		const std::wstring& wString		= StringConversion::to_wString( SNameList[i], ECodePage::UTF8 );
		m_SauceNameList[i]				= StringConversion::to_String( wString );
	}

	// 具材エリアの設定.
	m_IngredientAreaStateList.resize( INGREDIENTS_MAX, m_IngredientAreaStateBase );
	for ( int i = 0; i < INGREDIENTS_MAX; ++i ) {
		m_IngredientAreaStateList[i].Transform.Position.x = IngredientSpriteSize.w * ( i % 2 );
		m_IngredientAreaStateList[i].Transform.Position.y = IngredientSpriteSize.h * ( i / 2 ) + 60.0f;
		m_IngredientAreaStateList[i].AnimState.AnimNumber = i + 1;
	}

	// ソースエリアの設定.
	m_SauceAreaStateList.resize( SAUCE_MAX, m_SauceAreaStateBase );
	for ( int i = 0; i < SAUCE_MAX; ++i ) {
		m_SauceAreaStateList[i].Transform.Position.x = 0.0f;
		m_SauceAreaStateList[i].Transform.Position.y = SauceSpriteSize.h * i + 60.0f;
		m_SauceAreaStateList[i].AnimState.AnimNumber = i + 1;
	}

	// ソースの状態リストの初期化.
	for ( auto& [Sauce, Cheese] : m_SauceList ) {
		Sauce	= NO_SAUCE_NO;
		Cheese	= NO_SAUCE_NO;
	}

	// 具材のソースの初期化.
	m_IngredientSauceState.AnimState.IsSetAnimNumber = true;
	m_IngredientSauceState.Transform.AttachParent( &m_PizzaBaseState.Transform );

	// 保存しているピザの取得.
	std::unordered_map<std::string, json> PizzaData = CPizzaResource::GetPizzaData();
	m_SavePizzaNum = static_cast<int>( PizzaData.size() );
	for ( auto& [Name, Data] : PizzaData ) {
		// 古いピザデータから新しいピザデータに変換するコンバーター.
#if 0
		json j;
		if ( Data["Base"] == 0 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 2;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
			}
		}
		else if ( Data["Base"] == 1 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 2;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
		}
		else if ( Data["Base"] == 2 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 3;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
			}
		}
		else if ( Data["Base"] == 3 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 3;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
		}
		else if ( Data["Base"] == 4 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
		}
		else if ( Data["Base"] == 5 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
			j["Ingredients"][0]["Position"]["x"] = 0.0f;
			j["Ingredients"][0]["Position"]["y"] = 0.0f;
			j["Ingredients"][0]["Rotation"]		 = 0.0f;
			j["Ingredients"][0]["Type"]		 = -4;
		}
		else if ( Data["Base"] == 6 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
			j["Ingredients"][0]["Position"]["x"] = 0.0f;
			j["Ingredients"][0]["Position"]["y"] = 0.0f;
			j["Ingredients"][0]["Rotation"]		 = 0.0f;
			j["Ingredients"][0]["Type"]		 = -3;
		}
		
		for ( int i = 0; i < Data["Ingredients"].size(); ++i ) {
			j["Ingredients"][i]["Position"]["x"]	=  Data["Ingredients"][i]["Position"]["x"];
			j["Ingredients"][i]["Position"]["y"]	=  Data["Ingredients"][i]["Position"]["y"];
			j["Ingredients"][i]["Rotation"]			=  Data["Ingredients"][i]["Rotation"];
			if (  Data["Ingredients"][i]["Type"] <= 3 )  j["Ingredients"][i]["Type"] = 59;
			else if (  Data["Ingredients"][i]["Type"] <= 5 )  j["Ingredients"][i]["Type"] = 36;
			else if (  Data["Ingredients"][i]["Type"] <= 6 )  j["Ingredients"][i]["Type"] = 37;
			else if (  Data["Ingredients"][i]["Type"] <= 7 )  j["Ingredients"][i]["Type"] = 38;
			else if (  Data["Ingredients"][i]["Type"] <= 9 )  j["Ingredients"][i]["Type"] = 52;
			else if (  Data["Ingredients"][i]["Type"] <= 10 ) j["Ingredients"][i]["Type"] = 53;
			else if (  Data["Ingredients"][i]["Type"] <= 11 ) j["Ingredients"][i]["Type"] = 6*8-1;
			else if (  Data["Ingredients"][i]["Type"] <= 15 ) j["Ingredients"][i]["Type"] = 6*9+4-1;
			else if (  Data["Ingredients"][i]["Type"] <= 16 ) j["Ingredients"][i]["Type"] = 6*9+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 17 ) j["Ingredients"][i]["Type"] = 6*9+2-1;
			else if (  Data["Ingredients"][i]["Type"] <= 18 ) j["Ingredients"][i]["Type"] = 6*9+3-1;
			else if (  Data["Ingredients"][i]["Type"] <= 19 ) j["Ingredients"][i]["Type"] = 6*10+3-1;
			else if (  Data["Ingredients"][i]["Type"] <= 20 ) j["Ingredients"][i]["Type"] = 6*10+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 21 ) j["Ingredients"][i]["Type"] = 6*10+2-1;
			else if (  Data["Ingredients"][i]["Type"] <= 22 ) j["Ingredients"][i]["Type"] = 6*7+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 23 ) j["Ingredients"][i]["Type"] = 6*7+2-1;
			else if (  Data["Ingredients"][i]["Type"] <= 24 ) j["Ingredients"][i]["Type"] = 6*7+3-1;
			else if (  Data["Ingredients"][i]["Type"] <= 25 ) j["Ingredients"][i]["Type"] = 6*7+4-1;
			else if (  Data["Ingredients"][i]["Type"] <= 27 ) j["Ingredients"][i]["Type"] = 6*7+5-1;
			else if (  Data["Ingredients"][i]["Type"] <= 28 ) j["Ingredients"][i]["Type"] = 6*8+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 29 ) j["Ingredients"][i]["Type"] = 6*8+3-1;
			else  j["Ingredients"][i]["Type"] = 6*9+5-1;
		}
		FileManager::JsonSave("Data\\Parameter\\Pizza\\C\\" + Name + ".json", j);
#endif
		m_SavePizzaData.emplace_back( std::make_pair( Name, Data ) );
	}
	m_SamplePizzaBaseState = m_PizzaBaseState;
	m_SamplePizzaBaseState.Transform.pParent	= nullptr;
	m_SamplePizzaBaseState.Transform.Scale		= { 0.5f, 0.5f, 0.5f };
	m_SamplePizzaBaseState.AnimState.AnimNumber = 1;
	SavePizzaSampleInit();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CPizzaEditor::Update( const float& DeltaTime )
{
	m_DeltaTime							= DeltaTime;
	m_IsBButtonAction					= false;

	if ( m_GrabSauceNo == NO_GRAB_NO && m_GrabIngredientNo == NO_GRAB_NO ) {
		m_CursorState.AnimState.AnimNumber = NormaCursorl;
	}

	if ( m_IsPause ) {
		PauseOpen();				// ポーズ画面を開く.
		PauseUpdate();				// ポーズ中の更新.
	}
	else {
		CursorMove();				// カーソルの移動.
		NewSauce();					// 新しいソースを取得.
		NewIngredient();			// 新しい具材を取得.
		IngredientGrabEnd();		// 具材の掴みを終了する.
		IngredientSauceGrabEnd();	// 具材のソースの掴みを終了する.
		SauceGrabEnd();				// ソースの掴みを終了する.
		GrabStart();				// 具材の掴みを開始する.
		GradRotation();				// 掴んでる具材の回転.
		GradUpdate();				// 掴みの更新.
		TrashAreaUpdate();			// 削除エリアの更新.
		IngredientDeleteUpdate();	// 具材の削除の更新.
		PushUpDownButton();			// 上下ボタンを押した時の更新.
		PushLeftRightButton();		// 左右ボタンを押した時の更新.
		PushTextButton();			// テキストボタンを押した時の更新.
		PutInKilnAnimUpdate();		// 窯に入れるアニメーションの更新.

		PauseClose();				// ポーズ画面を閉じる.
		MovePause();				// ポーズに移動.

		TextUpdate();				// 操作説明の更新.

		SaveAreaUpdate();			// 保存エリアの更新.
		SaveAreaButtonUpdate();		// 保存エリアのボタンの更新.

		DragAndDropUpdate();		// ドラック&ドロップの更新.
	}

	// SEをループで再生.
	CSoundManager::PlaySELoop( PIZZAKILN_SE, true );
	CSoundManager::PlaySELoop( BACK_SE,		 true );
}

//---------------------------.
// UIの描画.
//---------------------------.
void CPizzaEditor::UIRender()
{
	m_pBack->RenderUI();
	for( auto& t : m_TextButtonStateList ) m_pTextButton->RenderUI( &t );
	m_pSpatula->RenderUI( &m_AnimSpatulaState );
	m_pSpatula->RenderUI( &m_SpatulaState );
	m_pPizzaBase->RenderUI( &m_PizzaBaseState );
	if ( m_IsDispSauce ) {
		for ( auto& s : m_SauceAreaStateList		) m_pSauceArea->RenderUI( &s );
	}
	else {
		for ( auto& i : m_IngredientAreaStateList	) m_pIngredientArea->RenderUI( &i );
	}
	for ( auto& b : m_UpDownButtonStateList		) m_pUpDownButton->RenderUI( &b );
	for ( auto& b : m_LeftRightButtonStateList	) m_pLeftRightButton->RenderUI( &b );
	for ( auto& t : m_TextStateList				) m_pText->RenderUI( &t );
	for ( auto& s : m_SaveAreaStateList			) m_pSaveArea->RenderUI( &s );
	SavePizzaSampleRender();
	for ( auto& a : m_TrashAreaStateList		) m_pTrashArea->RenderUI( &a );
	for ( auto& s : m_SauceStateList			) m_pSauce->RenderUI( &s );
	for ( auto& c : m_CheeseStateList			) m_pSauce->RenderUI( &c );
	for ( auto& i : m_IngredientStateList ) {
		if ( i.AnimState.AnimNumber < 0 ) {
			m_IngredientSauceState.Transform			= i.Transform;
			m_IngredientSauceState.AnimState.AnimNumber = std::abs( i.AnimState.AnimNumber ) - 1;
			m_pIngredientSauce->RenderUI( &m_IngredientSauceState );
			continue;
		}
		m_pIngredient->RenderUI( &i );
	}
	m_pKiln->RenderUI( &m_KilnState );
	m_pCursor->RenderUI( &m_CursorState );
	if ( m_GrabIngredientNo == NO_GRAB_NO && m_GrabSauceNo == NO_GRAB_NO ) {
		m_pFont->RenderUI( &m_FontState );
	}

	// ポーズの描画.
	if ( m_IsPauseAnim || m_IsPause ) {
		m_pPauseBack->RenderUI( &m_PauseBackState );
		for ( auto& t : m_PauseTextStateList ) m_pPauseText->RenderUI( &t );
		m_pPauseCursor->RenderUI( &m_PauseCursorState );
	}
}

//---------------------------.
// カーソルの移動.
//---------------------------.
void CPizzaEditor::CursorMove()
{
	if ( m_IsPutInKiln ) return;

	// 移動速度の変更.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_LEFT_THUMB ) ) {
		m_CursorMoveSpeedNo++;
		if ( m_CursorMoveSpeedNo >= 3 ) m_CursorMoveSpeedNo = 0;
	}

	// 座標の取得.
	D3DXPOSITION3* pPos = &m_CursorState.Transform.Position;

	// 左に移動.
	pPos->x += CXInput::GetLThumbX() * m_CursorMoveSpeedList[m_CursorMoveSpeedNo];
	pPos->y -= CXInput::GetLThumbY() * m_CursorMoveSpeedList[m_CursorMoveSpeedNo];

	// カーソルを範囲内に収める.
	pPos->x = std::clamp( pPos->x, 0.0f, FWND_W );
	pPos->y = std::clamp( pPos->y, 0.0f, FWND_H );
}

//---------------------------.
// 新しいソースの取得.
//---------------------------.
void CPizzaEditor::NewSauce()
{
	if ( m_IsPizzaSave 					) return;
	if ( m_IsPizzaLoad					) return;
	if ( m_IsPizzaDelete				) return;
	if ( m_IsPizzaDownload				) return;
	if ( m_IsDispSauce	== false		) return;
	if ( m_GrabSauceNo	!= NO_GRAB_NO	) return;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// 初期化.
	m_FontState.Text = "";

	// 具材エリアに重なっているか.
	const SSize& Size = m_pSauceArea->GetSpriteState().Disp;
	for ( auto& i : m_SauceAreaStateList ) {
		if ( i.Transform.Position.x <= Pos.x && Pos.x <= i.Transform.Position.x + Size.w &&
			 i.Transform.Position.y <= Pos.y && Pos.y <= i.Transform.Position.y + Size.h )
		{
			// 具材の名前の設定.
			if ( i.AnimState.AnimNumber == 0 ) return;
			m_FontState.Text					= m_SauceNameList[i.AnimState.AnimNumber - 1];
			m_FontState.Transform.Position.x	= i.Transform.Position.x + Size.w / 2.0f;
			m_FontState.Transform.Position.y	= i.Transform.Position.y + 10.0f;

			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber	= CanGrabCursor;

			// Bボタンを押した瞬間か.
			if ( m_IsBButtonAction								 ) return;
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;

			// ソースを掴む.
			m_GrabSauceNo						= i.AnimState.AnimNumber;
			m_GrabPosition						= INIT_FLOAT3;
			m_IsBButtonAction					= true;

			// カーソルの変更.
			if ( m_GrabSauceNo == 1 ) {
				m_CursorState.AnimState.AnimNumber = CheeseCursor;

				// 効果音の再生.
				CSoundManager::PlaySE( CHEESE_GRAB_SE );
			}
			else {
				m_CursorState.AnimState.AnimNumber = SauceCursor;

				// 効果音の再生.
				CSoundManager::PlaySE( SAUCE_SE );
			}
			return;
		}
	}
}

//---------------------------.
// 新しい具材の取得.
//---------------------------.
void CPizzaEditor::NewIngredient()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// 初期化.
	m_FontState.Text					= "";

	// 具材エリアに重なっているか.
	const SSize& Size = m_pIngredientArea->GetSpriteState().Disp;
	for ( auto& i : m_IngredientAreaStateList ) {
		if ( i.Transform.Position.x <= Pos.x && Pos.x <= i.Transform.Position.x + Size.w &&
			 i.Transform.Position.y <= Pos.y && Pos.y <= i.Transform.Position.y + Size.h )
		{
			// 具材の名前の設定.
			if ( i.AnimState.AnimNumber == 0 ) return;
			m_FontState.Text					= m_IngredientNameList[i.AnimState.AnimNumber - 1];
			m_FontState.Transform.Position.x	= i.Transform.Position.x + Size.w / 2.0f;
			m_FontState.Transform.Position.y	= i.Transform.Position.y + 10.0f;

			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber	= CanGrabCursor;

			// Bボタンを押した瞬間か.
			if ( m_IsBButtonAction								 ) return;
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			m_IsBButtonAction = true;

			// 具材のソースの選択.
			if ( i.AnimState.AnimNumber == 20 ) {
				m_CursorState.AnimState.AnimNumber	= MayonnaiseCursor;
				m_GrabIngredientNo					= GRAB_MAYONNAISE_NO;
				return;
			}
			else if ( i.AnimState.AnimNumber == 21 ) {
				m_CursorState.AnimState.AnimNumber	= TeriyakiCursor;
				m_GrabIngredientNo					= GRAB_TERIYAKI_NO;
				return;
			}

			// 新しい具材の追加.
			m_GrabIngredientNo = static_cast<int>( m_IngredientStateList.size() );
			m_IngredientStateList.emplace_back( m_IngredientStateBase );
			m_IngredientStateList.back().AnimState.AnimNumber	= CRandom::GetRand( m_IngredientTypeAreaList[i.AnimState.AnimNumber - 1] );
			m_IngredientStateList.back().Transform.Position.x	= Pos.x;
			m_IngredientStateList.back().Transform.Position.y	= Pos.y;
			m_IngredientStateList.back().Transform.Rotation.z	= CRandom::GetRand( Math::RADIAN_MIN, Math::RADIAN_MAX );
			m_GrabPosition										= INIT_FLOAT3;

			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber					= GrabCursor;

			// 効果音の再生.
			CSoundManager::PlaySE( INGREDIENT_GRAB_SE );
			return;
		}
	}
}

//---------------------------.
// 具材の掴みを開始する.
//---------------------------.
void CPizzaEditor::GrabStart()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;
	
	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// 画像サイズの取得.
	const SSize& SpriteSize = m_pIngredient->GetSpriteState().Disp;

	// マウスに重なっているか.
	float GrabDistance = 10000.0f;
	const int Size = static_cast<int>( m_IngredientStateList.size() );
	for ( int i = Size - 1; i >= 0; i-- ) {
		if ( m_IngredientStateList[i].UIAnimState.Flag_x ) continue;
		const D3DXPOSITION3& WorldScale	= m_IngredientStateList[i].Transform.GetWorldScale();
		const D3DXPOSITION3& WorldPos	= m_IngredientStateList[i].Transform.GetWorldPosition();
		const D3DXPOSITION3& LUPos		= WorldPos - D3DXPOSITION3( ( SpriteSize.w * WorldScale.x ) / 2.0f, ( SpriteSize.h * WorldScale.y ) / 2.0f, 0.0f );
		if ( LUPos.x <= Pos.x && Pos.x <= LUPos.x + SpriteSize.w * WorldScale.x &&
			 LUPos.y <= Pos.y && Pos.y <= LUPos.y + SpriteSize.h * WorldScale.y )
		{
			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = CanGrabCursor;

			// 距離を求める.
			if ( m_IsBButtonAction								 ) return;
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			const D3DXVECTOR3&	Vector	 = Pos - WorldPos;
			const float			Distance = D3DXVec3Length( &Vector );

			// 掴む具材の更新.
			if ( Distance >= GrabDistance ) continue;
			m_GrabIngredientNo = i;
			GrabDistance		= Distance;
			m_GrabPosition		= WorldPos - Pos;
		}
	}

	// 掴んだ具材を配列の一番後ろに移動させる.
	if ( m_GrabIngredientNo == NO_GRAB_NO ) return;
	const SSpriteRenderState Grab = m_IngredientStateList[m_GrabIngredientNo];
	m_IngredientStateList.erase( m_IngredientStateList.begin() + m_GrabIngredientNo );
	m_IngredientStateList.emplace_back( Grab );

	// 掴んでいる具材を保存.
	m_GrabIngredientNo = Size - 1;
	m_IsBButtonAction	= true;

	// カーソルの変更.
	m_CursorState.AnimState.AnimNumber = GrabCursor;

	// 効果音の再生.
	if ( Grab.AnimState.AnimNumber >= 0 )	CSoundManager::PlaySE( INGREDIENT_GRAB_SE  );
	else									CSoundManager::PlaySE( SAUCE_SE );
}

//---------------------------.
// 具材の掴みを終了する.
//---------------------------.
void CPizzaEditor::IngredientGrabEnd()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo <= NO_GRAB_NO	) return;
	if ( m_IsBButtonAction					) return;

	// Bボタンを押した瞬間か.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
	m_IsBButtonAction = true;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_IngredientStateList[m_GrabIngredientNo].Transform.GetWorldPosition();

	// 具材を離す.
	bool IsDelete = false;
	if ( Pos.x <= m_pTrashArea->GetSpriteState().Disp.w ||
		 Pos.x >= m_TrashAreaStateList[1].Transform.Position.x )
	{
		// 離した場所が具材エリアの場合削除する.
		m_IngredientStateList.erase( m_IngredientStateList.begin() + m_GrabIngredientNo );
		IsDelete = true;

		// 効果音の再生.
		CSoundManager::PlaySE( TRASH_SE );
	}
 
	// まな板の上ではないか.
	else if ( m_IngredientStateList[m_GrabIngredientNo].AnimState.AnimNumber >= 0 &&
				( 390.0f >= Pos.x || Pos.x >= 899.0f ||
				  107.0f >= Pos.y || Pos.y >= 608.0f ) )
	{
		m_IngredientStateList[m_GrabIngredientNo].UIAnimState.Flag_x = true;
		IsDelete = true;

		// 効果音の再生.
		CSoundManager::PlaySE( FALL_SE );
	}

	// ソースを真ん中に戻す.
	if ( IsDelete == false ) {
		if ( m_IngredientStateList[m_GrabIngredientNo].AnimState.AnimNumber < 0 ) {
			m_IngredientStateList[m_GrabIngredientNo].Transform.Position = INIT_FLOAT3;

			// 効果音の再生.
			CSoundManager::PlaySE( SAUCE_SE );
		}
		else {
			// 効果音の再生.
			CSoundManager::PlaySE( INGREDIENT_GRAB_END_SE );
		}
	}

	// 掴んでる具材を離す.
	m_GrabIngredientNo = NO_GRAB_NO;

	// カーソルの変更.
	m_CursorState.AnimState.AnimNumber = NormaCursorl;
}

//---------------------------.
// 具材のソースの掴みを終了する.
//---------------------------.
void CPizzaEditor::IngredientSauceGrabEnd()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo >= NO_GRAB_NO	) return;
	if ( m_IsBButtonAction					) return;

	// Bボタンを押した瞬間か.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
	m_IsBButtonAction = true;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// 具材を離す.
	if ( Pos.x <= m_pTrashArea->GetSpriteState().Disp.w ||
		 Pos.x >= m_TrashAreaStateList[1].Transform.Position.x )
	{
		// 掴んでる具材を離す.
		m_GrabIngredientNo = NO_GRAB_NO;

		// カーソルの変更.
		m_CursorState.AnimState.AnimNumber = NormaCursorl;
		return;
	}
 
	// 生地の上か確認.
	const D3DXPOSITION3& CenterPos	= m_PizzaBaseState.Transform.GetWorldPosition();
	D3DXVECTOR3			 Vector		= Pos - CenterPos;
	const float			 Distance	= D3DXVec3Length( &Vector );
	if ( Distance >= 180.0f ) return;

	// 具材のソースを配置.
	m_IngredientStateList.emplace_back( m_IngredientStateBase );
	if (		m_GrabIngredientNo == GRAB_MAYONNAISE_NO ) m_IngredientStateList.back().AnimState.AnimNumber = -1;
	else if (	m_GrabIngredientNo == GRAB_TERIYAKI_NO	 ) m_IngredientStateList.back().AnimState.AnimNumber = -2;
	m_IngredientStateList.back().Transform.Position.x = 0.0f;
	m_IngredientStateList.back().Transform.Position.y = 0.0f;
	m_IngredientStateList.back().Transform.Rotation.z = CRandom::GetRand( Math::RADIAN_MIN, Math::RADIAN_MAX );

	// 効果音の再生.
	CSoundManager::PlaySE( SAUCE_SE );
}

//---------------------------.
// ソースの掴みを終了する.
//---------------------------.
void CPizzaEditor::SauceGrabEnd()
{
	if ( m_IsPizzaSave					) return;
	if ( m_IsPizzaLoad					) return;
	if ( m_IsPizzaDelete				) return;
	if ( m_IsPizzaDownload				) return;
	if ( m_IsDispSauce	== false		) return;
	if ( m_GrabSauceNo	== NO_GRAB_NO	) return;
	if ( m_IsBButtonAction				) return;

	// Bボタンを押した瞬間か.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
	m_IsBButtonAction = true;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// 具材を離す.
	if ( Pos.x <= m_pTrashArea->GetSpriteState().Disp.w ||
		 Pos.x >= m_TrashAreaStateList[1].Transform.Position.x )
	{
		// 離した場所が具材エリアの場合削除する.
		m_GrabSauceNo = NO_GRAB_NO;

		// 効果音の再生.
		CSoundManager::PlaySE( TRASH_SE );
	}
 
	// 生地の上か確認.
	const D3DXPOSITION3& CenterPos	= m_PizzaBaseState.Transform.GetWorldPosition();
	D3DXVECTOR3			 Vector		= Pos - CenterPos;
	const float			 Distance	= D3DXVec3Length( &Vector );
	if ( Distance >= 180.0f ) return;

	// 角度を取得.
	float Ang = Math::ToDegree( atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x ) );
	if ( Ang < Math::DEGREE_MIN ) Ang += Math::DEGREE_MAX;

	// すでにチーズが配置されているか.
	const int AreaNo = static_cast<int>( Ang / 45.0f );
	if ( m_GrabSauceNo == 1 && m_SauceList[AreaNo].second != NO_SAUCE_NO ) return;

	// チーズを配置するか.
	const int SListNo = static_cast<int>( m_SauceStateList.size()  );
	const int CListNo = static_cast<int>( m_CheeseStateList.size() );
	if ( m_GrabSauceNo == 1 ) {
		m_CheeseStateList.emplace_back( m_SauceStateBase );
		m_CheeseStateList.back().AnimState.AnimNumber											= m_GrabSauceNo;
		m_SauceList[AreaNo].second																= CListNo;
		if ( m_SauceList[AreaNo].first == NO_SAUCE_NO ) m_CheeseStateList.back().pMaskTexture	= CRandom::GetRand( m_pSauceMaskList[AreaNo] );
		else											m_CheeseStateList.back().pMaskTexture	= m_SauceStateList[m_SauceList[AreaNo].first].pMaskTexture;

		// 効果音の再生.
		CSoundManager::PlaySE( CHEESE_GRAB_END_SE );
		return;
	}

	// すでにソースが塗られているか.
	if ( m_SauceList[AreaNo].first != NO_SAUCE_NO ) {
		// すでにあるソースを一番上に移動させる.
		const SSpriteRenderState Sauce = m_SauceStateList[m_SauceList[AreaNo].first];
		m_SauceStateList.erase( m_SauceStateList.begin() + m_SauceList[AreaNo].first );
		m_SauceStateList.emplace_back( Sauce );
		for ( auto& [Sauce, Cheese] : m_SauceList ) {
			if ( Sauce  > m_SauceList[AreaNo].first ) Sauce--;
		}

		// ソースを変更する.
		m_SauceList[AreaNo].first											= SListNo - 1;
		m_SauceStateList[m_SauceList[AreaNo].first].AnimState.AnimNumber	= m_GrabSauceNo;
		m_SauceStateList[m_SauceList[AreaNo].first].pMaskTexture			= CRandom::GetRand( m_pSauceMaskList[AreaNo] );

		// チーズがある場合隠れるため無くす.
		if ( m_SauceList[AreaNo].second != NO_SAUCE_NO ) {
			m_CheeseStateList.erase( m_CheeseStateList.begin() + m_SauceList[AreaNo].second );
			for ( auto& [Sauce, Cheese] : m_SauceList ) {
				if ( Cheese > m_SauceList[AreaNo].second ) Cheese--;
			}
			m_SauceList[AreaNo].second = NO_SAUCE_NO;
		}

		// 効果音の再生.
		CSoundManager::PlaySE( SAUCE_SE );
		return;
	}

	// ソースを塗る.
	m_SauceStateList.emplace_back( m_SauceStateBase );
	m_SauceStateList.back().AnimState.AnimNumber = m_GrabSauceNo;
	m_SauceStateList.back().pMaskTexture		 = CRandom::GetRand( m_pSauceMaskList[AreaNo] );
	m_SauceList[AreaNo].first					 = SListNo;

	// チーズがある場合隠れるため無くす.
	if ( m_SauceList[AreaNo].second != NO_SAUCE_NO ) {
		m_CheeseStateList.erase( m_CheeseStateList.begin() + m_SauceList[AreaNo].second );
		for ( auto& [Sauce, Cheese] : m_SauceList ) {
			if ( Cheese > m_SauceList[AreaNo].second ) Cheese--;
		}
		m_SauceList[AreaNo].second = NO_SAUCE_NO;
	}

	// 効果音の再生.
	CSoundManager::PlaySE( SAUCE_SE );
}

//---------------------------.
// 掴みの更新.
//---------------------------.
void CPizzaEditor::GradUpdate()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_GrabIngredientNo <= NO_GRAB_NO	) return;

	// 座標の更新.
	m_IngredientStateList[m_GrabIngredientNo].Transform.SetWorldPosition( m_CursorState.Transform.Position + m_GrabPosition );

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_IngredientStateList[m_GrabIngredientNo].Transform.GetWorldPosition();

	// コピーするか.
	if ( m_IsDispSauce ) return;
	if ( Pos.x > m_pTrashArea->GetSpriteState().Disp.w &&
		 Pos.x < m_TrashAreaStateList[1].Transform.Position.x && 
		 CXInput::IsKeyDown( XINPUT_GAMEPAD_A ) )
	{
		if ( m_IngredientStateList[m_GrabIngredientNo].AnimState.AnimNumber < 0 ) {
			// 生地の上か確認.
			const D3DXPOSITION3& CenterPos	= m_PizzaBaseState.Transform.GetWorldPosition();
			D3DXVECTOR3			 Vector		= Pos - CenterPos;
			const float			 Distance	= D3DXVec3Length( &Vector );
			if ( Distance >= 180.0f ) return;

			// 具材のコピーの配置.
			m_IngredientStateList.emplace_back( m_IngredientStateList[m_GrabIngredientNo] );
			m_IngredientStateList.back().Transform.Position = { 0.0f, 0.0f, 0.0f };

			// 効果音の再生.
			CSoundManager::PlaySE( SAUCE_SE );
		}
		else {
			// 具材のコピーの配置.
			m_IngredientStateList.emplace_back( m_IngredientStateList[m_GrabIngredientNo] );

			// 効果音の再生.
			CSoundManager::PlaySE( INGREDIENT_GRAB_END_SE );

			// まな板の上ではないか.
			if ( 390.0f >= Pos.x || Pos.x >= 899.0f ||
				 107.0f >= Pos.y || Pos.y >= 608.0f )
			{
				m_IngredientStateList.back().UIAnimState.Flag_x = true;

				// 効果音の再生.
				CSoundManager::PlaySE( FALL_SE );
			}
		}
	}
}

//---------------------------.
// 掴んでる具材の回転.
//---------------------------.
void CPizzaEditor::GradRotation()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo <= NO_GRAB_NO	) return;

	// 移動速度の変更.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_RIGHT_THUMB ) ) {
		m_IngredientRotSpeedNo++;
		if ( m_IngredientRotSpeedNo >= 3 ) m_IngredientRotSpeedNo = 0;
	}

	// 具材の回転.
	m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z -= Math::ToRadian( m_IngredientRotSpeedList[m_IngredientRotSpeedNo] ) * CXInput::GetLTrigger();
	m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z += Math::ToRadian( m_IngredientRotSpeedList[m_IngredientRotSpeedNo] ) * CXInput::GetRTrigger();
	
	// 角度を範囲内に収める.
	if ( m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z < Math::RADIAN_MIN ) {
		m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z += Math::RADIAN_MAX;
	}
	if ( m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z > Math::RADIAN_MAX ) {
		m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z -= Math::RADIAN_MAX;
	}
}

//---------------------------.
// 上下ボタンを押した時の更新.
//---------------------------.
void CPizzaEditor::PushUpDownButton()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsBButtonAction					) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;
	if ( m_GrabSauceNo		!= NO_GRAB_NO	) return;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// ボタンに触れているか.
	const SSize& Size = m_pUpDownButton->GetSpriteState().Disp;
	for ( auto& b : m_UpDownButtonStateList ) {
		// 押しても意味がないか.
		if ( m_IsDispSauce ) {
			if ( static_cast<int>( std::ceil( ( m_SauceAreaStateBase.AnimState.AnimNumberMax - 1 ) / SAUCE_MAX ) ) - 1 == 0 ) {
				if (	  b.AnimState.AnimNumber == UpButton	) b.AnimState.AnimNumber = PushUpButton;
				else if ( b.AnimState.AnimNumber == DownButton	) b.AnimState.AnimNumber = PushDownButton;
				continue;
			}
		}
		else {
			if ( static_cast<int>( std::ceil( m_IngredientAreaStateBase.AnimState.AnimNumberMax / INGREDIENTS_MAX ) ) == 0 ) {
				if (	  b.AnimState.AnimNumber == UpButton	) b.AnimState.AnimNumber = PushUpButton;
				else if ( b.AnimState.AnimNumber == DownButton	) b.AnimState.AnimNumber = PushDownButton;
				continue;
			}
		}

		// ボタンの見た目の変更.
		if ( CXInput::IsKeyUp( XINPUT_GAMEPAD_B ) ) {
			if (	  b.AnimState.AnimNumber == PushUpButton	) b.AnimState.AnimNumber = UpButton;
			else if ( b.AnimState.AnimNumber == PushDownButton	) b.AnimState.AnimNumber = DownButton;
		}

		// ボタンに触れているか.
		if ( b.Transform.Position.x <= Pos.x - 32.0f && Pos.x <= b.Transform.Position.x + Size.w &&
			 b.Transform.Position.y <= Pos.y - 32.0f && Pos.y <= b.Transform.Position.y + Size.h )
		{
			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// Bボタンを押した瞬間か.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			if ( b.AnimState.AnimNumber == 0 )	m_AreaPageNo--;
			else								m_AreaPageNo++;
			m_IsBButtonAction = true;
			b.AnimState.AnimNumber++;

			// 具材置き場のページの上限処理.
			if ( m_IsDispSauce ) ChangeSauce();
			else				 ChangeIngredient();

			// 効果音の再生.
			CSoundManager::PlaySE( BUTTON_SE );
		}
		else {
			// ボタンの見た目をもとに戻す.
			if (	  b.AnimState.AnimNumber == PushUpButton	) b.AnimState.AnimNumber = UpButton;
			else if ( b.AnimState.AnimNumber == PushDownButton	) b.AnimState.AnimNumber = DownButton;
		}
	}
}

//---------------------------.
// 左右ボタンを押した時の更新.
//---------------------------.
void CPizzaEditor::PushLeftRightButton()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsBButtonAction					) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;
	if ( m_GrabSauceNo		!= NO_GRAB_NO	) return;

	// 座標の取得.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// ボタンに触れているか.
	const SSize& Size = m_pLeftRightButton->GetSpriteState().Disp;
	for ( auto& b : m_LeftRightButtonStateList ) {
		// 押しても意味がないか.
		if ( static_cast<int>( std::ceilf( m_SavePizzaNum / 6.0f ) ) - 1 == 0 ) {
			if (	  b.AnimState.AnimNumber == LeftButton	) b.AnimState.AnimNumber = PushLeftButton;
			else if ( b.AnimState.AnimNumber == RightButton ) b.AnimState.AnimNumber = PushRightButton;
			continue;
		}

		// ボタンの見た目の変更.
		if ( CXInput::IsKeyUp( XINPUT_GAMEPAD_B ) ) {
			if (	  b.AnimState.AnimNumber == PushLeftButton	) b.AnimState.AnimNumber = LeftButton;
			else if ( b.AnimState.AnimNumber == PushRightButton	) b.AnimState.AnimNumber = RightButton;
		}

		// ボタンに触れているか.
		if ( b.Transform.Position.x <= Pos.x - 32.0f && Pos.x <= b.Transform.Position.x + Size.w &&
			 b.Transform.Position.y <= Pos.y - 32.0f && Pos.y <= b.Transform.Position.y + Size.h )
		{
			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// Bボタンを押した瞬間か.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			if ( b.AnimState.AnimNumber == 0 )	m_SavePizzaPageNo--;
			else								m_SavePizzaPageNo++;
			m_IsBButtonAction = true;
			b.AnimState.AnimNumber++;

			// ページの上限処理.
			if ( m_SavePizzaPageNo < 0 ) {
				m_SavePizzaPageNo = static_cast<int>( std::ceilf( m_SavePizzaNum / 6.0f ) ) - 1;
			}
			else if ( m_SavePizzaPageNo * 6 >= m_SavePizzaNum ) {
				m_SavePizzaPageNo = 0;
			}

			// 保存しているピザのサンプルの初期化.
			SavePizzaSampleInit();

			// 効果音の再生.
			CSoundManager::PlaySE( BUTTON_SE );
		}
		else {
			// ボタンの見た目をもとに戻す.
			if (	  b.AnimState.AnimNumber == PushLeftButton	) b.AnimState.AnimNumber = LeftButton;
			else if ( b.AnimState.AnimNumber == PushRightButton	) b.AnimState.AnimNumber = RightButton;
		}
	}
}

//---------------------------.
// ソースの変更.
//---------------------------.
void CPizzaEditor::ChangeSauce()
{
	// ページの上限処理.
	if ( m_AreaPageNo < 0 ) {
		m_AreaPageNo = static_cast<int>( std::ceil( ( m_SauceAreaStateBase.AnimState.AnimNumberMax - 1 ) / SAUCE_MAX ) ) - 1;
	}
	else if ( m_AreaPageNo * SAUCE_MAX + 1 >= m_SauceAreaStateBase.AnimState.AnimNumberMax ) {
		m_AreaPageNo = 0;
	}

	// 具材置き場の見た目の変更.
	for ( int i = 0; i < SAUCE_MAX; ++i ) {
		m_SauceAreaStateList[i].AnimState.AnimNumber = m_AreaPageNo * SAUCE_MAX + i + 1;
		
		// 用意されていない場合空箱に変更する.
		if ( m_SauceAreaStateList[i].AnimState.AnimNumber >= m_SauceAreaStateList[i].AnimState.AnimNumberMax ) {
			m_SauceAreaStateList[i].AnimState.AnimNumber = 0;
		}
	}
}

//---------------------------.
// 具材の変更.
//---------------------------.
void CPizzaEditor::ChangeIngredient()
{
	// ページの上限処理.
	if ( m_AreaPageNo < 0 ) {
		m_AreaPageNo = static_cast<int>( std::ceil( m_IngredientAreaStateBase.AnimState.AnimNumberMax / INGREDIENTS_MAX ) );
	}
	else if ( m_AreaPageNo * INGREDIENTS_MAX + 1 >= m_IngredientAreaStateBase.AnimState.AnimNumberMax ) {
		m_AreaPageNo = 0;
	}

	// 具材置き場の見た目の変更.
	for ( int i = 0; i < INGREDIENTS_MAX; ++i ) {
		m_IngredientAreaStateList[i].AnimState.AnimNumber = m_AreaPageNo * INGREDIENTS_MAX + i + 1;
		
		// 用意されていない場合空箱に変更する.
		if ( m_IngredientAreaStateList[i].AnimState.AnimNumber >= m_IngredientAreaStateList[i].AnimState.AnimNumberMax ) {
			m_IngredientAreaStateList[i].AnimState.AnimNumber = 0;
		}
	}
}

//---------------------------.
// テキストボタンを押した時の更新.
//---------------------------.
void CPizzaEditor::PushTextButton()
{
	if ( m_IsPutInKiln								) return;
	if ( m_IsBButtonAction							) return;
	if ( m_GrabIngredientNo			!= NO_GRAB_NO	) return;
	if ( m_GrabSauceNo				!= NO_GRAB_NO	) return;

	for( auto& t : m_TextButtonStateList ){
		if ( t.IsDisp == false ) continue;

		// ボタンの見た目の変更.
		int* pButtonNo = &t.AnimState.AnimNumber;
		if ( CXInput::IsKeyUp( XINPUT_GAMEPAD_B ) ) {
			if ( *pButtonNo % 2 != 0 ) *pButtonNo -= 1;
		}

		// ボタンに触れているか.
		const SSize&		 ButtonSize	= m_pTextButton->GetSpriteState().Disp;
		const float			 SizeAdjust = t.AnimState.AnimNumber >= YesButton ? 46.0f : 0.0f;
		const D3DXPOSITION3& ButtonPos	= t.Transform.Position;
		const D3DXPOSITION3& CursorPos	= m_CursorState.Transform.Position;
		if ( ButtonPos.x - ButtonSize.w / 2.0f + SizeAdjust + 24.0f <= CursorPos.x && CursorPos.x <= ButtonPos.x + ButtonSize.w / 2.0f - SizeAdjust + 24.0f &&
			 ButtonPos.y - ButtonSize.h / 2.0f				+ 24.0f	<= CursorPos.y && CursorPos.y <= ButtonPos.y + ButtonSize.h / 2.0f				+ 24.0f )
		{
			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// Bボタンを押した瞬間か.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			*pButtonNo += 1;
			m_IsBButtonAction = true;

			// ボタンを押したとき.
			if ( *pButtonNo == PushDecisionButton ) {
				// 具材の切り替え.
				m_IsDispSauce	= false;
				*pButtonNo		= PushPutInKilnButton;
			}
			else if ( *pButtonNo == PushPutInKilnButton	) {
				// 窯に入れるアニメーションの開始.
				m_IsPutInKiln				= true;
				m_AnimSpatulaState.IsDisp	= true;
				m_PutInKilnAnimNo			= SpatulaDispAnim;

				// ゴミ箱エリアの設定.
				for ( auto& a : m_TrashAreaStateList ) {
					a.Color.w				= Color::ALPHA_MIN;
					a.AnimState.AnimNumber	= 1;
				}

				// SEの再生.
				CSoundManager::PlaySE( SPATULA_SE );
			}
			else {
				if (	  m_IsPizzaSave		) PizzaSave(	 *pButtonNo );
				else if ( m_IsPizzaLoad		) PizzaLoad(	 *pButtonNo );
				else if ( m_IsPizzaDelete	) PizzaDelete(	 *pButtonNo );
				else if ( m_IsPizzaDownload	) PizzaDownload( *pButtonNo );
			}

			// 効果音の再生.
			CSoundManager::PlaySE( BUTTON_SE );
		}
		else {
			// ボタンの見た目をもとに戻す.
			if ( *pButtonNo % 2 != 0 ) *pButtonNo -= 1;
		}
	}
}

//---------------------------.
// ピザの保存.
//---------------------------.
void CPizzaEditor::PizzaSave( const int ButtonNo )
{
	m_IsPizzaSave = false;

	// 具材の配置に戻る.
	m_TextButtonStateList[0].AnimState.AnimNumber	= PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// ピザデータの作成.
	if ( ButtonNo != PushYesButton ) return;
	const int SSize = static_cast<int>( m_SauceStateList.size()		 );
	const int CSize = static_cast<int>( m_CheeseStateList.size()	 );
	const int ISize = static_cast<int>( m_IngredientStateList.size() );
	json j;
	j["Sauce"] = {};
	for ( int i = 0; i < SSize; ++i ) {
		j["Sauce"][i]["Type"]					= m_SauceStateList[i].AnimState.AnimNumber;
		j["Sauce"][i]["MaskType"]				= m_pSauceMaskNoMap[m_SauceStateList[i].pMaskTexture].first;
		j["Sauce"][i]["MaskNo"]					= m_pSauceMaskNoMap[m_SauceStateList[i].pMaskTexture].second;
	}
	j["Cheese"] = {};
	for ( int i = 0; i < CSize; ++i ) {
		j["Cheese"][i]["Type"]					= m_CheeseStateList[i].AnimState.AnimNumber;
		j["Cheese"][i]["MaskType"]				= m_pSauceMaskNoMap[m_CheeseStateList[i].pMaskTexture].first;
		j["Cheese"][i]["MaskNo"]				= m_pSauceMaskNoMap[m_CheeseStateList[i].pMaskTexture].second;
	}
	j["Ingredients"] = {};
	for ( int i = 0; i < ISize; ++i ) {
		j["Ingredients"][i]["Position"]["x"]	= m_IngredientStateList[i].Transform.Position.x;
		j["Ingredients"][i]["Position"]["y"]	= m_IngredientStateList[i].Transform.Position.y;
		j["Ingredients"][i]["Rotation"]			= m_IngredientStateList[i].Transform.Rotation.z;
		j["Ingredients"][i]["Type"]				= m_IngredientStateList[i].AnimState.AnimNumber;
	}

	// 保存しているピザの数を加算.
	m_SavePizzaNum++;

	// ピザの名前の作成.
	const std::vector<std::string>& PizzaList = CPizzaResource::GetPizzaList();
	std::string			FileName	= "Pizz_" + std::to_string( m_SavePizzaNum );
	bool				IsNameOK	= true;
	int					NameNo		= 0;
	const std::string	OldName		= FileName;
	do {
		IsNameOK = true;
		for ( auto& p : PizzaList ) {
			// 同じの名前のファイルがあるため後ろにかっこを付ける.
			if ( p == FileName ) {
				NameNo++;
				FileName = OldName + "(" + std::to_string( NameNo ) + ")";

				// かっこを付けた状態で名前がかぶらないかもう一度確認する.
				IsNameOK = false;
				break;
			}
		}
	} while ( !IsNameOK );

	// ピザデータを保存.
	const std::string FilePath = SAVE_FILE_PATH + FileName + ".json";
	FileManager::JsonSave( FilePath, j );

	// ピザデータをリソースに追加する.
	CPizzaResource::PushPizzaData( FileName, j );
	m_SavePizzaData.emplace_back( std::make_pair( FileName, j ) );

	// 保存しているピザのサンプルの更新.
	SavePizzaSampleInit();

	// SEの再生.
	CSoundManager::PlaySE( DECISION_SE );
}

//---------------------------.
// ピザの読み込み.
//---------------------------.
void CPizzaEditor::PizzaLoad( const int ButtonNo )
{
	// 初期化.
	m_IsDispSauce	= false;
	m_IsPizzaLoad	= false;
	m_ButtonAnimCnt	= INIT_FLOAT;

	// 具材の配置に戻る.
	m_TextButtonStateList[0].AnimState.AnimNumber	= PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// 現在のピザをサンプルのピザで上書きする.
	if ( ButtonNo != PushYesButton ) return;
	const int PizzaNo = std::stoi( m_pSelectSaveAreaState->No ) - 6 * m_SavePizzaPageNo;
	m_PizzaBaseState.AnimState.AnimNumber	= 0;
	m_SauceStateList						= m_SampleSauceStateList[PizzaNo];
	m_CheeseStateList						= m_SampleCheeseStateList[PizzaNo];
	m_IngredientStateList					= m_SampleIngredientStateList[PizzaNo];
	for ( auto& s : m_SauceStateList		) s.Transform.pParent = &m_PizzaBaseState.Transform;
	for ( auto& c : m_CheeseStateList ) {
		c.Transform.pParent		= &m_PizzaBaseState.Transform;
		c.AnimState.AnimNumber	= 1;
	}
	for ( auto& i : m_IngredientStateList ) {
		i.Transform.pParent = &m_PizzaBaseState.Transform;
		if ( i.AnimState.AnimNumber < 0 ) {
			i.AnimState.AnimNumber += static_cast<int>( m_IngredientSauceState.AnimState.AnimNumberMax / 2.0f );
		}
		else {
			i.AnimState.AnimNumber -= static_cast<int>( i.AnimState.AnimNumberMax / 2.0f );;
		}
	}

	// 選択しているセーブエリアの情報を破棄する.
	m_pSelectSaveAreaState = nullptr;

	// SEの再生.
	CSoundManager::PlaySE( DECISION_SE );
}

//---------------------------.
// ピザの削除.
//---------------------------.
void CPizzaEditor::PizzaDelete( const int ButtonNo )
{
	// 初期化.
	m_IsPizzaDelete		= false;
	m_ButtonAnimCnt		= INIT_FLOAT;

	// 具材の配置に戻る.
	if ( m_IsDispSauce ) m_TextButtonStateList[0].AnimState.AnimNumber	= DecisionButton;
	else				 m_TextButtonStateList[0].AnimState.AnimNumber	= PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// ピザを削除.
	if ( ButtonNo != PushYesButton ) return;
	const int PizzaNo = std::stoi( m_pSelectSaveAreaState->No );
	CPizzaResource::PizzaDelete( m_SavePizzaData[PizzaNo].first );
	m_SavePizzaData.erase( m_SavePizzaData.begin() + PizzaNo );

	// 保存しているピザの数を減らす.
	m_SavePizzaNum--;

	// ページが空っぽになるか確認.
	if ( m_SavePizzaPageNo * 6 >= m_SavePizzaNum ) {
		m_SavePizzaPageNo = static_cast<int>( std::ceilf( m_SavePizzaNum / 6.0f ) ) - 1;
	}

	// 保存しているピザのサンプルの更新.
	SavePizzaSampleInit();

	// 選択しているセーブエリアの情報を破棄する.
	m_pSelectSaveAreaState = nullptr;

	// SEの再生.
	CSoundManager::PlaySE( TRASH_SE );
}

//---------------------------.
// ピザのダウンロード.
//---------------------------.
void CPizzaEditor::PizzaDownload( const int ButtonNo )
{
	// 初期化.
	m_IsPizzaDownload	= false;
	m_ButtonAnimCnt		= INIT_FLOAT;

	// 具材の配置に戻る.
	if ( m_IsDispSauce ) m_TextButtonStateList[0].AnimState.AnimNumber = DecisionButton;
	else				 m_TextButtonStateList[0].AnimState.AnimNumber = PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// ユーザー名の取得.
	if ( ButtonNo != PushYesButton ) return;
	char UserName[256];
	DWORD Size = sizeof( UserName );
	if ( GetUserNameA( UserName, &Size ) == 0 ) {
		// ユーザー名の取得に失敗した.
		ErrorMessage( "ピザのダウンロードに失敗しました。" );
		return;
	}

	// ダウンロードフォルダの絶対パスの作成.
	const int PizzaNo = std::stoi( m_pSelectSaveAreaState->No );
	std::string Path = "C:\\Users\\" + std::string( UserName ) + "\\Downloads\\" + m_SavePizzaData[PizzaNo].first + ".json";

	// ピザデータを出力.
	FileManager::JsonSave( Path, m_SavePizzaData[PizzaNo].second );

	// 選択しているセーブエリアの情報を破棄する.
	m_pSelectSaveAreaState = nullptr;

	// SEの再生.
	CSoundManager::PlaySE( DECISION_SE );
}

//---------------------------.
// 削除エリアの更新.
//---------------------------.
void CPizzaEditor::TrashAreaUpdate()
{
	if ( m_IsPutInKiln		) return;
	if ( m_IsPizzaSave		) return;
	if ( m_IsPizzaLoad		) return;
	if ( m_IsPizzaDelete	) return;
	if ( m_IsPizzaDownload	) return;

	// 透明にしていく.
	if ( m_GrabIngredientNo == NO_GRAB_NO && m_GrabSauceNo == NO_GRAB_NO ) {
		if ( m_TrashAreaStateList[0].Color.w == Color::ALPHA_MIN ) return;
		for ( auto& a : m_TrashAreaStateList ) {
			a.Color.w -= Color::ALPHA_MAX * 3.0f * m_DeltaTime;
			if ( a.Color.w <= Color::ALPHA_MIN ) a.Color.w = Color::ALPHA_MIN;
		}
		return;
	}
	
	// 不透明にしていく.
	if ( m_TrashAreaStateList[0].Color.w == Color::ALPHA_MAX ) return;
	for ( auto& a : m_TrashAreaStateList ) {
		a.AnimState.AnimNumber = 0;
		a.Color.w += Color::ALPHA_MAX * 3.0f * m_DeltaTime;
		if ( a.Color.w >= Color::ALPHA_MAX ) a.Color.w = Color::ALPHA_MAX;
	}
}

//---------------------------.
// 具材の削除の更新.
//---------------------------.
void CPizzaEditor::IngredientDeleteUpdate()
{
	int		No	= 0;
	auto	itr = m_IngredientStateList.begin();
	while ( itr != m_IngredientStateList.end() ) {
		if ( itr->UIAnimState.Flag_x ) {
			// 具材を小さくしていく.
			float Scale = itr->Transform.Scale.x - 1.5f * m_DeltaTime;
			if ( Scale <= 0.0f ) Scale = 0.0f;
			itr->Transform.Scale		= { Scale, Scale, Scale };
			itr->Color.w				= Scale;
			itr->Transform.Rotation.z	+= Math::ToRadian( 90.0f ) * m_DeltaTime;

			// 見えなくなるまで小さくなったか.
			if ( Scale == 0.0f ) {
				itr = m_IngredientStateList.erase( itr );
				if ( m_GrabIngredientNo > No ) m_GrabIngredientNo--;
			}
			else {
				itr++;
			}
		}
		else {
			itr++;
		}
		No++;
	}
}

//---------------------------.
// 窯に入れるアニメーションの更新.
//---------------------------.
void CPizzaEditor::PutInKilnAnimUpdate()
{
	if ( m_IsPutInKiln == false ) return;

	switch ( m_PutInKilnAnimNo ) {
	case SpatulaDispAnim:		SpatulaDisp();		break;	// ヘラを表示.
	case SpatulaBackAnim:		SpatulaBack();		break;	// ヘラの戻す.
	case PutInKilnAnim:			PutInKiln();		break;	// 窯に入れる.
	case CookPizzaAnim:			CookPizza();		break;	// ピザを焼く.
	case RemoveFromKilnAnim:	RemoveFromKiln();	break;	// 窯から出す.
	default:										break;
	}
}

//---------------------------.
// ヘラを表示.
//---------------------------.
void CPizzaEditor::SpatulaDisp()
{
	const SSize& SpatulaSize = m_pSpatula->GetSpriteState().Disp;

	// アニメーション用のカウントの加算.
	m_PutInKilnAnimCnt += m_DeltaTime;
	if ( m_PutInKilnAnimCnt >= 1.5f ) {
		// 次のアニメーションの準備.
		m_AnimSpatulaState.Transform.Position.y = FWND_H - 64.0f;
		m_PutInKilnAnimCnt						= INIT_FLOAT;
		m_AnimSpatulaState.IsDisp				= false;
		m_SpatulaState.IsDisp					= true;
		m_PutInKilnAnimNo						= PutInKilnAnim;

		// ヘラの外の具材は削除する.
		for ( auto& i : m_IngredientStateList ) {
			const D3DXPOSITION3& Pos = i.Transform.GetWorldPosition();
			if ( 428.0f >= Pos.x || Pos.x >= 858.0f ||
				 140.0f >= Pos.y || Pos.y >= 565.0f )
			{
				i.UIAnimState.Flag_x = true;
				i.Transform.DetachParent();
			}
		}
		return;
	}

	// ヘラを取り出す.
	m_AnimSpatulaState.Transform.Position.y = FWND_H + SpatulaSize.h + m_PutInKilnAnimVector.y * CEasing::OutQuart( m_PutInKilnAnimCnt, 1.5f );
	
	// テキストボタンを非表示にしていく.
	if ( m_TextButtonStateList[0].Color.w <= Color::ALPHA_MIN ) return;
	m_TextButtonStateList[0].Color.w = Color::ALPHA_MAX - m_PutInKilnAnimCnt;
	if ( m_TextButtonStateList[0].Color.w <= Color::ALPHA_MIN ) m_TextButtonStateList[0].Color.w = Color::ALPHA_MIN;
	for ( auto& a : m_TrashAreaStateList ) {
		a.Color.w = m_PutInKilnAnimCnt;
		if ( a.Color.w >= Color::ALPHA_MAX ) a.Color.w = Color::ALPHA_MAX;
	}
}

//---------------------------.
// ヘラを戻す.
//---------------------------.
void CPizzaEditor::SpatulaBack()
{
	const SSize& SpatulaSize = m_pSpatula->GetSpriteState().Disp;

	// アニメーション用のカウントの加算.
	m_PutInKilnAnimCnt += m_DeltaTime;
	if ( m_PutInKilnAnimCnt >= 1.2f ) {
		// アニメーションの終了.
		m_AnimSpatulaState.Transform.Position.y = FWND_H + SpatulaSize.h;
		m_PutInKilnAnimCnt						= INIT_FLOAT;
		m_AnimSpatulaState.IsDisp				= false;
		m_SpatulaState.IsDisp					= false;
		m_IsPutInKiln							= false;
		m_IsPizzaSave							= true;
		return;
	}

	// ヘラを取り出す.
	m_AnimSpatulaState.Transform.Position.y = FWND_H + SpatulaSize.h + m_PutInKilnAnimVector.y * ( 1.0f - CEasing::OutQuart( m_PutInKilnAnimCnt, 5.0f ) );

	// テキストボタンを表示にしていく.
	if ( m_TextButtonStateList[0].Color.w >= Color::ALPHA_MAX ) return;
	for ( auto& t : m_TextButtonStateList ) {
		t.Color.w = m_PutInKilnAnimCnt;
		if ( t.Color.w >= Color::ALPHA_MAX ) t.Color.w = Color::ALPHA_MAX;
	}

	m_FontState.Color.w = m_PutInKilnAnimCnt;
	if ( m_FontState.Color.w >= Color::ALPHA_MAX ) {
		m_FontState.Color.w = Color::ALPHA_MAX;

		// SEの再生.
		CSoundManager::PlaySE( OPEN_SE );
	}
}

//---------------------------.
// 窯に入れる.
//---------------------------.
void CPizzaEditor::PutInKiln()
{
	// 窯に入れる.
	m_SpatulaState.Transform.Position.y -= 3.0f;
	if ( m_SpatulaState.Transform.Position.y <= 40.0f ) {
		// 次のアニメーションの準備.
		m_SpatulaState.Transform.Position.y = 40.0f;
		m_PutInKilnAnimNo					= CookPizzaAnim;

		// SEの再生.
		CSoundManager::PlaySE( COOK_PIZZA_SE );
		return;
	};
}

//---------------------------.
// ピザを焼く.
//---------------------------.
void CPizzaEditor::CookPizza()
{
	// 一定時間待機する.
	m_PutInKilnAnimCnt += m_DeltaTime;
	if ( m_PutInKilnAnimCnt > 5.0f ) {
		// 生地の見た目の変更.
		m_PizzaBaseState.AnimState.AnimNumber = 1;
		// チーズの見た目の変更.
		for ( auto& c : m_CheeseStateList ) {
			if ( c.AnimState.AnimNumber > 1 ) continue;
			c.AnimState.AnimNumber = 0;
		}

		// 具材の見た目の変更.
		auto itr = m_IngredientStateList.begin();
		while ( itr != m_IngredientStateList.end() ) {
			// 生地の上か確認.
			const float Distance = D3DXVec3Length( &itr->Transform.Position );
			if ( Distance >= 90.0f ) {
				// 生地の外にあるため削除する.
				itr = m_IngredientStateList.erase( itr );
			}
			else{
				// 焼けてあるやつか.
				if ( itr->AnimState.AnimNumber >= static_cast<int>( itr->AnimState.AnimNumberMax / 2.0f ) ||
					 itr->AnimState.AnimNumber < -static_cast<int>( m_IngredientSauceState.AnimState.AnimNumberMax / 2.0f ) )
				{
					itr++;
					continue;
				}
				
				// 具材のソースの見た目の変更.
				if ( itr->AnimState.AnimNumber < 0 ) {
					itr->AnimState.AnimNumber -= static_cast<int>( m_IngredientSauceState.AnimState.AnimNumberMax / 2.0f );
					itr++;
					continue;
				}
				itr->AnimState.AnimNumber += static_cast<int>( itr->AnimState.AnimNumberMax / 2.0f );
				itr++;
			}
		}

		// 次のアニメーションの準備.
		m_PutInKilnAnimCnt	= INIT_FLOAT;
		m_PutInKilnAnimNo	= RemoveFromKilnAnim;
		return;
	};
}

//---------------------------.
// 窯から出す.
//---------------------------.
void CPizzaEditor::RemoveFromKiln()
{
	// 窯から出す.
	m_SpatulaState.Transform.Position.y += 2.0f;
	if ( m_SpatulaState.Transform.Position.y >= FWND_H - 64.0f ) {
		// 次のアニメーションの準備.
		m_SpatulaState.Transform.Position.y = FWND_H - 64.0f;
		m_PutInKilnAnimNo					= SpatulaBackAnim;
		m_AnimSpatulaState.IsDisp			= true;
		m_SpatulaState.IsDisp				= false;

		// ボタンの準備をする.
		m_TextButtonStateList[0].IsDisp					= true;
		m_TextButtonStateList[1].IsDisp					= true;
		m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
		m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
		m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
		m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
		m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
		m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

		// テキストの準備をする.
		m_FontState.Text								= "ピザを保存しますか？";
		m_FontState.Transform.Position.x				= FWND_W / 2.0f;
		m_FontState.Transform.Position.y				= 130.0f;
		m_FontState.Color.w								= Color::ALPHA_MIN;

		// SEの再生.
		CSoundManager::PlaySE( SPATULA_SE );
		return;
	};
}

//---------------------------.
// ポーズに移動.
//---------------------------.
void CPizzaEditor::MovePause()
{
	if ( m_IsPizzaSave		) return;
	if ( m_IsPizzaLoad		) return;
	if ( m_IsPizzaDelete	) return;
	if ( m_IsPizzaDownload	) return;
	if ( m_IsPutInKiln		) return;

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
void CPizzaEditor::PauseOpen()
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
void CPizzaEditor::PauseClose()
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
void CPizzaEditor::PauseUpdate()
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
		// 「ピザ作りに戻る」を選択したとき.
		case CHOICE_STARTCHOICE:
			CSoundManager::PlaySE( CLOSE_SE );
			m_IsPause		= false;
			m_IsPauseAnim	= true;
			break;
		// 「ピザを初期化する」を選択したとき.
		case CHOICE_CENTERCHOICE:
			m_SauceStateList.clear();
			m_CheeseStateList.clear();
			m_IngredientStateList.clear();
			m_AreaPageNo									= INIT_INT;
			m_GrabIngredientNo								= NO_GRAB_NO;
			m_GrabSauceNo									= NO_GRAB_NO;
			m_IsDispSauce									= true;
			m_PizzaBaseState.AnimState.AnimNumber			= 0;
			m_TextButtonStateList[0].AnimState.AnimNumber	= PushDecisionButton;
			m_TextButtonStateList[1].IsDisp					= false;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
			for ( auto& [Sauce, Cheese] : m_SauceList ) {
				Sauce	= NO_SAUCE_NO;
				Cheese	= NO_SAUCE_NO;
			}

			m_IsPause								= false;
			m_IsPauseAnim							= true;

			// SEの再生.
			CSoundManager::PlaySE( TRASH_SE );
			break;
		// 「ピザ作りを終了する」を選択したとき.
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

//---------------------------.
// 操作説明の更新.
//---------------------------.
void CPizzaEditor::TextUpdate()
{
	// ソースを掴む/塗る.
	if ( m_IsDispSauce ) {
		if ( m_GrabSauceNo		== NO_GRAB_NO ) m_TextStateList[4].AnimState.AnimNumber = GrabSauceText;
		else									m_TextStateList[4].AnimState.AnimNumber = GrabEndSauceText;
	}
	// 具材を掴む/離す.
	else {
		if ( m_GrabIngredientNo	== NO_GRAB_NO ) m_TextStateList[4].AnimState.AnimNumber = GrabText;
		else									m_TextStateList[4].AnimState.AnimNumber = GrabEndText;
	}

	// 具材を掴んでいるか.
	if ( m_GrabIngredientNo == NO_GRAB_NO ) {
		m_TextStateList[5].AnimState.AnimNumber = Gray_CopyText;
		m_TextStateList[6].AnimState.AnimNumber = Gray_ClockwiseText;
		m_TextStateList[7].AnimState.AnimNumber = Gray_AnticlockwiseText;
	}
	else {
		m_TextStateList[5].AnimState.AnimNumber = CopyText;
		m_TextStateList[6].AnimState.AnimNumber = ClockwiseText;
		m_TextStateList[7].AnimState.AnimNumber = AnticlockwiseText;
	}
}

//---------------------------.
// セーブエリアの更新.
//---------------------------.
void CPizzaEditor::SaveAreaUpdate()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsBButtonAction					) return;
	if ( m_GrabSauceNo		!= NO_GRAB_NO	) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;

	// カーソルの位置を取得.
	const D3DXPOSITION3& CursorPos = m_CursorState.Transform.Position;

	// 保存エリアのボタンの更新.
	for ( auto& s : m_SaveAreaStateList ) {
		s.AnimState.AnimNumber = 0;
		if ( s.No == "" ) {
			s.AnimState.AnimNumber = 2;
			continue;
		}

		// エリアの位置を取得.
		const D3DXPOSITION3& Pos = s.Transform.Position;

		// ピザを作るボタンの上か.
		if ( Pos.x + 135.0f + 7.0f <= CursorPos.x && CursorPos.x <= Pos.x + 175.0f	+ 25.0f &&
			 Pos.y + 1.0f	+ 7.0f <= CursorPos.y && CursorPos.y <= Pos.y + 41.0f	+ 25.0f )
		{
			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// Bボタンを押したか.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) continue;
			m_IsBButtonAction	= true;
			m_IsPizzaLoad		= true;

			// ゴミ箱エリアの設定.
			for ( auto& a : m_TrashAreaStateList ) {
				a.Color.w				= Color::ALPHA_MIN;
				a.AnimState.AnimNumber	= 1;
			}

			// ボタンの準備をする.
			m_ButtonAnimCnt									= INIT_FLOAT;
			m_TextButtonStateList[0].IsDisp					= true;
			m_TextButtonStateList[1].IsDisp					= true;
			m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
			m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
			m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

			// テキストの準備をする.
			m_FontState.Text								= "ピザを読み込みますか？";
			m_FontState.Transform.Position.x				= FWND_W / 2.0f;
			m_FontState.Transform.Position.y				= 130.0f;
			m_FontState.Color.w								= Color::ALPHA_MIN;

			// セーブエリアの情報を取得.
			m_pSelectSaveAreaState = &s;

			// SEの再生.
			CSoundManager::PlaySE( OPEN_SE );
			break;
		}

		// ピザを削除ボタンの上か.
		else if (	Pos.x + 135.0f	+ 7.0f <= CursorPos.x && CursorPos.x <= Pos.x + 175.0f	+ 25.0f &&
					Pos.y + 42.0f	+ 7.0f <= CursorPos.y && CursorPos.y <= Pos.y + 82.0f	+ 25.0f)
		{
			if ( m_SavePizzaNum <= 1 ) break;

			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// Bボタンを押したか.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) continue;
			m_IsBButtonAction	= true;
			m_IsPizzaDelete		= true;

			// ゴミ箱エリアの設定.
			for ( auto& a : m_TrashAreaStateList ) {
				a.Color.w				= Color::ALPHA_MIN;
				a.AnimState.AnimNumber	= 1;
			}

			// ボタンの準備をする.
			m_ButtonAnimCnt									= INIT_FLOAT;
			m_TextButtonStateList[0].IsDisp					= true;
			m_TextButtonStateList[1].IsDisp					= true;
			m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
			m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
			m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

			// テキストの準備をする.
			m_FontState.Text								= "ピザを削除しますか？";
			m_FontState.Transform.Position.x				= FWND_W / 2.0f;
			m_FontState.Transform.Position.y				= 130.0f;
			m_FontState.Color.w								= Color::ALPHA_MIN;

			// セーブエリアの情報を取得.
			m_pSelectSaveAreaState = &s;

			// SEの再生.
			CSoundManager::PlaySE( OPEN_SE );
			break;
		}

		// ピザをダウンロードボタンの上か.
		else if (	Pos.x + 135.0f	+ 7.0f <= CursorPos.x && CursorPos.x <= Pos.x + 175.0f	+ 25.0f &&
					Pos.y + 83.0f	+ 7.0f <= CursorPos.y && CursorPos.y <= Pos.y + 123.0f	+ 25.0f )
		{
			// カーソルの変更.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// Bボタンを押したか.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) continue;
			m_IsBButtonAction	= true;
			m_IsPizzaDownload	= true;

			// ゴミ箱エリアの設定.
			for ( auto& a : m_TrashAreaStateList ) {
				a.Color.w				= Color::ALPHA_MIN;
				a.AnimState.AnimNumber	= 1;
			}

			// ボタンの準備をする.
			m_ButtonAnimCnt									= INIT_FLOAT;
			m_TextButtonStateList[0].IsDisp					= true;
			m_TextButtonStateList[1].IsDisp					= true;
			m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
			m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
			m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

			// テキストの準備をする.
			m_FontState.Text								= "ピザをダウンロードしますか？";
			m_FontState.Transform.Position.x				= FWND_W / 2.0f;
			m_FontState.Transform.Position.y				= 130.0f;
			m_FontState.Color.w								= Color::ALPHA_MIN;

			// セーブエリアの情報を取得.
			m_pSelectSaveAreaState = &s;

			// SEの再生.
			CSoundManager::PlaySE( OPEN_SE );
			break;
		}
	}

	// ピザが一個しかないとき一番初めの見た目を変更する.
	if ( m_SavePizzaNum <= 1 ) {
		m_SaveAreaStateList[0].AnimState.AnimNumber = 1;
	}
}

//---------------------------.
// セーブエリアのボタンの更新.
//---------------------------.
void CPizzaEditor::SaveAreaButtonUpdate()
{
	if ( m_IsPizzaLoad		== false &&
		 m_IsPizzaDelete	== false &&
		 m_IsPizzaDownload	== false ) return;
	if ( m_TrashAreaStateList[0].Color.w >= Color::ALPHA_MAX ) return;

	// アニメーション用のカウントの加算.
	m_ButtonAnimCnt += m_DeltaTime;

	// 削除エリアを表示していく.
	for ( auto& a : m_TrashAreaStateList ) {
		a.Color.w = m_ButtonAnimCnt;
		if ( a.Color.w >= Color::ALPHA_MAX ) a.Color.w = Color::ALPHA_MAX;
	}

	// テキストボタンを表示していく.
	for ( auto& t : m_TextButtonStateList ) {
		t.Color.w = m_ButtonAnimCnt;
		if ( t.Color.w >= Color::ALPHA_MAX ) t.Color.w = Color::ALPHA_MAX;
	}

	// テキストを表示していく.
	m_FontState.Color.w = m_ButtonAnimCnt;
	if ( m_FontState.Color.w >= Color::ALPHA_MAX ) m_FontState.Color.w = Color::ALPHA_MAX;
}

//---------------------------.
// 保存しているピザのサンプルの初期化.
//---------------------------.
void CPizzaEditor::SavePizzaSampleInit()
{
	// ピザデータの開始位置の取得.
	const int DataStartNo = 6 * m_SavePizzaPageNo;

	// 表示数の取得.
	int RenderNum = m_SavePizzaNum - DataStartNo;
	if ( RenderNum > 6 ) RenderNum = 6;

	// サンプルの表示数の設定.
	m_SampleSauceStateList.resize( RenderNum );
	m_SampleCheeseStateList.resize( RenderNum );
	m_SampleIngredientStateList.resize( RenderNum );

	// ピザ情報の初期化.
	for ( auto& a : m_SaveAreaStateList ) a.No = "";

	// サンプルの初期化.
	for ( int i = 0; i < RenderNum; ++i ) {
		// ピザの情報の取得.
		m_SaveAreaStateList[i].No	= std::to_string( DataStartNo + i );
		json PizzaData				= m_SavePizzaData[DataStartNo + i].second;

		// ピザの初期化.
		const int SSize = static_cast<int>( PizzaData["Sauce"].size()		);
		const int CSize = static_cast<int>( PizzaData["Cheese"].size()		);
		const int ISize = static_cast<int>( PizzaData["Ingredients"].size() );
		m_SampleSauceStateList[i].resize(		SSize, m_SauceStateBase );
		m_SampleCheeseStateList[i].resize(		CSize, m_SauceStateBase );
		m_SampleIngredientStateList[i].resize(	ISize, m_IngredientStateBase );

		// ソースの設定.
		for ( int SauceNo = 0; SauceNo < SSize; ++SauceNo ) {
			json SauceData = PizzaData["Sauce"][SauceNo];
			m_SampleSauceStateList[i][SauceNo].AnimState.AnimNumber			= SauceData["Type"];
			m_SampleSauceStateList[i][SauceNo].pMaskTexture					= m_pSauceMaskList[SauceData["MaskType"]][SauceData["MaskNo"]];
			m_SampleSauceStateList[i][SauceNo].Transform.pParent			= &m_SamplePizzaBaseState.Transform;
		}

		// チーズの設定.
		for ( int CheeseNo = 0; CheeseNo < CSize; ++CheeseNo ) {
			json CheeseData = PizzaData["Cheese"][CheeseNo];
			m_SampleCheeseStateList[i][CheeseNo].AnimState.AnimNumber		= CheeseData["Type"];
			m_SampleCheeseStateList[i][CheeseNo].pMaskTexture				= m_pSauceMaskList[CheeseData["MaskType"]][CheeseData["MaskNo"]];
			m_SampleCheeseStateList[i][CheeseNo].Transform.pParent			= &m_SamplePizzaBaseState.Transform;
		}

		// 具材の設定.
		for ( int IngredientNo = 0; IngredientNo < ISize; ++IngredientNo ) {
			json IngredientData = PizzaData["Ingredients"][IngredientNo];
			m_SampleIngredientStateList[i][IngredientNo].AnimState.AnimNumber		= IngredientData["Type"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.Position.x		= IngredientData["Position"]["x"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.Position.y		= IngredientData["Position"]["y"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.Rotation.z		= IngredientData["Rotation"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.pParent			= &m_SamplePizzaBaseState.Transform;
		}
	}
}

//---------------------------.
// 保存しているピザのサンプルの描画.
//---------------------------.
void CPizzaEditor::SavePizzaSampleRender()
{
	// 表示する個数の取得.
	const int RenderNum = static_cast<int>( m_SampleSauceStateList.size() );

	// サンプルの表示.
	for ( int i = 0; i < RenderNum; ++i ) {
		// ピザ生地の描画.
		m_SamplePizzaBaseState.Transform.Position.x = 996.0f + 177.0f * ( i % 2 );
		m_SamplePizzaBaseState.Transform.Position.y = 68.0f	 + 125.0f * ( i / 2 );
		m_pPizzaBase->RenderUI( &m_SamplePizzaBaseState );

		// 具材の描画.
		for ( auto& s : m_SampleSauceStateList[i]	) m_pSauce->RenderUI( &s );
		for ( auto& c : m_SampleCheeseStateList[i]	) m_pSauce->RenderUI( &c );
		for ( auto& i : m_SampleIngredientStateList[i] ) {
			if ( i.AnimState.AnimNumber < 0 ) {
				m_IngredientSauceState.Transform			= i.Transform;
				m_IngredientSauceState.AnimState.AnimNumber = std::abs( i.AnimState.AnimNumber ) - 1;
				m_pIngredientSauce->RenderUI( &m_IngredientSauceState );
				continue;
			}
			m_pIngredient->RenderUI( &i );
		}
	}
}

//---------------------------.
// ドラック&ドロップの更新.
//---------------------------.
void CPizzaEditor::DragAndDropUpdate()
{
	if ( m_IsPizzaSave		== false &&
		 m_IsPizzaLoad		== false &&
		 m_IsPizzaDelete	== false &&
		 m_IsPizzaDownload	== false &&
		 m_IsPutInKiln		== false &&
		 m_IsPause			== false )
	{
		// ドラック&ドロップを有効にする.
		CDragAndDrop::Open();
	}
	else {
		// ドラック&ドロップを無効にする.
		CDragAndDrop::Close();
	}

	// ドラック&ドロップされたか.
	if ( CDragAndDrop::GetIsDrop() == false ) return;

	// ドラック&ドロップしたファイルリストを取得.
	std::vector<std::string> FileList = CDragAndDrop::GetFilePath();
	for ( auto& f : FileList ) {
		// jsonファイルか.
		const std::string Extension = f.substr( f.length() - 4 );
		if ( Extension != "json" && Extension != "JSON" ) continue;

		// ピザデータか.
		json j = FileManager::JsonLoad( f );
		if ( j.find( "Ingredients" ) == j.end() ) continue;

		// ファイル名を取得.
		std::string FileName = "";
		std::string::size_type Spos = f.rfind( "\\" );
		if ( Spos != std::string::npos ) {
			std::string::size_type Epos = f.find( ".", Spos + 1 );
			if ( Epos != std::string::npos ) {
				FileName = f.substr( Spos + 1, Epos - Spos - 1 );
			}
		}

		// 名前が同じフォルダが無いか確認.
		const std::vector<std::string>& PizzaList = CPizzaResource::GetPizzaList();
		bool				IsNameOK	= true;
		int					NameNo		= 0;
		const std::string	OldName		= FileName;
		do {
			IsNameOK = true;
			for ( auto& p : PizzaList ) {
				// 同じの名前のファイルがあるため後ろにかっこを付ける.
				if ( p == FileName ) {
					NameNo++;
					FileName = OldName + "(" + std::to_string( NameNo ) + ")";

					// かっこを付けた状態で名前がかぶらないかもう一度確認する.
					IsNameOK = false;
					break;
				}
			}
		} while ( !IsNameOK );

		// 移動先のファイルパスの作成.
		const std::string NewFilePath = SAVE_FILE_PATH + FileName + ".json";

		// ファイルを移動させる.
		std::filesystem::rename( f, NewFilePath );

		// リソースに追加.
		CPizzaResource::PushPizzaData( FileName, j );
		m_SavePizzaData.emplace_back( std::make_pair( FileName, j ) );
		m_SavePizzaNum++;
	}

	// ドラック&ドロップ終了.
	CDragAndDrop::End();

	// 保存しているピザのサンプルの更新.
	SavePizzaSampleInit();
}
