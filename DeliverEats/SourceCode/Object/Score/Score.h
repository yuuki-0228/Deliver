#pragma once
#include "..\..\Object\GameObject\Actor\RuckSack\RuckSack.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Common\Font\Font.h"

/************************************************
*	スコア/日給管理クラス.
*		井谷凌.
**/
class CScore final
{
public:
	CScore();
	~CScore();

	// 日給を設定.
	static void SetScore ( const SFoodState& Wage );

	// 日給を取得.
	static SFoodState GetScore() { return CScore::GetInstance()->m_FinalWage; }

	// 初期化.
	static void Init();

	// 更新.
	static void Update( const float& DeltaTime );

	// 描画.
	static void Render();

	// 強制的にゲームメインが終了した際に呼び出す.
	static void SetGameEnd() { CScore::GetInstance()->m_EndFlag  = true; }
	static void SetHomeEnd() { CScore::GetInstance()->m_HomeFlag = true; }
	static bool GetGameEnd() { return CScore::GetInstance()->m_EndFlag; }

	// ログの出力.
	static void SaveLog();

private:
	// インスタンスの取得.
	static CScore* GetInstance();

	// 日給の描画処理.
	void RenderWage();

	// 日給の内訳の描画処理.
	void RenderWageDetails();

	// スクロール処理.
	void WageDetailsScroll();

private:
	float				m_DeltaTime;

	// スコア(日給).
	int					m_TreatmentPay;
	SFoodState			m_FinalWage;
	float				m_Alpha;	// 透過度.
	bool				m_EndFlag;	// ゲームメインが強制終了になるとtrueになる.
	bool				m_HomeFlag;	// 帰宅するとtrueになる.

	// スコアの詳細.
	SFoodState			m_FoodState;

	CSprite*			m_pWageDetailsFrame;
	SSpriteRenderState	m_WageDetailsFrameState;

	// スコアの内訳表示時間.
	bool					m_IsDisp;	// 表示中か?.
	float					m_ViewTime;	// 表示時間.
	float					m_AddScroll;// スクロールで加算される値.
	bool					m_IsReflect;// 日給の詳細が折り返したか.

	CFont*					m_pFont;
	SFontRenderStateList	m_FontStateList;

	std::string				m_ViewFinalWage;

private:
	// プレイデータ構造体.
	struct stPlayData {
		int			EndScore;		// 終了スコア.
		int			GetScore;		// 取得スコア.
		int			HotBonus;		// 温度ボーナス.
		int			GoldenBonus;	// ゴールデンタイムボーナス.
		int			DiscountMoney;	// 傾き割引.
		int			EndNo;			// 終了タイプ.
	} typedef SPlayData;
	std::vector<SPlayData> m_PlayDataList;		// プレイデータ.
};