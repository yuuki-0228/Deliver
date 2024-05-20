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
// ������.
//---------------------------.
bool CWorld::Init()
{
	// ��̏�����.
	m_pSky = std::make_unique<CSky>();
	m_pSky->Init();

	// �_�̏�����.
	m_pCloud = std::make_unique<CCloud>();
	m_pCloud->Init();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CWorld::Update( const float& DeltaTime )
{	
	m_pCloud->Update( DeltaTime );
	m_pSky->Update( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CWorld::Render()
{
	m_pCloud->Render();
	m_pSky->Render();
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CWorld::Sprite3DRender_A()
{
	m_pCloud->Sprite3DRender_A();
	m_pSky->Sprite3DRender_A();
}
void CWorld::Sprite3DRender_B()
{
}