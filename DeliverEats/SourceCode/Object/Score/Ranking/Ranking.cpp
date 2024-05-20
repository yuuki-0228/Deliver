#include "Ranking.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Resource\FontResource\FontResource.h"
#include "..\..\..\Object\GameObject\Widget\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\Score.h"
#include "..\..\..\Scene\SceneManager\SceneManager.h"

namespace {
	constexpr char	FILE_PATH[]			= "Data\\Parameter\\Main\\Ranking.bin";
	constexpr int	GRADE_MAX			= static_cast<int>( EGrade::Max );
	constexpr int	GRADE_RANK_MAX		= static_cast<int>( EGrade::Rank_Max );
	constexpr int	GRADE_FIRST			= static_cast<int>( EGrade::First );
	constexpr int	GRADE_SECOND		= static_cast<int>( EGrade::Second );
	constexpr int	GRADE_THIRD			= static_cast<int>( EGrade::Third );
	constexpr int	GRADE_FOURTH		= static_cast<int>( EGrade::Fourth );
	constexpr int	GRADE_FIFTH			= static_cast<int>( EGrade::Fifth );
	constexpr int	GRADE_NOW			= static_cast<int>( EGrade::Now );
}

CRanking::CRanking()
	: m_Ranking		( GRADE_MAX )
	, m_pFont		( nullptr )
	, m_FontState	()
	, m_No			( GRADE_NOW )
{
	// フォントの取得.
	m_pFont		= CFontResource::GetFont( "NasuM", &m_FontState );
	m_FontState.Transform.Scale = { 0.6f, 0.6f, 0.6f };
	m_FontState.IsBold = true;
	m_FontState.OutLineSize = 1.0f;
}

CRanking::~CRanking()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CRanking* CRanking::GetInstance()
{
	static std::unique_ptr<CRanking> pInstance = std::make_unique<CRanking>();
	return pInstance.get();
}

//---------------------------.
// ファイルの読み込み.
//---------------------------.
void CRanking::Load()
{
	CRanking* pI = GetInstance();

	// スコアを読み込む.
	int Data[GRADE_RANK_MAX];
	FileManager::BinaryLoad( FILE_PATH, Data );

	// ランキングの保存.
	pI->m_Ranking.resize( GRADE_MAX );
	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		pI->m_Ranking[i] = std::make_pair( Data[i], static_cast<EGrade>( i ) );
	}
	pI->m_Ranking[GRADE_NOW] = std::make_pair( 0, EGrade::Now );

	// 仮ランキングをランキングと同じにしておく.
	pI->m_ProvisionalRanking	= pI->m_Ranking;
}

//---------------------------.
// ファイルの書き込み.
//---------------------------.
void CRanking::Save()
{
	CRanking* pI = GetInstance();

	// 今回の結果を確定させる.
	pI->m_Ranking = pI->m_ProvisionalRanking;

	// スコアを書き込む.
	int Out[GRADE_RANK_MAX];
	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		Out[i] = pI->m_Ranking[i].first;
	}
	FileManager::BinarySave( FILE_PATH, Out );
}

//---------------------------.
// リセット.
//---------------------------.
void CRanking::Reset()
{
	CRanking* pI = GetInstance();

	// スコアを書き込む.
	int Out[GRADE_RANK_MAX];
	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		pI->m_Ranking[i] = std::make_pair( 0, static_cast<EGrade>( i ) );
		Out[i]			 = 0;
	}
	pI->m_Ranking[GRADE_NOW] = std::make_pair( 0, EGrade::Now );
	FileManager::BinarySave( FILE_PATH, Out );

	// 仮ランキングをランキングと同じにしておく.
	pI->m_ProvisionalRanking = pI->m_Ranking;
}

//---------------------------.
// 初期化処理.
//---------------------------.
void CRanking::Init()
{
	CRanking* pI = GetInstance();

	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		// 順位を設定する.
		pI->m_Ranking[i].second = static_cast<EGrade>(i);
	}
	// 現在のスコアは初期化する.
	pI->m_Ranking[GRADE_NOW] = std::make_pair( 0, EGrade::Now );

	// 仮ランキングをランキングと同じにしておく.
	pI->m_ProvisionalRanking = pI->m_Ranking;
}

//---------------------------.
// ランキングの更新.
//---------------------------.
void CRanking::Update()
{
	CRanking* pI = GetInstance();

	// 仮のランキングに保存しておく.
	pI->m_ProvisionalRanking	= pI->m_Ranking;
	const int NowScore			= CScore::GetScore().EndMoney;
	pI->m_ProvisionalRanking[GRADE_NOW].first	= NowScore;
	pI->m_ProvisionalRanking[GRADE_NOW].second	= static_cast<EGrade>( EGrade::Now );

	// ランキングが更新されなければ,以後の処理は行わない.
	if ( NowScore <= pI->m_ProvisionalRanking[GRADE_FIFTH].first ) return;
	// ソートを行う.
	std::sort( pI->m_ProvisionalRanking.rbegin (), pI->m_ProvisionalRanking.rend () );

	// 現在の順位を保存する.
	const int OldNo = pI->m_No;
	// 順位の数だけループする.
	for ( int i = 0; i < GRADE_MAX; ++i ) {
		if ( pI->m_ProvisionalRanking[i].second != EGrade::Now ) continue;
		pI->m_No = i;
		break;
	}
	if ( pI->m_No >= OldNo )return;

	// ランキングの更新がされたことを通知する.
	std::string Message = "ランキング更新!!\n" + std::to_string( pI->m_No + 1 ) + "位にランクイン!!";
	CNotificationUI::PushMessage( EMessageType::RankingUpdate, Message );
}

//---------------------------.
// ランキングの描画.
//---------------------------.
void CRanking::RankingRender( const float AdjustX, const float AdjustY )
{
	CRanking* pI = GetInstance();

	// 題名の表示.
	pI->m_FontState.TextAlign				= ETextAlign::Left;
	pI->m_FontState.Color					= Color4::Black;
	pI->m_FontState.Transform.Position.x	= WND_W - 220.0f + AdjustX;
	pI->m_FontState.Transform.Position.y	= WND_H - 370.0f + AdjustY;
	pI->m_pFont->RenderUI( "日給ランキング", &pI->m_FontState );

	int s = 0;
	for ( auto& [num, No] : pI->m_ProvisionalRanking ) {
		if ( No == EGrade::Now ) {
			s = num;
			break;
		}
	}

	bool IsViewNowScore = false;	// 同率の場合,1番上だけ赤くするのに必要.
	std::string RendStr;
	// 1位から5位まで描画.
	for ( int No = GRADE_FIRST; No < GRADE_RANK_MAX; ++No ) {
		pI->m_FontState.Transform.Position.y = WND_H - 290.0f + No * 35.0f + AdjustY;

		// 文字の描画.
		pI->m_FontState.TextAlign				= ETextAlign::Left;
		pI->m_FontState.Color = Color4::Black;
		pI->m_FontState.Transform.Position.x	= WND_W - 220.0f + AdjustX;
		RendStr = std::to_string( No + 1 ) + "位：￥";
		pI->m_pFont->RenderUI( RendStr, &pI->m_FontState );

		if ( CSceneManager::GetNowScene() != ESceneList::Title &&
			IsViewNowScore == false &&
			pI->m_ProvisionalRanking[No].first == s ) {
			pI->m_FontState.Color = Color4::Red;

			// 以後、同率は黒色で表示される.
			IsViewNowScore = true;
		}

		// スコアを3桁ごとにカンマを入れる.
		std::string Text = std::to_string( pI->m_ProvisionalRanking[No].first );
		for ( int i = static_cast<int>( Text.length() ) - 3; i > 0; i -= 3 ) {
			Text.insert( i, "," );
		}

		// 数字の描画.
		pI->m_FontState.TextAlign				= ETextAlign::Right;
		pI->m_FontState.Transform.Position.x	= WND_W - 35.0f + AdjustX;
		pI->m_pFont->RenderUI( Text, &pI->m_FontState );
	}
}

// 治療費の加算.
void CRanking::AddTreatmentPay( const int TreatmentPay )
{
	CRanking* pI = GetInstance();

	pI->m_ProvisionalRanking[pI->m_No].first += TreatmentPay;

	// ソートを行う.
	std::sort( pI->m_ProvisionalRanking.rbegin(), pI->m_ProvisionalRanking.rend() );
}
