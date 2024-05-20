#include "RucksackUI.h"
#include "TemperatureGaugeUI\TemperatureGaugeUI.h"
#include "ShakingBarUI\ShakingBarUI.h"
#include "PizzaUI\PizzaUI.h"
#include "DeliverCustomerIconUI\DeliverCustomerIconUI.h"
#include "GetMoneyUI\GetMoneyUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"

CRucksackUI::CRucksackUI()
	: m_pPlayer				( nullptr )
	, m_pRucksackLid		( nullptr )
	, m_RucksackLidState	()
	, m_pTemperatureGaugeUI	( nullptr )
	, m_pShakingBarUI		( nullptr )
	, m_pPizzaUI			( nullptr )
	, m_pIconUI				( nullptr )
	, m_pGetMoneyUI			( nullptr )
	, m_RucksackPos			( INIT_FLOAT3 )
{
}

CRucksackUI::~CRucksackUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CRucksackUI::Init()
{
	// ���x�Q�[�W�̏�����.
	m_pTemperatureGaugeUI	= std::make_unique<CTemperatureGaugeUI>();
	m_pTemperatureGaugeUI->Init();
	m_RucksackPos			= m_pTemperatureGaugeUI->GetRucksackPos();

	// �X���Q�[�W�̏�����.
	m_pShakingBarUI			= std::make_unique<CShakingBarUI>();
	m_pShakingBarUI->SetPosition( m_RucksackPos );
	m_pShakingBarUI->Init();

	// �s�U�̏�����.
	m_pPizzaUI				= std::make_unique<CPizzaUI>();
	m_pPizzaUI->SetPosition( m_RucksackPos );
	m_pPizzaUI->Init();

	// �n���ɍs�����q����̃A�C�R���̏�����.
	m_pIconUI				= std::make_unique<CDeliverCustomerIconUI>();
	m_pIconUI->SetPosition( m_RucksackPos );
	m_pIconUI->Init();

	// �Œ�����炦�邨���̕\���̏�����.
	m_pGetMoneyUI			= std::make_unique<CGetMoneyUI>();
	m_pGetMoneyUI->SetPosition( m_RucksackPos );
	m_pGetMoneyUI->Init();

	// �����b�N�̊W�̎擾.
	m_pRucksackLid = CSpriteResource::GetSprite( "RucksackLid", &m_RucksackLidState );
	m_RucksackLidState.Transform.Position = m_RucksackPos;
	m_pRucksackLid->SetDither( true );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CRucksackUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	m_pTemperatureGaugeUI->Update( DeltaTime );
	m_pShakingBarUI->Update( DeltaTime );
	m_pPizzaUI->Update( DeltaTime );
	m_pIconUI->Update( DeltaTime );
	m_pGetMoneyUI->Update( DeltaTime );
	RucksackLidUpdate();
}

//---------------------------.
// �`��.
//---------------------------.
void CRucksackUI::Render()
{
	m_pTemperatureGaugeUI->Render();
	m_pShakingBarUI->Render();
	m_pPizzaUI->Render();
	m_pIconUI->Render();
	m_pGetMoneyUI->Render();
	m_pRucksackLid->RenderUI( &m_RucksackLidState );
}

//---------------------------.
// �v���C���[��ݒ�.
//---------------------------.
void CRucksackUI::SetPlayer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
	m_pTemperatureGaugeUI->SetPlayer( pPlayer );
	m_pShakingBarUI->SetPlayer( pPlayer );
	m_pPizzaUI->SetPlayer( pPlayer );
	m_pIconUI->SetPlayer( pPlayer );
	m_pGetMoneyUI->SetPlayer( pPlayer );
}

//---------------------------.
// �����̃A�j���[�V�������s����.
//---------------------------.
void CRucksackUI::SetIsAnimPlay( bool Flag )
{
	m_pIconUI->SetIsAnimPlay( Flag );
}

//---------------------------.
// �����b�N�̊W�̍X�V.
//---------------------------.
void CRucksackUI::RucksackLidUpdate()
{
	// �����ɂ��Ă���.
	if ( m_pPlayer->GetIsFoodHave() ) {
		if ( m_RucksackLidState.Color.w <= Color::ALPHA_MIN ) return;
		m_RucksackLidState.Color.w -= m_DeltaTime;
		if ( m_RucksackLidState.Color.w <= Color::ALPHA_MIN ) m_RucksackLidState.Color.w = Color::ALPHA_MIN;
		return;
	}
	
	// �s�����ɂ��Ă���.
	if ( m_RucksackLidState.Color.w >= Color::ALPHA_MAX ) return;
	m_RucksackLidState.Color.w += m_DeltaTime;
	if ( m_RucksackLidState.Color.w >= Color::ALPHA_MAX ) m_RucksackLidState.Color.w = Color::ALPHA_MAX;
}
