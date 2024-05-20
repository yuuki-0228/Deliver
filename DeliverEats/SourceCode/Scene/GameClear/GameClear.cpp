#include "GameClear.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\GameClearWidget\GameClearWidget.h"
#include "..\..\Object\Score\Score.h"
#include "..\..\Object\Score\Ranking\Ranking.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Utility\Constant\Constant.h"
#include "..\..\Utility\Random\Random.h"

namespace {
	constexpr int HEAT_BONUS			= static_cast<int>(EShowState::HeatBonus);
	constexpr int GOLDEN_TIME_BONUS		= static_cast<int>(EShowState::GoldenTimeBonus);
	constexpr int TILT_DISCOUNT			= static_cast<int>(EShowState::TiltDiscount);
	constexpr int TREATMENT_PAY			= static_cast<int>(EShowState::TreatmentPay);
	constexpr int ORIGIN_WAGE			= static_cast<int>(EShowState::OriginWage);
	constexpr int DETAIL_MAX			= 5;

	constexpr int TREATMENT_PAY_VALUE	= -200000;

	constexpr float SCALE				= 3.0f;
	constexpr float	INTERVAL_MAX		= 1.0f;
	constexpr float	ADJUST_X			= 440.0f;
	constexpr float	ADJUST_Y			= 130.0f;
	constexpr float STRING_SIZE_X		= 50.0f;
	constexpr float STRING_SIZE_Y		= 24.0f;

	constexpr float WAGE_SPEED			= 1536.0f;
	constexpr float ALPHA_SPEED			= 3.0f;
	constexpr float INTERVAL_SPEED		= 3.0f;

	enum enSpriteList :unsigned char {
		BackGround,		// 日給を表示する背景.
		Frame,			// 数字の上に表示する背景.
		Yubi,			// カーソル.
		Choices_1,		// 選択肢1.
		Choices_2,		// 選択肢2.

		Max
	}typedef ESpriteList;

	enum enFontNo :unsigned char {
		WageNumber,		// 日給の数字.
		WageYen,		// 最後に表示する「￥」.
		DetailString,	// 日給詳細の文字.
		DetailNumber,	// 日給詳細の数字.

		Font_Max
	}typedef EFontNo;

	// 効果音.
	constexpr int	DIGIT_SE_MAX		= 7;
	constexpr char	DIGIT_1_SE[]		= "Digit_1";
	constexpr char	DIGIT_10_SE[]		= "Digit_10";
	constexpr char	DIGIT_100_SE[]		= "Digit_100";
	constexpr char	DIGIT_1000_SE[]		= "Digit_1000";
	constexpr char	DIGIT_10000_SE[]	= "Digit_10000";
	constexpr char	DIGIT_100000_SE[]	= "Digit_100000";
	constexpr char	DIGIT_1000000_SE[]	= "Digit_1000000";

	constexpr char	DRUMROLL_SE[]		= "DrumRoll";
	constexpr char	CASH_REGISTER_SE[]	= "CashRegister";
	constexpr char	GET_BONUS_SE[]		= "GetBonus";
	constexpr char	GET_PENALTY_SE[]	= "GetPenalty";

	constexpr char	CURSOR_MOVE_SE[]	= "CursorMove";
}

CGameClear::CGameClear()
	: m_DeltaTime			()
	, m_pGameClearWidget	( nullptr )
	, m_pSpriteList			( Max )
	, m_SpriteStateList		( Max )
	, m_IsStartDisp			( false )
	, m_CanOperate			( false )
	, m_Alpha				( INIT_FLOAT )
	, m_Choice				( INIT_INT )

	, m_ShowState			( EShowState::OriginWage )
	, m_ValueList			( DETAIL_MAX )
	, m_PosX				()
	, m_WageDigits			( INIT_INT )
	, m_WageFinalXPos		()
	, m_IsPlayDigitSE		()
	, m_DigitSE				()
	, m_ThroughStringCnt	()
	, m_IsPlayGetBonusSE	()
	, m_pFont				( nullptr )
	, m_FontStateList		( Font_Max )
{
}

CGameClear::~CGameClear()
{
}

//---------------------------.
// 読み込み.
//---------------------------.
bool CGameClear::Init()
{
	// 定数の初期化.
	CConstant::GameClearLoad();

	m_pGameClearWidget = std::make_unique<CGameClearWidget>();
	m_pGameClearWidget->Init();

	m_IsStartDisp	= false;
	m_CanOperate	= false;
	m_Alpha			= INIT_FLOAT;
	m_Choice		= INIT_INT;

	m_pSpriteList[BackGround] = CSpriteResource::GetSprite( "result_back_1110", &m_SpriteStateList[BackGround]	);
	std::string FrameFileName = "";
	if ( CScore::GetGameEnd() ) {
		FrameFileName = CRandom::Probability( 1, 4 ) ? "result_screen2_2" : "result_screen2_1";
	}
	else {
		FrameFileName = "result_screen1_1";
	}
	m_pSpriteList[Frame] =		CSpriteResource::GetSprite( FrameFileName,		&m_SpriteStateList[Frame]		);

	m_pSpriteList[Yubi] =		CSpriteResource::GetSprite( "Yubi",				&m_SpriteStateList[Yubi]		);
	m_pSpriteList[Choices_1] =	CSpriteResource::GetSprite( "result_txt_1109",	&m_SpriteStateList[Choices_1]	);
	m_pSpriteList[Choices_2] =	CSpriteResource::GetSprite( "result_txt_1109",	&m_SpriteStateList[Choices_2]	);

	m_SpriteStateList[BackGround].Transform.Position =	{ 171.0f, 136.0f, 0.0f };
	m_SpriteStateList[Yubi].Transform.Position =		{ 400.0f, 590.0f, 0.0f };
	const SSize ChoicesSize = m_pSpriteList[Choices_1]->GetSpriteState().Disp;
	m_SpriteStateList[Choices_1].Transform.Position =	{ 430.0f, 590.0f - (ChoicesSize.h / 2.0f), 0.0f };
	m_SpriteStateList[Choices_1].AnimState.IsSetAnimNumber = true;
	m_SpriteStateList[Choices_1].AnimState.AnimNumber =	0;
	m_SpriteStateList[Choices_2].Transform.Position =	{ 430.0f, 680.0f - (ChoicesSize.h / 2.0f), 0.0f };
	m_SpriteStateList[Choices_2].AnimState.IsSetAnimNumber = true;
	m_SpriteStateList[Choices_2].AnimState.AnimNumber = 2;

	m_SpriteStateList[Yubi].Transform.Scale = { 1.0f, 1.0f, 1.0f };

	m_SpriteStateList[Yubi].Color.w			= m_Alpha;
	m_SpriteStateList[Choices_1].Color.w	= m_Alpha;
	m_SpriteStateList[Choices_2].Color.w	= m_Alpha;

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// BGM名の設定.
	m_BGMName = "GameClear";

	// ゲーム終了時の日給を取得.
	const SFoodState FinalWage = CScore::GetScore();
	m_ValueList[ORIGIN_WAGE].Value =
		FinalWage.EndMoney - FinalWage.HotBonus -
		FinalWage.GoldenBonus + FinalWage.DiscountMoney;
	m_ValueList[HEAT_BONUS].Value =
		FinalWage.HotBonus;
	m_ValueList[GOLDEN_TIME_BONUS].Value =
		FinalWage.GoldenBonus;
	m_ValueList[TILT_DISCOUNT].Value =
		-FinalWage.DiscountMoney;
	m_ValueList[TREATMENT_PAY].Value =
		TREATMENT_PAY_VALUE;

	m_WageDigits = static_cast<int>(std::to_string(
		m_ValueList[ORIGIN_WAGE].Value ).length());

	m_WageDigits += (m_WageDigits - 1) / 3;
	// 3桁ごとにカンマを入れる.
	m_PosX.resize( m_WageDigits );
	for ( auto& p : m_PosX ) p = -30.0f;

	m_WageFinalXPos.resize( m_WageDigits );
	for ( int i = 0; i < m_WageDigits; ++i ) {
		m_WageFinalXPos[i] =
			850.0f - (((m_WageDigits - 1) - i) * STRING_SIZE_X);
	}

	for (auto& r : m_ValueList) {
		r.Alpha = Color::ALPHA_MIN;
	}
	m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MAX;

	// 効果音の設定.
	m_IsPlayDigitSE.resize( DIGIT_SE_MAX );
	m_DigitSE = DIGIT_1_SE;
	m_ThroughStringCnt = 0;

	// フォントを取得する.
	m_pFont = CFontResource::GetFont("NasuM");
	for (auto& f : m_FontStateList) {
		// フォントの描画情報を取得する.
		f = m_pFont->GetRenderState();
		// 文字を右揃えにする.
		f.TextAlign = ETextAlign::Right;
		// 文字の色を黒色にする.
		f.Color = Color4::Black;
	}

	m_FontStateList[WageNumber].Transform.Position.x	= 0.0f;
	m_FontStateList[WageNumber].Transform.Position.y	= 150.0f;
	m_FontStateList[WageNumber].Transform.Scale			= { 2.0f, SCALE, 1.0f };

	m_FontStateList[WageYen].Text						= "￥";
	m_FontStateList[WageYen].Transform.Position.x		= -30.0f;
	m_FontStateList[WageYen].Transform.Position.y		= 150.0f;
	m_FontStateList[WageYen].Transform.Scale			= { 2.0f, SCALE, 1.0f };

	m_FontStateList[DetailString].TextAlign				= ETextAlign::Left;
	m_FontStateList[DetailString].Transform.Position.x	= 200.0f;

	m_FontStateList[DetailNumber].Transform.Position.x	= 850.0f;
	m_FontStateList[DetailNumber].Transform.Scale		= { 1.0f, 2.0f, 1.0f };

	// ランキングの保存.
	if ( CScore::GetGameEnd() ) {
		CRanking::AddTreatmentPay( TREATMENT_PAY_VALUE );
	}
	CScore::SaveLog();
	CRanking::Save();

	// SEの再生.
	CSoundManager::PlaySE( DRUMROLL_SE );

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameClear::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// BGMの再生.
	CSoundManager::PlayBGM( m_BGMName );

	// UIの更新.
	m_pGameClearWidget->Update( DeltaTime );

	if ( m_IsStartDisp ) {
		// 不透明にしていく.
		AddAlpha();

		if ( m_CanOperate ) {
			// カーソルの移動処理.
			m_SpriteStateList[Yubi].Transform.Position.y =
				590.0f + m_Choice * 90.0f;

			// 選択.
			if ( CInput::IsKeyDown( "Up" ) ) {
				// カーソルを上げる.
				--m_Choice;
				if ( m_Choice < 0 )m_Choice = 1;

				// SEの再生.
				CSoundManager::PlaySE( CURSOR_MOVE_SE );
			}
			else if ( CInput::IsKeyDown( "Down" ) ) {
				// カーソルを下げる.
				++m_Choice;
				if ( m_Choice > 1 )m_Choice = 0;

				// SEの再生.
				CSoundManager::PlaySE( CURSOR_MOVE_SE );
			}
			// 決定.
			else if ( CInput::IsKeyDown( "Decision" ) ) {
				CSoundManager::PlaySE( "Decision" );
				// 選択肢を取得する.
				switch ( m_Choice ) {
				case 0:
					// ゲームメインに遷移する.
					CSceneManager::SceneChange( ESceneList::GameMain, EFadeType::MaskFade );
					// ゲームで使用するランキングを初期化する.
					CRanking::Init();
					break;
				case 1:
					// タイトル画面に戻る.
					CSceneManager::SceneChange( ESceneList::Title, EFadeType::MaskFade );
					break;
				default:
					break;
				}
			}
		}

		return;
	}

	const int State = static_cast<int>(m_ShowState);
	switch ( m_ShowState ) {
	case EShowState::OriginWage:		// 日給表示中の処理.
		ShowOriginWage();
		break;
	case EShowState::HeatBonus:			// 温度ボーナス表示中の処理.
	case EShowState::GoldenTimeBonus:	// ゴールデンタイムボーナス表示中の処理.
	case EShowState::TiltDiscount:		// 傾き割引表示中の処理.
	case EShowState::TreatmentPay:		// 治療費表示中の処理.
		ShowDetails( State );
		break;
	case EShowState::FinalWage:			// 日給表示後の処理.
		UpdateAfterShowTotalWage();
		break;	
	default:
		break;
	}

	m_ValueList[ORIGIN_WAGE].Alpha += ALPHA_SPEED * m_DeltaTime;
	if ( m_ValueList[ORIGIN_WAGE].Alpha < Color::ALPHA_MAX ) return;
	m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MAX;
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CGameClear::ModelRender()
{
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CGameClear::SpriteUIRender()
{
	// UIの描画.
	m_pGameClearWidget->Render();

	m_pSpriteList[BackGround]->RenderUI( &m_SpriteStateList[BackGround] );

	// 日給の表示.
	RenderOriginWage();

	// 「￥」の表示.
	m_pFont->RenderUI( &m_FontStateList[WageYen] );

	// 詳細情報の表示.
	RenderDetails();

	m_SpriteStateList[Choices_1].AnimState.AnimNumber = m_Choice;
	m_SpriteStateList[Choices_2].AnimState.AnimNumber = 3 - m_Choice;

	m_pSpriteList[Frame]->RenderUI( &m_SpriteStateList[Frame] );
	for ( int i = Yubi; i < Max; ++i ) {
		m_SpriteStateList[i].Color.w = m_Alpha;
		m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
	}

	// ランキングの描画.
	CRanking::RankingRender(-80.0f);
}

//---------------------------.
// スプライト(3D)/Effectの描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CGameClear::Sprite3DRender_A()
{
}
void CGameClear::Sprite3DRender_B()
{
}

//---------------------------.
// 日給の描画処理.
//---------------------------.
void CGameClear::RenderOriginWage()
{
	// カンマ挿入処理.
	std::string ValueStr = std::to_string( m_ValueList[ORIGIN_WAGE].Value );
	const int Size = static_cast<int>(ValueStr.length());

	// 「-」が見つかればそれは桁数に含めない.
	int DeleteDigit = 0;
	if ( ValueStr.find( "-" ) != std::string::npos ) {
		++DeleteDigit;
	}
	int ConmaNumber = 0;
	// 3桁ごとにカンマを入れる.
	for ( int i = Size - DeleteDigit - 3; i > 0; i -= 3 ) {
		const int InsertPos = i + DeleteDigit;
		ValueStr.insert( InsertPos, "," );
		++ConmaNumber;
	}

	// 桁数分繰り返す.
	for ( int Digit = Size + ConmaNumber - 1; Digit >= 0; --Digit ) {
		// 文字列のDigit番目の要素を格納.
		m_FontStateList[WageNumber].Text = ValueStr.substr( Digit, 1 );

		// 表示座標を桁に合わせてずらす.
		m_FontStateList[WageNumber].Transform.Position.x = m_PosX[Digit];
		// 透過処理.
		m_FontStateList[WageNumber].Color.w = m_ValueList[ORIGIN_WAGE].Alpha;

		// 数字の描画.
		m_pFont->RenderUI( &m_FontStateList[WageNumber] );
	}
}

//---------------------------.
// 詳細情報の描画処理.
//---------------------------.
void CGameClear::RenderDetails()
{
	// 入院費が発生していないとき、文字同士の間隔をあける.
	const float AdjustY =
		CScore::GetGameEnd() == false ? STRING_SIZE_Y / 2.0f : 0.0f;

	for ( int i = 0; i < DETAIL_MAX; ++i ) {
		// 非表示なら処理を中断する.
		if ( m_ValueList[i].IsDisp == false ) return;

		float ScaleX = 2.0f;
		const float ScaleY = 2.0f;
		// 表示位置の変更.
		m_FontStateList[DetailString].Transform.Position.y =
			m_FontStateList[DetailNumber].Transform.Position.y =
			300.0f + (50.0f + AdjustY * ScaleY) * i;

		// 文字の大きさの変更.
		if ( i == GOLDEN_TIME_BONUS )ScaleX /= 2.0f;
		m_FontStateList[DetailString].Transform.Scale = { ScaleX, ScaleY, 1.0f };
		// 表示する文字の設定.
		std::string str = "";
		switch ( i ) {
		case HEAT_BONUS:		str = "温度ボーナス";				break;
		case GOLDEN_TIME_BONUS:	str = "ゴールデンタイムボーナス"; break;
		case TILT_DISCOUNT:		str = "傾き割引";				break;
		case TREATMENT_PAY:		str = "入院費";					break;
		default:												break;
		}
		m_FontStateList[DetailString].Text = str;
		m_FontStateList[DetailNumber].Color.w = m_ValueList[i].Alpha;
		// 文字の表示.
		m_pFont->RenderUI( &m_FontStateList[DetailString] );

		// カンマ挿入処理.
		std::string ValueStr = std::to_string( m_ValueList[i].Value );
		int Size = static_cast<int>(ValueStr.length());
		// 「-」が見つかればそれは桁数に含めない.
		int DeleteDigit = 0;
		if ( ValueStr.find( "-" ) != std::string::npos ) {
			++DeleteDigit;
		}
		// 3桁ごとにカンマを入れる.
		for ( int InsertNo = Size - DeleteDigit - 3; InsertNo > 0; InsertNo -= 3 ) {
			const int InsertPos = InsertNo + DeleteDigit;
			ValueStr.insert( InsertPos, "," );
		}

		// 表示する文字の設定.
		m_FontStateList[DetailNumber].Text = ValueStr;
		// 数字の表示.
		m_pFont->RenderUI( &m_FontStateList[DetailNumber] );
	}
}

//---------------------------.
// 日給表示中の処理.
//---------------------------.
void CGameClear::ShowOriginWage()
{
	// 指定した桁の数字を右に移動させる.
	const int DigitsNo = m_WageDigits - 1;
	m_PosX[DigitsNo] += WAGE_SPEED * m_DeltaTime;

	// 数字が指定位置に到着していない、かつ決定ボタンが押されていないなら以後の処理は行わない.
	if ( m_PosX[DigitsNo] < m_WageFinalXPos[DigitsNo] && CInput::IsKeyDown( "Decision" ) == false ) return;
	m_PosX[DigitsNo] = m_WageFinalXPos[DigitsNo];
	m_ValueList[ORIGIN_WAGE].IntervalCount += INTERVAL_SPEED * m_DeltaTime;

	// 表示している文字がコンマではなく、まだSEを再生していないなら.
	if ( m_ThroughStringCnt != 3 && m_ThroughStringCnt != 7 && m_IsPlayDigitSE[DigitsNo] == false ) {
		// SEを再生.
		CSoundManager::PlaySE( m_DigitSE );
		// 再生するSEを変更する.
		m_DigitSE += "0";
		// 再生済み.
		m_IsPlayDigitSE[DigitsNo] = true;
	}

	// 表示後の待ち時間が終了していない、かつ決定ボタンが押されていないなら以後の処理は行わない.
	if ( m_ValueList[ORIGIN_WAGE].IntervalCount < INTERVAL_MAX && CInput::IsKeyDown( "Decision" ) == false ) return;
	m_ValueList[ORIGIN_WAGE].IntervalCount = 0.0f;
	// 移動させる桁を1桁ずらす.
	m_WageDigits--;

	m_ThroughStringCnt++;

	// まだ表示させる桁が残っているなら以後の処理は行わない.
	if ( m_WageDigits > 0 ) return;
	m_ValueList[HEAT_BONUS].IsDisp = true;
	m_ShowState = EShowState::HeatBonus;
}

//---------------------------.
// 日給表示後の処理.
//---------------------------.
void CGameClear::UpdateAfterShowTotalWage()
{
	const float EndPosX = m_WageFinalXPos[0] - STRING_SIZE_X * 1.5f;
	float* YenPosX = &m_FontStateList[WageYen].Transform.Position.x;
	// 「￥」を右に移動させる.
	*YenPosX += WAGE_SPEED * m_DeltaTime;

	// 「￥」が指定位置に到着しておらず,決定ボタンを押していないなら処理を中断.
	if ( *YenPosX < EndPosX && CInput::IsKeyDown( "Decision" ) == false )return;

	// 「￥」が指定位置に到着するか,決定ボタンを押したら.
	// 移動完了後の座標にする.
	*YenPosX = EndPosX;

	// SEの再生.
	CSoundManager::PlaySE( CASH_REGISTER_SE );

	// 選択肢を表示する.
	m_IsStartDisp = true;
}

//---------------------------.
// 各種ボーナス表示中、決定ボタンを押された時の処理.
//---------------------------.
void CGameClear::PushButton( int Type )
{
	// 決定ボタンが押されていないなら処理を行わない.
	if ( CInput::IsKeyDown( "Decision" ) == false )return;

	// 決定ボタンを押したとき.
	// 詳細情報の文字が完全に表示されていないなら.
	if ( m_ValueList[Type].Alpha != Color::ALPHA_MAX ) {
		m_ValueList[Type].Alpha = Color::ALPHA_MAX;

		// 日給を透明にする.
		m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MIN;
		// 日給にボーナス,割引を加える.
		m_ValueList[ORIGIN_WAGE].Value += m_ValueList[Type].Value;
		// 日給の桁を再設定する.
		ResetWageDigit();

		// まだSEを再生していないなら.
		if ( m_IsPlayGetBonusSE == false ) {
			// 表示された詳細情報が温度ボーナス,ゴールデンタイムボーナスなら.
			if ( Type == HEAT_BONUS || Type == GOLDEN_TIME_BONUS ) {
				// SEを再生する.
				CSoundManager::PlaySE( GET_BONUS_SE );
			}
			// 表示された詳細情報が傾き割引,治療費なら.
			else {
				// SEを再生する.
				CSoundManager::PlaySE( GET_PENALTY_SE );
			}
			// 再生済.
			m_IsPlayGetBonusSE = true;
		}
	}
	// 詳細情報の文字表示後、日給が完全に表示されていないなら.
	else if ( m_ValueList[ORIGIN_WAGE].Alpha != Color::ALPHA_MAX ) {
		m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MAX;
	}
	// 次の詳細情報への待機時間が終了していないなら.
	else if ( m_ValueList[Type].IntervalCount != INTERVAL_MAX ) {
		m_ValueList[Type].IntervalCount = INIT_FLOAT;
		const int Next = Type + 1;

		// 表示している詳細に応じて処理を切り替える.
		switch ( Type ) {
		case HEAT_BONUS:		// 温度ボーナス.
		case GOLDEN_TIME_BONUS:	// ゴールデンタイムボーナス.
			m_ValueList[Next].IsDisp = true;
			m_ShowState = static_cast<EShowState>(Next);
			break;
		case TILT_DISCOUNT:		// 傾き割引.
			// もし、治療費が存在するなら.
			if ( CScore::GetGameEnd() ) {
				m_ValueList[TREATMENT_PAY].IsDisp = true;
				m_ShowState = EShowState::TreatmentPay;
				break;
			}

			m_ShowState = EShowState::FinalWage;
			break;
		case TREATMENT_PAY:		// 治療費.
			m_ShowState = EShowState::FinalWage;
			break;
		default:
			break;
		}

		m_IsPlayGetBonusSE = false;
	}
}

//---------------------------.
// 詳細情報の値の透明度を増加させる処理.
//---------------------------.
void CGameClear::AddAlpha( int Type )
{
	// すでに詳細情報の値が不透明なら処理を行わない.
	if ( m_ValueList[Type].Alpha >= Color::ALPHA_MAX )return;
	//  詳細情報の値の透明度を増加させる.
	m_ValueList[Type].Alpha += ALPHA_SPEED * m_DeltaTime;

	//  詳細情報の値が不透明でないなら以後の処理は行わない.
	if ( m_ValueList[Type].Alpha < Color::ALPHA_MAX ) return;
	m_ValueList[Type].Alpha = Color::ALPHA_MAX;

	// 日給を透明にする.
	m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MIN;
	// 日給に詳細情報の値を加算する.
	m_ValueList[ORIGIN_WAGE].Value += m_ValueList[Type].Value;
	// 日給の桁を再設定する.
	ResetWageDigit();

	// まだSEを再生していないなら.
	if ( m_IsPlayGetBonusSE == false ) {
		// 表示された詳細情報が温度ボーナス,ゴールデンタイムボーナスなら.
		if ( Type == HEAT_BONUS || Type == GOLDEN_TIME_BONUS ) {
			// SEを再生する.
			CSoundManager::PlaySE( GET_BONUS_SE );
		}
		// 表示された詳細情報が傾き割引,治療費なら.
		else {
			// SEを再生する.
			CSoundManager::PlaySE( GET_PENALTY_SE );
		}
		// 再生済.
		m_IsPlayGetBonusSE = true;
	}
}

//---------------------------.
// ボーナス,割引増減後の処理.
//---------------------------.
void CGameClear::AfterAddDetails( int Type )
{
	// 詳細情報の値の数字が不透明でない、もしくは日給が不透明でないなら以後の処理は行わない.
	if ( m_ValueList[Type].Alpha < Color::ALPHA_MAX || m_ValueList[ORIGIN_WAGE].Alpha != Color::ALPHA_MAX ) return;
	m_ValueList[Type].IntervalCount += INTERVAL_SPEED * m_DeltaTime;

	// 待ち時間が終わっていないなら以後の処理は行わない.
	if ( m_ValueList[Type].IntervalCount < INTERVAL_MAX ) return;
	m_ValueList[Type].IntervalCount = INIT_FLOAT;
	const int Next = Type + 1;

	// 表示している詳細に応じて処理を切り替える.
	switch ( Type ) {
	case HEAT_BONUS:		// 温度ボーナス.
	case GOLDEN_TIME_BONUS:	// ゴールデンタイムボーナス.
		m_ValueList[Next].IsDisp = true;
		m_ShowState = static_cast<EShowState>(Next);
		break;
	case TILT_DISCOUNT:		// 傾き割引.
		// もし、治療費が存在するなら.
		if ( CScore::GetGameEnd() ) {
			m_ValueList[TREATMENT_PAY].IsDisp = true;
			m_ShowState = EShowState::TreatmentPay;
			break;
		}

		m_ShowState = EShowState::FinalWage;
		break;
	case TREATMENT_PAY:		// 治療費.
		m_ShowState = EShowState::FinalWage;
		break;
	default:
		break;
	}

	m_IsPlayGetBonusSE = false;
}

//---------------------------.
// 詳細情報の処理.
//---------------------------.
void CGameClear::ShowDetails( int Type )
{
	PushButton( Type );
	AddAlpha( Type );
	AfterAddDetails( Type );
}

//---------------------------.
// 日給の桁を再設定する.
//---------------------------.
void CGameClear::ResetWageDigit()
{
	const std::string ValueStr = std::to_string( m_ValueList[ORIGIN_WAGE].Value );
	m_WageDigits = static_cast<int>(ValueStr.length());
	// 「-」が見つかればそれは桁数に含めない.
	int DeleteDigit = 0;
	if ( ValueStr.find( "-" ) != std::string::npos ) {
		++DeleteDigit;
	}

	m_WageDigits += (m_WageDigits - DeleteDigit - 1) / 3;
	m_PosX.resize( m_WageDigits );
	m_WageFinalXPos.resize( m_WageDigits );
	for ( int i = 0; i < m_WageDigits; ++i ) {
		m_WageFinalXPos[i] = 850.0f - (((m_WageDigits - 1) - i) * STRING_SIZE_X);
		m_PosX[i] = m_WageFinalXPos[i];
	}
}

//---------------------------.
// UIの不透明化処理.
//---------------------------.
void CGameClear::AddAlpha()
{
	if ( m_CanOperate )return;
	if ( m_IsStartDisp == false )return;

	m_Alpha += m_DeltaTime;
	if ( m_Alpha >= Color::ALPHA_MAX ) {
		m_Alpha = Color::ALPHA_MAX;

		m_CanOperate = true;
	}
}
