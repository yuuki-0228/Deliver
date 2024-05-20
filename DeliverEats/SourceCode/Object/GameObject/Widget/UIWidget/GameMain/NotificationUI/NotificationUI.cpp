#include "NotificationUI.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\..\..\..\Resource\FontResource\FontResource.h"

namespace {
	// �ʒm��No.
	enum enNotificationNo : unsigned char {
		Now,	// ���̒ʒm�̃X�v���C�g.
		Next,	// ���̒ʒm�̃X�v���C�g.

		Notificatione_Max
	} typedef ENotificationNo;

	// �t�H���gNo.
	enum enFontNo : unsigned char {
		Name,		// �A�C�R���̖��O.
		Message,	// ���b�Z�[�W.
		Time,		// �����Ă�������.

		Font_Max
	} typedef EFontNo;

	// �A�C�R��No.
	enum enIconNo : unsigned char {
		DeliverEatsIcon,	// ���q����̕]���⃉���L���O�̍X�V�p�A�C�R��.
		NewsIcon,			// ���������������p�̃A�C�R��.
		MessageIcon,		// �S�[���f���^�C����c�莞�ԗp�A�C�R��.
		EmergencyIcon,		// �}�b�v�O�̌x���p�A�C�R��.

		Icon_Max
	} typedef EIconNo;
}

CNotificationUI::CNotificationUI()
	: m_pBackSprite		( nullptr )
	, m_pIconSprite		( nullptr )
	, m_pFont			( nullptr )
	, m_BackState		( Notificatione_Max )
	, m_IconState		( Notificatione_Max )
	, m_FontState		( Notificatione_Max )
	, m_IconNoList		()
	, m_IconNameList	()
	, m_IconSEList		()
	, m_MessageList		()
	, m_Message			( Notificatione_Max, SMessage() )
	, m_IsOpen			( Notificatione_Max, false )
	, m_MessageScale	( Notificatione_Max, STransform::NORMAL_SCALE_VEC3 )
	, m_MessageAlpha	( Notificatione_Max, Color::ALPHA_MAX )
	, m_IsClose			( false )
	, m_IsChangeClose	( false )
	, m_IsChange		( false )
	, m_IsDisp			( false )
	, m_DispTimeCnt		( INIT_FLOAT )
{
}

CNotificationUI::~CNotificationUI()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CNotificationUI* CNotificationUI::GetInstance()
{
	static std::unique_ptr<CNotificationUI> pInstance = std::make_unique<CNotificationUI>();
	return pInstance.get();
}

//---------------------------.
// ������.
//---------------------------.
bool CNotificationUI::Init()
{
	CNotificationUI* pI = GetInstance();

	// �摜�̎擾.
	pI->m_pBackSprite	= CSpriteResource::GetSprite( "NotificationBack" );
	pI->m_pIconSprite	= CSpriteResource::GetSprite( "NotificationIcon" );

	// �摜�̏��̎擾.
	for ( auto& s : pI->m_BackState ) s = pI->m_pBackSprite->GetRenderState();
	for ( int i = 0; i < Notificatione_Max; ++i ) {
		pI->m_IconState[i] = pI->m_pIconSprite->GetRenderState();
		pI->m_IconState[i].Transform.AttachParent( &pI->m_BackState[i].Transform );
		pI->m_IconState[i].AnimState.IsSetAnimNumber = true;
	}

	// �t�H���g�̎擾.
	pI->m_pFont = CFontResource::GetFont( "�R�[�|���[�g�E���S ver2 Medium" );
	for ( int n = 0; n < Notificatione_Max; ++n ) {
		pI->m_FontState[n].resize( Font_Max );
		for ( int f = 0; f < Font_Max; ++f ) {
			pI->m_FontState[n][f] = pI->m_pFont->GetRenderState();
			pI->m_FontState[n][f].Transform.AttachParent( &pI->m_BackState[n].Transform );
			pI->m_FontState[n][f].Transform.Scale = { 0.8f, 0.8f, 0.8f };
		}
	}

	// �`��G���A�̐ݒ�.
	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize	= CMiniMapUI::GetScreenSize();
	for ( auto& s : pI->m_BackState ) s.RenderArea = { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	for ( auto& s : pI->m_IconState ) s.RenderArea = { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	for ( auto& List : pI->m_FontState ) {
		for ( auto& s : List ) s.RenderArea = { ScreenPos.x - ScreenSize.w / 2.0f, ScreenPos.y - ScreenSize.h / 2.0f, ScreenSize.w, ScreenSize.h };
	}

	// �A�C�R����No�ݒ�.
	pI->m_IconNoList[EMessageType::Evaluation]		= DeliverEatsIcon;
	pI->m_IconNoList[EMessageType::RankingUpdate]	= DeliverEatsIcon;
	pI->m_IconNoList[EMessageType::WindIsGenerated]	= NewsIcon;
	pI->m_IconNoList[EMessageType::GoldenTime]		= MessageIcon;
	pI->m_IconNoList[EMessageType::TimeLeft]		= DeliverEatsIcon;
	pI->m_IconNoList[EMessageType::Warning]			= EmergencyIcon;

	// �A�C�R���̖��O�̐ݒ�.
	pI->m_IconNameList[DeliverEatsIcon]				= "DeliverEats";
	pI->m_IconNameList[NewsIcon]					= "�V�C";
	pI->m_IconNameList[MessageIcon]					= "Message";
	pI->m_IconNameList[EmergencyIcon]				= "�ً}����";
	
	// �A�C�R����SE���̐ݒ�.
	pI->m_IconSEList[DeliverEatsIcon]				= "DeliverEatsIcon";
	pI->m_IconSEList[NewsIcon]						= "NewsIcon";
	pI->m_IconSEList[MessageIcon]					= "MessageIcon";
	pI->m_IconSEList[EmergencyIcon]					= "EmergencyIcon";

	// �ϐ��̏�����.
	pI->m_MessageList.clear();
	for ( auto&  m : pI->m_Message		) m = SMessage();
	for ( auto&& f : pI->m_IsOpen		) f = false;
	for ( auto&  s : pI->m_MessageScale ) s = STransform::NORMAL_SCALE_VEC3;
	for ( auto&  a : pI->m_MessageAlpha ) a = Color::ALPHA_MAX;
	pI->m_IsClose		= false;
	pI->m_IsChangeClose = false;
	pI->m_IsChange		= false;
	pI->m_IsDisp		= false;
	pI->m_DispTimeCnt	= INIT_FLOAT;
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CNotificationUI::Update( const float& deltaTime )
{
	// �J������.
	OpenUpdate( Now );
	OpenUpdate( Next );
	// ���鏈��.
	CloseUpdate();
	// �ύX�ɂ����鏈��.
	ChangeCloseUpdate();
	// �ύX����.
	ChangeUpdate();
	// �����ŕ��鏈��.
	AutoCloseUpdate( deltaTime );

	// ���b�Z�[�W��D�揇�ɕ��בւ���.
	MessageSort();
}

//---------------------------.
// �`��.
//---------------------------.
void CNotificationUI::Render()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_Message[Now].Type == EMessageType::None ) return;

	// �ʒm�̕`��.
	const int Max = pI->m_IsChange ? Notificatione_Max : 1;
	for ( int n = 0; n < Max; ++n ) {
		pI->m_BackState[n].Transform.Scale	= pI->m_MessageScale[n];
		pI->m_BackState[n].Color.w			= pI->m_MessageAlpha[n];
		pI->m_IconState[n].Color.w			= pI->m_MessageAlpha[n];
		pI->m_pBackSprite->RenderUI( &pI->m_BackState[n] );
		pI->m_pIconSprite->RenderUI( &pI->m_IconState[n] );

		// �e�L�X�g�̕`��.
		for ( int f = 0; f < Font_Max; ++f ) {
			pI->m_FontState[n][f].Color.w	= pI->m_MessageAlpha[n];
			pI->m_pFont->RenderUI( &pI->m_FontState[n][f] );
		}
	}
}

//---------------------------.
// �����̒ǉ�.
//---------------------------.
void CNotificationUI::PushMessage( const EMessageType& Type, const std::string& Text )
{
	CNotificationUI* pI = GetInstance();

	// ���ݒʒm��\�����Ă��Ȃ��ꍇ�ʒm��\������.
	if ( pI->m_Message[Now].Type == EMessageType::None ) {
		pI->m_Message[Now] = SMessage( Type, Text, CTime::GetElapsedTime() );
		OpenSetting( Now );
		return;
	}

	// ���\�����Ă���ʒm���D��x���Ⴂ�ꍇ.
	if ( pI->m_Message[Now].Type > Type || pI->m_IsChange ) {
		// ���X�g�ɒǉ�����.
		pI->m_MessageList.emplace_back( SMessage( Type, Text, CTime::GetElapsedTime() ) );

		// �D�揇�ʂ̕��тɃ\�[�g��������.
		MessageSort();
		return;
	}

	// ���\�����Ă���ʒm���D��x���������ߒʒm���X�V����.
	pI->m_Message[Next] = SMessage( Type, Text, CTime::GetElapsedTime() );
	ChangeSetting();
}

//---------------------------.
// �\������.
//---------------------------.
void CNotificationUI::OpenUpdate( const int Type )
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsOpen[Type] == false ) return;

	const D3DXPOSITION3&	ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&			ScreenSize	= CMiniMapUI::GetScreenSize();
	D3DXPOSITION3*			BackPos		= &pI->m_BackState[Type].Transform.Position;
	const SSize&			BackSize	= pI->m_pBackSprite->GetSpriteState().Disp;
	const float				EndPosY		= ScreenPos.y - ScreenSize.h / 2 + BackSize.h / 2.0f + pI->CONSTANT.DISP_ICON_POSITION_Y;

	// �s�����ɂ��Ă���.
	if ( pI->m_MessageAlpha[Type] < Color::ALPHA_MAX ) {
		pI->m_MessageAlpha[Type] += pI->CONSTANT.ALPHA_SPEED;
		if ( pI->m_MessageAlpha[Type] >= Color::ALPHA_MAX ) {
			pI->m_MessageAlpha[Type] = Color::ALPHA_MAX;
		}
	}

	// �ʒm�������Ă���.
	BackPos->y += pI->CONSTANT.MOVE_SPEED;

	// �ʒm�̓������~�߂�.
	if ( BackPos->y > EndPosY ) {
		BackPos->y			= EndPosY;
		pI->m_IsOpen[Type]	= false;
		pI->m_IsDisp		= true;
		pI->m_DispTimeCnt	= INIT_FLOAT;
	}
}

//---------------------------.
// ���鏈��.
//---------------------------.
void CNotificationUI::CloseUpdate()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsClose == false ) return;

	const D3DXPOSITION3&	ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&			ScreenSize	= CMiniMapUI::GetScreenSize();
	D3DXPOSITION3*			BackPos		= &pI->m_BackState[Now].Transform.Position;
	const SSize&			BackSize	= pI->m_pBackSprite->GetSpriteState().Disp;
	const float				EndPosY		= ScreenPos.y - ScreenSize.h / 2 - BackSize.h / 2.0f;

	// �����ɂ��Ă���.
	if ( pI->m_MessageAlpha[Now] > Color::ALPHA_MIN ) {
		pI->m_MessageAlpha[Now] -= pI->CONSTANT.ALPHA_SPEED;
		if ( pI->m_MessageAlpha[Now] <= Color::ALPHA_MIN ) {
			pI->m_MessageAlpha[Now] = Color::ALPHA_MIN;
		}
	}

	// �ʒm�������Ă���.
	BackPos->y -= pI->CONSTANT.MOVE_SPEED;

	// �ʒm�̓������~�߂�.
	if ( BackPos->y >= EndPosY ) return;
	BackPos->y			= EndPosY;
	pI->m_IsClose		= false;
	pI->m_IsDisp		= false;
	pI->m_DispTimeCnt	= INIT_FLOAT;
	pI->m_Message[Now]	= SMessage();
}

//---------------------------.
// �ύX�ɂ����鏈��.
//---------------------------.
void CNotificationUI::ChangeCloseUpdate()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsChangeClose == false ) return;

	// �k�����Ă���.
	float Scale = pI->m_MessageScale[Now].x;
	Scale -= pI->CONSTANT.SCALE_SPEED;

	// �k�����~������.
	if ( Scale < STransform::SCALE_MIN ) {
		Scale				= STransform::SCALE_MIN;
		pI->m_IsChangeClose = false;
	}
	pI->m_MessageScale[Now] = D3DXSCALE3( Scale, Scale, Scale );
}

//---------------------------.
// �ʒm�̕ύX�I������.
//---------------------------.
void CNotificationUI::ChangeUpdate()
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsChange == false	) return;
	if ( pI->m_IsOpen[Next]			) return;

	// ���݂̒ʒm�ɕύX.
	pI->m_Message[Now]						= pI->m_Message[Next];
	pI->m_Message[Next]						= SMessage();
	pI->m_BackState[Now]					= pI->m_BackState[Next];
	pI->m_IconState[Now]					= pI->m_IconState[Next];
	pI->m_IconState[Now].Transform.pParent	= &pI->m_BackState[Now].Transform;
	for ( int f = 0; f < Font_Max; ++f ) {
		pI->m_FontState[Now][f]						= pI->m_FontState[Next][f];
		pI->m_FontState[Now][f].Transform.pParent	= &pI->m_BackState[Now].Transform;
	}

	// �g�k�����ɖ߂�.
	pI->m_MessageScale[Now] = STransform::NORMAL_SCALE_VEC3;

	// �ʒm�̕ύX�I��.
	pI->m_IsChange		= false;
	pI->m_IsChangeClose = false;
	pI->m_IsDisp		= true;
	pI->m_DispTimeCnt	= INIT_FLOAT;
}

//---------------------------.
// �����ŕ��鏈��.
//---------------------------.
void CNotificationUI::AutoCloseUpdate( const float& deltaTime )
{
	CNotificationUI* pI = GetInstance();

	if ( pI->m_IsDisp == false ) return;

	// �\�����Ԃ𑝂₵�Ă���.
	pI->m_DispTimeCnt += deltaTime;

	// ��莞�ԕ\�����Ă���ꍇ�ʒm�����.
	if ( pI->m_DispTimeCnt < pI->CONSTANT.DISP_TIME ) return;
	pI->m_DispTimeCnt	= INIT_FLOAT;
	pI->m_IsDisp		= false;

	// �\������ʒm�����邩.
	if ( pI->m_MessageList.empty() ) {
		//  �ʒm�����.
		pI->m_IsClose = true;
		return;
	}
	
	// ���̒ʒm�ɐ؂�ւ���.
	pI->m_Message[Next] = pI->m_MessageList.back();
	ChangeSetting();
	pI->m_MessageList.pop_back();
}

//---------------------------.
// �\���ݒ�.
//---------------------------.
void CNotificationUI::OpenSetting( const int Type )
{
	CNotificationUI* pI = GetInstance();

	pI->m_IsOpen[Type]	= true;
	pI->m_IsDisp		= false;

	const D3DXPOSITION3& ScreenPos	= CMiniMapUI::GetScreenPos();
	const SSize&		 ScreenSize = CMiniMapUI::GetScreenSize();

	// �����ɂ���.
	pI->m_MessageAlpha[Type] = Color::ALPHA_MIN;

	// �ʒm�̔w�i�̕ύX.
	STransform*		BackTransform	= &pI->m_BackState[Type].Transform;
	const SSize&	BackSize		= pI->m_pBackSprite->GetSpriteState().Disp;
	BackTransform->Position.x		= ScreenPos.x;
	BackTransform->Position.y		= ScreenPos.y - ScreenSize.h / 2.0f - BackSize.h / 2;

	// �ʒm�̃A�C�R���̐ݒ�.
	D3DXPOSITION3*	IconPos						= &pI->m_IconState[Type].Transform.Position;
	const SSize&	IconSize					= pI->m_pIconSprite->GetSpriteState().Disp;
	const int		IconNo						= pI->m_IconNoList[pI->m_Message[Type].Type];
	IconPos->x									= -BackSize.w / 2.0f + IconSize.w / 2.0f + pI->CONSTANT.ICON_POSITION.x;
	IconPos->y									= -BackSize.h / 2.0f + IconSize.h / 2.0f + pI->CONSTANT.ICON_POSITION.y;
	pI->m_IconState[Type].AnimState.AnimNumber	= IconNo;

	// �t�H���g(�A�C�R���̖��O)�̐ݒ�.
	D3DXPOSITION3* FontNamePos				= &pI->m_FontState[Type][Name].Transform.Position;
	FontNamePos->x							= IconPos->x + pI->CONSTANT.FONT_POSITION.x;
	FontNamePos->y							= IconPos->y;
	pI->m_FontState[Type][Name].Color		= Color4::Gray;
	pI->m_FontState[Type][Name].Text		= pI->m_IconNameList[IconNo];

	// �t�H���g(���b�Z�[�W)�̐ݒ�.
	D3DXPOSITION3* FontMessagePos			= &pI->m_FontState[Type][Message].Transform.Position;
	FontMessagePos->x						= IconPos->x;
	if ( pI->m_Message[Type].Text.find( "\n" ) != std::string::npos ){
		FontMessagePos->y					= IconPos->y + IconSize.w / 2.0f + pI->CONSTANT.FONT_POSITION.y * 0.5f;
	}
	else {
		FontMessagePos->y					= IconPos->y + IconSize.w / 2.0f + pI->CONSTANT.FONT_POSITION.y;
	}
	pI->m_FontState[Type][Message].Color	= Color4::Black;
	pI->m_FontState[Type][Message].Text		= pI->m_Message[Type].Text;

	// �ʒm�̎擾���ԃe�L�X�g�̍쐬.
	std::string	TimeText					= "��";
	const int	Minute						= CTime::GetElapsedMinute( pI->m_Message[Type].ElapsedTime );
	if (		Minute >= 60 ) TimeText		= std::to_string( Minute / 60 ) + "���ԑO";
	else if (	Minute >   0 ) TimeText		= std::to_string( Minute ) + "���O";

	// �t�H���g(�ʒm�̎擾����)�̐ݒ�.
	D3DXPOSITION3* FontTimePos				= &pI->m_FontState[Type][Time].Transform.Position;
	FontTimePos->x							= BackSize.w / 2.0f - pI->CONSTANT.FONT_POSITION.x / 2.0f;
	FontTimePos->y							= IconPos->y;
	pI->m_FontState[Type][Time].TextAlign	= ETextAlign::Right;
	pI->m_FontState[Type][Time].Color		= Color4::Gray;
	pI->m_FontState[Type][Time].Text		= TimeText;

	// SE�̍Đ�.
	CSoundManager::PlaySE( pI->m_IconSEList[IconNo] );
}

//---------------------------.
// �ύX�ݒ�.
//---------------------------.
void CNotificationUI::ChangeSetting()
{
	CNotificationUI* pI = GetInstance();

	pI->m_IsChange		= true;

	// ���ɕ\������ʒm��\��������.
	OpenSetting( Next );

	// ���\�����Ă���ʒm���k���Ŕ�\���ɂ���.
	pI->m_IsChangeClose = true;
}

//---------------------------.
// ���b�Z�[�W����ёւ���.
//---------------------------.
void CNotificationUI::MessageSort()
{
	CNotificationUI* pI = GetInstance();

	// �d�v�x���Ⴂ���ɕ��בւ���.
	std::vector<std::pair<EMessageType, int>> ImpoList;
	const int ListSize = static_cast<int>( pI->m_MessageList.size() );
	for ( int i = 0; i < ListSize; ++i ){
		ImpoList.emplace_back( std::make_pair( pI->m_MessageList[i].Type, i ) );
	}
	std::sort( ImpoList.begin(), ImpoList.end() );

	// ���ёւ������ԂɕύX����.
	MessageList OutList;
	for ( auto& [Type, No] : ImpoList ) OutList.emplace_back( pI->m_MessageList[No] );
	pI->m_MessageList = OutList;
}
