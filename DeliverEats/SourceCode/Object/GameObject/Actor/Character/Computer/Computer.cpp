#include "Computer.h"
#include "..\DeliveryManager\DeliveryManager.h"
#include "..\DeliveryManager\Customer\Customer.h"
#include "..\DeliveryManager\Employee\Employee.h"
#include "..\..\..\Widget\UIWidget\GameMain\MiniMapUI\MiniMapUI.h"
#include "..\..\..\..\..\Common\EffectManager\EffectManager.h"
#include "..\..\..\..\..\Common\Mesh\CollisionMesh\CollisionMesh.h"
#include "..\..\..\..\..\Common\Mesh\SkinMesh\SkinMesh.h"
#include "..\..\..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\..\Utility\Math\Math.h"

namespace{
	constexpr char		JETPACK_CENTER_BONE[]	= "p_jet_center";			// �W�F�b�g�p�b�N�̒��S�̈ʒu�̃{�[����[Body].
	constexpr char		L_JETPACK_JETPOS_BONE[]	= "jetpack_l_jet_place";	// ���W�F�b�g�p�b�N�̕��ˌ��̃{�[����[LJetPack].
	constexpr char		L_JETPACK_SETPOS_BONE[]	= "p_jet_l";				// ���W�F�b�g�p�b�N�̐ݒu�ꏊ�̃{�[����[Body].
	constexpr char		R_JETPACK_JETPOS_BONE[]	= "jetpack_r_jet_place";	// �E�W�F�b�g�p�b�N�̕��ˌ��̃{�[����[RJetPack].
	constexpr char		R_JETPACK_SETPOS_BONE[]	= "p_jet_r";				// ���W�F�b�g�p�b�N�̐ݒu�ꏊ�̃{�[����[Body].
	constexpr char		BAG_SETPOS_BONE[]		= "p_jet_center";			// �����b�N�̐ݒu�ꏊ�̃{�[����[Bag].
	constexpr char		LOOK_POS_BONE[]			= "neck";					// �X��/���q���񂪌���ʒu.

	// �X�L�����b�V����No.
	enum enMeshNo : unsigned char {
		Mesh_Body,		// �\������v���C���[�{��.
		Mesh_LJetPack,	// �\�����鍶�W�F�b�g�p�b�N.
		Mesh_RJetPack,	// �\������E�W�F�b�g�p�b�N.
		Mesh_Bag,		// �\�����郊���b�N.

		Mesh_Max,		// ���b�V���̌�.
	} typedef EMeshNo;
}


CComputer::CComputer()
	: m_pSkinMesh		()
	, m_TransformList	()
{
}

CComputer::~CComputer()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CComputer::Init()
{
	// ���b�V���̎擾.
	m_pSkinMesh.resize( Mesh_Max );
	m_pSkinMesh[Mesh_Body]		= CMeshResource::GetSkin( "player_s" );
	m_pSkinMesh[Mesh_LJetPack]	= CMeshResource::GetSkin( "l_jet_s" );
	m_pSkinMesh[Mesh_RJetPack]	= CMeshResource::GetSkin( "r_jet_s" );
	m_pSkinMesh[Mesh_Bag]		= CMeshResource::GetSkin( "p_bag_s" );

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CComputer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

}

//---------------------------.
// �����蔻��I����Ăяo�����X�V.
//---------------------------.
void CComputer::LateUpdate( const float& DeltaTime )
{
}

//---------------------------.
// �`��.
//---------------------------.
void CComputer::Render()
{
	TransformUpdate();

	// �v���C���[�̕`��.
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_pSkinMesh[i]->Render( &m_TransformList[i] );
	}
}

//---------------------------.
// �X�v���C�g(3D)�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CComputer::Sprite3DRender_A()
{
}

//---------------------------.
// �����蔻��֐�.
//---------------------------.
void CComputer::Collision( CActor* pActor )
{
}

//---------------------------.
// �g�����X�t�H�[���̍X�V.
//---------------------------.
void CComputer::TransformUpdate()
{
	// �v���C���[�{�̂̃��f���̍��W��ݒ肷��.
	m_pSkinMesh[Mesh_Body]->SetTransform( m_TransformList[Mesh_Body] );

	// �W�F�b�g�p�b�N�̈ʒu�̍X�V.
	m_TransformList[Mesh_LJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( L_JETPACK_SETPOS_BONE );
	m_TransformList[Mesh_RJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( R_JETPACK_SETPOS_BONE );

	// �����b�N�̈ʒu�̍X�V.
	m_TransformList[Mesh_Bag].Position		= m_pSkinMesh[Mesh_Body]->GetPosFromBone( BAG_SETPOS_BONE );

//	// �������Ă���ꍇ�����b�N��������Ɉړ�������.
//	const float AddY = m_Transform.Position.y - m_TransformList[Mesh_Body].Position.y;
//	m_TransformList[Mesh_Bag].Position.y += sinf( Math::ToRadian( m_GetFoodAngCnt ) );
//	if ( AddY > 0.0f ) m_TransformList[Mesh_Bag].Position.y += AddY;

	// �O���Ŏg�p����g�����X�t�H�[�����v���C���[�{�̂Ɠ����ɂ���.
	m_Transform	= m_TransformList[Mesh_Body];
}
