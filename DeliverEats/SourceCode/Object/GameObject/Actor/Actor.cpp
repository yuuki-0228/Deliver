#include "Actor.h"
#include "..\..\Camera\CameraManager\CameraManager.h"
#include <cmath>

CActor::CActor()
	: m_pCollisions		()
	, m_IsDisp			( true )
	, m_Distance		( 1.0f )
{
}

CActor::~CActor()
{
}

//---------------------------.
// �X�N���[�����W���擾.
//---------------------------.
D3DXVECTOR3 CActor::GetScreenPos()
{
	// �r���[�|�[�g�s��.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity( &mViewport );
	mViewport._11 = FWND_W *  0.5f;
	mViewport._22 = FWND_H * -0.5f;
	mViewport._41 = FWND_W *  0.5f;
	mViewport._42 = FWND_H *  0.5f;

	// �X�N���[�����W.
	D3DXMATRIX	m	= CCameraManager::GetViewProjMatrix() * mViewport;
	D3DXVECTOR3 Out = { 0.0f, 0.0f, 0.0f };
	D3DXVec3TransformCoord( &Out, &m_Transform.Position, &m );
	if ( !std::isfinite(  Out.x ) ) Out.x = 0.0f;
	if ( !std::isfinite(  Out.y ) ) Out.y = 0.0f;
	if ( !std::isfinite(  Out.z ) ) Out.z = 0.0f;

	return Out;
}

//---------------------------.
// ��ʓ��O�ɂ��邩.
//---------------------------.
bool CActor::IsScreenOut( const float& Size )
{
	// �X�N���[�����W���擾.
	D3DXVECTOR3	ScreenPos	= Math::Round_n( GetScreenPos(), 5 );

	// ��ʓ��ɂ��邩.
	if ( -Size * 2 < ScreenPos.x && ScreenPos.x < FWND_W + Size * 2 &&
		 -Size * 2 < ScreenPos.y && ScreenPos.y < FWND_H + Size * 2 &&
		 ScreenPos.z < 1.15f ) return false;

	return true;	// ��ʊO.
}

//---------------------------.
// �����ɂ��t�F�[�h�̍X�V.
//---------------------------.
void CActor::DistanceFadeUpdate( const float& StartDistance, const float& AlphaDistance )
{
	if ( m_IsDisp == false ) return;

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );

	// ���݂̋������A���t�@�t�F�[�h�����鋗���ȏォ.
	m_Distance = Color::ALPHA_MAX;
	if ( Distance <= StartDistance ) return;
	const float OverDistance = Distance - StartDistance;
	
	// ������ύX������.
	m_Distance = Color::ALPHA_MAX - OverDistance / AlphaDistance / 10.0f;

	// ��\���ɂ���.
	if ( m_Distance <= Color::ALPHA_MIN ) {
		m_Distance	= Color::ALPHA_MIN;
		m_IsDisp	= false;
	}
}
