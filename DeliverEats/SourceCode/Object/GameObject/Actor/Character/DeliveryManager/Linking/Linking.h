#pragma once
#include <vector>
#include "..\..\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\..\..\Utility\Constant\Constant.h"

class CEmployee;
class CCustomer;

namespace {
	// 紐づけを行っていない
	constexpr int NO_LINK_HANDLE = -1;
}

/************************************************
*	お客さんと店員の紐づけクラス.
*		﨑田友輝.
**/
class CLinking final
{
public:
	// 紐づけハンドル.
	using Handle = int;

private:
	// 紐づけ構造体.
	struct stLink {
		stLink()
			: stLink( nullptr, nullptr, INIT_FLOAT3, INIT_FLOAT )
		{}
		stLink( CEmployee* pE, CCustomer* pC, const D3DXCOLOR3& Color, const float& End )
			: pEmployee		( pE )
			, pCustomer		( pC )
			, Color			( Color )
			, EndTimeCnt	( INIT_FLOAT )
			, EndTime		( End )
			, IsStop		( false )
			, IsDeleteHand	( false )
			, IsGoToPass	( false )
		{}

		CEmployee*	pEmployee;		// 店員.
		CCustomer*	pCustomer;		// お客さん.
		D3DXCOLOR3	Color;			// 紐づけの色.
		float		EndTimeCnt;		// 紐づけ解除用のカウント.
		float		EndTime;		// 紐づけ解除時間.
		bool		IsStop;			// 紐づけの解除の停止するか.
		bool		IsDeleteHand;	// 使用していないハンドルか.
		bool		IsGoToPass;		// 渡しに行くキャラか.

		// 紐づけの解除の更新.
		bool EndTimeUpdate( const float& DeltaTime, CLinking::Handle Handle );
		// 紐づけの解除.
		void LinkEnd();
	} typedef SLink;

public:
	CLinking();
	~CLinking();

	// インスタンスの取得.
	static CLinking* GetInstance();

	// 初期化.
	static bool Init();

	// 更新.
	static void Update( const float& DeltaTime );

	// 紐づけされている店員を取得.
	static CEmployee* GetEmployee( const Handle& handle ) { return GetInstance()->m_LinkList[handle].pEmployee; }
	// 紐づけされているお客さんを取得.
	static CCustomer* GetCustomer( const Handle& handle ) { return GetInstance()->m_LinkList[handle].pCustomer; }
	// 紐づけの解除時間カウントの取得.
	static float GetLinkEndTimeCnt( const Handle& handle ) { return GetInstance()->m_LinkList[handle].EndTimeCnt; }
	// 紐づけの解除時間の取得.
	static float GetLinkEndTime( const Handle& handle ) { return GetInstance()->m_LinkList[handle].EndTime; }
	// 紐づけの色の取得.
	static D3DXCOLOR3 GetLinkColor( const Handle& handle ) { return GetInstance()->m_LinkList[handle].Color; }

	// 紐づけの解除の停止.
	static void SetIsLinkEndStop( const Handle& handle, const bool Flag ) { GetInstance()->m_LinkList[handle].IsStop = Flag; }
	// 渡しに行くお客さんか設定.
	static void SetIsGoToPass( const Handle& handle, const bool Flag ) { GetInstance()->m_LinkList[handle].IsGoToPass = Flag; }

	// 紐づけをする.
	static void Link();
	// 紐づけの解除.
	static void LinkEnd( const Handle& handle );
	// 全ての紐づけを解除.
	static void AllLinkEnd();

	// 使用しているハンドルか取得.
	static bool GetIsUseHandle( const Handle& handle );

private:
	// 開始時の紐づけ.
	static void StartLink();
	// 紐づけを行う時間の更新.
	static void LinkTimeUpdate();
	// 紐づけ中の更新.
	static void NowLinkUpdate();
	// 紐づけリストの更新.
	static void LinkListUpdate();

private:
	CDeliveryManager::EmployeeList	m_pEmployeeList;	// 店員リスト.
	CDeliveryManager::CustomerList	m_pCustomerList;	// お客さんリスト.
	std::vector<SLink>				m_LinkList;			// 紐づけリスト.
	std::vector<Handle>				m_DeleteHandleList;	// 解除して使用していないハンドルリスト.
	float							m_LinkTimeCnt;		// 紐づけを行う時間用のカウント.
	float							m_DeltaTime;		// デルタタイム.
	bool							m_IsStartLinkEnd;	// 開始時の紐づけが終了したか.

private:
	// 外部データの定数.
	const ConstantStruct::SLinking& CONSTANT = CConstant::GetLinkingr();
};
