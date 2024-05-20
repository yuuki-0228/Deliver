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
// �C���X�^���X�̎擾.
//---------------------------.
CConstant* CConstant::GetInstance()
{
	static std::unique_ptr<CConstant> pInstance = std::make_unique<CConstant>();
	return pInstance.get();
}

//---------------------------.
// �^�C�g���Ŏg�p����萔�̓ǂݍ���.
//---------------------------.
void CConstant::TitleLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ �^�C�g���Ŏg�p����萔�̓ǂݍ��݊J�n -------" );

	CLog::PushLog( "------ �^�C�g���Ŏg�p����萔�̓ǂݍ��ݏI�� -------" );
#endif
}

//---------------------------.
// �Q�[�����C���Ŏg�p����萔�̓ǂݍ���.
//---------------------------.
void CConstant::GameMainLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ �Q�[�����C���Ŏg�p����萔�̓ǂݍ��݊J�n -------" );

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

	CLog::PushLog( "------ �Q�[�����C���Ŏg�p����萔�̓ǂݍ��ݏI�� -------" );
#endif
}

//---------------------------.
// �Q�[���N���A�Ŏg�p����萔�̓ǂݍ���.
//---------------------------.
void CConstant::GameClearLoad()
{
#ifdef _DEBUG
	CConstant* pI = GetInstance();
	CLog::PushLog( "------ �Q�[���N���A�Ŏg�p����萔�̓ǂݍ��݊J�n -------" );

	CLog::PushLog( "------ �Q�[���N���A�Ŏg�p����萔�̓ǂݍ��ݏI�� -------" );
#endif
}