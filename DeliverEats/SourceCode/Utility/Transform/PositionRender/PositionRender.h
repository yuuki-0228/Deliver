#pragma once
#include "..\..\..\Utility\Transform\Transform.h"

class CStaticMesh;

/************************************************
*	座標の表示クラス.
**/
class CPositionRender final
{
public:
	CPositionRender();
	~CPositionRender();

	// インスタンスの取得.
	static CPositionRender* GetInstance();

	// 更新.
	static void Update( const float& DeltaTime );

	// 描画.
	static void Render( const STransform& Transform );

private:
	CStaticMesh*	m_pMesh;		// モデル.
	STransform		m_Transform;	// トランスフォーム.
	CBool			m_IsRender;		// 表示するか.
};