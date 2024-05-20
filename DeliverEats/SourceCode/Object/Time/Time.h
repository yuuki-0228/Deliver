#pragma once
#include "..\..\Global.h"
#include "..\..\Utility\Constant\Constant.h"

/************************************************
*	ゲーム内の時間クラス.
*		﨑田友輝.
**/
class CTime final
{
public:
	using Time			= std::pair<int, int>;
	using Time_String	= std::pair<std::string, std::string>;

public:
	CTime();
	~CTime();

	// インスタンスの取得.
	static CTime* GetInstance();

	// 初期化.
	static void Init();

	// 更新.
	static void Update( const float& DeltaTime );
	// デバックの更新.
	static void DebugUpdate( const float& DeltaTime );


	// 現在の時刻を取得<時, 分>.
	static Time GetTime();
	// 現在の時刻を文字列で取得<時, 分>.
	static Time_String GetTimeString();

	// 経過時間の取得.
	static float GetElapsedTime() { return GetInstance()->m_ElapsedTime; }
	// 現在の時間を取得.
	static int GetHour() { return GetInstance()->m_Hour; }
	// 現在の分を取得.
	static int GetMinute() { return GetInstance()->m_Minute; }

	// 指定した時間から経過した分の取得.
	static int GetElapsedMinute( const float ElapsedTime );
	// 指定した時間から経過した時間の取得.
	static int GetElapsedHour( const float ElapsedTime );

	// 時間の進み具合を取得( 0.0f ~ 1.0f ).
	static float GetTimeRatio();

	// ゴールデンタイムか取得.
	static bool GetIsGoldenTime() { return GetInstance()->m_IsGoldenTime; }
	// ゲームが終了したか取得.
	static bool GetIsGameEnd() { return GetInstance()->m_IsEnd; }
	// ゲームクリアに移動させるか取得.
	static bool GetIsClearSceneMove() { return GetInstance()->m_IsClearSceneMove; }

private:
	float				m_RealElapsedTime;			// 現実の経過時間(秒).
	float				m_ElapsedTime;				// 経過時間(秒).
	int					m_Minute;					// 現在の分(分).
	int					m_Hour;						// 現在の時間(時).
	bool				m_IsGoldenTime;				// ゴールデンタイムか.
	bool				m_IsEnd;					// ゲームが終了したか.
	bool				m_IsClearSceneMove;			// ゲームクリアに移動させるか.
	std::vector<bool>	m_IsPushTimeNotification;	// 時間による通知を送ったか.

private:
	// 外部データの定数.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};