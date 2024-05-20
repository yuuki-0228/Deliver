#include "Score.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "Ranking\Ranking.h"

namespace {
	constexpr float FSCALE_WAGE				= 0.8f;	// 日給の文字の大きさ.
	constexpr float FSCALE_WAGE_DETAILS		= 0.6f;	// 日給内訳の文字の大きさ.
	constexpr char	SCORE_LIST_FILE_PATH[]	= "Data\\Parameter\\Main\\PlayData.csv";	// スコアリストデータのファイルパス.

	enum enFontNo :unsigned char{
		Wage,			// 日給.
		WageDetailStr,	// 日給詳細の文字.
		WageDetailNum,	// 日給詳細の数字.

		Font_Max
	}typedef EFontNo;
}

CScore::CScore ()
	: m_DeltaTime				( INIT_FLOAT )
	, m_TreatmentPay			( INIT_INT )
	, m_FinalWage				()
	, m_Alpha					( Color::ALPHA_MIN )
	, m_EndFlag					( false )
	, m_HomeFlag				( false )
	, m_FoodState				()
	, m_pWageDetailsFrame		( nullptr )
	, m_WageDetailsFrameState	()
	, m_IsDisp					( false )
	, m_ViewTime				( INIT_FLOAT )
	, m_AddScroll				( INIT_FLOAT )
	, m_IsReflect				( false )
	, m_pFont					( nullptr )
	, m_FontStateList			( Font_Max )

	, m_ViewFinalWage			( "0" )
	, m_PlayDataList			()
{
	// プレイデータの取得.
	std::vector<std::string> Data = FileManager::TextLoad( SCORE_LIST_FILE_PATH );
	const int Size = static_cast<int>( Data.size() ) - 28;
	m_PlayDataList.resize( static_cast<int>( Size / 8 ) );
	for ( int i = 0; i < Size / 8; ++i ) {
		m_PlayDataList[i].EndScore		= std::stoi( Data[i * 8 + 28] );
		m_PlayDataList[i].GetScore		= std::stoi( Data[i * 8 + 29] );
		m_PlayDataList[i].HotBonus		= std::stoi( Data[i * 8 + 30] );
		m_PlayDataList[i].GoldenBonus	= std::stoi( Data[i * 8 + 31] );
		m_PlayDataList[i].DiscountMoney = std::stoi( Data[i * 8 + 32] );
		if (		Data[i * 8 + 33] != "" ) m_PlayDataList[i].EndNo = 0;
		else if (	Data[i * 8 + 34] != "" ) m_PlayDataList[i].EndNo = 1;
		else if (	Data[i * 8 + 35] != "" ) m_PlayDataList[i].EndNo = 2;
	}
}

CScore::~CScore ()
{
}

//---------------------------.
// 日給を設定.
//---------------------------.
void CScore::SetScore( const SFoodState& Wage )
{
	CScore* pI = GetInstance();

	pI->m_FoodState = Wage;
	// 日給の内訳を表示する.
	pI->m_IsDisp	= true;
	// 日給の内訳を表示する時間を設定.
	pI->m_ViewTime = 5.0f * 60.0f;

	pI->m_AddScroll = -130.0f;
	pI->m_IsReflect = false;
	// 透明度を設定.
	pI->m_Alpha		= Color::ALPHA_MIN;

	// 日給を加算.
	pI->m_FinalWage.EndMoney		+= Wage.EndMoney;
	pI->m_FinalWage.HotBonus		+= Wage.HotBonus;
	pI->m_FinalWage.GoldenBonus		+= Wage.GoldenBonus;
	pI->m_FinalWage.DiscountMoney	+= Wage.DiscountMoney;

	// 3桁ごとにカンマを入れる.
	pI->m_ViewFinalWage = std::to_string( pI->m_FinalWage.EndMoney );
	for ( int i = static_cast<int>(pI->m_ViewFinalWage.length()) - 3; i > 0; i -= 3 ) {
		pI->m_ViewFinalWage.insert( i, "," );
	}

	// ランキングの更新.
	CRanking::Update();
}

//---------------------------.
// 初期化.
//---------------------------.
void CScore::Init()
{
	CScore* pI = GetInstance();

	// 初期化.
	pI->m_TreatmentPay			= INIT_INT;
	pI->m_FinalWage				= SFoodState();
	pI->m_Alpha					= Color::ALPHA_MIN;
	pI->m_EndFlag				= false;
	pI->m_HomeFlag				= false;
	pI->m_FoodState				= SFoodState();
	pI->m_IsDisp				= false;
	pI->m_ViewTime				= INIT_FLOAT;
	pI->m_AddScroll				= INIT_FLOAT;
	pI->m_IsReflect				= false;
	pI->m_ViewFinalWage			= "0";

	// フォントの取得.
	pI->m_pFont = CFontResource::GetFont( "NasuM" );
	for ( auto& f : pI->m_FontStateList ) {
		f = pI->m_pFont->GetRenderState();
		// 文字を右揃えにする.
		f.TextAlign = ETextAlign::Right;
	}

	pI->m_FontStateList[WageDetailStr].Color = Color4::Black;

	pI->m_FontStateList[WageDetailStr].RenderArea = { WND_W - 300.0f, 32.0f, 300.0f, 256.0f };
	pI->m_FontStateList[WageDetailNum].RenderArea = { WND_W - 300.0f, 32.0f, 300.0f, 256.0f };

	pI->m_FontStateList[WageDetailNum].IsOutLine = true;
	pI->m_FontStateList[WageDetailNum].OutLineColor = Color4::Black;
	pI->m_FontStateList[WageDetailNum].OutLineSize = 2.0f;

	// 構造体の初期化.
	pI->m_FoodState = SFoodState();

	pI->m_pWageDetailsFrame = CSpriteResource::GetSprite( "WageDetailsFrame", &pI->m_WageDetailsFrameState );
	pI->m_WageDetailsFrameState.Transform.Position.x = WND_W - 310.0f;
	pI->m_WageDetailsFrameState.Transform.Position.y = INIT_FLOAT;

	pI->m_WageDetailsFrameState.RenderArea = { WND_W - 310.0f, 32.0f, 310.0f, 256.0f };
	
	// 日給の内訳を表示する.
	pI->m_IsDisp	= false;
	// 日給の内訳を表示する時間を設定.
	pI->m_ViewTime	= 0.0f;
	pI->m_AddScroll = -130.0f;
	pI->m_IsReflect = false;

	// 透明度を設定.
	pI->m_Alpha		= Color::ALPHA_MAX;

	// 強制終了フラグを初期化.
	pI->m_EndFlag	= false;
	pI->m_HomeFlag	= false;

	// 日給を初期化.
	pI->m_FinalWage.EndMoney = INIT_INT;
}

//---------------------------.
// 更新.
//---------------------------.
void CScore::Update ( const float& DeltaTime )
{
	CScore* pI = GetInstance();

	pI->m_DeltaTime = DeltaTime;

	pI->WageDetailsScroll();
	
	// 日給の文字が完全不透明ならば以下の処理は行わない.
	if ( pI->m_Alpha == Color::ALPHA_MAX )return;
	pI->m_Alpha += 5.0f * pI->m_DeltaTime;
	if ( pI->m_Alpha >= Color::ALPHA_MAX )pI->m_Alpha = Color::ALPHA_MAX;
}

//---------------------------.
// 描画.
//---------------------------.
void CScore::Render()
{
	CScore* pI = GetInstance();

	// 日給の内訳が表示されている場合、日給の内訳の表示.
	if ( pI->m_IsDisp ) pI->RenderWageDetails();
	
	// 日給の表示.
	pI->RenderWage();
}

//---------------------------.
// ログの出力.
//---------------------------.
void CScore::SaveLog()
{
	CScore* pI = GetInstance();

	// プレイデータの追加.
	SPlayData NowData;
	if ( pI->m_EndFlag ) NowData.EndScore	= pI->m_FinalWage.EndMoney - 200000;
	else				 NowData.EndScore	= pI->m_FinalWage.EndMoney;
	NowData.GetScore		= pI->m_FinalWage.EndMoney;
	NowData.HotBonus		= pI->m_FinalWage.HotBonus;
	NowData.GoldenBonus		= pI->m_FinalWage.GoldenBonus;
	NowData.DiscountMoney	= pI->m_FinalWage.DiscountMoney;
	if (		pI->m_EndFlag  ) NowData.EndNo = 2;
	else if (	pI->m_HomeFlag ) NowData.EndNo = 1;
	else						 NowData.EndNo = 0;
	pI->m_PlayDataList.emplace_back( NowData );

	// プレイデータの保存.
	std::string ScoreText = 
		"プレイ回数,=COUNT(A3:A100000),クリア回数,=COUNT(F3:F100000),帰宅回数,=COUNT(G3:G100000),事故回数,=COUNT(H3:H100000),"
		"最大スコア,=MAX(A3:A100000),平均スコア,=AVEDEV(A3:A100000),平均取得スコア,=AVEDEV(B3:B100000),"
		"平均温度ボーナス,=AVEDEV(C3:C100000),平均ゴールデンタイムボーナス,=AVEDEV(D3:D100000),平均傾き割引,=AVEDEV(E3:E100000)\n"
		"最終スコア,獲得スコア,温度ボーナス,ゴールデンタイムボーナス,傾き割引,クリア,帰宅,事故,";
	for ( auto& d : pI->m_PlayDataList ) {
		ScoreText += "\n" +
			std::to_string( d.EndScore ) + "," + std::to_string( d.GetScore ) + "," + std::to_string( d.HotBonus ) + "," +
			std::to_string( d.GoldenBonus ) + "," + std::to_string( d.DiscountMoney ) + ",";
		if (		d.EndNo == 0 ) ScoreText += "1,,,";
		else if (	d.EndNo == 1 ) ScoreText += ",1,,";
		else if (	d.EndNo == 2 ) ScoreText += ",,1,";
	}
	FileManager::TextSave( SCORE_LIST_FILE_PATH, ScoreText );
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CScore* CScore::GetInstance ()
{
	static std::unique_ptr<CScore> pInstance = std::make_unique<CScore>();
	return pInstance.get();
}

//---------------------------.
// 日給の描画処理.
//---------------------------.
void CScore::RenderWage()
{
	CScore* pI = GetInstance();
	
	SFontRenderState* pState = &pI->m_FontStateList[Wage];
	pState->Color = Color4::Black;
	pState->Transform.Position.y = 5.0f;
	pState->Transform.Scale = { FSCALE_WAGE, FSCALE_WAGE, 1.0f };

	// 文字の描画.
	pState->Transform.Position.x = WND_W - 280.0f * FSCALE_WAGE;
	pI->m_pFont->RenderUI( "日給:￥", pState );

	// 日給の描画.
	pState->Color.w = pI->m_Alpha;
	pState->Transform.Position.x = WND_W - 5.0f;
	pI->m_pFont->RenderUI( pI->m_ViewFinalWage, pState );
}

//---------------------------.
// 日給の内訳の描画処理.
//---------------------------.
void CScore::RenderWageDetails()
{
	CScore* pI = GetInstance();

	pI->m_pWageDetailsFrame->RenderUI( &pI->m_WageDetailsFrameState );

	SFontRenderState* pStrState = &pI->m_FontStateList[WageDetailStr];
	SFontRenderState* pNumState = &pI->m_FontStateList[WageDetailNum];

	// 必要な情報を抽出する.
	std::vector<int> WageDetails;
	WageDetails.resize( 5 );
	WageDetails[0] = static_cast<int>( pI->m_FoodState.Money );
	WageDetails[1] = static_cast<int>( pI->m_FoodState.DiscountMoney );
	WageDetails[2] = static_cast<int>( pI->m_FoodState.HotBonus );
	WageDetails[3] = static_cast<int>( pI->m_FoodState.GoldenBonus );
	WageDetails[4] = static_cast<int>( pI->m_FoodState.EndMoney );

	// 日給の内訳の名称リスト.
	std::pair<std::string, D3DXCOLOR4> DetailsName[] = {
		{ "商品価値",				Color4::RGBA( 0.0f, 0.9f, 0.0f )	},
		{ "傾き割引",				Color4::Red							},
		{ "温度ボーナス",				Color4::RGBA( 0.0f, 0.9f, 0.0f )	},
		{ "ゴールデンタイムボーナス", Color4::RGBA( 0.0f, 0.9f, 0.0f )		},
		{ "合計"	,					Color4::White						}
	};

	// ゴールデンタイムボーナスを表示しないか.
	pStrState->Transform.Scale = pNumState->Transform.Scale = { FSCALE_WAGE_DETAILS, FSCALE_WAGE_DETAILS, 0.0f };
	int WDSize = 5;
	if ( pI->m_FoodState.GoldenBonus == 0 ) {
		WDSize			= 4;
		WageDetails[3]	= static_cast<int>( pI->m_FoodState.EndMoney );
		DetailsName[3]	= { "合計", Color4::White };
	}

	// 内訳の数だけループする.
	for ( int i = 0; i < WDSize; ++i ) {
		pStrState->Transform.Position.y = pNumState->Transform.Position.y = (pI->m_AddScroll + i * 35.0f) * FSCALE_WAGE_DETAILS;

		// 文字の描画.
		pStrState->Transform.Position.x = WND_W - 175.0f * FSCALE_WAGE_DETAILS;
		pI->m_pFont->RenderUI( DetailsName[i].first + ":", pStrState );

		// 数字の描画.
		pNumState->Color = DetailsName[i].second;
		pNumState->Transform.Position.x = WND_W - 5.0f;
		pI->m_pFont->RenderUI( "￥" + std::to_string( WageDetails[i] ), pNumState );
	}

	pI->m_WageDetailsFrameState.Transform.Position.y = (pI->m_AddScroll + 35.0f * WDSize) * FSCALE_WAGE_DETAILS;
}

//---------------------------.
// スクロール処理.
//---------------------------.
void CScore::WageDetailsScroll()
{
	CScore* pI = GetInstance();

	//if ( pI->m_IsDisp == false )return;

	if ( pI->m_IsReflect == true && pI->m_AddScroll <= -130.0f )return;

	// 日給内訳をスクロールする.
	if ( pI->m_IsReflect )pI->m_AddScroll -= 1.0f;
	else pI->m_AddScroll += 1.0f;

	// 一定値までついたなら待機させる.
	if ( pI->m_AddScroll >= 60.0f ) {
		pI->m_AddScroll = 60.0f;
		pI->m_ViewTime -= 2.0f * 60.0f * pI->m_DeltaTime;
	}

	// 待機時間が一定値を超えたなら待機終了.
	if ( pI->m_ViewTime <= 0.0f ) pI->m_IsReflect = true;
}
