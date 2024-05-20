#include "Title.h"
#include "..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\TitleWidget\TitleWidget.h"
#include "..\..\Utility\Constant\Constant.h"
#include "..\..\Object\Score\Ranking\Ranking.h"
#include "..\..\Object\GameObject\Widget\UIWidget\Title\CofilmationWindowUI\ConfilmationWindowUI.h"
#include "..\..\Object\Light\Light.h"

namespace {
	constexpr char  DOOR_POSITION_BONE_NAME[]	= "p_house_b";						// ドアの位置のボーン名.
	constexpr float INIT_CAMERA_POS[3]			= {  7.374f, 7.050f, -16.516f };	// カメラの位置.
	constexpr float INIT_CAMERA_LOOK[3]			= {  7.086f, 7.000f, -15.558f };	// カメラの注視点.
	constexpr float INIT_CAMERA_ROT[3]			= {  0.050f, 2.850f,   0.000f };	// カメラの回転値.
	constexpr float INIT_LIGHT_DIRECTION[3]		= { -1.000f, 1.170f,   2.772f };	// 初期のライトの位置.
	constexpr float INIT_LIGHT_INTENSITY		= 1.374f;							// 初期のライトの強さ.
}

CTitle::CTitle()
	: m_pHouse					( nullptr )
	, m_pDoor					( nullptr )
	, m_pTitleWidget			( nullptr )
	, m_pConfilmationWindowUI	( nullptr )
	, m_DoorTransform			()
{
}

CTitle::~CTitle()
{
}

//---------------------------.
// 読み込み.
//---------------------------.
bool CTitle::Init()
{
	// 定数の初期化.
	CConstant::TitleLoad();

	// 家のメッシュtの取得.
	m_pHouse	= CMeshResource::GetSkin( "p_house_s" );
	m_pDoor		= CMeshResource::GetSkin( "p_house_door_s" );

	// ドアの位置の設定.
	m_DoorTransform.Position = m_pHouse->GetPosFromBone( DOOR_POSITION_BONE_NAME );

	m_pTitleWidget = std::make_unique<CTitleWidget>();
	m_pTitleWidget->Init();

	m_pConfilmationWindowUI = std::make_unique<CConfilmationWindowUI>();
	m_pConfilmationWindowUI->Init();
	// タイトル画面のウィジェットに確認画面を設定する.
	m_pTitleWidget->SetConfilmationWindowUI(*m_pConfilmationWindowUI.get());

	// カメラの設定.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// カメラの設定.
	m_pCameras->GetCamera<CNormalCamera>()->SetPosition(		INIT_CAMERA_POS );
	m_pCameras->GetCamera<CNormalCamera>()->SetLookPosition(	INIT_CAMERA_LOOK );
	m_pCameras->GetCamera<CNormalCamera>()->SetRotation(		INIT_CAMERA_ROT );

	// BGM名の設定.
	m_BGMName = "Title";
	
	// ライトの初期化.
	CLight::SetDirection( INIT_LIGHT_DIRECTION );
	CLight::SetIntensity( INIT_LIGHT_INTENSITY );

	// ランキングの読み込み.
	CRanking::Load();
	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CTitle::Update( const float& DeltaTime )
{
	// UIの更新.
	m_pTitleWidget->Update( DeltaTime );

	// 確認画面の更新.
	m_pConfilmationWindowUI->Update( DeltaTime );

	// BGMの再生.
	CSoundManager::PlayBGM( m_BGMName );
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CTitle::ModelRender()
{
	m_pHouse->Render();
	m_pDoor->Render( &m_DoorTransform );
}

//---------------------------.
// スプライト(UI)の描画.
//---------------------------.
void CTitle::SpriteUIRender()
{
	// UIの描画.
	m_pTitleWidget->Render();

	// ランキングの描画.
	CRanking::RankingRender();

	// 確認画面の描画.
	m_pConfilmationWindowUI->Render();
}

//---------------------------.
// スプライト(3D)/Effectの描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CTitle::Sprite3DRender_A()
{
}
void CTitle::Sprite3DRender_B()
{
}
