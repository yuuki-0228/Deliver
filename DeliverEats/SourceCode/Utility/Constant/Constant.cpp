#include "Constant.h"

CConstant::CConstant()
	: m_PlayerData				()
	, m_CustomerDireData		()
	, m_WindData				()
	, m_WindUIData				()
	, m_RuckSackData			()
	, m_ApartmentData			()
	, m_PizzaKilnFireData		()
	, m_IslandData				()
	, m_DeliveryCharacterData	()
	, m_LinkingData				()
	, m_SmokeData				()
	, m_MiniMapData				()
	, m_Notification			()
	, m_Time					()
	, m_ShopInfo				()
{
}

CConstant::~CConstant()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CConstant* CConstant::GetInstance()
{
	static std::unique_ptr<CConstant> pInstance = std::make_unique<CConstant>();
	return pInstance.get();
}

//---------------------------.
// タイトルで使用する定数の読み込み.
//---------------------------.
void CConstant::TitleLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ タイトルで使用する定数の読み込み開始 -------" );

	CLog::PushLog( "------ タイトルで使用する定数の読み込み終了 -------" );
#endif
}

//---------------------------.
// ゲームメインで使用する定数の読み込み.
//---------------------------.
void CConstant::GameMainLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ ゲームメインで使用する定数の読み込み開始 -------" );

	pI->m_FreeCameraData.Load();
	pI->m_PlayerData.Load();
	pI->m_CustomerDireData.Load();
	pI->m_WindData.Load();
	pI->m_WindUIData.Load();
	pI->m_RuckSackData.Load();
	pI->m_StageData.Load();
	pI->m_ApartmentData.Load();
	pI->m_PizzaKilnFireData.Load();
	pI->m_IslandData.Load();
	pI->m_DeliveryCharacterData.Load();
	pI->m_LinkingData.Load();
	pI->m_SmokeData.Load();
	pI->m_MiniMapData.Load();
	pI->m_Notification.Load();
	pI->m_Time.Load();
	pI->m_ShopInfo.Load();

	CLog::PushLog( "------ ゲームメインで使用する定数の読み込み終了 -------" );
#endif
}

//---------------------------.
// ゲームクリアで使用する定数の読み込み.
//---------------------------.
void CConstant::GameClearLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ ゲームクリアで使用する定数の読み込み開始 -------" );

	CLog::PushLog( "------ ゲームクリアで使用する定数の読み込み終了 -------" );
#endif
}