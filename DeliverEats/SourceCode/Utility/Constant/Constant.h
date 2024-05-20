#pragma once
#include "..\FileManager\FileManager.h"
#include "ConstantStruct.h"

/************************************************
*	�萔�N���X.
*		���c�F�P.
**/
class CConstant final
{
public:
	CConstant();
	~CConstant();

	// �C���X�^���X�̎擾.
	static CConstant* GetInstance();

	// �^�C�g���Ŏg�p����萔�̓ǂݍ���.
	static void TitleLoad();
	// �Q�[�����C���Ŏg�p����萔�̓ǂݍ���.
	static void GameMainLoad();
	// �Q�[���N���A�Ŏg�p����萔�̓ǂݍ���.
	static void GameClearLoad();

	// �萔�̎擾.
	static const ConstantStruct::SPlayer&				GetPlayer()					{ return GetInstance()->m_PlayerData; }
	static const ConstantStruct::SCustomerDire&			GetCustomerDire()			{ return GetInstance()->m_CustomerDireData; }
	static const ConstantStruct::SFreeCamera&			GetFreeCamera()				{ return GetInstance()->m_FreeCameraData; }
	static const ConstantStruct::SCamera&				GetCamera()					{ return GetInstance()->m_CameraData; }
	static const ConstantStruct::SWind&					GetWind()					{ return GetInstance()->m_WindData; }
	static const ConstantStruct::SWindUI&				GetWindUI()					{ return GetInstance()->m_WindUIData; }
	static const ConstantStruct::SRuckSack&				GetRuckSack()				{ return GetInstance()->m_RuckSackData; }
	static const ConstantStruct::SStage&				GetStage()					{ return GetInstance()->m_StageData; }
	static const ConstantStruct::SApartment&			GetApartment()				{ return GetInstance()->m_ApartmentData; }
	static const ConstantStruct::SPizzaKilnFire&		GetPizzaKilnFire()			{ return GetInstance()->m_PizzaKilnFireData; }
	static const ConstantStruct::SIsland&				GetIsland()					{ return GetInstance()->m_IslandData; }
	static const ConstantStruct::SDeliveryCharacter&	GetDeliveryCharacter()		{ return GetInstance()->m_DeliveryCharacterData; }
	static const ConstantStruct::SLinking&				GetLinkingr()				{ return GetInstance()->m_LinkingData; }
	static const ConstantStruct::SSmoke&				GetSmoke()					{ return GetInstance()->m_SmokeData; }
	static const ConstantStruct::SMiniMap&				GetMiniMap()				{ return GetInstance()->m_MiniMapData; }
	static const ConstantStruct::SNotification&			GetNotification()			{ return GetInstance()->m_Notification; }
	static const ConstantStruct::STime&					GetTime()					{ return GetInstance()->m_Time; }
	static const ConstantStruct::SShopInfo&				GetShopInfo()				{ return GetInstance()->m_ShopInfo; }

private:
	// �萔�f�[�^.
	ConstantStruct::SPlayer					m_PlayerData;
	ConstantStruct::SCustomerDire			m_CustomerDireData;
	ConstantStruct::SFreeCamera				m_FreeCameraData;
	ConstantStruct::SCamera					m_CameraData;
	ConstantStruct::SWind					m_WindData;
	ConstantStruct::SWindUI					m_WindUIData;
	ConstantStruct::SRuckSack				m_RuckSackData;
	ConstantStruct::SStage					m_StageData;
	ConstantStruct::SApartment				m_ApartmentData;
	ConstantStruct::SPizzaKilnFire			m_PizzaKilnFireData;
	ConstantStruct::SIsland					m_IslandData;
	ConstantStruct::SDeliveryCharacter		m_DeliveryCharacterData;
	ConstantStruct::SLinking				m_LinkingData;
	ConstantStruct::SSmoke					m_SmokeData;
	ConstantStruct::SMiniMap				m_MiniMapData;
	ConstantStruct::SNotification			m_Notification;
	ConstantStruct::STime					m_Time;
	ConstantStruct::SShopInfo				m_ShopInfo;
};
