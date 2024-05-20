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
// 初期化.
//---------------------------.
bool CRucksackUI::Init()
{
	// 温度ゲージの初期化.
	m_pTemperatureGaugeUI	= std::make_unique<CTemperatureGaugeUI>();
	m_pTemperatureGaugeUI->Init();
	m_RucksackPos			= m_pTemperatureGaugeUI->GetRucksackPos();

	// 傾きゲージの初期化.
	m_pShakingBarUI			= std::make_unique<CShakingBarUI>();
	m_pShakingBarUI->SetPosition( m_RucksackPos );
	m_pShakingBarUI->Init();

	// ピザの初期化.
	m_pPizzaUI				= std::make_unique<CPizzaUI>();
	m_pPizzaUI->SetPosition( m_RucksackPos );
	m_pPizzaUI->Init();

	// 渡しに行くお客さんのアイコンの初期化.
	m_pIconUI				= std::make_unique<CDeliverCustomerIconUI>();
	m_pIconUI->SetPosition( m_RucksackPos );
	m_pIconUI->Init();

	// 最低限もらえるお金の表示の初期化.
	m_pGetMoneyUI			= std::make_unique<CGetMoneyUI>();
	m_pGetMoneyUI->SetPosition( m_RucksackPos );
	m_pGetMoneyUI->Init();

	// リュックの蓋の取得.
	m_pRucksackLid = CSpriteResource::GetSprite( "RucksackLid", &m_RucksackLidState );
	m_RucksackLidState.Transform.Position = m_RucksackPos;
	m_pRucksackLid->SetDither( true );
	return true;
}

//---------------------------.
// 更新.
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
// 描画.
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
// プレイヤーを設定.
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
// 注視のアニメーションを行うか.
//---------------------------.
void CRucksackUI::SetIsAnimPlay( bool Flag )
{
	m_pIconUI->SetIsAnimPlay( Flag );
}

//---------------------------.
// リュックの蓋の更新.
//---------------------------.
void CRucksackUI::RucksackLidUpdate()
{
	// 透明にしていく.
	if ( m_pPlayer->GetIsFoodHave() ) {
		if ( m_RucksackLidState.Color.w <= Color::ALPHA_MIN ) return;
		m_RucksackLidState.Color.w -= m_DeltaTime;
		if ( m_RucksackLidState.Color.w <= Color::ALPHA_MIN ) m_RucksackLidState.Color.w = Color::ALPHA_MIN;
		return;
	}
	
	// 不透明にしていく.
	if ( m_RucksackLidState.Color.w >= Color::ALPHA_MAX ) return;
	m_RucksackLidState.Color.w += m_DeltaTime;
	if ( m_RucksackLidState.Color.w >= Color::ALPHA_MAX ) m_RucksackLidState.Color.w = Color::ALPHA_MAX;
}
