#include "StageEditor.h"
#include "..\..\Object\GameObject\Actor\StageManager\StageManager.h"
#include "..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"
#include "..\..\Utility\Input\Input.h"

namespace {
	constexpr char STAGE_FILE_PATH[] = "Data\\Parameter\\Stage.json";
}

CStageEditor::CStageEditor()
	: m_pStage			( nullptr )
	, m_SelectObject	( "ATypeApartment" )
{
}

CStageEditor::~CStageEditor()
{
}

//---------------------------.
// 初期化.
//---------------------------.
bool CStageEditor::Init()
{
	// ステージの初期化.
	m_pStage = std::make_unique<CStageManager>();
	m_pStage->Init();

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CStageEditor::Update( const float& DeltaTime )
{
	m_pStage->Update( DeltaTime );
	m_pStage->Update( DeltaTime );
}

//---------------------------.
// ImGui描画.
//---------------------------.
void CStageEditor::ImGuiRender()
{
	ImGui::Begin( "StageEditor" );
	if ( ImGui::BeginTabBar( "TabBar", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton ) ) {
		// シーンタブ.
		if ( ImGui::BeginTabItem( "Scene" ) ) {
			ImGui::BeginChild( ImGui::GetID( (void*) 0 ), ImVec2( 0.0f, 0.0f ), ImGuiWindowFlags_NoTitleBar );

			// ステージの設定.
			m_pStage->StageEditorImGuiRender();

			// 保存.
			if ( CImGuiManager::Button( "Save" ) || CKeyInput::IsANDKeyDown( VK_CONTROL, 'S' ) ) {
				FileManager::JsonSave( STAGE_FILE_PATH, m_pStage->GetStageData() );
				CMessageWindow::PushMessage( u8"ステージを保存しました", Color4::Magenta );
			}
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		// 追加タブ.
		if ( ImGui::BeginTabItem( "NewObject" ) ) {
			// 追加するオブジェクトのコンボボックス.
			CImGuiManager::Combo( u8"UIリスト", &m_SelectObject, { "ATypeApartment", "BTypeApartment", "Island" } );
			ImGui::SameLine();

			// スプライトの追加ボタン.
			if ( CImGuiManager::Button( "Add" ) ) {
				m_pStage->AddObject( m_SelectObject );
			}

			// ステージの情報の表示.
			ImGui::BeginChild( ImGui::GetID( (void*) 0 ), ImVec2( 0.0f, 0.0f ), ImGuiWindowFlags_NoTitleBar );
			m_pStage->StageStateImGuiRender();
			ImGui::EndChild();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

//---------------------------.
// モデルの描画.
//---------------------------.
void CStageEditor::ModelRender()
{
	m_pStage->Sprite3DRender_B();
	m_pStage->Render();
	m_pStage->Sprite3DRender_A();
}

//---------------------------.
// UIの描画.
//---------------------------.
void CStageEditor::UIRender()
{
}
