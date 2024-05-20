#include "DeliveryManager.h"
#include "Employee\Employee.h"
#include "Customer\Customer.h"
#include "..\Player\Player.h"
#include "..\..\..\..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\..\..\Time\Time.h"

namespace {
	constexpr char PIZZA_KILN_SE[] = "PizzaKiln"; // ピザ窯SE.
}

CDeliveryManager::CDeliveryManager()
	: m_pAllEmployeeList		()
	, m_pAllCustomerList		()
	, m_pWaitEmployeeList		()
	, m_pWaitCustomerList		()
	, m_pLinkEmployeeList		()
	, m_pLinkCustomerList		()
	, m_pShortlyEmployeeList	()
	, m_pShortlyCustomerList	()
	, m_pLinkSortEmployeeList	()
	, m_pKilnEmployeeList		()
	, m_pGetFoodEmployee		( nullptr )
	, m_pGoHandingCustomer		( nullptr )
	, m_pClosestEmployee		( nullptr )
	, m_pClosestCustomer		( nullptr )
	, m_pPlayer					( nullptr )
{
}

CDeliveryManager::~CDeliveryManager()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CDeliveryManager* CDeliveryManager::GetInstance()
{
	static std::unique_ptr<CDeliveryManager> pInstance = std::make_unique<CDeliveryManager>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
void CDeliveryManager::Init()
{
	CDeliveryManager* pI = GetInstance();

	pI->m_pAllEmployeeList.clear();
	pI->m_pAllCustomerList.clear();
	pI->m_pWaitCustomerList.clear();
	pI->m_pLinkSortEmployeeList.clear();
	pI->m_pLinkEmployeeList.clear();
	pI->m_pLinkCustomerList.clear();
	pI->m_pShortlyEmployeeList.clear();
	pI->m_pShortlyCustomerList.clear();
	pI->m_pWaitEmployeeList.clear();
	pI->m_pKilnEmployeeList.clear();
	pI->m_pGetFoodEmployee		= nullptr;
	pI->m_pGoHandingCustomer	= nullptr;
	pI->m_pClosestEmployee		= nullptr;
	pI->m_pClosestCustomer		= nullptr;
	pI->m_pPlayer				= nullptr;
}

//---------------------------.
// 全ての初期化が終わってから行う初期化.
//---------------------------.
void CDeliveryManager::EndInit()
{
	CDeliveryManager* pI = GetInstance();

	// サイズの設定.
	const int EmployeeSize = static_cast<int>( pI->m_pAllEmployeeList.size() );
	const int CustomerSize = static_cast<int>( pI->m_pAllCustomerList.size() );
	pI->m_pLinkEmployeeList.reserve(		EmployeeSize );
	pI->m_pLinkCustomerList.reserve(		CustomerSize );
	pI->m_pWaitEmployeeList.reserve(		EmployeeSize );
	pI->m_pWaitCustomerList.reserve(		CustomerSize );
	pI->m_pShortlyEmployeeList.reserve(		EmployeeSize );
	pI->m_pShortlyCustomerList.reserve(		CustomerSize );
	pI->m_pLinkSortEmployeeList.reserve(	EmployeeSize );
	pI->m_pKilnEmployeeList.reserve(		EmployeeSize );
}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CDeliveryManager::LateUpdate( const float& DeltaTime )
{
	CDeliveryManager* pI = GetInstance();

	// 店員のリストの更新.
	WaitAndLinkListUpdate();
	
	// 近くの紐づけされている店員の更新.
	pI->m_pShortlyEmployeeList.clear();
	for ( auto& [Tag, List] : pI->m_pLinkEmployeeList ) {
		for ( auto& e : List ) {
			if ( e->GetIsShortly() == false ) continue;
			pI->m_pShortlyEmployeeList[Tag].emplace_back( e );
		}
	}
	// 近くの紐づけされているお客さんの更新.
	pI->m_pShortlyCustomerList.clear();
	for ( auto& [Tag, List] : pI->m_pLinkCustomerList ) {
		for ( auto& c : List ) {
			if ( c->GetIsShortly() == false ) continue;
			pI->m_pShortlyCustomerList[Tag].emplace_back( c );
		}
	}

	// 商品をもらう店員の更新.
	pI->m_pGetFoodEmployee = nullptr;
	for ( auto& [Tag, List] : pI->m_pLinkEmployeeList ) {
		for ( auto& e : List ) {
			if ( e->GetIsGetFoodEmployee() == false ) continue;
			pI->m_pGetFoodEmployee = e;
			break;
		}
	}
	// 渡しに行くお客さんの更新.
	pI->m_pGoHandingCustomer = nullptr;
	for ( auto& [Tag, List] : pI->m_pLinkCustomerList ) {
		for ( auto& c : List ) {
			if ( c->GetIsGoHandingCustomer() == false ) continue;
			pI->m_pGoHandingCustomer = c;
			break;
		}
	}

	// プレイヤーの座標の取得.
	const D3DXPOSITION3 PlayerPos = pI->m_pPlayer->GetPosition();

	// 一番近い紐づけされている店員の更新.
	pI->m_pClosestEmployee = nullptr;
	std::pair<CEmployee*, float> ClosestEmployee;
	if ( pI->m_pLinkEmployeeList.empty() == false ) {
		// 一番初めの店員を一番近い店員として入れておく.
		D3DXVECTOR3 FirstVector		= PlayerPos - pI->m_pLinkEmployeeList[EObjectTag::None][0]->GetPosition();
		float		FirstDistance	= D3DXVec3Length( &FirstVector );
		ClosestEmployee				= std::make_pair( pI->m_pLinkEmployeeList[EObjectTag::None][0], FirstDistance );

		// 一番近い店員を調べていく.
		for ( auto& e : pI->m_pLinkEmployeeList[EObjectTag::None] ) {
			D3DXVECTOR3 Vector		= PlayerPos - e->GetPosition();
			float		Distance	= D3DXVec3Length( &Vector );
			if ( Distance	>= ClosestEmployee.second ) continue;
			ClosestEmployee  = std::make_pair( e, Distance );
		}

		// 一番近い店員の更新.
		pI->m_pClosestEmployee = ClosestEmployee.first;
	}
	// 一番近い紐づけされているお客さんの更新.
	pI->m_pClosestCustomer = nullptr;
	std::pair<CCustomer*, float> ClosestCustomer;
	if ( pI->m_pLinkCustomerList.empty() == false ) {
		// 一番初めの店員を一番近い店員として入れておく.
		D3DXVECTOR3 FirstVector		= PlayerPos - pI->m_pLinkCustomerList[EObjectTag::None][0]->GetPosition();
		float		FirstDistance	= D3DXVec3Length( &FirstVector );
		ClosestCustomer				= std::make_pair( pI->m_pLinkCustomerList[EObjectTag::None][0], FirstDistance );

		// 一番近い店員を調べていく.
		for ( auto& c : pI->m_pLinkCustomerList[EObjectTag::None] ) {
			D3DXVECTOR3 Vector		= PlayerPos - c->GetPosition();
			float		Distance	= D3DXVec3Length( &Vector );
			if ( Distance	>= ClosestCustomer.second ) continue;
			ClosestCustomer  = std::make_pair( c, Distance );
		}

		// 一番近い店員の更新.
		pI->m_pClosestCustomer = ClosestCustomer.first;
	}

	// ピザ窯SEを再生するか.
	if ( pI->m_pKilnEmployeeList.empty() ) {
		// ピザ窯SEを停止.
		CSoundManager::StopSE( PIZZA_KILN_SE );
		return;
	}

	// 一番近いピザ窯SEを再生する店員を探す.
	CEmployee*	pPlayEmployee	= nullptr;
	float		PlayDistance	= 10000.0f;
	for ( auto& e : pI->m_pKilnEmployeeList ) {
		const D3DXVECTOR3&	Vector		= PlayerPos - e->GetPosition();
		const float			Distance	= D3DXVec3Length( &Vector );
		
		// 再生する店員を変更.
		if ( Distance >= PlayDistance ) continue;
		PlayDistance	= Distance;
		pPlayEmployee	= e;
	}

	// ピザ窯SEの再生.
	CSoundManager::PlaySELoop3D( PIZZA_KILN_SE, pPlayEmployee->GetPosition(), false );
	pI->m_pKilnEmployeeList.clear();
}

//---------------------------.
// 待機中リストの更新.
//---------------------------.
void CDeliveryManager::WaitAndLinkListUpdate()
{
	CDeliveryManager* pI = GetInstance();

	// 店員のリストの更新.
	pI->m_pLinkEmployeeList.clear();
	pI->m_pWaitEmployeeList.clear();
	for ( auto& [Tag, List] : pI->m_pAllEmployeeList ) {
		for ( auto& e : List ) {
			if ( e->GetIsLink() || e->GetIsWait() )	pI->m_pLinkEmployeeList[Tag].emplace_back( e );
			else									pI->m_pWaitEmployeeList[Tag].emplace_back( e );
		}
	}

	// お客さんのリストの更新.
	pI->m_pLinkCustomerList.clear();
	pI->m_pWaitCustomerList.clear();
	for ( auto& [Tag, List] : pI->m_pAllCustomerList ) {
		for ( auto& c : List ) {
			if ( c->GetIsLink() || c->GetIsWait() )	pI->m_pLinkCustomerList[Tag].emplace_back( c );
			else									pI->m_pWaitCustomerList[Tag].emplace_back( c );
		}
	}

	// 店員を近い順に並び替える.
	if ( pI->m_pLinkEmployeeList.empty() ) return;
	const D3DXPOSITION3& PlayerPos = pI->m_pPlayer->GetPosition();
	std::vector<std::pair<float, CEmployee*>> SortList;
	for ( auto& e : pI->m_pLinkEmployeeList[EObjectTag::None] ) {
		if ( e->GetHandle() == NO_LINK_HANDLE ) continue;
		D3DXVECTOR3 Vector		= PlayerPos - e->GetPosition();
		float		Distance	= D3DXVec3Length( &Vector );
		SortList.emplace_back( std::make_pair( Distance, e ) );
	}
	std::sort( SortList.begin(), SortList.end() );

	// 近い順の店員リストの更新.
	pI->m_pLinkSortEmployeeList.clear();
	for ( auto& [Distance, pEmployee] : SortList ) {
		pI->m_pLinkSortEmployeeList.emplace_back( pEmployee );
	}
}

//---------------------------.
// 店員の追加.
//---------------------------.
void CDeliveryManager::AddEmployee( CEmployee* pEmployee, const EObjectTag Tag )
{
	CDeliveryManager* pI = GetInstance();

	pI->m_pAllEmployeeList[EObjectTag::None].emplace_back( pEmployee );

	// マンションか浮島の場合追加する.
	if ( Tag != EObjectTag::Apartment && Tag != EObjectTag::Island ) return;
	pI->m_pAllEmployeeList[Tag].emplace_back( pEmployee );
}

//---------------------------.
// ピザ窯のSEを再生する店員の追加.
//---------------------------.
void CDeliveryManager::AddKilnEmployee( CEmployee* pEmployee )
{
	CDeliveryManager* pI = GetInstance();

	pI->m_pKilnEmployeeList.emplace_back( pEmployee );
}

//---------------------------.
// お客さんの追加.
//---------------------------.
void CDeliveryManager::AddEmployee( CCustomer* pCustomer, const EObjectTag Tag )
{
	CDeliveryManager* pI = GetInstance();

	pI->m_pAllCustomerList[EObjectTag::None].emplace_back( pCustomer );

	// マンションか浮島の場合追加する.
	if ( Tag != EObjectTag::Apartment && Tag != EObjectTag::Island ) return;
	pI->m_pAllCustomerList[Tag].emplace_back( pCustomer );
}
