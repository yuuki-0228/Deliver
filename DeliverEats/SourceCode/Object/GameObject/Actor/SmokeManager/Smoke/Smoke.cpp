#include "Smoke.h"
#include "..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\..\Utility\Math\Math.h"

namespace{
	constexpr float EXPANSION_MAX_DANG	= 90.0f;	// �ő�܂Ŋg�債�����̊p�x(�x).
	constexpr float SHRINK_END_DANG		= 180.0f;	// �ő�܂ŏk���������̊p�x(�x).
}

CSmoke::CSmoke()
	: m_pSmokeMesh	( nullptr )
	, m_MoveVec		( 0.0f, 0.0f, 0.0f )
	, m_AddMoveVec	( 0.0f, 0.0f, 0.0f )
	, m_DAng		( 0.0f	)
	, m_ScaleSpeed	( 0.0f )
	, m_Alpha		( 1.0f )
	, m_AnimEnd		( true )
{
	Init();
}

CSmoke::~CSmoke()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CSmoke::Init()
{
	m_pSmokeMesh = CMeshResource::GetCollision( "Sphere_c" );

	// �g�k�����邽�߈�ԏ��������Ă���.
	m_Transform.Scale	= { 0.0f, 0.0f, 0.0f };
	m_AnimEnd			= true;
	return true;
}

//------------------------------------.
// �X�V.
//	�\�����Ă��Ȃ���Ԃ̏ꍇ�������s��Ȃ�.
//------------------------------------.
void CSmoke::Update( const float& deltaTime )
{
	if ( m_AnimEnd ) return;

	m_IsDisp = true;
	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );	// �����ɂ��A���t�@�t�F�[�h�̍X�V.

	// �g�k���Ă���.
	m_DAng += m_ScaleSpeed * deltaTime;
	const float Scale = sinf( Math::ToRadian( m_DAng ) ) * CONSTANT.SCALE_MAX;
	m_Transform.Scale = { Scale, Scale, Scale };

	// �����ɂ��Ă���.
	m_Alpha = sinf( Math::ToRadian( m_DAng ) ) * CONSTANT.ALPHA;

	// �ړ�.
	m_Transform.Position += m_MoveVec * CONSTANT.MOVE_SPEED + m_AddMoveVec * Scale;

	// �ő�T�C�Y�ɂȂ�����g�k���x�����炷.
	if ( m_DAng < EXPANSION_MAX_DANG ) return;
	m_ScaleSpeed = CONSTANT.SHRINK_SPEED;

	// �ő�܂ŏk�������ꍇ��\���ɂ���.
	if ( m_DAng < SHRINK_END_DANG ) return;
	m_AnimEnd = true;
}

//------------------------------------.
// �`��.
//	�\�����Ă��Ȃ���Ԃ̏ꍇ�������s��Ȃ�.
//------------------------------------.
void CSmoke::Render()
{
	if ( m_AnimEnd			) return;
	if ( m_IsDisp == false	) return;

	m_pSmokeMesh->SetDither( true );
	m_pSmokeMesh->SetColor( CONSTANT.COLOR );
	m_pSmokeMesh->SetAlpha( m_Distance - ( 1.0f - m_Alpha ) );
	m_pSmokeMesh->Render( &m_Transform );
	m_pSmokeMesh->SetDither( false );
}

//------------------------------------.
// �A�j���[�V�����̊J�n.
//------------------------------------.
void CSmoke::AnimPlay( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& AddMoveVec )
{
	// ������.
	m_DAng					= 0.0f;
	m_Transform.Scale		= { 0.0f, 0.0f, 0.0f };
	m_ScaleSpeed			= CONSTANT.EXPANSION_SPEED;

	// �����_���̐ݒ�.
	const float Rand_DAng	= CRandom::GetRand( -CONSTANT.MOVEVEC_RAND_ROT_MAX,		CONSTANT.MOVEVEC_RAND_ROT_MAX	);
	const float Rand_UpVec	= CRandom::GetRand(  CONSTANT.MOVEVEC_RAND_MOVEY_MIN,	CONSTANT.MOVEVEC_RAND_MOVEY_MAX	);
	const float Rand_Move	= CRandom::GetRand( -CONSTANT.STARTPOS_RAND_MOVE_MAX,	CONSTANT.STARTPOS_RAND_MOVE_MAX	);

	// ���W�𓮂����x�N�g�����쐬.
	D3DXVECTOR3 PosMoveVec;
	PosMoveVec.x = MoveVec.z;
	PosMoveVec.y = 0.0f;
	PosMoveVec.z = -PosMoveVec.x;

	// ���W�̐ݒ�.
	m_Transform.Position = Pos;
	m_Transform.Position += PosMoveVec * Rand_Move;

	// �ړ��x�N�g���̐ݒ�.
	const float RAng = Math::ToRadian( Rand_DAng );
	m_MoveVec.x = MoveVec.x * cosf( RAng ) - MoveVec.z * sinf( RAng );
	m_MoveVec.y = Rand_UpVec;
	m_MoveVec.z = MoveVec.x * sinf( RAng ) + MoveVec.z * cosf( RAng );
	m_AnimEnd	= false;

	// �ǉ��̈ړ��x�N�g���̐ݒ�.
	m_AddMoveVec = AddMoveVec;
}
