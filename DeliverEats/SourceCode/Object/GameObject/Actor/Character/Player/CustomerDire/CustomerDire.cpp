#include "CustomerDire.h"
#include "..\Player.h"
#include "..\..\DeliveryManager\DeliveryManager.h"
#include "..\..\DeliveryManager\Customer\Customer.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr int DIRE_NUM	= 2;	// 方向の表示する数.
	constexpr int ANIM_NO	= 1;	// アニメーションを行う場所.
}

CCustomerDire::CCustomerDire()
	: m_pCustomerDire		( nullptr )
	, m_pPlayer				( nullptr )
	, m_pGoHandingCustomer	( nullptr )
	, m_mCustomerDireRot	()
	, m_TransformList		()
	, m_AnimCnt				( INIT_FLOAT )
	, m_DecAlpha			( Color::ALPHA_MIN )
{
}

CCustomerDire::~CCustomerDire()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CCustomerDire::Init()
{
	m_pCustomerDire = CMeshResource::GetCollision( "Dire_c" );
	m_pCustomerDire->SetDither( true );
	m_TransformList.resize( DIRE_NUM );
	for ( int i = 0; i < DIRE_NUM; ++i ) {
		m_TransformList[i].Scale = CONSTANT.SIZE_LIST[i];
	}
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CCustomerDire::Update( const float& DeltaTime )
{
	const bool IsEndDire = CTime::GetIsGameEnd() && m_pPlayer->GetIsFoodHave() == false;
	m_pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( m_pGoHandingCustomer == nullptr && IsEndDire == false ) return;

	// 矢印が向く場所を保存.
	D3DXPOSITION3 CPos = INIT_FLOAT3;
	if ( IsEndDire )	CPos = m_pPlayer->GetStartPosition();
	else				CPos = m_pGoHandingCustomer->GetPosition() + CONSTANT.CUSTOMER_ADD_POS;

	// ベクトルの作成.
	const D3DXPOSITION3& PPos	= m_pPlayer->GetPosition();
	D3DXVECTOR3 Vector			= CPos - PPos;
	const float Distance		= D3DXVec3Length( &Vector );
	D3DXVec3Normalize( &Vector, &Vector );

	// 場所の更新.
	for ( auto& t : m_TransformList ) t.Position = PPos + Vector * CONSTANT.DISTANCE;

	// 移動アニメーション.
	m_AnimCnt += CONSTANT.MOVE_ANIM_SPEED * DeltaTime;
	if ( m_AnimCnt >= Math::DEGREE_MAX ) m_AnimCnt -= Math::DEGREE_MAX;
	m_TransformList[ANIM_NO].Position += Vector * std::abs( sinf( Math::ToRadian( m_AnimCnt ) ) );

	// 向きの更新.
	m_mCustomerDireRot = Math::GetLookRotationMatrix( PPos, CPos );

	// 渡しに行くお客さんに近づくごとに透明にしていく.
	m_DecAlpha = Color::ALPHA_MIN;
	if ( Distance < CONSTANT.ALPHA_START_DISTANCE ) {
		const float OverDistance = CONSTANT.ALPHA_START_DISTANCE - Distance;
		m_DecAlpha = OverDistance / CONSTANT.ALPHA_DEC_DISTANCE / 10.0f;
		if ( m_DecAlpha > Color::ALPHA_MAX ) m_DecAlpha = Color::ALPHA_MAX;
	}
}

//---------------------------.
// 描画.
//---------------------------.
void CCustomerDire::Render()
{
	const bool IsEndDire = CTime::GetIsGameEnd() && m_pPlayer->GetIsFoodHave() == false;
	m_pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( m_pGoHandingCustomer == nullptr && IsEndDire == false ) return;

	// 影の描画.
	CDirectX11::SetDepth( false );
	for ( int i = 0; i < DIRE_NUM; ++i ) {
		m_pCustomerDire->SetColor( Color::Black );
		m_pCustomerDire->SetAlpha( Color::AlphaClamp( 0.2f - m_DecAlpha ) );
		m_pCustomerDire->Render( &m_TransformList[i], &m_mCustomerDireRot );
	}
	CDirectX11::SetDepth( true );

	// 矢印の描画.
	for ( int i = 0; i < DIRE_NUM; ++i ) {
		if ( IsEndDire )	m_pCustomerDire->SetColor( Color::Dodgerblue );
		else				m_pCustomerDire->SetColor( m_pGoHandingCustomer->GetLinkColor() );
		m_pCustomerDire->SetAlpha( Color::AlphaClamp( CONSTANT.ALPHA_LIST[i] - m_DecAlpha ) );
		m_pCustomerDire->Render( &m_TransformList[i], &m_mCustomerDireRot );
	}
}
