#include "FoodGaugeUI.h"
#include "..\..\..\..\..\GameObject\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\..\GameObject\Actor\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\..\GameObject\Actor\Character\DeliveryManager\Customer\Customer.h"

CFoodGaugeUI::CFoodGaugeUI()
	: m_ReceiveGauge			()
	, m_SendGauge				()
{
}

CFoodGaugeUI::~CFoodGaugeUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CFoodGaugeUI::Init()
{
	m_ReceiveGauge.Value	= INIT_FLOAT;
	m_ReceiveGauge.Alpha	= Color::ALPHA_MIN;
	m_SendGauge.Value		= INIT_FLOAT;
	m_SendGauge.Alpha		= Color::ALPHA_MIN;

	m_pSpriteState["FoodGauge"][0]->RenderArea.z = INIT_FLOAT;
	SetReceiveDisp( false );
	SetSendDisp( false );
	return true;
}
//---------------------------.
// �X�V.
//---------------------------.
void CFoodGaugeUI::Update( const float& DeltaTime )
{
	CEmployee* pEmployee = CDeliveryManager::GetGetFoodEmployee();
	CCustomer* pCustomer = CDeliveryManager::GetGoHandingCustomer();

	if ( pEmployee == nullptr && pCustomer == nullptr ) {
		m_pSpriteState["FoodGauge"][0]->IsDisp			= false;
		m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= false;
		m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= false;
	}
	else {
		m_pSpriteState["FoodGauge"][0]->Color.w			= Color::ALPHA_MIN;
		m_pSpriteState["FoodGaugeBack"][0]->Color.w		= Color::ALPHA_MIN;
		m_pSpriteState["FoodGaugeFrame"][0]->Color.w	= Color::ALPHA_MIN;
		m_pSpriteState["FoodGauge"][0]->IsDisp			= true;
		m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= true;
		m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= true;
	}

	// �󂯓n���Q�[�W�̍X�V.
	SendGauge( DeltaTime, pCustomer );
	// �󂯎��Q�[�W�̍X�V.
	ReceiveGauge( DeltaTime, pEmployee );
}

//---------------------------.
// �`��.
//---------------------------.
void CFoodGaugeUI::Render()
{
}

//---------------------------.
// �󂯎��Q�[�W�̏���.
//---------------------------.
void CFoodGaugeUI::ReceiveGauge( const float& DeltaTime, CEmployee* pEmployee )
{
	// �Ώۂ����Ȃ��Ȃ�e��ݒ������������,�ȉ��̏����͍s��Ȃ�.
	if ( pEmployee == nullptr ) {
		m_ReceiveGauge.Value							= 0.0f;
		m_ReceiveGauge.Alpha							= Color::ALPHA_MIN;
		m_pSpriteState["TakeFoodString"][0]->Color.w	= Color::ALPHA_MIN;
		m_pSpriteState["TakeFoodString"][0]->IsDisp		= false;
		return;
	}
	// �Ώۂ�����Ȃ�󂯎��Q�[�W��\������.
	else {
		m_pSpriteState["TakeFoodString"][0]->IsDisp		= true;;
	}

	// �摜�̃T�C�Y���擾����.
	const SSize& Size = m_pSpriteList["FoodGauge"][0]->GetSpriteState().Base;
	// �摜�̃T�C�Y�ɍ��킹�ăQ�[�W�𑝂₷.
	m_ReceiveGauge.Value = pEmployee->GetGauge() * Size.w;

	// �A���t�@�l�̐ݒ�.
	m_ReceiveGauge.Alpha = pEmployee->GetDeleteTimeCnt() / CConstant::GetDeliveryCharacter().GAUGE_DELETE_TIME;
	if ( m_ReceiveGauge.Alpha == Color::ALPHA_MIN )	SetReceiveDisp( false );
	else											SetReceiveAlpha( m_ReceiveGauge.Alpha );

	// �摜�̍��W���擾.
	const D3DXVECTOR3& Position		= m_pSpriteState["FoodGauge"][0]->Transform.Position;
	// �`��͈͂�ݒ�.
	const D3DXVECTOR4& RenderArea	= { Position.x, Position.y, m_ReceiveGauge.Value, Size.h };
	m_pSpriteState["FoodGauge"][0]->RenderArea = RenderArea;
}

//---------------------------.
// �󂯓n���Q�[�W�̏���.
//---------------------------.
void CFoodGaugeUI::SendGauge( const float& DeltaTime, CCustomer* pCustomer )
{
	// �Ώۂ����Ȃ��Ȃ�e��ݒ������������,�ȉ��̏����͍s��Ȃ�.
	if ( pCustomer == nullptr ) {
		m_SendGauge.Value								= 0.0f;
		m_SendGauge.Alpha								= Color::ALPHA_MIN;
		m_pSpriteState["SendFoodString"][0]->Color.w	= Color::ALPHA_MIN;
		m_pSpriteState["SendFoodString"][0]->IsDisp		= false;
		return;
	}
	// �Ώۂ�����Ȃ�󂯓n���Q�[�W��\������.
	else {
		m_pSpriteState["SendFoodString"][0]->IsDisp		= true;
	}

	// �摜�̃T�C�Y���擾����.
	const SSize& Size = m_pSpriteList["FoodGauge"][0]->GetSpriteState().Base;
	// �摜�̃T�C�Y�ɍ��킹�ăQ�[�W�𑝂₷.
	m_SendGauge.Value = pCustomer->GetGauge() * Size.w;
	 
	// �A���t�@�l�̐ݒ�.
	m_SendGauge.Alpha = pCustomer->GetDeleteTimeCnt() / CConstant::GetDeliveryCharacter().GAUGE_DELETE_TIME;
	if ( m_SendGauge.Alpha == Color::ALPHA_MIN )	SetSendDisp( false );
	else											SetSendAlpha( m_SendGauge.Alpha );

	// �摜�̍��W���擾.
	const D3DXVECTOR3& Position		= m_pSpriteState["FoodGauge"][0]->Transform.Position;
	// �`��͈͂�ݒ�.
	const D3DXVECTOR4& RenderArea	= { Position.x, Position.y, m_SendGauge.Value, Size.h };
	m_pSpriteState["FoodGauge"][0]->RenderArea = RenderArea;
}

//---------------------------.
// �\��/��\���؂�ւ�.
//---------------------------.
void CFoodGaugeUI::SetReceiveDisp( bool Flag )
{
	m_pSpriteState["FoodGauge"][0]->IsDisp			= Flag;
	m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= Flag;
	m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= Flag;
	m_pSpriteState["TakeFoodString"][0]->IsDisp		= Flag;
}

//---------------------------.
// Alpha�̈ꊇ�Ǘ�.
//---------------------------.
void CFoodGaugeUI::SetReceiveAlpha( float Alpha )
{
	m_pSpriteState["FoodGauge"][0]->Color.w			= Alpha;
	m_pSpriteState["FoodGaugeBack"][0]->Color.w		= Alpha;
	m_pSpriteState["FoodGaugeFrame"][0]->Color.w	= Alpha;
	m_pSpriteState["TakeFoodString"][0]->Color.w	= Alpha;
}

//---------------------------.
// �\��/��\���؂�ւ�.
//---------------------------.
void CFoodGaugeUI::SetSendDisp( bool Flag )
{
	m_pSpriteState["FoodGauge"][0]->IsDisp			= Flag;
	m_pSpriteState["FoodGaugeBack"][0]->IsDisp		= Flag;
	m_pSpriteState["FoodGaugeFrame"][0]->IsDisp		= Flag;
	m_pSpriteState["SendFoodString"][0]->IsDisp		= Flag;
}

//---------------------------.
// Alpha�̈ꊇ�Ǘ�.
//---------------------------.
void CFoodGaugeUI::SetSendAlpha( float Alpha )
{
	m_pSpriteState["FoodGauge"][0]->Color.w			= Alpha;
	m_pSpriteState["FoodGaugeBack"][0]->Color.w		= Alpha;
	m_pSpriteState["FoodGaugeFrame"][0]->Color.w	= Alpha;
	m_pSpriteState["SendFoodString"][0]->Color.w	= Alpha;
}
