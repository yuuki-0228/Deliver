#include "Easing.h"
#include "..\Math\Math.h"
#include <limits>
#include <algorithm>

CEasing::CEasing()
{
}

CEasing::~CEasing()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CEasing* CEasing::GetInstance()
{
	static std::unique_ptr<CEasing> pInstance = std::make_unique<CEasing>();
	return pInstance.get();
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::Linear( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	return ( Max - Min ) * Time / TotalTime + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InSine( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max -= Min;

	return -Max * std::cos( Time * Math::ToRadian( 90.0f ) / TotalTime ) + Max + Min;
}
float CEasing::OutSine( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max -= Min;

	return Max * std::sin( Time * Math::ToRadian( 90.0f ) / TotalTime ) + Min;
}
float CEasing::InOutSine( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max -= Min;

	return -Max / 2.0f * ( std::cos( Time * Math::PI / TotalTime ) - 1 ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InCubic( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	return Max * Time * Time * Time + Min;
}
float CEasing::OutCubic( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	 = Time / TotalTime - 1.0f;

	return Max * ( Time * Time * Time + 1.0f ) + Min;
}
float CEasing::InOutCubic( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	if ( Time / 2.0f < 1.0f )
		return Max / 2.0f * Time * Time * Time + Min;

	Time -= 2.0f;

	return Max / 2.0f * ( Time * Time * Time + 2.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InQuint( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	return Max * Time * Time * Time * Time * Time + Min;
}
float CEasing::OutQuint( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	 = Time / TotalTime - 1.0f;

	return Max * ( Time * Time * Time * Time * Time + 1.0f ) + Min;
}
float CEasing::InOutQuint( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	if ( Time / 2.0f < 1.0f )
		return Max / 2.0f * Time * Time * Time * Time * Time + Min;

	Time -= 2.0f;

	return Max / 2.0f * ( Time * Time * Time * Time * Time + 2.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InCirc( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;
	Time	 = ( std::max )( ( std::min )( Time, 1.0f ), -1.0f );

	return -Max * ( std::sqrt( 1.0f - Time * Time ) - 1.0f ) + Min;
}
float CEasing::OutCirc( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= ( TotalTime - 1.0f );
	Time	 = ( std::max )( ( std::min )( Time, 1.0f ), -1.0f );

	return Max * std::sqrt( 1.0f - Time * Time ) + Min;
}
float CEasing::InOutCirc( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;
	Time	 = ( std::max )( ( std::min )( Time, 1.0f ), -1.0f );


	if ( Time / 2.0f < 1.0f )
		return -Max / 2.0f * ( std::sqrt( 1.0f - Time * Time ) - 1.0f ) + Min;

	Time -= 2.0f;

	return Max / 2.0f * ( std::sqrt( 1.0f - Time * Time ) + 1.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InQuad( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	return Max * Time * Time + Min;
}
float CEasing::OutQuad( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	return -Max * Time * ( Time - 2.0f ) + Min;
}
float CEasing::InOutQuad( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	if ( Time / 2.0f < 1.0f )
		return Max / 2.0f * Time * Time + Min;

	Time--;

	return -Max * ( Time * ( Time - 2.0f ) - 1.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InQuart( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	return Max * Time * Time * Time * Time + Min;
}
float CEasing::OutQuart( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	 = Time / TotalTime - 1.0f;

	return -Max * ( Time * Time * Time * Time - 1.0f ) + Min;
}
float CEasing::InOutQuart( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	if ( Time / 2.0f < 1.0f )
		return Max / 2.0f * Time * Time * Time * Time + Min;

	Time -= 2.0f;

	return -Max / 2.0f * ( Time * Time * Time * Time - 2.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InExpo( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max -= Min;

	return CheckZero( Time, 0.0f ) ?
		Min :
		Max * std::pow( 2.0f, 10.0f * ( Time / TotalTime - 1.0f ) ) + Min;
}
float CEasing::OutExpo( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max -= Min;

	return CheckZero( Time, TotalTime ) ?
		Max + Min :
		Max * ( -std::pow( 2.0f, -10.0f * Time / TotalTime ) + 1.0f ) + Min;

}
float CEasing::InOutExpo( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime				) return Max;
	if ( CheckZero( Time, TotalTime	)	) return Max;
	if ( CheckZero( Time, 0.0f		)	) return Min;

	Max		-= Min;
	Time	/= TotalTime;

	if ( Time / 2.0f < 1.0f )
		return Max / 2.0f * std::pow( 2.0f, 10.0f * ( Time - 1.0f ) ) + Min;

	Time--;

	return Max / 2.0f * ( -std::pow( 2.0f, -10.0f * Time ) + 2.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InBack( float Time, float TotalTime, float Back, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	return Max * Time * Time * ( ( Back + 1.0f ) * Time - Back ) + Min;
}
float CEasing::OutBack( float Time, float TotalTime, float Back, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	 = Time / TotalTime - 1.0f;

	return Max * ( Time * Time * ( ( Back + 1.0f ) * Time * Back ) + 1.0f ) + Min;
}
float CEasing::InOutBack( float Time, float TotalTime, float Back, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Back	*= 1.525f;

	if ( Time / 2.0f < 1.0f )
		return Max * ( Time * Time * ( ( Back + 1.0f ) * Time - Back ) ) + Min;

	Time -= 2.0f;

	return Max / 2.0f * ( Time * Time * ( ( Back + 1.0f ) * Time + Back ) + 2.0f ) + Min;
}

//----------------------------.
// イージング関数.
//	各イージングの動作は以下で確認することができます.
//	https://easings.net/ja.
//----------------------------.
float CEasing::InBounce( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	return Max - OutBounce( TotalTime - Time, TotalTime, Max - Min, 0.0f ) + Min;
}
float CEasing::OutBounce( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	Max		-= Min;
	Time	/= TotalTime;

	if ( Time < 1.0f / 2.75f ) {
		return Max * ( 7.5625f * Time * Time ) + Min;
	}
	else if ( Time < 2.0f / 2.75f ) {
		Time -= 1.5f / 2.75f;

		return Max * ( 7.5625f * Time * Time + 0.75f ) + Min;
	}
	else if ( Time < 2.5f / 2.75f ) {
		Time -= 2.25f / 2.75f;

		return Max * ( 7.5625f * Time * Time + 0.9375f ) + Min;
	}
	else {
		Time -= 2.625f / 2.75f;

		return Max * ( 7.5625f * Time * Time + 0.984375f ) + Min;
	}
}
float CEasing::InOutBounce( float Time, float TotalTime, float Max, float Min )
{
	if ( Time >= TotalTime ) return Max;

	if ( Time < TotalTime / 2.0f ) {
		return InBounce( Time * 2.0f, TotalTime, Max - Min, Max ) * 0.5f + Min;
	}
	else {
		return OutBounce( Time * 2.0f - TotalTime, TotalTime, Max - Min, 0.0f ) * 0.5f + Min + ( Max - Min ) * 0.5f;
	}
}

//---------------------------.
// イージングリストの取得.
//---------------------------.
std::vector<std::string> CEasing::GetEasingList()
{
	return {
		"Linear",
		"InSine",	"OutSine",		"InOutSine",
		"InCubic",	"OutCubic",		"InOutCubic",
		"InQuint",	"OutQuint",		"InOutQuint",
		"InCirc",	"OutCirc",		"InOutCirc",
		"InQuad",	"OutQuad",		"InOutQuad",
		"InQuart",	"OutQuart",		"InOutQuart",
		"InExpo",	"OutExpo",		"InOutExpo",
		"InBack",	"OutBack",		"InOutBack",
		"InBounce",	"OutBounce",	"InOutBounce",
	};
}

//---------------------------.
// イージングリストからイージングを行う.
//---------------------------.
float CEasing::Easing( const std::string& Name, float Time, float TotalTime, float Back, float Min, float Max )
{
	if (		Name == "Linear"		) return Linear(		Time, TotalTime, Min, Max );
	else if (	Name == "InSine"		) return InSine(		Time, TotalTime, Min, Max );
	else if (	Name == "OutSine"		) return OutSine(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutSine"		) return InOutSine(		Time, TotalTime, Min, Max );
	else if (	Name == "InCubic"		) return InCubic(		Time, TotalTime, Min, Max );
	else if (	Name == "OutCubic"		) return OutCubic(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutCubic"	) return InOutCubic(	Time, TotalTime, Min, Max );
	else if (	Name == "InQuint"		) return InQuint(		Time, TotalTime, Min, Max );
	else if (	Name == "OutQuint"		) return OutQuint(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutQuint"	) return InOutQuint(	Time, TotalTime, Min, Max );
	else if (	Name == "InCirc"		) return InCirc(		Time, TotalTime, Min, Max );
	else if (	Name == "OutCirc"		) return OutCirc(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutCirc"		) return InOutCirc(		Time, TotalTime, Min, Max );
	else if (	Name == "InQuad"		) return InQuad(		Time, TotalTime, Min, Max );
	else if (	Name == "OutQuad"		) return OutQuad(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutQuad"		) return InOutQuad(		Time, TotalTime, Min, Max );
	else if (	Name == "InQuart"		) return InQuart(		Time, TotalTime, Min, Max );
	else if (	Name == "OutQuart"		) return OutQuart(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutQuart"	) return InOutQuart(	Time, TotalTime, Min, Max );
	else if (	Name == "InExpo"		) return InExpo(		Time, TotalTime, Min, Max );
	else if (	Name == "OutExpo"		) return OutExpo(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutExpo"		) return InOutExpo(		Time, TotalTime, Min, Max );
	else if (	Name == "InBack"		) return InBack(		Time, TotalTime, Back, Min, Max );
	else if (	Name == "OutBack"		) return OutBack(		Time, TotalTime, Back, Min, Max );
	else if (	Name == "InOutBack"		) return InOutBack(		Time, TotalTime, Back, Min, Max );
	else if (	Name == "InBounce"		) return InBounce(		Time, TotalTime, Min, Max );
	else if (	Name == "OutBounce"		) return OutBounce(		Time, TotalTime, Min, Max );
	else if (	Name == "InOutBounce"	) return InOutBounce(	Time, TotalTime, Min, Max );
	return 0.0f;
}

//---------------------------.
// 二つの数値を引いたときに0になるか調べる.
//---------------------------.
bool CEasing::CheckZero( const float EpsilonNum, const float Num )
{
	return std::abs( EpsilonNum - Num ) <= std::numeric_limits<float>::epsilon();
}
