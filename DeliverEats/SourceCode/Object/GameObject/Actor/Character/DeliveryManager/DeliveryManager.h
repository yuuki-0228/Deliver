#pragma once
#include "..\..\..\..\..\Global.h"
#include "..\..\..\GameObjectList.h"

class CEmployee;
class CCustomer;
class CPlayer;

/************************************************
*	お客さんと店員のマネージャークラス.
**/
class CDeliveryManager
{
public:
	using EmployeeList = std::unordered_map<EObjectTag, std::vector<CEmployee*>>;
	using CustomerList = std::unordered_map<EObjectTag, std::vector<CCustomer*>>;

public:
	CDeliveryManager();
	~CDeliveryManager();

	// インスタンスの取得.
	static CDeliveryManager* GetInstance();

	// 初期化.
	static void Init();
	// 全ての初期化が終わってから行う初期化.
	static void EndInit();

	// 当たり判定終了後呼び出される更新.
	static void LateUpdate( const float& DeltaTime );

	// 待機中/紐づけされているリストの更新.
	static void WaitAndLinkListUpdate();

	// 全て店員の数.
	static int GetAllEmployeeSize() { return static_cast<int>( GetInstance()->m_pAllEmployeeList.size() ); }
	// 全てのお客さんの数.
	static int GetAllCustomerSize() { return static_cast<int>( GetInstance()->m_pAllCustomerList.size() ); }

	// 全て店員リストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static EmployeeList GetAllEmployeeList() { return GetInstance()->m_pAllEmployeeList; }
	// 全てお客さんリストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static CustomerList GetAllCustomerList() { return GetInstance()->m_pAllCustomerList; }

	// 待機中の店員リストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static EmployeeList GetWaitEmployeeList() { return GetInstance()->m_pWaitEmployeeList; }
	// 待機中のお客さんリストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static CustomerList GetWaitCustomerList() { return GetInstance()->m_pWaitCustomerList; }

	// 紐づけされている店員リストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static EmployeeList GetLinkEmployeeList() { return GetInstance()->m_pLinkEmployeeList; }
	// 紐づけされているお客さんリストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static CustomerList GetLinkCustomerList() { return GetInstance()->m_pLinkCustomerList; }

	// 近くの紐づけされている店員リストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static EmployeeList GetShortlyEmployeeList() { return GetInstance()->m_pShortlyEmployeeList; }
	// 近くの紐づけされているお客さんリストの取得.
	//	[None] : 全て, [Apartment] : マンションのみ, [Island] : 浮島のみ.
	static CustomerList GetShortlyCustomerList() { return GetInstance()->m_pShortlyCustomerList; }

	// 商品をもらう店員の取得.
	static CEmployee* GetGetFoodEmployee() { return GetInstance()->m_pGetFoodEmployee; }
	// 渡しに行くお客さんの取得.
	static CCustomer* GetGoHandingCustomer() { return GetInstance()->m_pGoHandingCustomer; }

	// 一番近い紐づけされている店員の取得.
	static CEmployee* GetClosestEmployee() { return GetInstance()->m_pClosestEmployee; }
	// 一番近い紐づけされているお客さんの取得.
	static CCustomer* GetClosestCustomer() { return GetInstance()->m_pClosestCustomer; }

	// 近い順の店員リストの取得.
	static std::vector<CEmployee*> GetLinkSortEmployee() { return GetInstance()->m_pLinkSortEmployeeList; }

	// プレイヤーの設定.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

	// 店員の追加.
	static void AddEmployee( CEmployee* pEmployee, const EObjectTag Tag );
	// ピザ窯のSEを再生する店員の追加.
	static void AddKilnEmployee( CEmployee* pEmployee );
	// お客さんの追加.
	static void AddEmployee( CCustomer* pCustomer, const EObjectTag Tag );

private:
	EmployeeList			m_pAllEmployeeList;			// 店員リスト.
	CustomerList			m_pAllCustomerList;			// お客さんリスト.
	EmployeeList			m_pWaitEmployeeList;		// 待機中の店員リスト.
	CustomerList			m_pWaitCustomerList;		// 待機中のお客さんリスト.
	EmployeeList			m_pLinkEmployeeList;		// 紐づけされている店員リスト.
	CustomerList			m_pLinkCustomerList;		// 紐づけされているお客さんリスト.
	EmployeeList			m_pShortlyEmployeeList;		// 近くの紐づけされている店員リスト.
	CustomerList			m_pShortlyCustomerList;		// 近くの紐づけされているお客さんリスト.
	std::vector<CEmployee*>	m_pLinkSortEmployeeList;	// 紐づけされている近い順の店員リスト.
	std::vector<CEmployee*>	m_pKilnEmployeeList;		// ピザ窯のSEを再生する店員リスト.
	CEmployee*				m_pGetFoodEmployee;			// 商品をもらう店員.
	CCustomer*				m_pGoHandingCustomer;		// 渡しに行くお客さん.
	CEmployee*				m_pClosestEmployee;			// 一番近い紐づけされている店員.
	CCustomer*				m_pClosestCustomer;			// 一番近い紐づけされているお客さん.
	CPlayer*				m_pPlayer;					// プレイヤー.
};