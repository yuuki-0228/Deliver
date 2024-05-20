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
// 初期化.
//---------------------------.
bool CGameMain::Init()
{
	// 定数の初期化.
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

	// プレイヤーを開始位置に設定する.
	m_pStageManager->SetPlayerStartPos( m_pPlayer.get() );

	CWind::SetPlayer( m_pPlayer.get() );
	m_GameMainWidget->SetPlayer( m_pPlayer.get() );
	
	// ライトの初期化.
	CLight::SetDirection( INIT_LIGHT_DIRECTION );
	CLight::SetIntensity( INIT_LIGHT_INTENSITY );

	// BGMの設定.
	m_BGMName = "12-13";
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CGameMain::Update( const float& DeltaTime )
{
	// ポーズ中でないなら.
	if ( CSceneManager::GetIsPause() == false ) {
		// ゲームが終了したか.
		if ( CTime::GetIsGameEnd() ) {
			CLinking::AllLinkEnd();
		}

		// 制限時間が一定数経過したか.
		if ( CTime::GetIsClearSceneMove() ) {
			CScore::SetGameEnd();
			CWind::StopWind();
			CSceneManager::SceneChange( ESceneList::GameClear, EFadeType::NormalFade );
		}

		// 更新.
		CTime::Update( DeltaTime );
		CWind::Update( DeltaTime );
		m_pPlayer->Update( DeltaTime );
		m_pStageManager->Update( DeltaTime );
		CLinking::Update( DeltaTime );

		// 当たり判定処理.
		CActorCollisionManager::Collision();

		// 当たり判定終了後の更新.
		CDeliveryManager::LateUpdate( DeltaTime );
		m_pPlayer->LateUpdate( DeltaTime );
		m_pStageManager->LateUpdate( DeltaTime );

		// デバックの更新.
		m_pPlayer->DebugUpdate( DeltaTime );
		m_pStageManager->DebugUpdate( DeltaTime );
		CWind::DebugUpdate( DeltaTime );
		CTime::DebugUpdate( DeltaTime );
	}

	// UIの更新.
	m_GameMainWidget->Update( DeltaTime );

	// BGM/SEの更新.
	CWind::SEUpdate( DeltaTime );
	BGMUpdate();
}

//---------------------------.
// 描画.
//---------------------------.
void CGameMain::ModelRender()
{
	m_pStageManager->Render();
	m_pPlayer->Render();
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CGameMain::SpriteUIRender()
{
	m_GameMainWidget->Render();
}

//---------------------------.
// スプライト(3D)/Effectの描画.
//	_A：後に表示される / _B：先に表示される.
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
// BGMの更新.
//---------------------------.
void CGameMain::BGMUpdate()
{
	// BGMを変える.
	const int NowHour = CTime::GetHour();
	if (		12 <= NowHour && NowHour < 13 ) m_BGMName = "12-13";
	else if (	13 <= NowHour && NowHour < 15 ) m_BGMName = "13-15";
	else if (	15 <= NowHour && NowHour < 18 ) m_BGMName = "15-18";
	else if (	18 <= NowHour && NowHour < 21 ) m_BGMName = "18-21";
	else if (	21 <= NowHour && NowHour < 24 ) m_BGMName = "21-24";

	// BGMを少しずつ小さくしていく.
	if ( CTime::GetIsGameEnd() ) {
		const float NewTime = CTime::GetElapsedTime();
		const float Volume	= 1.0f - ( NewTime - CONSTANT.TIME_MAX ) / ( CONSTANT.CLEAR_SCENE_MOVE_TIME_MAX - CONSTANT.TIME_MAX );
		CSoundManager::SetBGMVolume( m_BGMName, Volume );
	}

	// BGMを再生.
	CSoundManager::PlayFadeBGM( m_BGMName, 10 );
}
