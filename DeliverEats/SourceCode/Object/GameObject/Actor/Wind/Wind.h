#pragma once
#include "..\..\..\..\Global.h"
#include "..\..\..\..\Utility\Constant\Constant.h"

class CPlayer;

/************************************************
*	風クラス.
*		﨑田友輝.
**/
class CWind final
{
public:
	CWind();
	~CWind();

	// インスタンスの取得.
	static CWind* GetInstance();

	// 初期化.
	static void Init();

	// 更新.
	static void Update( const float& DeltaTime );
	// デバックの更新.
	static void DebugUpdate( const float& DeltaTime );
	// SEの再生.
	static void SEUpdate( const float& DeltaTime );

	// 風の停止.
	static void StopWind();

	// 風ベクトルの取得.
	static D3DXVECTOR3 GetWindVector( const float& Y );
	// 風の強さを取得.
	static float GetWindPower();
	// 風の方向を取得.
	static std::string GetWindDirection() { return GetInstance()->m_NowWindDire; }
	// 風が吹いているか取得.
	static bool GetIsWind() { return GetInstance()->m_IsWind.get(); }

	// プレイヤーの設定.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

private:
	CPlayer*					m_pPlayer;			// プレイヤー.
	CBool						m_IsWind;			// 風が吹いているか.
	std::string					m_NowWindDire;		// 現在の風の向き.
	int							m_OldHour;			// 前の時間.
	float						m_Power;			// 風の強さ.
	float						m_SEVolume;			// SEの音量.
	bool						m_IsWindStop;		// 風を吹いたり/止めたりを停止させる.

private:
	// 外部データの定数.
	const ConstantStruct::SWind& CONSTANT = CConstant::GetWind();
};