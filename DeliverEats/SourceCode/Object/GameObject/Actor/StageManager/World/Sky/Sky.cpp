#include "Sky.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\Mesh\SkyBoxMesh\SkyBoxMesh.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr float		END_TIME		= 10800.0f;	// �Q�[���I������.
	constexpr float		CLOUD_SPPED		= 0.03f;	// �_�̈ړ����x.
	const D3DXVECTOR3	START_COLOR		= { 1.00f, 0.98f, 0.80f };
	const D3DXVECTOR3	CENTER_COLOR	= Color::Orange;
	const D3DXVECTOR3	END_COLOR		= Color::Gray;
}

CSky::CSky()
	: m_pColorMesh			( nullptr )
	, m_pSunAndMonn			( nullptr )
	, m_pCloudMesh			( nullptr )
	, m_CloudTransform		()
	, m_SunAndMonnTransform	()
	, m_SkyColor			( Color::White )
	, m_SkyColorVector		()
{
}

CSky::~CSky()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CSky::Init()
{
	// ���b�V���̎擾.
	m_pColorMesh	= CMeshResource::GetSkyBox( "sky_b" );
	m_pSunAndMonn	= CMeshResource::GetSkyBox( "sun_moon_b" );
	m_pCloudMesh	= CMeshResource::GetSkyBox( "cloud_b" );

	// ��̐F�̕ω��p�x�N�g���̍쐬.
	m_SkyColorVector.resize( 3 );
	m_SkyColorVector[0] = START_COLOR	- Color::White;
	m_SkyColorVector[1] = CENTER_COLOR	- START_COLOR;
	m_SkyColorVector[2] = END_COLOR		- CENTER_COLOR;

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CSky::Update( const float& DeltaTime )
{
	// �_����������]������.
	m_CloudTransform.Rotation.y += Math::ToRadian( CLOUD_SPPED );
	m_CloudTransform.RotationClamp();

	// ���z�ƌ�����������]������.
	const float Speed = Math::DEGREE_MID * ( CTime::GetElapsedTime() / CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX );
	m_SunAndMonnTransform.Rotation.z = Math::ToRadian( Speed );
	m_SunAndMonnTransform.RotationClamp();

	// ��̐F�̕ύX.
	const int No = static_cast<int>( CTime::GetElapsedTime() ) / static_cast<int>( END_TIME );
	switch ( No ) {
	case 0:
		m_SkyColor = Color::White + m_SkyColorVector[0] * ( CTime::GetElapsedTime() / END_TIME );
		break;
	case 1:
		m_SkyColor = START_COLOR  + m_SkyColorVector[1] * ( ( CTime::GetElapsedTime() - END_TIME ) / END_TIME );
		break;
	case 2:
		m_SkyColor = CENTER_COLOR + m_SkyColorVector[2] * ( ( CTime::GetElapsedTime() - END_TIME * 2.0f ) / END_TIME );
		break;
	default:
		break;
	}

	// �_�̐F�̕ύX.
	const float Color = Color::COLOR_MAX - Color::COLOR_MID * ( CTime::GetElapsedTime() / CConstant::GetTime().CLEAR_SCENE_MOVE_TIME_MAX );
	m_CloudColor = { Color, Color, Color };
}

//---------------------------.
// �`��.
//---------------------------.
void CSky::Render()
{
	CDirectX11::SetAlphaBlend( true );

	// ��̐F�̕`��.
	m_pColorMesh->SetColor( m_SkyColor );
	m_pColorMesh->Render();

	// �_�̕`��.
	m_pCloudMesh->SetColor( m_CloudColor );
	m_pCloudMesh->Render( &m_CloudTransform );

	CDirectX11::SetAlphaBlend( false );

	// ���z�ƌ��̕`��.
	m_pSunAndMonn->SetDither( true );
	m_pSunAndMonn->Render( &m_SunAndMonnTransform );
	m_pSunAndMonn->SetDither( false );
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CSky::Sprite3DRender_A()
{
}
