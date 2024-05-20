#pragma once
#include "..\SceneBase.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Common\Font\Font.h"

class CNormalCamera;
class CGameClearWidget;

namespace {
	// リザルトシーン表示順管理列挙体.
	enum class enShowState : unsigned char {
		HeatBonus,			// 温度ボーナス.
		GoldenTimeBonus,	// ゴールデンタイムボーナス.
		TiltDiscount,		// 傾き割引.
		TreatmentPay,		// 治療費.

		OriginWage,			// ボーナス、割引を無視した金額.

		FinalWage			// 最終的な日給.
	} typedef EShowState;

	// 各値のパラメータ管理構造体.
	struct stResultValue {
		bool				IsDisp;			// 表示/非表示の切り替え.
		int					Value;			// 値.
		float				Alpha;			// 透明度.
		float				IntervalCount;	// 次の文字が動く/表示されるまでの間.

		stResultValue()
			:stResultValue( false, 0, 0.0f, 0.0f )
		{}
		stResultValue( bool isdisp, int value, float alpha, float intervalcnt )
			: IsDisp		( isdisp )
			, Value			( value )
			, Alpha			( alpha )
			, IntervalCount	( intervalcnt )
		{}
	} typedef SResultValue;
}

/************************************************
*	ゲームクリアクラス.
*		井谷凌.
**/
class CGameClear final
	: public CSceneBase
{
public:
	CGameClear();
	~CGameClear();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// モデルの描画.
	virtual void ModelRender() override;
	// スプライト(UI)の描画.
	virtual void SpriteUIRender() override;
	// スプライト(3D)/Effectの描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	// 日給の描画処理.
	void RenderOriginWage();
	// 詳細情報の描画処理.
	void RenderDetails();

	// 日給表示中の処理.
	void ShowOriginWage();
	// 日給表示後の処理.
	void UpdateAfterShowTotalWage();

	// 各種ボーナス表示中、決定ボタンを押された時の処理.
	void PushButton( int Type );
	// 詳細情報の値の透明度を増加させる処理.
	void AddAlpha( int Type );
	// ボーナス,割引増減後の処理.
	void AfterAddDetails( int Type );

	// 詳細情報の処理.
	void ShowDetails( int Type );

	// 日給の桁を再設定する.
	void ResetWageDigit();

	// UIの不透明化処理.
	void AddAlpha();

private:
	float								m_DeltaTime;

	// UI.
	std::unique_ptr<CGameClearWidget>	m_pGameClearWidget;

	std::vector<CSprite*>				m_pSpriteList;
	std::vector<SSpriteRenderState>		m_SpriteStateList;
	bool								m_IsStartDisp;
	bool								m_CanOperate;
	float								m_Alpha;
	int									m_Choice;

	// 現在表示している種類.
	EShowState							m_ShowState;

	// 詳細情報.
	std::vector<SResultValue>			m_ValueList;

	// 日給の表示に使用.
	std::vector<float>					m_PosX;				// 1桁ずつ動かすのに使用.
	int									m_WageDigits;		// 日給の桁数.
	std::vector<float>					m_WageFinalXPos;	// 桁ごとの最終的なX座標.

	// 効果音に使用.
	std::vector<bool>					m_IsPlayDigitSE;	// 効果音を再生したか(桁).
	std::string							m_DigitSE;			// 再生する効果音(桁ごとに変更).
	int									m_ThroughStringCnt;	// 通過した文字数をカウントする.
	bool								m_IsPlayGetBonusSE;	// 効果音を再生したか(ボーナス取得).

	// フォント.
	CFont*								m_pFont;
	std::vector<SFontRenderState>		m_FontStateList;	// フォントの設定.
};