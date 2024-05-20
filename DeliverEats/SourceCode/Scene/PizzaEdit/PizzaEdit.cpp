#include "PizzaEdit.h"
#include "..\..\Editor\PizzaEditor\PizzaEditor.h"
#include "..\..\Common\DirectX\DirectX11.h"
#include "..\..\Object\GameObject\Actor\Wind\Wind.h"
#include "..\..\Utility\Random\Random.h"

CPizzaEdit::CPizzaEdit()
	: m_pPizzaEditor	( nullptr )
{
	// �J�����̕ύX���֎~����.
	CCameraManager::SetIsCameraChangeLock( true );
}

CPizzaEdit::~CPizzaEdit()
{
	// �J�����̕ύX�̋֎~������.
	CCameraManager::SetIsCameraChangeLock( false );

	// BGM�̒�~.
	CSoundManager::StopSE( m_BGMName );
}

//---------------------------.
// ������.
//---------------------------.
bool CPizzaEdit::Init()
{
	// UI�G�f�B�^�̏�����.
	m_pPizzaEditor = std::make_unique<CPizzaEditor>();
	m_pPizzaEditor->Init();

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::FreeCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CFreeCamera>() );

	// ������v���C���[���폜���Ă���.
	CWind::SetPlayer( nullptr );

	// BGM�̐ݒ�.
	m_BGMName = "PizzaEditor_" + std::to_string( CRandom::GetRand( 1, 20 ) );
	CSoundManager::PlaySE( m_BGMName );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CPizzaEdit::Update( const float& DeltaTime )
{
	// UI�G�f�B�^�̍X�V.
	m_pPizzaEditor->Update( DeltaTime );

	// BGM�̍Đ�.
	if ( CSoundManager::IsStoppedSE( m_BGMName ) == false ) return;
	m_BGMName = "PizzaEditor_" + std::to_string( CRandom::GetRand( 1, 20 ) );
	CSoundManager::PlaySE( m_BGMName );
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CPizzaEdit::ModelRender()
{
	m_pPizzaEditor->ModelRender();
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CPizzaEdit::SpriteUIRender()
{
	m_pPizzaEditor->ImGuiRender();
	m_pPizzaEditor->UIRender();
}
