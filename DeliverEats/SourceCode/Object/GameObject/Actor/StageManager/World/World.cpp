#include "World.h"
#include "Sky\Sky.h"
#include "Cloud\Cloud.h"

CWorld::CWorld()
	: m_pSky			( nullptr )
	, m_pCloud			( nullptr )
{
}

CWorld::~CWorld()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CWorld::Init()
{
	// 空の初期化.
	m_pSky = std::make_unique<CSky>();
	m_pSky->Init();

	// 雲の初期化.
	m_pCloud = std::make_unique<CCloud>();
	m_pCloud->Init();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CWorld::Update( const float& DeltaTime )
{	
	m_pCloud->Update( DeltaTime );
	m_pSky->Update( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CWorld::Render()
{
	m_pCloud->Render();
	m_pSky->Render();
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CWorld::Sprite3DRender_A()
{
	m_pCloud->Sprite3DRender_A();
	m_pSky->Sprite3DRender_A();
}
void CWorld::Sprite3DRender_B()
{
}