#include "GameMainWidget.h"
#include "..\..\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\UIWidget\GameMain\HeightGaugeUI\HeightGaugeUI.h"
#include "..\..\UIWidget\GameMain\RucksackUI\RucksackUI.h"
#include "..\..\UIWidget\GameMain\FoodGaugeUI\FoodGaugeUI.h"
#include "..\..\UIWidget\GameMain\WarningUI\WarningUI.h"
#include "..\..\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\..\UIWidget\GameMain\StatusBarUI\StatusBarUI.h"
#include "..\..\UIWidget\GameMain\WindUI\WindUI.h"
#include "..\..\UIWidget\GameMain\NightUI\NightUI.h"
#include "..\..\UIWidget\GameMain\ShopInfoManagerUI\ShopInfoManagerUI.h"
#include "..\..\UIWidget\GameMain\TimeGaugeUI\TimeGaugeUI.h"
#include "..\..\UIWidget\GameMain\GoldenTimeUI\GoldenTimeUI.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Object\Score\Score.h"
#include "..\..\..\..\..\Object\Score\Ranking\Ranking.h"
#include "..\..\..\..\..\Object\GameObject\Actor\Wind\Wind.h"
#include "..\..\..\..\..\Utility\Input\Input.h"

namespace {
	enum enSpriteList : unsigned char {
		BackGround,	// �w�i.
		Yubi,		// �J�[�\��.
		Text,		// �e�L�X�g.
		Choice_1,	// �I����1.
		Choice_2,	// �I����2.

		Max
	}typedef ESpriteList;

	enum enTextNo : unsigned char {
		PauseText,
		SelectToBack,
		NotToBack,
		SelectToClear,
		NotToClear
	} typedef ETextNo;

	constexpr int	CHOICE_STARTCHOICE	= static_cast<int>( EChoice::StartChoice );	// �ŏ��̑I����.
	constexpr int	CHOICE_ENDCHOICE	= static_cast<int>( EChoice::EndChoice );	// �Ō�̑I����.
	constexpr int	CHOICE_MAX			= static_cast<int>( EChoice::Max );			// �ő�v�f��.

	constexpr int	SCALING_NONE		= static_cast<int>( EScaling::None );		// �ҋ@��.
	constexpr int	SCALING_EXPAND		= static_cast<int>( EScaling::Expand );		// �g�咆.
	constexpr int	SCALING_SHRINK		= static_cast<int>( EScaling::Shrink );		// �k����.

	constexpr char	OPEN_SE[]			= "Open";									// �|�[�Y���J������SE.
	constexpr char	CLOSE_SE[]			= "Close";									// �|�[�Y����鎞��SE.
	constexpr char	CURSOR_MOVE_SE[]	= "CursorMove";								// �J�[�\���ړ�����SE.
}

CGameMainWidget::CGameMainWidget()
	: m_DeltaTime			( INIT_FLOAT )
	, m_pHeightGaugeUI		( nullptr )
	, m_pRucksackUI			( nullptr )
	, m_pFoodGaugeUI		( nullptr )
	, m_pWarningUI			( nullptr )
	, m_pStatusBarUI		( nullptr )
	, m_pWindUI				( nullptr )
	, m_pNightUI			( nullptr )
	, m_pShopInfoUI			( nullptr )
	, m_pTimeGaugeUI		( nullptr )
	, m_pGoldenTimeUI		( nullptr )

	, m_Choice				( CHOICE_STARTCHOICE )

	, m_pSpriteList			( Max )
	, m_SpriteStateList		( Max )
	, m_ScalingSwitch		( INIT_INT )
	, m_PauseWindowScale	( INIT_FLOAT )
	, m_AnimCnt				( INIT_FLOAT )
{
}

CGameMainWidget::~CGameMainWidget()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CGameMainWidget::Init()
{
	SetSceneName( ESceneList::GameMain );
	GetSpriteList();

	// �ʒm�̏�����.
	CNotificationUI::Init();

	m_pHeightGaugeUI	= std::make_unique<CHeightGaugeUI>();
	if ( m_pHeightGaugeUI->Init() == false ) return false;

	m_pRucksackUI		= std::make_unique<CRucksackUI>();
	if ( m_pRucksackUI->Init() == false ) return false;

	m_pFoodGaugeUI		= std::make_unique<CFoodGaugeUI>();
	m_pFoodGaugeUI->SetSpriteState( m_SpriteState );
	m_pFoodGaugeUI->SetSpriteList( m_SpriteList );
	if ( m_pFoodGaugeUI->Init() == false ) return false;

	m_pWarningUI		= std::make_unique<CWarningUI>();
	m_pWarningUI->SetSpriteState( m_SpriteState );
	if ( m_pWarningUI->Init() == false ) return false;

	m_pStatusBarUI		= std::make_unique<CStatusBarUI>();
	m_pStatusBarUI->SetSpriteState( m_SpriteState );
	if ( m_pStatusBarUI->Init() == false ) return false;

	m_pWindUI			= std::make_unique<CWindUI>();
	if ( m_pWindUI->Init()		== false ) return false;

	m_pNightUI			= std::make_unique<CNightUI>();
	if ( m_pNightUI->Init()		== false ) return false;

	m_pShopInfoUI		= std::make_unique<CShopInfoManagerUI>();
	if ( m_pShopInfoUI->Init()	== false ) return false;

	m_pTimeGaugeUI		= std::make_unique<CTimeGaugeUI>();
	if ( m_pTimeGaugeUI->Init()	== false ) return false;

	m_pGoldenTimeUI		= std::make_unique<CGoldenTimeUI>();
	if ( m_pGoldenTimeUI->Init()	== false ) return false;

	m_pSpriteList[BackGround	] = CSpriteResource::GetSprite( "back",			&m_SpriteStateList[BackGround]	);
	m_pSpriteList[Yubi			] =	CSpriteResource::GetSprite( "Yubi",			&m_SpriteStateList[Yubi]		);
	m_pSpriteList[Text			] =	CSpriteResource::GetSprite( "PauseText",	&m_SpriteStateList[Text]		);
	m_pSpriteList[Choice_1		] =	CSpriteResource::GetSprite( "PauseText",	&m_SpriteStateList[Choice_1]	);
	m_pSpriteList[Choice_2		] =	CSpriteResource::GetSprite( "PauseText",	&m_SpriteStateList[Choice_2]	);

	m_SpriteStateList[Yubi		].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Text		].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Choice_1	].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Choice_2	].Transform.AttachParent( &m_SpriteStateList[BackGround].Transform );
	m_SpriteStateList[Yubi		].Transform.Scale			= { 1.0f, 1.0f, 1.0f };
	m_SpriteStateList[Text		].AnimState.AnimNumber		= PauseText;
	m_SpriteStateList[Choice_1	].AnimState.IsSetAnimNumber	= true;
	m_SpriteStateList[Choice_1	].AnimState.AnimNumber		= SelectToBack;
	m_SpriteStateList[Choice_2	].AnimState.IsSetAnimNumber = true;
	m_SpriteStateList[Choice_2	].AnimState.AnimNumber		= NotToClear;

	m_SpriteStateList[BackGround].Transform.Position.x = FWND_W / 2.0f;
	m_SpriteStateList[BackGround].Transform.Position.y = FWND_H / 2.0f;
	m_SpriteStateList[Text		].Transform.Position.y = -100.0f;
	m_SpriteStateList[Choice_1	].Transform.Position.y =   64.0f;
	m_SpriteStateList[Choice_2	].Transform.Position.y =  192.0f;

	m_Choice			= CHOICE_STARTCHOICE;
	m_ScalingSwitch		= SCALING_NONE;
	m_PauseWindowScale	= 0.0f;
	SetPauseScaleAndAlpha( m_PauseWindowScale );
	SwitchDisp( false );
	CSceneManager::SetIsPause( false );

	CScore::Init();

	CMiniMapUI::Init();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGameMainWidget::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;
	const bool IsPause = CSceneManager::GetIsPause();

	PauseWindowUpdate();
	switch ( static_cast<EScaling>( m_ScalingSwitch ) ) {
	// �k�����Ȃ�.
	case EScaling::Shrink:
		// �t�F�[�h�A�E�g�̏������s��.
		UpdateFadeOut();
		break;
	// �g�咆�Ȃ�.
	case EScaling::Expand:
		// �t�F�[�h�C���̏������s��.
		UpdateFadeIn();
		break;
	// �g�k�����������Ȃ�.
	case EScaling::None:
		// �|�[�Y���Ȃ�.
		if ( IsPause ) UpdatePauseWindow();
		// �|�[�Y���łȂ��Ȃ�.
		else UpdatePushPause();
		break;
	default:
		break;
	}

	// �|�[�Y���̓A�j���[�V�������~�߂�.
	m_pRucksackUI->SetIsAnimPlay( !IsPause );
	// �����|�[�Y���Ȃ�Ȍ�̏������s��Ȃ�.
	if ( IsPause ) return;
	// ���x�v�̍X�V.
	m_pHeightGaugeUI->Update( DeltaTime );
	// �����b�N�̍X�V
	m_pRucksackUI->Update( DeltaTime );
	// �󂯓n���Q�[�W�̍X�V.
	m_pFoodGaugeUI->Update( DeltaTime );
	// �x���̍X�V.
	m_pWarningUI->Update( DeltaTime );
	// �X�e�[�^�X�o�[�̍X�V.
	m_pStatusBarUI->Update( DeltaTime );
	// ��UI�̍X�V.
	m_pWindUI->Update( DeltaTime );
	// ��̍X�V.
	m_pNightUI->Update( DeltaTime );
	// �����̍X�V.
	CScore::Update( DeltaTime );
	// �ʒm�̍X�V.
	CNotificationUI::Update( DeltaTime );
	// ���X�̏ڍ׏��̍X�V.
	m_pShopInfoUI->Update( DeltaTime );
	// ���ԃQ�[�W�̍X�V.
	m_pTimeGaugeUI->Update( DeltaTime );
	// �S�[���f���^�C���̍X�V.
	m_pGoldenTimeUI->Update( DeltaTime );
}

//---------------------------.
// �\��/��\���̈ꊇ�ؑ�.
//---------------------------.
void CGameMainWidget::SwitchDisp( bool flag )
{
	// UI��\��/��\������.
	m_SpriteStateList[BackGround].IsDisp = flag;
	m_SpriteStateList[Yubi		].IsDisp = flag;
	m_SpriteStateList[Text		].IsDisp = flag;
	m_SpriteStateList[Choice_1	].IsDisp = flag;
	m_SpriteStateList[Choice_2	].IsDisp = flag;
}

//---------------------------.
// �|�[�Y��ʂ̊g�k���ꊇ�Ǘ�.
//---------------------------.
void CGameMainWidget::SetPauseScaleAndAlpha( float value )
{
	const D3DXVECTOR3 Scale = { value, value, 1.0f };

	m_SpriteStateList[BackGround].Transform.Scale	= Scale;
	m_SpriteStateList[Yubi		].Transform.Scale	= Scale;
	m_SpriteStateList[Text		].Transform.Scale	= Scale;
	m_SpriteStateList[Choice_1	].Transform.Scale	= Scale;
	m_SpriteStateList[Choice_2	].Transform.Scale	= Scale;

	m_SpriteStateList[BackGround].Color.w			= value;
	m_SpriteStateList[Yubi		].Color.w			= value;
	m_SpriteStateList[Text		].Color.w			= value;
	m_SpriteStateList[Choice_1	].Color.w			= value;
	m_SpriteStateList[Choice_2	].Color.w			= value;
}

//---------------------------.
// �v���C���[��ݒ�.
//---------------------------.
void CGameMainWidget::SetPlayer( CPlayer* pPlayer )
{
	m_pHeightGaugeUI->SetPlayer( pPlayer );
	m_pRucksackUI->SetPlayer( pPlayer );
	m_pWarningUI->SetPlayer( pPlayer );
	m_pShopInfoUI->SetPlayer( pPlayer );
}

//---------------------------.
// UI�G�f�B�^�̑O�ɒǉ��ōs���`��.
//---------------------------.
void CGameMainWidget::FastAddRender()
{
	// ��̕`��.
	m_pNightUI->Render();

	// ��UI�̕`��.
	m_pWindUI->Render();

	// �S�[���f���^�C���̕`��.
	m_pGoldenTimeUI->Render();
}

//---------------------------.
// UI�G�f�B�^�̌�ɒǉ��ōs���`��.
//---------------------------.
void CGameMainWidget::LateAddRender()
{
	// ���x�Q�[�W�̕`��.
	m_pHeightGaugeUI->Render();

	// �����b�N�̕`��.
	m_pRucksackUI->Render();

	// �����̕`��.
	CScore::Render();

	// ���ԃQ�[�W�̕`��.
	m_pTimeGaugeUI->Render();

	// �|�[�Y����.
	if ( CSceneManager::GetIsPause() ) {
		// �����L���O�̕`��.
		CRanking::RankingRender();

		// �X�e�[�^�X�o�[�̕`��.
		m_pStatusBarUI->Render();

		// �|�[�Y�̕`��.
		for ( int i = 0; i < Max; ++i ) m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
		return;
	}
	
	// �~�j�}�b�v�̕`��.
	CMiniMapUI::Render();

	// �ʒm�̕`��.
	CNotificationUI::Render();

	// ���X�̏ڍ׏��̕`��.
	m_pShopInfoUI->Render();

	// �X�e�[�^�X�o�[�̕`��.
	m_pStatusBarUI->Render();
	
	// �|�[�Y�̕`��.
	for ( int i = 0; i < Max; ++i ) m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
}

//---------------------------.
// �|�[�YUI�̍X�V.
//---------------------------.
void CGameMainWidget::PauseWindowUpdate()
{
	if ( m_SpriteStateList[BackGround].IsDisp == false ) return;

	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_AnimCnt += Math::ToRadian( 80.0f ) * m_DeltaTime;
	if ( m_AnimCnt >= Math::RADIAN_MID ) m_AnimCnt -= Math::RADIAN_MID;

	// �I�����̉摜�̕ύX.
	const SSize Size = m_pSpriteList[Choice_1]->GetSpriteState().Disp;
	switch ( m_Choice ) {
	case CHOICE_STARTCHOICE:	// �u�߂�v��I�������Ƃ�.
		m_SpriteStateList[Choice_1].AnimState.AnimNumber	= SelectToBack;
		m_SpriteStateList[Choice_2].AnimState.AnimNumber	= NotToClear;
		m_SpriteStateList[Yubi].Transform.Position			= m_SpriteStateList[Choice_1].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x		-= Size.w / 2.0f + 32.0f - 16.0f * sinf( m_AnimCnt );
		break;
	case CHOICE_ENDCHOICE:		// �u�A���v��I�������Ƃ�.
		m_SpriteStateList[Choice_1].AnimState.AnimNumber	= NotToBack;
		m_SpriteStateList[Choice_2].AnimState.AnimNumber	= SelectToClear;
		m_SpriteStateList[Yubi].Transform.Position			= m_SpriteStateList[Choice_2].Transform.Position;
		m_SpriteStateList[Yubi].Transform.Position.x		-= Size.w / 2.0f + 32.0f - 16.0f * sinf( m_AnimCnt );
		break;
	}
}

//---------------------------.
// �t�F�[�h�C���̏���.
//---------------------------.
void CGameMainWidget::UpdateFadeIn()
{
	// �|�[�Y��ʂ��g�債�Ȃ���t�F�[�h�C��������.
	m_PauseWindowScale += 3.0f * m_DeltaTime;
	// �|�[�Y��ʂ��t�F�[�h�C�����������Ȃ�.
	if ( m_PauseWindowScale >= Color::ALPHA_MAX ) {
		m_PauseWindowScale	= Color::ALPHA_MAX;
		m_ScalingSwitch		= SCALING_NONE;
	}

	SetPauseScaleAndAlpha( m_PauseWindowScale );
}

//---------------------------.
// �t�F�[�h�A�E�g�̏���.
//---------------------------.
void CGameMainWidget::UpdateFadeOut()
{
	// �|�[�Y��ʂ��k�����Ȃ���t�F�[�h�A�E�g������.
	m_PauseWindowScale -= 3.0f * m_DeltaTime;
	// �|�[�Y��ʂ��t�F�[�h�A�E�g���������Ȃ�.
	if ( m_PauseWindowScale <= Color::ALPHA_MIN ) {
		m_PauseWindowScale	= Color::ALPHA_MIN;
		m_ScalingSwitch		= SCALING_NONE;

		// �I����������������.
		m_Choice = CHOICE_STARTCHOICE;

		// �|�[�Y��ʂ��\���ɂ���.
		SwitchDisp( false );
	}

	SetPauseScaleAndAlpha( m_PauseWindowScale );
}

//---------------------------.
// �|�[�Y��ʂ̏���.
//---------------------------.
void CGameMainWidget::UpdatePauseWindow()
{
	if ( CInput::IsKeyDown( "Start" ) ) {
		// ���ʉ���炷.
		CSoundManager::PlaySE( CLOSE_SE );

		// �|�[�Y�����̏���.
		CSceneManager::SetIsPause( false );
		m_ScalingSwitch = SCALING_SHRINK;
	}

	// �I��.
	if ( CInput::IsKeyDown( "Up" ) ) {
		// �J�[�\�����グ��.
		m_Choice--;
		if ( m_Choice < CHOICE_STARTCHOICE ) m_Choice = CHOICE_ENDCHOICE;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Down" ) ) {
		// �J�[�\����������.
		m_Choice++;
		if ( m_Choice > CHOICE_ENDCHOICE ) m_Choice = CHOICE_STARTCHOICE;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Decision" ) ) {
		CSoundManager::PlaySE( "Decision" );

		switch ( m_Choice ) {
		case CHOICE_STARTCHOICE:	// �u�߂�v��I�������Ƃ�.
			// �|�[�Y��������.
			CSceneManager::SetIsPause( false );
			m_ScalingSwitch = SCALING_SHRINK;

			// SE�̍Đ�.
			CSoundManager::PlaySE( CLOSE_SE );
			break;
		case CHOICE_ENDCHOICE:		// �u�A���v��I�������Ƃ�.
			// ���U���g�V�[���ɑJ�ڂ���.
			CWind::StopWind();
			CScore::SetHomeEnd();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::MaskFade );
			break;
		}
	}
}

//---------------------------.
// �|�[�Y�������ꂽ�Ƃ��̏���.
//---------------------------.
void CGameMainWidget::UpdatePushPause()
{
	if ( CInput::IsKeyDown( "Start" ) == false ) return;

	// ���ʉ���炷.
	CSoundManager::PlaySE( "Decision" );
	CSoundManager::PlaySE( OPEN_SE );

	// �|�[�Y�̏���.
	CSceneManager::SetIsPause( true );
	m_ScalingSwitch = SCALING_EXPAND;
	SwitchDisp( true );

	// �U�����~������.
	CXInput::SetVibration( 0.0f, 0.0f );
}
