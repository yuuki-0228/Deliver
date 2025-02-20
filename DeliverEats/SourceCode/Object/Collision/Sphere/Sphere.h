#pragma once
#include "..\ColliderBase.h"

class CStaticMesh;

/**************************************
*	ΜNX.
*		ϊ±cFP.
**/
class CSphere final
	: public CColliderBase
{
public:
	CSphere();
	virtual ~CSphere();

	// ½θ»θΜε«³ΙΘιScaleπΤ·.
	D3DXSCALE3 GetCollisionScale( const LPD3DXMESH& pMesh );

	// Όa(·³)πζΎ.
	inline float GetRadius() const {
		return m_Radius;
	}

	// bVΙν΅ΔΌaπέθ.
	HRESULT SetMeshRadius( const CStaticMesh& pMesh, const float vScale = 1.0f );
	// Όa(·³)πέθ.
	inline void SetRadius( const float radius ) {
		m_Radius  = radius;
	}

private:
	float m_Radius;		//Όa(·³).
};