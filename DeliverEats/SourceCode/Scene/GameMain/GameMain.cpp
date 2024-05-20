#include "GameMain.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Object\Light\Light.h"
#include "..\..\Object\Score\Score.h"
#include "..\..\Object\Time\Time.h"
#include "..\..\Object\GameObject\Actor\Character\Player\Player.h"
#include "..\..\Object\GameObject\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\Object\GameObject\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\Object\GameObject\Actor\StageManager\StageManager.h"
#include "..\..\Object\GameObject\Actor\Wind\Wind.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\GameMainWidget\GameMainWidget.h"
#include "..\..\Object\GameObject\ActorCollisionManager\ActorCollisionManager.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\Constant\Constant.h"

namespace {
	constexpr float INIT_LIGHT_DIRECTION[3] = { 0.0, 1.0f, 0.0f };
	constexpr float INIT_LIGHT_INTENSITY	= 1.0f;
}

CGameMain::CGameMain()
	: m_pPlayer			( nullptr )
	, m_pStageManager	( nullptr )
	, m_GameMainWidget	( nullptr )
{
}

CGameMain::~CGameMain()
{
	CSoundManager::ResetBGMVolume( m_BGMName );
}

//---------------------------.
// ������.
//---------------------------.
bool CGameMain::Init()
{
	// �萔�̏�����.
	CConstant::GameMainLoad();

	m_pPlayer			= std::make_shared<CPlayer>();
	m_pStageManager		= std::make_shared<CStageManager>();
	m_GameMainWidget	= std::make_unique<CGameMainWidget>();

	CDeliveryManager::Init();
	m_pPlayer->Init();
	m_pStageManager->Init();
	m_GameMainWidget->Init();
	CWind::Init();
	CScore::Init();
	CTime::Init();
	CLinking::Init();
	CDeliveryManager::EndInit();

	// �v���C���[���J�n�ʒu�ɐݒ肷��.
	m_pStageManager->SetPlayerStartPos( m_pPlayer.get() );

	CWind::SetPlayer( m_pPlayer.get() );
	m_GameMainWidget->SetPlayer( m_pPlayer.get() );
	
	// ���C�g�̏�����.
	CLight::SetDirection( INIT_LIGHT_DIRECTION );
	CLight::SetIntensity( INIT_LIGHT_INTENSITY );

	// BGM�̐ݒ�.
	m_BGMName = "12-13";
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGameMain::Update( const float& DeltaTime )
{
	// �|�[�Y���łȂ��Ȃ�.
	if ( CSceneManager::GetIsPause() == false ) {
		// �Q�[�����I��������.
		if ( CTime::GetIsGameEnd() ) {
			CLinking::AllLinkEnd();
		}

		// �������Ԃ���萔�o�߂�����.
		if ( CTime::GetIsClearSceneMove() ) {
			CScore::SetGameEnd();
			CWind::StopWind();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade );
		}

		// �X�V.
		CTime::Update( DeltaTime );
		CWind::Update( DeltaTime );
		m_pPlayer->Update( DeltaTime );
		m_pStageManager->Update( DeltaTime );
		CLinking::Update( DeltaTime );

		// �����蔻�菈��.
		CActorCollisionManager::Collision();

		// �����蔻��I����̍X�V.
		CDeliveryManager::LateUpdate( DeltaTime );
		m_pPlayer->LateUpdate( DeltaTime );
		m_pStageManager->LateUpdate( DeltaTime );

		// �f�o�b�N�̍X�V.
		m_pPlayer->DebugUpdate( DeltaTime );
		m_pStageManager->DebugUpdate( DeltaTime );
		CWind::DebugUpdate( DeltaTime );
		CTime::DebugUpdate( DeltaTime );
	}

	// UI�̍X�V.
	m_GameMainWidget->Update( DeltaTime );

	// BGM/SE�̍X�V.
	CWind::SEUpdate( DeltaTime );
	BGMUpdate();
}

//---------------------------.
// �`��.
//---------------------------.
void CGameMain::ModelRender()
{
	m_pStageManager->Render();
	m_pPlayer->Render();
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CGameMain::SpriteUIRender()
{
	m_GameMainWidget->Render();
}

//---------------------------.
// �X�v���C�g(3D)/Effect�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CGameMain::Sprite3DRender_A()
{
	m_pStageManager->Sprite3DRender_A();
	m_pPlayer->Sprite3DRender_A();
}
void CGameMain::Sprite3DRender_B()
{
	m_pStageManager->Sprite3DRender_B();
	m_pPlayer->Sprite3DRender_B();
}

//---------------------------.
// BGM�̍X�V.
//---------------------------.
void CGameMain::BGMUpdate()
{
	// BGM��ς���.
	const int NowHour = CTime::GetHour();
	if (		12 <= NowHour && NowHour < 13 ) m_BGMName = "12-13";
	else if (	13 <= NowHour && NowHour < 15 ) m_BGMName = "13-15";
	else if (	15 <= NowHour && NowHour < 18 ) m_BGMName = "15-18";
	else if (	18 <= NowHour && NowHour < 21 ) m_BGMName = "18-21";
	else if (	21 <= NowHour && NowHour < 24 ) m_BGMName = "21-24";

	// BGM�����������������Ă���.
	if ( CTime::GetIsGameEnd() ) {
		const float NewTime = CTime::GetElapsedTime();
		const float Volume	= 1.0f - ( NewTime - CONSTANT.TIME_MAX ) / ( CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX - CONSTANT.TIME_MAX );
		CSoundManager::SetBGMVolume( m_BGMName, Volume );
	}

	// BGM���Đ�.
	CSoundManager::PlayFadeBGM( m_BGMName, 10 );
}
