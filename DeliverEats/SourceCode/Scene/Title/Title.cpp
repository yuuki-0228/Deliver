#include "Title.h"
#include "..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\TitleWidget\TitleWidget.h"
#include "..\..\Utility\Constant\Constant.h"
#include "..\..\Object\Score\Ranking\Ranking.h"
#include "..\..\Object\GameObject\Widget\UIWidget\Title\CofilmationWindowUI\ConfilmationWindowUI.h"
#include "..\..\Object\Light\Light.h"

namespace {
	constexpr char  DOOR_POSITION_BONE_NAME[]	= "p_house_b";						// �h�A�̈ʒu�̃{�[����.
	constexpr float INIT_CAMERA_POS[3]			= {  7.374f, 7.050f, -16.516f };	// �J�����̈ʒu.
	constexpr float INIT_CAMERA_LOOK[3]			= {  7.086f, 7.000f, -15.558f };	// �J�����̒����_.
	constexpr float INIT_CAMERA_ROT[3]			= {  0.050f, 2.850f,   0.000f };	// �J�����̉�]�l.
	constexpr float INIT_LIGHT_DIRECTION[3]		= { -1.000f, 1.170f,   2.772f };	// �����̃��C�g�̈ʒu.
	constexpr float INIT_LIGHT_INTENSITY		= 1.374f;							// �����̃��C�g�̋���.
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
// �ǂݍ���.
//---------------------------.
bool CTitle::Init()
{
	// �萔�̏�����.
	CConstant::TitleLoad();

	// �Ƃ̃��b�V��t�̎擾.
	m_pHouse	= CMeshResource::GetSkin( "p_house_s" );
	m_pDoor		= CMeshResource::GetSkin( "p_house_door_s" );

	// �h�A�̈ʒu�̐ݒ�.
	m_DoorTransform.Position = m_pHouse->GetPosFromBone( DOOR_POSITION_BONE_NAME );

	m_pTitleWidget = std::make_unique<CTitleWidget>();
	m_pTitleWidget->Init();

	m_pConfilmationWindowUI = std::make_unique<CConfilmationWindowUI>();
	m_pConfilmationWindowUI->Init();
	// �^�C�g����ʂ̃E�B�W�F�b�g�Ɋm�F��ʂ�ݒ肷��.
	m_pTitleWidget->SetConfilmationWindowUI(*m_pConfilmationWindowUI.get());

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// �J�����̐ݒ�.
	m_pCameras->GetCamera<CNormalCamera>()->SetPosition(		INIT_CAMERA_POS );
	m_pCameras->GetCamera<CNormalCamera>()->SetLookPosition(	INIT_CAMERA_LOOK );
	m_pCameras->GetCamera<CNormalCamera>()->SetRotation(		INIT_CAMERA_ROT );

	// BGM���̐ݒ�.
	m_BGMName = "Title";
	
	// ���C�g�̏�����.
	CLight::SetDirection( INIT_LIGHT_DIRECTION );
	CLight::SetIntensity( INIT_LIGHT_INTENSITY );

	// �����L���O�̓ǂݍ���.
	CRanking::Load();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CTitle::Update( const float& DeltaTime )
{
	// UI�̍X�V.
	m_pTitleWidget->Update( DeltaTime );

	// �m�F��ʂ̍X�V.
	m_pConfilmationWindowUI->Update( DeltaTime );

	// BGM�̍Đ�.
	CSoundManager::PlayBGM( m_BGMName );
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CTitle::ModelRender()
{
	m_pHouse->Render();
	m_pDoor->Render( &m_DoorTransform );
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CTitle::SpriteUIRender()
{
	// UI�̕`��.
	m_pTitleWidget->Render();

	// �����L���O�̕`��.
	CRanking::RankingRender();

	// �m�F��ʂ̕`��.
	m_pConfilmationWindowUI->Render();
}

//---------------------------.
// �X�v���C�g(3D)/Effect�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CTitle::Sprite3DRender_A()
{
}
void CTitle::Sprite3DRender_B()
{
}
