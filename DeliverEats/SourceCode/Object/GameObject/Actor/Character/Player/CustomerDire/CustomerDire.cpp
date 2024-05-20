#include "CustomerDire.h"
#include "..\Player.h"
#include "..\..\DeliveryManager\DeliveryManager.h"
#include "..\..\DeliveryManager\Customer\Customer.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Utility\Math\Math.h"

namespace {
	constexpr int DIRE_NUM	= 2;	// �����̕\�����鐔.
	constexpr int ANIM_NO	= 1;	// �A�j���[�V�������s���ꏊ.
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
// ������.
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
// �X�V.
//---------------------------.
void CCustomerDire::Update( const float& DeltaTime )
{
	const bool IsEndDire = CTime::GetIsGameEnd() && m_pPlayer->GetIsFoodHave() == false;
	m_pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( m_pGoHandingCustomer == nullptr && IsEndDire == false ) return;

	// ��󂪌����ꏊ��ۑ�.
	D3DXPOSITION3 CPos = INIT_FLOAT3;
	if ( IsEndDire )	CPos = m_pPlayer->GetStartPosition();
	else				CPos = m_pGoHandingCustomer->GetPosition() + CONSTANT.CUSTOMER_ADD_POS;

	// �x�N�g���̍쐬.
	const D3DXPOSITION3& PPos	= m_pPlayer->GetPosition();
	D3DXVECTOR3 Vector			= CPos - PPos;
	const float Distance		= D3DXVec3Length( &Vector );
	D3DXVec3Normalize( &Vector, &Vector );

	// �ꏊ�̍X�V.
	for ( auto& t : m_TransformList ) t.Position = PPos + Vector * CONSTANT.DISTANCE;

	// �ړ��A�j���[�V����.
	m_AnimCnt += CONSTANT.MOVE_ANIM_SPEED * DeltaTime;
	if ( m_AnimCnt >= Math::DEGREE_MAX ) m_AnimCnt -= Math::DEGREE_MAX;
	m_TransformList[ANIM_NO].Position += Vector * std::abs( sinf( Math::ToRadian( m_AnimCnt ) ) );

	// �����̍X�V.
	m_mCustomerDireRot = Math::GetLookRotationMatrix( PPos, CPos );

	// �n���ɍs�����q����ɋ߂Â����Ƃɓ����ɂ��Ă���.
	m_DecAlpha = Color::ALPHA_MIN;
	if ( Distance < CONSTANT.ALPHA_START_DISTANCE ) {
		const float OverDistance = CONSTANT.ALPHA_START_DISTANCE - Distance;
		m_DecAlpha = OverDistance / CONSTANT.ALPHA_DEC_DISTANCE / 10.0f;
		if ( m_DecAlpha > Color::ALPHA_MAX ) m_DecAlpha = Color::ALPHA_MAX;
	}
}

//---------------------------.
// �`��.
//---------------------------.
void CCustomerDire::Render()
{
	const bool IsEndDire = CTime::GetIsGameEnd() && m_pPlayer->GetIsFoodHave() == false;
	m_pGoHandingCustomer = CDeliveryManager::GetGoHandingCustomer();
	if ( m_pGoHandingCustomer == nullptr && IsEndDire == false ) return;

	// �e�̕`��.
	CDirectX11::SetDepth( false );
	for ( int i = 0; i < DIRE_NUM; ++i ) {
		m_pCustomerDire->SetColor( Color::Black );
		m_pCustomerDire->SetAlpha( Color::AlphaClamp( 0.2f - m_DecAlpha ) );
		m_pCustomerDire->Render( &m_TransformList[i], &m_mCustomerDireRot );
	}
	CDirectX11::SetDepth( true );

	// ���̕`��.
	for ( int i = 0; i < DIRE_NUM; ++i ) {
		if ( IsEndDire )	m_pCustomerDire->SetColor( Color::Dodgerblue );
		else				m_pCustomerDire->SetColor( m_pGoHandingCustomer->GetLinkColor() );
		m_pCustomerDire->SetAlpha( Color::AlphaClamp( CONSTANT.ALPHA_LIST[i] - m_DecAlpha ) );
		m_pCustomerDire->Render( &m_TransformList[i], &m_mCustomerDireRot );
	}
}
