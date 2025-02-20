#pragma once
#include "..\ColliderBase.h"

class CStaticMesh;

/**************************************
*	ρ]΅Θ’~NX.
*		ϊ±cFP.
**/
class CCylinder final
	: public CColliderBase
{
public:
	CCylinder();
	virtual ~CCylinder();

	// ½θ»θΜε«³ΙΘιScaleπΤ·.
	D3DXSCALE3 GetCollisionScale( const LPD3DXMESH& pMesh );

	// Όa(·³)πζΎ.
	inline float GetRadius() const {
		return m_Radius;
	}
	// γΊΜγΐπζΎ.
	inline float GetHeight() const {
		return m_Height;
	}

	// bVΙν΅ΔΌaπέθ.
	HRESULT SetMeshRadius( const LPD3DXMESH& pMesh, const float vScale = 1.0f );
	// Όa(·³)πέθ.
	inline void SetRadius( const float radius ) {
		m_Radius = radius;
	}
	// ³πέθ.
	inline void SetHeight( const float height ) {
		m_Height = height;
	}

private:
	float m_Radius;		// Όa(·³).
	float m_Height;		// ³.
};