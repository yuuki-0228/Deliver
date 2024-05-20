#include "GazingCamera.h"
#include "..\..\..\Utility\Easing\Easing.h"

namespace {
	constexpr float ADJUST_POSITION = 15.0f;	// �J�������v���C���[�̌��ɂ���悤�ɒ�������l.
}

CGazingCamera::CGazingCamera()
	: m_pPlayer			( nullptr )
	, m_IsSwitching		( false )
	, m_StartPosition	( INIT_FLOAT3 )
	, m_Count			( INIT_FLOAT )
	, m_RotX			( INIT_FLOAT )
{
}

CGazingCamera::~CGazingCamera()
{
}

//---------------------------.
// �X�V.
//	�^�[�Q�b�g���ݒ肳��Ă��Ȃ��ꍇ�����͍s��Ȃ�.
//---------------------------.
void CGazingCamera::Update( const float& DeltaTime )
{
	// �����_���v���C���[�ɍ��킹��.
	m_LookPosition = m_pPlayer->GetPosition();

	// �J��������~���Ă���(��O�ɏo����ǂ�n�ʂɌ��˂����ꍇ)�Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_IsMoveStop )return;

	// �v���C���[����^�[�Q�b�g�Ɍ������x�N�g�����v�Z����.
	D3DXVECTOR3 Vec = m_pPlayer->GetPosition() - m_pTarget->GetPosition() - D3DXVECTOR3( 0.0f, 2.0f, 0.0f );
	// �x�N�g���𐳋K������.
	D3DXVec3Normalize( &Vec, &Vec );
	// �J�����̈ʒu.
	const D3DXVECTOR3 GazingPos = m_pPlayer->GetPosition() + Vec * ADJUST_POSITION;

	// ���C���J��������؂�ւ��A�ړ����������Ă��Ȃ��Ȃ�.
	if ( m_IsSwitching ) {
		MoveFromMainCamera( GazingPos, DeltaTime );
		return;
	}

	// �ړ������������Ȃ�.
	// �J�����̈ʒu���v���C���[�ɍ��킹��.
	m_Transform.Position = GazingPos;
}

//---------------------------.
// ���C���J��������؂�ւ�����ۂɌĂяo��.
//---------------------------.
void CGazingCamera::StartMove()
{
	m_IsSwitching = true;

	m_StartPosition = m_Transform.Position;
	m_Count = INIT_FLOAT;
}

//---------------------------.
// ���C���J��������؂�ւ�������̏���.
//---------------------------.
void CGazingCamera::MoveFromMainCamera( const D3DXVECTOR3 GoalPos, const float& DeltaTime )
{
	// �؂�ւ����̓��C���J�����̓������Č�����.
	const float Distance = -15.0f;
	// �^�[�Q�b�g�̌������擾.
	const D3DXVECTOR3 TarVec = m_pPlayer->GetTransform().GetRotationVector();
	m_StartPosition.x = m_pPlayer->GetPosition().x + Distance * sinf( m_RotX ) * TarVec.x;
	m_StartPosition.y = m_pPlayer->GetPosition().y + Distance * cosf( m_RotX );
	m_StartPosition.z = m_pPlayer->GetPosition().z + Distance * sinf( m_RotX ) * TarVec.z;

	// �؂�ւ�����.

	m_Count += DeltaTime;
	if ( m_Count >= 1.0f )m_Count = 1.0f;

	D3DXVECTOR3 ToStartVector = m_StartPosition - GoalPos;
	float EaseCount = CEasing::InOutSine( m_Count, 1.0f );
	m_Transform.Position = m_StartPosition - ToStartVector * EaseCount;

	if ( m_Count >= 1.0f ) {
		m_IsSwitching = false;
		m_Transform.Position = GoalPos;
	}
}
