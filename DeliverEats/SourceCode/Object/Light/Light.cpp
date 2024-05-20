#include "Light.h"
#include "..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"
#include "..\..\Utility\Math\Math.h"

CLight::CLight()
	: m_Direction		( 0.0, 1.0f, 0.0f )
	, m_Intensity		( 1.0f )
{
}

CLight::~CLight()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CLight* CLight::GetInstance()
{
	static std::unique_ptr<CLight> pInstance = std::make_unique<CLight>();
	return pInstance.get();
}

//---------------------------.
// 更新.
//---------------------------.
void CLight::Update()
{
#ifdef _DEBUG
	CDebugWindow::PushProc( "Light", [&]() {
		D3DXVECTOR3	Dir = CLight::GetDirection();
		float		Int = CLight::GetIntensity();

		CImGuiManager::SliderVec3(  "Direction", &Dir, Math::RADIAN_MIN, Math::RADIAN_MAX );
		CImGuiManager::SliderFloat( "Intensity", &Int, 0.0f, 10.0f );

		CLight::SetDirection( Dir );
		CLight::SetIntensity( Int );
		} );
#endif	// #ifdef _DEBUG.
}
