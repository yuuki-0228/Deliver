#pragma once
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\Common\Font\Font.h"

namespace {
	// 順位列挙体.
	enum class enGrade : unsigned char {
		First,				// 1位.
		Second,				// 2位.
		Third,				// 3位.
		Fourth,				// 4位.
		Fifth,				// 5位.

		Now,				// 現在のスコア.

		Max,				// 要素数.

		Rank_Max = Fifth + 1// 順位の数.
	} typedef EGrade;
}

/************************************************
*	ランキングクラス.
*		井谷凌.
**/
class CRanking final
{
public:
	using RankInt = std::vector<std::pair<int, EGrade>>;

public:
	CRanking();
	~CRanking();

	// ファイルの読み込み.
	static void Load();

	// ファイルの書き込み.
	static void Save();

	// リセット.
	static void Reset();

	// 初期化処理.
	static void Init();

	// ランキングの更新.
	static void Update();

	// ランキングの描画.
	static void RankingRender( const float AdjustX = 0.0f, const float AdjustY = 0.0f );

	// 治療費の加算.
	static void AddTreatmentPay( const int TreatmentPay );

private:
	// インスタンスの取得.
	static CRanking* GetInstance();

private:
	RankInt				m_Ranking;				// ランキング.
	RankInt				m_ProvisionalRanking;	// 仮ランキング.
	
	CFont*				m_pFont;
	SFontRenderState	m_FontState;

	int					m_No;					// 現在の順位.
};