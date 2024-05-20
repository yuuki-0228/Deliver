#include "Smoke.h"
#include "..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\..\Utility\Math\Math.h"

namespace{
	constexpr float EXPANSION_MAX_DANG	= 90.0f;	// 最大まで拡大した時の角度(度).
	constexpr float SHRINK_END_DANG		= 180.0f;	// 最大まで縮小した時の角度(度).
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
// 初期化.
//------------------------------------.
bool CSmoke::Init()
{
	m_pSmokeMesh = CMeshResource::GetCollision( "Sphere_c" );

	// 拡縮させるため一番小さくしておく.
	m_Transform.Scale	= { 0.0f, 0.0f, 0.0f };
	m_AnimEnd			= true;
	return true;
}

//------------------------------------.
// 更新.
//	表示していない状態の場合処理を行わない.
//------------------------------------.
void CSmoke::Update( const float& deltaTime )
{
	if ( m_AnimEnd ) return;

	m_IsDisp = true;
	DistanceFadeUpdate( CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );	// 距離によるアルファフェードの更新.

	// 拡縮していく.
	m_DAng += m_ScaleSpeed * deltaTime;
	const float Scale = sinf( Math::ToRadian( m_DAng ) ) * CONSTANT.SCALE_MAX;
	m_Transform.Scale = { Scale, Scale, Scale };

	// 透明にしていく.
	m_Alpha = sinf( Math::ToRadian( m_DAng ) ) * CONSTANT.ALPHA;

	// 移動.
	m_Transform.Position += m_MoveVec * CONSTANT.MOVE_SPEED + m_AddMoveVec * Scale;

	// 最大サイズになったら拡縮速度を減らす.
	if ( m_DAng < EXPANSION_MAX_DANG ) return;
	m_ScaleSpeed = CONSTANT.SHRINK_SPEED;

	// 最大まで縮小した場合非表示にする.
	if ( m_DAng < SHRINK_END_DANG ) return;
	m_AnimEnd = true;
}

//------------------------------------.
// 描画.
//	表示していない状態の場合処理を行わない.
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
// アニメーションの開始.
//------------------------------------.
void CSmoke::AnimPlay( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& AddMoveVec )
{
	// 初期化.
	m_DAng					= 0.0f;
	m_Transform.Scale		= { 0.0f, 0.0f, 0.0f };
	m_ScaleSpeed			= CONSTANT.EXPANSION_SPEED;

	// ランダムの設定.
	const float Rand_DAng	= CRandom::GetRand( -CONSTANT.MOVEVEC_RAND_ROT_MAX,		CONSTANT.MOVEVEC_RAND_ROT_MAX	);
	const float Rand_UpVec	= CRandom::GetRand(  CONSTANT.MOVEVEC_RAND_MOVEY_MIN,	CONSTANT.MOVEVEC_RAND_MOVEY_MAX	);
	const float Rand_Move	= CRandom::GetRand( -CONSTANT.STARTPOS_RAND_MOVE_MAX,	CONSTANT.STARTPOS_RAND_MOVE_MAX	);

	// 座標を動かすベクトルを作成.
	D3DXVECTOR3 PosMoveVec;
	PosMoveVec.x = MoveVec.z;
	PosMoveVec.y = 0.0f;
	PosMoveVec.z = -PosMoveVec.x;

	// 座標の設定.
	m_Transform.Position = Pos;
	m_Transform.Position += PosMoveVec * Rand_Move;

	// 移動ベクトルの設定.
	const float RAng = Math::ToRadian( Rand_DAng );
	m_MoveVec.x = MoveVec.x * cosf( RAng ) - MoveVec.z * sinf( RAng );
	m_MoveVec.y = Rand_UpVec;
	m_MoveVec.z = MoveVec.x * sinf( RAng ) + MoveVec.z * cosf( RAng );
	m_AnimEnd	= false;

	// 追加の移動ベクトルの設定.
	m_AddMoveVec = AddMoveVec;
}
