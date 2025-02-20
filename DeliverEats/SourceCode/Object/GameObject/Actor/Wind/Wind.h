#pragma once
#include "..\..\..\..\Global.h"
#include "..\..\..\..\Utility\Constant\Constant.h"

class CPlayer;

/************************************************
*	NX.
*		ú±cFP.
**/
class CWind final
{
public:
	CWind();
	~CWind();

	// CX^XÌæ¾.
	static CWind* GetInstance();

	// ú».
	static void Init();

	// XV.
	static void Update( const float& DeltaTime );
	// fobNÌXV.
	static void DebugUpdate( const float& DeltaTime );
	// SEÌÄ¶.
	static void SEUpdate( const float& DeltaTime );

	// Ìâ~.
	static void StopWind();

	// xNgÌæ¾.
	static D3DXVECTOR3 GetWindVector( const float& Y );
	// Ì­³ðæ¾.
	static float GetWindPower();
	// Ìûüðæ¾.
	static std::string GetWindDirection() { return GetInstance()->m_NowWindDire; }
	// ª¢Ä¢é©æ¾.
	static bool GetIsWind() { return GetInstance()->m_IsWind.get(); }

	// vC[ÌÝè.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

private:
	CPlayer*					m_pPlayer;			// vC[.
	CBool						m_IsWind;			// ª¢Ä¢é©.
	std::string					m_NowWindDire;		// »ÝÌÌü«.
	int							m_OldHour;			// OÌÔ.
	float						m_Power;			// Ì­³.
	float						m_SEVolume;			// SEÌ¹Ê.
	bool						m_IsWindStop;		// ð¢½è/~ß½èðâ~³¹é.

private:
	// Of[^Ìè.
	const ConstantStruct::SWind& CONSTANT = CConstant::GetWind();
};