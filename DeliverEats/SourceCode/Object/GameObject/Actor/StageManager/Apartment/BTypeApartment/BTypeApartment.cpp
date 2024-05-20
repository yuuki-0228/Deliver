#include "BTypeApartment.h"
#include "..\ApartmentFloor\BTypeApartmentFloor\BTypeApartmentFloor.h"
#include "..\..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\Camera\CameraManager\CameraManager.h"
#include "..\..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\..\Utility\ImGuiManager\DebugWindow\DebugWindow.h"

#include "..\..\..\..\..\..\Utility\Random\Random.h"

namespace {
	constexpr char ROOFTOP_UP_BONE_NAME[] = "apart_8x2_top2";	// ����̈�ԏ�̃{�[����.
}

CBTypeApartment::CBTypeApartment()
	: m_pEmptyFloor			()
	, m_pFloor				()
	, m_pRooftop			( nullptr )
	, m_pCollisionMesh		( nullptr )
	, m_CollisionTransform	()
	, m_CreatePosition		( INIT_FLOAT3 )
	, m_Alpha				( Color::ALPHA_MAX )
{
}

CBTypeApartment::~CBTypeApartment()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CBTypeApartment::Init()
{
	// ����̏�����.
	m_pRooftop = CMeshResource::GetSkin( "apart_8x2_top_s" );
	m_pRooftop->SetDistanceAlphaOrMore( true, CONSTANT.START_DISTANCE, CONSTANT.ALPHA_DISTANCE );
	m_Transform.Position = m_pFloor.back()->GetSettingPosition();
	m_pRooftop->SetPosition( m_Transform.Position );
	m_CollisionTransform			= m_Transform;
	m_CollisionTransform.Position	= m_pRooftop->GetPosFromBone( ROOFTOP_UP_BONE_NAME );

	// �^�O�̐ݒ�.
	m_ObjectTag = EObjectTag::ApartmentB;

	InitCollision();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CBTypeApartment::Update( const float& DeltaTime )
{
	for ( auto& f : m_pEmptyFloor	) f->Update( DeltaTime );
	for ( auto& f : m_pFloor		) f->Update( DeltaTime );

	// �����ɂ��A���t�@�t�F�[�h�̍X�V.
//	m_IsDisp = true;
//	DistanceFadeUpdate( CONSTANT.START_DISTANCE + CONSTANT.FLOOR_SIZE, CONSTANT.ALPHA_DISTANCE );

	const D3DXVECTOR3&	DistanceVector	= m_Transform.Position - CCameraManager::GetPosition();
	const float			Distance		= D3DXVec3Length( &DistanceVector );
	m_IsDisp = Distance <= CONSTANT.START_DISTANCE * 3.0f;

	// �J�����ɂ��A���t�@�l�̍X�V.
	CameraAlphaUpdate();

	// �~�j�}�b�v�ɃA�C�R����ǉ�����.
	CMiniMapUI::PushApartmentBIcon( this );

	UpdateCollision();
	CActorCollisionManager::PushObject( this );
}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CBTypeApartment::LateUpdate( const float& DeltaTime )
{
	for ( auto& f : m_pFloor ) f->LateUpdate( DeltaTime );
}

//---------------------------.
// �`��.
//---------------------------.
void CBTypeApartment::Render()
{
	for ( auto& f : m_pEmptyFloor ) {
		f->SetAlpha( m_Alpha );
		f->Render();
	}
	for ( auto& f : m_pFloor ) {
		f->SetAlpha( m_Alpha );
		f->Render();
	}

	// ����̕`��.
	if ( m_IsDisp == false ) return;
	m_pRooftop->SetDither( true );
	m_pRooftop->SetAlpha( m_Distance - ( Color::ALPHA_MAX - m_Alpha ) );
	m_pRooftop->Render( &m_Transform );
	m_pRooftop->SetDither( false );

	CCollisionRender::PushMesh( m_pCollisionMesh, &m_CollisionTransform );
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CBTypeApartment::Sprite3DRender_A()
{
	for ( auto& f : m_pFloor ) f->Sprite3DRender_A();
}

//---------------------------.
// �X�e�[�W�G�f�B�^��ImGui�̕`��.
//---------------------------.
void CBTypeApartment::StageEditorImGuiRender()
{
	// �}���V�����̈ʒu�̐ݒ�.
	const D3DXVECTOR3&	OldApartmentPos = m_pEmptyFloor[0]->GetPosition();
	D3DXVECTOR3			ApartmentPos	= OldApartmentPos;
	CImGuiManager::InputVec3( u8"�}���V�����̔z�u���W", &ApartmentPos );
	if ( ApartmentPos.y != OldApartmentPos.y ) ApartmentPos.y = OldApartmentPos.y;
	if ( ApartmentPos	!= OldApartmentPos ) {
		m_CreatePosition = ApartmentPos;

		// �����̖����t���A�̈ړ�.
		for ( auto& f : m_pEmptyFloor ) {
			f->FloorMove( m_CreatePosition );
			m_CreatePosition = f->GetSettingPosition();
		}

		// �t���A�̈ړ�.
		for ( auto& f : m_pFloor ) {
			f->FloorMove( m_CreatePosition );
			m_CreatePosition = f->GetSettingPosition();
		}
		m_Transform.Position = m_CreatePosition;
	}

	// �K�w�̐ݒ�.
	const int	OldFloarSize	= static_cast<int>( m_pFloor.size() );
	int			FloarSize		= OldFloarSize;
	ImGui::SetNextTreeNodeOpen( true, ImGuiCond_Once );
	if ( ImGui::TreeNode( u8"�}���V�����̊K�w" ) ) {
		ImGui::InputInt( "##FloorSize", &FloarSize );
		ImGui::TreePop();
	}
	if ( FloarSize <= 0 ) FloarSize = 1;
	ImGui::Separator();

	// �K�w��������.
	if ( FloarSize > OldFloarSize ) {
		// �t���A�̒ǉ�.
		const int Num = FloarSize - OldFloarSize;
		for ( int i = 0; i < Num; ++i ) {
			m_pFloor.emplace_back( std::make_unique<CBTypeApartmentFloor>() );
			m_pFloor.back()->SetPosition( m_CreatePosition );

			// �t���A�̍쐬.
			m_pFloor.back()->SetEmptyFloor();
			m_pFloor.back()->Create( nullptr );
			m_pFloor.back()->Init();
			m_CreatePosition = m_pFloor.back()->GetSettingPosition();
		}
		m_Transform.Position = m_CreatePosition;
	}

	// �K�w��������.
	if ( FloarSize < OldFloarSize ) {
		// �t���A�̍폜.
		const int Num = OldFloarSize - FloarSize;
		for ( int i = 0; i < Num; ++i ) m_pFloor.pop_back();
		m_CreatePosition	 = m_pFloor.back()->GetSettingPosition();
		m_Transform.Position = m_CreatePosition;
	}

	// �t���A�̐ݒ�.
	const int FloorSize = static_cast<int>( m_pFloor.size() );
	for ( int i = 0; i < FloorSize; ++i ) {
		if ( CImGuiManager::Button( std::string( "x##" + std::to_string( i ) ).c_str(), FloorSize > 1, Color4::Red ) ) {
			m_pFloor.erase( m_pFloor.begin() + i );

			// �t���A�̈ʒu�̍X�V.
			m_CreatePosition = m_pEmptyFloor.back()->GetSettingPosition();
			for ( auto& f : m_pFloor ) {
				f->FloorMove( m_CreatePosition );
				m_CreatePosition = f->GetSettingPosition();
			}
			m_Transform.Position = m_CreatePosition;
			break;
		}
		ImGui::SameLine();
		if ( ImGui::TreeNode( std::string( "Floor_" + std::to_string( i ) ).c_str() ) ) {
			ImGui::Separator();
			m_pFloor[i]->StageEditorImGuiRender();
			ImGui::TreePop();
		}
		ImGui::Separator();
	}
}

//---------------------------.
// �t���A�̏���ImGui�ŕ`��.
//---------------------------.
void CBTypeApartment::FloorStateImGuiRender()
{
	ImGui::Text( u8"�K�w            : %dF", m_pFloor.size() );
	ImGui::Separator();
	int ShopNum = 0, RoomNum = 0, EmptyRoomNum = 0;
	for ( auto& f : m_pFloor ) {
		std::pair<int, std::pair<int, int>> Num = f->GetRoomNum();
		ShopNum			+= Num.first;
		RoomNum			+= Num.second.first;
		EmptyRoomNum	+= Num.second.second;
	}
	ImGui::Text( u8"Shop            : %d��", ShopNum );
	ImGui::Text( u8"Room            : %d��", RoomNum );
	ImGui::Text( u8"EmptyRoom       : %d��", EmptyRoomNum );
	ImGui::Separator();
}

//---------------------------.
// �}���V�����̃f�[�^�̎擾.
//---------------------------.
json CBTypeApartment::GetApartmentData()
{
	json Out;
	const D3DXPOSITION3& Pos = m_pEmptyFloor[0]->GetPosition();
	Out["Position"]["x"] = Pos.x;
	Out["Position"]["z"] = Pos.z;
	for ( auto& f : m_pFloor ) Out["Floar"].emplace_back( f->GetFloorData() );
	return Out;
}

//---------------------------.
// �}���V�����̍쐬.
//---------------------------.
void CBTypeApartment::Create( json j )
{
	// �}���V�����̍쐬����ꏊ�̎擾.
	m_CreatePosition.x = j["Position"]["x"];
	m_CreatePosition.y = CConstant::GetStage().CLOUD_DOWN_POS_Y;
	m_CreatePosition.z = j["Position"]["z"];

	// �����̖����t���A��ςݏグ��.
	m_pEmptyFloor.resize( CONSTANT.START_NOROOM_FLOOR_NUM );
	for ( int i = 0; i < CONSTANT.START_NOROOM_FLOOR_NUM; ++i ) {
		m_pEmptyFloor[i] = std::make_unique<CBTypeApartmentFloor>();
		m_pEmptyFloor[i]->SetPosition( m_CreatePosition );

		// �t���A�̍쐬.
		m_pEmptyFloor[i]->SetEmptyFloor();
		m_pEmptyFloor[i]->Create( nullptr );
		m_pEmptyFloor[i]->Init();
		m_CreatePosition = m_pEmptyFloor[i]->GetSettingPosition();
	}

	// �t���A��ςݏグ�Ă���.
	const int FloarSize = static_cast<int>( j["Floar"].size() );
	m_pFloor.resize( FloarSize );
	for ( int i = 0; i < FloarSize; ++i ) {
		m_pFloor[i] = std::make_unique<CBTypeApartmentFloor>();
		m_pFloor[i]->SetPosition( m_CreatePosition );

		// �t���A�̍쐬.
		if ( j["Floar"][i] == nullptr ) m_pFloor[i]->SetEmptyFloor();
		m_pFloor[i]->Create( j["Floar"][i] );
		m_pFloor[i]->Init();
		m_CreatePosition = m_pFloor[i]->GetSettingPosition();
	}
}

//---------------------------.
// �����蔻��̏�����.
//---------------------------.
void CBTypeApartment::InitCollision()
{
	m_pCollisionMesh = CMeshResource::GetCollision( "apart_8x2_c" );

	m_pCollisions = std::make_unique<CCollisions>();
	m_pCollisions->InitCollision( ECollType::Mesh );
	m_pCollisions->GetCollision<CMesh>()->SetMesh( m_pCollisionMesh->GetMeshList() );
}

//---------------------------.
// �����蔻��̍X�V.
//---------------------------.
void CBTypeApartment::UpdateCollision()
{
	m_pCollisionMesh->SetTransform( m_CollisionTransform );
	m_pCollisions->GetCollision<CMesh>()->SetTransform( m_CollisionTransform );
}

//---------------------------.
// �J�����ɂ��A���t�@�l�̍X�V.
//---------------------------.
void CBTypeApartment::CameraAlphaUpdate()
{
	if ( m_IsDisp == false ) return;

	// �J�������B���Ă��邩.
	bool IsHit = Coll::IsRayToMesh(
		CCameraManager::GetRay(),
		m_pCollisions->GetCollision<CMesh>() );

	// �����ɂ��Ă���.
	if ( IsHit ) {
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN ) return;
		m_Alpha -= CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha <= CONSTANT.CAMERA_ALPHA_MIN )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MIN;
	}
	// �s�����ɂ��Ă���.
	else {
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX ) return;
		m_Alpha += CONSTANT.CAMERA_ALPHA_SPEED;
		if ( m_Alpha >= CONSTANT.CAMERA_ALPHA_MAX )
			m_Alpha = CONSTANT.CAMERA_ALPHA_MAX;
	}
}