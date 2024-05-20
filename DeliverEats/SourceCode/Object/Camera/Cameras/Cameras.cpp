#include "Cameras.h"

CCameras::CCameras()
	: m_pMainCamera				( nullptr )
	, m_pNormalCamera			( nullptr )
	, m_pFreeCamera				( nullptr )
	, m_pMoveCamera				( nullptr )
	, m_pThirdPersonCamera		( nullptr )
{
}

CCameras::~CCameras()
{
}

//---------------------------.
// éwíËÇµÇΩìñÇΩÇËîªíËÇÃèâä˙âª.
//---------------------------.
void CCameras::InitCamera( const ECameraType& Type )
{
	switch ( Type ) {
	case ECameraType::MainCamera:
		m_pMainCamera			= std::make_unique<CMainCamera>();
		break;
	case ECameraType::NormalCamera:
		m_pNormalCamera			= std::make_unique<CNormalCamera>();
		break;
	case ECameraType::FreeCamera:
		m_pFreeCamera			= std::make_unique<CFreeCamera>();
		break;
	case ECameraType::MoveCamera:
		m_pMoveCamera			= std::make_unique<CMoveCamera>();
		break;
	case ECameraType::ThirdPersonCamera:
		m_pThirdPersonCamera	= std::make_unique<CThirdPersonCamera>();
		break;
	default:
		break;
	}
}
