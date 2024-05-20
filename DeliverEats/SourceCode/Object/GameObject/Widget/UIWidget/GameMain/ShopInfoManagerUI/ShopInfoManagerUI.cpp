#include "ShopInfoManagerUI.h"
#include "ShopInfoUI\ShopInfoUI.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\..\..\Time\Time.h"

namespace {
	constexpr int SHOP_INFO_NUM_MAX = 3;	// お店の詳細情報の数の上限.
	constexpr int SHOP_INFO_LEFT	= 0;	// 左側のお店の詳細情報.
	constexpr int SHOP_INFO_CENTER	= 1;	// 真ん中のお店の詳細情報.
	constexpr int SHOP_INFO_RIGHT	= 2;	// 右側のお店の詳細情報.
}

CShopInfoManagerUI::CShopInfoManagerUI()
	: m_pPlayer				( nullptr )
	, m_pShopInfoList		( SHOP_INFO_NUM_MAX )
	, m_pDispEmployeeList	( SHOP_INFO_NUM_MAX )
	, m_DispNum				( INIT_INT )
	, m_CoolTime			( INIT_FLOAT )
{
}

CShopInfoManagerUI::~CShopInfoManagerUI()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CShopInfoManagerUI::Init()
{
	// 詳細情報の初期化.
	for ( auto& i : m_pShopInfoList ) {
		i = std::make_unique<CShopInfoUI>();
		i->Init();
	}

	// 店員の初期化.
	for ( auto& e : m_pDispEmployeeList ) e = nullptr;
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CShopInfoManagerUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	OpenUpdate();		// 表示の更新.
	CloseUpdate();		// 非表示の更新.
	ChangeUpdate();		// 変更の更新.
	CoolTimeUpdate();	// クールタイムの更新.

	for ( auto& i : m_pShopInfoList ) i->Update( DeltaTime );
}

//---------------------------.
// 描画.
//---------------------------.
void CShopInfoManagerUI::Render()
{
	const int Size = static_cast<int>(m_pShopInfoList.size());
	for ( int i = Size - 1; i >= 0; --i ) {
		m_pShopInfoList[i]->Render();
	}
}

//---------------------------.
// プレイヤーの設定.
//---------------------------.
void CShopInfoManagerUI::SetPlayer( CPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
	for ( auto& i : m_pShopInfoList ) i->SetPlayer( pPlayer );
}

//---------------------------.
// 表示の更新.
//---------------------------.
void CShopInfoManagerUI::OpenUpdate()
{
	if ( CTime::GetIsGameEnd()						) return;
	if ( m_pPlayer->GetIsFoodHave()					) return;
	if ( m_pShopInfoList.size() > SHOP_INFO_NUM_MAX ) return;
	if ( m_CoolTime				> 0.0f				) return;

	// 店員のリストを取得.
	CDeliveryManager::WaitAndLinkListUpdate();
	std::vector<CEmployee*> EmployeeList = CDeliveryManager::GetLinkSortEmployee();

	// 表示できる店が少ない場合表示数を減らす.
	const int EmployeeSize = static_cast<int>( EmployeeList.size() );
	if ( EmployeeSize < m_DispNum ) {
		// 表示している数を減らす.
		m_DispNum--;

		// お店の詳細情報を非表示にする.
		m_pShopInfoList[m_DispNum]->Close();
		m_CoolTime = CONSTANT.ANIM_COOL_TIME;
		return;
	}
	if ( EmployeeSize == 0 ) return;

	// 表示する店員の取得.
	std::vector<CEmployee*> pDispEmployeeList( m_DispNum );
	for ( int i = 0; i < m_DispNum; ++i ) {
		pDispEmployeeList[i] = EmployeeList[i];
	}

	// 表示している店員を変更するか.
	for ( int i = 0; i < m_DispNum; ++i ) {
		// 表示しているか調べる.
		bool		IsDisp	= false;
		const int	Size	= static_cast<int>( pDispEmployeeList.size() );
		for ( int ENo = 0; ENo < Size; ++ENo ) {
			if ( m_pDispEmployeeList[i] != pDispEmployeeList[ENo] ) continue;
			pDispEmployeeList.erase( pDispEmployeeList.begin() + ENo );
			IsDisp = true;
			break;
		}

		// 表示していない場合切り替える.
		if ( IsDisp == false ) {
			if ( m_pShopInfoList[i]->GetIsAnimation() ) continue;

			// 現在の位置より後ろで表示しているお店の情報を削除する.
			for ( int DNo = i + i; DNo < m_DispNum; ++DNo ) {
				const int Size = static_cast<int>( pDispEmployeeList.size() );
				for ( int ENo = 0; ENo < Size; ++ENo ) {
					if ( m_pDispEmployeeList[DNo] != pDispEmployeeList[ENo] ) continue;
					pDispEmployeeList.erase( pDispEmployeeList.begin() + ENo );
					break;
				}
			}

			// 表示しているお店の変更.
			m_pDispEmployeeList[i] = pDispEmployeeList[0];

			// 現在の詳細情報を閉じる.
			m_pShopInfoList[i]->Close();

			// 新しく開く.
			m_pShopInfoList.emplace_back( std::make_unique<CShopInfoUI>() );
			m_pShopInfoList.back()->SetPlayer( m_pPlayer );
			m_pShopInfoList.back()->Init();
			m_pShopInfoList.back()->Open( i, pDispEmployeeList[0]->GetHandle() );

			// 要素の位置を変更.
			std::swap( m_pShopInfoList[i], m_pShopInfoList.back() );
			m_CoolTime = CONSTANT.ANIM_COOL_TIME;
			return;
		}
	}

	// 新しく追加する.
	if ( m_DispNum == SHOP_INFO_NUM_MAX ) return;
	m_CoolTime = CONSTANT.ANIM_COOL_TIME;

	// 店員の保存.
	m_pDispEmployeeList[m_DispNum] = EmployeeList[m_DispNum];

	// 詳細情報の設定.
	m_pShopInfoList[m_DispNum]->Open( m_DispNum, EmployeeList[m_DispNum]->GetHandle() );

	// 表示するの加算.
	m_DispNum++;
}

//---------------------------.
// 非表示の更新.
//---------------------------.
void CShopInfoManagerUI::CloseUpdate()
{
	if ( m_pPlayer->GetIsFoodHave() == false && CTime::GetIsGameEnd() == false	) return;
	if ( m_DispNum					<= 0										) return;
	if ( m_pShopInfoList.size()		> SHOP_INFO_NUM_MAX							) return;
	if ( m_CoolTime					> 0.0f										) return;

	// 表示している数を減らす.
	m_DispNum--;

	// お店の詳細情報を非表示にする.
	m_pShopInfoList[m_DispNum]->Close();
	m_CoolTime = CONSTANT.ANIM_COOL_TIME;
}

//---------------------------.
// 詳細情報の変更の更新.
//---------------------------.
void CShopInfoManagerUI::ChangeUpdate()
{
	const int Size = static_cast<int>( m_pShopInfoList.size() );
	if ( Size <= SHOP_INFO_NUM_MAX ) return;

	// 変更が終了したか.
	for ( int i = Size - 1; i >= SHOP_INFO_NUM_MAX; --i ) {
		if ( m_pShopInfoList[i]->GetIsDisp() ) return;
		m_pShopInfoList.pop_back();
	}
}

//---------------------------.
// クールタイムの更新.
//---------------------------.
void CShopInfoManagerUI::CoolTimeUpdate()
{
	if ( m_CoolTime <= 0.0f ) return;

	// クールタイムを減らす.
	m_CoolTime -= m_DeltaTime;
	if ( m_CoolTime <= 0.0f ) m_CoolTime = 0.0f;
}
