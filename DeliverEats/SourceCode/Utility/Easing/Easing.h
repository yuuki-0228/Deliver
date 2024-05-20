#pragma once
#include "..\..\Global.h"

/************************************************
*	イージングクラス.
*		﨑田友輝.
**/
class CEasing final
{
public:
	CEasing();
	~CEasing();

	// インスタンスの取得.
	static CEasing* GetInstance();

	// イージング関数.
	//	 各イージングの動作は以下で確認することができます.
	//	 https://easings.net/ja.
	static float Linear(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InSine(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutSine(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutSine(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InCubic(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutCubic(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutCubic(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InQuint(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutQuint(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutQuint(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InCirc(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutCirc(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutCirc(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InQuad(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutQuad(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutQuad(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InQuart(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutQuart(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutQuart(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	
	static float InExpo(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutExpo(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutExpo(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	static float InBack(		float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );
	static float OutBack(		float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );
	static float InOutBack(		float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );
	
	static float InBounce(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float OutBounce(		float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );
	static float InOutBounce(	float Time, float TotalTime, float Max = 1.0f, float Min = 0.0f );

	// イージングリストの取得.
	static std::vector<std::string> GetEasingList();

	// イージングリストからイージングを行う.
	static float Easing( const std::string& Name, float Time, float TotalTime, float Back, float Max = 1.0f, float Min = 0.0f );

private:
	// 2つの数値を引いたときに0になるか調べる.
	static bool CheckZero( const float EpsilonNum, const float Num );

private:
	// コピー・ムーブコンストラクタ, 代入演算子の削除.
	CEasing( const CEasing & )				= delete;
	CEasing& operator = ( const CEasing & ) = delete;
	CEasing( CEasing && )					= delete;
	CEasing& operator = ( CEasing && )		= delete;
};