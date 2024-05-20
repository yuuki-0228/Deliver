#include "PizzaKilnFire.h"
#include "..\..\..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\..\..\Common\Mesh\StaticMesh\StaticMesh.h"
#include "..\..\..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr float ADD_COLOR_ANG_END = 180.0f;	// ���Z����F�p�̍ŏI�p�x(�x).
}

CPizzaKilnFire::CPizzaKilnFire()
	: m_pFire		( nullptr )
	, m_Color		( Color::Red )
	, m_AddColor	( Color::INIT )
	, m_Alpha		( Color::ALPHA_MAX )
	, m_AddColorAng	( INIT_FLOAT )
	, m_FloorIsDisp	( true )
{
}

CPizzaKilnFire::~CPizzaKilnFire()
{

}

//---------------------------.
// ������.
//---------------------------.
bool CPizzaKilnFire::Init()
{
	// ���b�V���̎擾.
	m_pFire = CMeshResource::GetStatic( "pizza_fire" );
	m_pFire->SetTransform( m_Transform );

	// �F�̐ݒ�.
	m_Color		= CONSTANT.START_COLOR;
	m_AddColor	= CONSTANT.END_COLOR - CONSTANT.START_COLOR;

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CPizzaKilnFire::Update( const float& DeltaTime )
{
	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
	m_IsDisp = true;
	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );

	// �F���������ω�������.
	m_AddColorAng += CONSTANT.ADD_COLOR_SPEED * DeltaTime;
	if ( m_AddColorAng >= ADD_COLOR_ANG_END ) m_AddColorAng -= ADD_COLOR_ANG_END;
	m_Color = CONSTANT.START_COLOR + m_AddColor * sinf( Math::ToRadian( m_AddColorAng ) );
}

//---------------------------.
// �`��.
//---------------------------.
void CPizzaKilnFire::Render()
{
	if ( m_FloorIsDisp	== false ) return;
	if ( m_IsDisp		== false ) return;

	CDirectX11::SetAlphaBlend( true );

	m_pFire->SetDither( true );
	m_pFire->SetColor( Color4::RGBA( m_Color ) );
	m_pFire->SetAlpha( ( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) ) * CONSTANT.ALPHA );
	m_pFire->Render( &m_Transform );
	m_pFire->SetDither( false );

	CDirectX11::SetAlphaBlend( false );
}
