#include "Linking.h"
#include "..\Customer\Customer.h"
#include "..\Employee\Employee.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"
#include "..\..\..\..\..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include <functional>
#include <algorithm>
#include <memory>

// 紐づけのメッセージを紐づけの色と同じにするか.
//#define LINK_MESSAGE_COLOR;

CLinking::CLinking()
	: m_pEmployeeList		()
	, m_pCustomerList		()
	, m_LinkList			()
	, m_DeleteHandleList	()
	, m_LinkTimeCnt			( INIT_FLOAT )
	, m_DeltaTime			( INIT_FLOAT )
	, m_IsStartLinkEnd		( false )
{
}

CLinking::~CLinking()
{
}

//----------------------------.
// インスタンスの取得.
//----------------------------.
CLinking* CLinking::GetInstance()
{
	static std::unique_ptr<CLinking> pInstance = std::make_unique<CLinking>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
bool CLinking::Init()
{
	CLinking* pI = GetInstance();

	pI->m_pEmployeeList.clear();
	pI->m_pCustomerList.clear();
	pI->m_LinkList.clear();
	pI->m_DeleteHandleList.clear();
	pI->m_LinkTimeCnt		= 0.0f;
	pI->m_DeltaTime			= 0.0f;
	pI->m_IsStartLinkEnd	= false;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CLinking::Update( const float& DeltaTime )
{
	CLinking* pI = GetInstance();

	pI->m_DeltaTime = DeltaTime;

	// リストの更新.
	CDeliveryManager::WaitAndLinkListUpdate();
	pI->m_pEmployeeList = CDeliveryManager::GetWaitEmployeeList();
	pI->m_pCustomerList = CDeliveryManager::GetWaitCustomerList();

	StartLink();		// 開始時の紐づけ.
	LinkTimeUpdate();	// 紐づけを行う時間の更新.
	NowLinkUpdate();	// 紐づけ中の更新.
	LinkListUpdate();	// 紐づけリストの更新.
}

//---------------------------.
// 紐づけをする.
//---------------------------.
void CLinking::Link()
{
	CLinking* pI = GetInstance();

	// リストの更新.
	CDeliveryManager::WaitAndLinkListUpdate();
	pI->m_pEmployeeList = CDeliveryManager::GetWaitEmployeeList();
	pI->m_pCustomerList = CDeliveryManager::GetWaitCustomerList();

	// 店員/お客さんの数の確認.
	const int E_Max = static_cast<int>( pI->m_pEmployeeList[EObjectTag::None].size() );
	const int C_Max = static_cast<int>( pI->m_pCustomerList[EObjectTag::None].size() );
	if ( E_Max <= 0 || C_Max <= 0 ) return;

	// 店員/お客さんのタイプごとの数の取得.
	const int E_A_Size = static_cast<int>( pI->m_pEmployeeList[EObjectTag::Apartment].size()	);
	const int E_I_Size = static_cast<int>( pI->m_pEmployeeList[EObjectTag::Island].size()		);
	const int C_A_Size = static_cast<int>( pI->m_pCustomerList[EObjectTag::Apartment].size()	);
	const int C_I_Size = static_cast<int>( pI->m_pCustomerList[EObjectTag::Island].size()		);

	// 店員の紐づける種類をランダムで選ぶ.
	EObjectTag E_Tag = EObjectTag::None;
	if ( E_A_Size > 0 && E_I_Size > 0 ) {
		if ( CRandom::Probability( 1, 2 ) )	E_Tag = EObjectTag::Apartment;
		else								E_Tag = EObjectTag::Island;
	}
	else {
		if ( E_A_Size > 0 )	E_Tag = EObjectTag::Apartment;
		else				E_Tag = EObjectTag::Island;
	}

	// お客さんの紐づける種類をランダムで選ぶ.
	EObjectTag C_Tag = EObjectTag::None;
	if ( C_A_Size > 0 && C_I_Size > 0 ) {
		if ( CRandom::Probability( 1, 2 ) )	C_Tag = EObjectTag::Apartment;
		else								C_Tag = EObjectTag::Island;
	}
	else {
		if ( C_A_Size > 0 )	C_Tag = EObjectTag::Apartment;
		else				C_Tag = EObjectTag::Island;
	}

	// 紐づけする店員/お客さんをランダムで選ぶ.
	const int E_Size	= static_cast<int>( pI->m_pEmployeeList[E_Tag].size() );
	const int C_Size	= static_cast<int>( pI->m_pCustomerList[C_Tag].size() );
	const int E_No		= CRandom::GetRand( 0, E_Size - 1 );
	const int C_No		= CRandom::GetRand( 0, C_Size - 1 );

	// 削除したハンドルを再利用するか.
	Handle handle = static_cast<Handle>( pI->m_LinkList.size() );
	if ( pI->m_DeleteHandleList.size() != 0 ) {
		handle = pI->m_DeleteHandleList.back();
		pI->m_DeleteHandleList.pop_back();
	}
	else pI->m_LinkList.emplace_back();

	// 紐づけを行う.
	pI->m_LinkList[handle] = SLink(
		pI->m_pEmployeeList[E_Tag][E_No],
		pI->m_pCustomerList[C_Tag][C_No],
		pI->CONSTANT.COLOR_LIST[handle],
		pI->CONSTANT.LINK_END_TIME );

	// ハンドルを設定.
	pI->m_pCustomerList[C_Tag][C_No]->AttachLink( handle );
	pI->m_pEmployeeList[E_Tag][E_No]->AttachLink( handle );

	CMessageWindow::PushMessage( u8"お客さんとお店を紐づけました。"
#ifdef LINK_MESSAGE_COLOR
		, pI->CONSTANT.COLOR_LIST[handle]
#endif	// #ifdef LINK_MESSAGE_COLOR.
	);
}

//---------------------------.
// 紐づけの解除.
//---------------------------.
void CLinking::LinkEnd( const Handle& handle )
{
	CLinking* pI = GetInstance();

	// 解除したハンドルを保存.
	pI->m_DeleteHandleList.emplace_back( handle );

	// 紐づけの解除.
	pI->m_LinkList[handle].LinkEnd();
}

//---------------------------.
// 全ての紐づけを解除.
//---------------------------.
void CLinking::AllLinkEnd()
{
	CLinking* pI = GetInstance();

	for ( auto& l : pI->m_LinkList ) {
		if ( l.IsGoToPass ) continue;

		// 紐づけの解除.
		l.LinkEnd();
	}
}

//---------------------------.
// 使用しているハンドルか取得.
//---------------------------.
bool CLinking::GetIsUseHandle( const Handle& handle )
{
	CLinking* pI = GetInstance();

	if ( handle >= pI->m_LinkList.size() ) return false;
	for ( auto& d : pI->m_DeleteHandleList ) {
		if ( handle == d ) return false;
	}
	return true;
}

//---------------------------.
// 開始時の紐づけ.
//---------------------------.
void CLinking::StartLink()
{
	CLinking* pI = GetInstance();

	if ( pI->m_IsStartLinkEnd ) return;

	// 開始時の紐づけを行えるか確認.
	const int E_Size = static_cast<int>( pI->m_pEmployeeList[EObjectTag::None].size() );
	const int C_Size = static_cast<int>( pI->m_pCustomerList[EObjectTag::None].size() );
	if ( E_Size < pI->CONSTANT.START_LINK_NUM || C_Size < pI->CONSTANT.START_LINK_NUM ) return;

	// 紐づけを行う.
	for ( int i = 0; i < pI->CONSTANT.START_LINK_NUM; ++i ) Link();
	pI->m_IsStartLinkEnd = true;
}

//---------------------------.
// 紐づけを行う時間の更新.
//---------------------------.
void CLinking::LinkTimeUpdate()
{
	CLinking* pI = GetInstance();

	if ( CTime::GetIsGameEnd()			) return;
	if ( pI->m_pEmployeeList.empty()	) return;
	if ( pI->m_pCustomerList.empty()	) return;

	// 紐づけを行う時間のカウントの加算.
	pI->m_LinkTimeCnt += pI->m_DeltaTime;

	// 紐づけを行う時間の設定.
	const float EndTime = CTime::GetIsGoldenTime() ?
		pI->CONSTANT.GOLDEN_TIME_LINK_TIME :
		pI->CONSTANT.NORMAL_LINK_TIME;

	// 紐づけを行う.
	if ( pI->m_LinkTimeCnt >= EndTime ) {
		pI->m_LinkTimeCnt = 0.0f;
		Link();
	}
}

//---------------------------.
// 紐づけ中の更新.
//---------------------------.
void CLinking::NowLinkUpdate()
{
	CLinking* pI = GetInstance();

	const int Size = static_cast<int>( pI->m_LinkList.size() );
	for ( int i = 0; i < Size; ++i ) {
		if ( pI->m_LinkList[i].IsDeleteHand ) continue;

		// 紐づけの解除のカウントの更新.
		if ( pI->m_LinkList[i].EndTimeUpdate( pI->m_DeltaTime, i ) ) {

			// 紐づけが解除されたためハンドルを保存する.
			pI->m_DeleteHandleList.emplace_back( i );
		}
	}
}

//---------------------------.
// 紐づけリストの更新.
//---------------------------.
void CLinking::LinkListUpdate()
{
	CLinking* pI = GetInstance();

	// 解除して使用していないハンドルリストを降順に並び変える.
	std::sort( pI->m_DeleteHandleList.begin(), pI->m_DeleteHandleList.end(), std::greater<int>() );

	// 使用していないハンドルの削除.
	const int Size = static_cast<int>( pI->m_LinkList.size() );
	for ( int i = Size - 1; i >= 0; --i ) {
		if ( pI->m_LinkList[i].IsDeleteHand == false ) break;
		pI->m_LinkList.pop_back();

		// 解除して使用していないハンドルの削除.
		if ( pI->m_DeleteHandleList.empty() ) continue;
		if ( pI->m_DeleteHandleList[0] != i ) continue;
		pI->m_DeleteHandleList.erase( pI->m_DeleteHandleList.begin() );
	}
}

//---------------------------.
// 紐づけの解除の更新.
//---------------------------.
bool CLinking::stLink::EndTimeUpdate( const float& DeltaTime, CLinking::Handle Handle )
{
	if ( IsStop ) return false;

	// カウントの追加.
	EndTimeCnt += DeltaTime;

	// 紐づけを解除する.
	if ( EndTimeCnt >= EndTime ) {
		LinkEnd();
		CMessageWindow::PushMessage( u8"紐づけを解除しました。"
#ifdef LINK_MESSAGE_COLOR
			, CConstant::GetLinkingr().COLOR_LIST[Handle]
#endif	// #ifdef LINK_MESSAGE_COLOR.
		);
		return true;
	}
	return false;
}

//---------------------------.
// 紐づけの解除.
//---------------------------.
void CLinking::stLink::LinkEnd()
{
	// アニメーションをもとに戻す.
	pEmployee->ChangeNormal();
	pCustomer->ChangeNormal();

	// 紐づけを解除.
	pEmployee->DetachLink();
	pCustomer->DetachLink();
	IsDeleteHand = true;
}