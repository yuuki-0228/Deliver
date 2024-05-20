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
	constexpr char		JETPACK_CENTER_BONE[]	= "p_jet_center";			// ジェットパックの中心の位置のボーン名[Body].
	constexpr char		L_JETPACK_JETPOS_BONE[]	= "jetpack_l_jet_place";	// 左ジェットパックの噴射口のボーン名[LJetPack].
	constexpr char		L_JETPACK_SETPOS_BONE[]	= "p_jet_l";				// 左ジェットパックの設置場所のボーン名[Body].
	constexpr char		R_JETPACK_JETPOS_BONE[]	= "jetpack_r_jet_place";	// 右ジェットパックの噴射口のボーン名[RJetPack].
	constexpr char		R_JETPACK_SETPOS_BONE[]	= "p_jet_r";				// 左ジェットパックの設置場所のボーン名[Body].
	constexpr char		BAG_SETPOS_BONE[]		= "p_jet_center";			// リュックの設置場所のボーン名[Bag].
	constexpr char		LOOK_POS_BONE[]			= "neck";					// 店員/お客さんが見る位置.

	// スキンメッシュのNo.
	enum enMeshNo : unsigned char {
		Mesh_Body,		// 表示するプレイヤー本体.
		Mesh_LJetPack,	// 表示する左ジェットパック.
		Mesh_RJetPack,	// 表示する右ジェットパック.
		Mesh_Bag,		// 表示するリュック.

		Mesh_Max,		// メッシュの個数.
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
// 初期化.
//---------------------------.
bool CComputer::Init()
{
	// メッシュの取得.
	m_pSkinMesh.resize( Mesh_Max );
	m_pSkinMesh[Mesh_Body]		= CMeshResource::GetSkin( "player_s" );
	m_pSkinMesh[Mesh_LJetPack]	= CMeshResource::GetSkin( "l_jet_s" );
	m_pSkinMesh[Mesh_RJetPack]	= CMeshResource::GetSkin( "r_jet_s" );
	m_pSkinMesh[Mesh_Bag]		= CMeshResource::GetSkin( "p_bag_s" );

	return true;
}

//---------------------------.
// 更新.
//---------------------------.
void CComputer::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

}

//---------------------------.
// 当たり判定終了後呼び出される更新.
//---------------------------.
void CComputer::LateUpdate( const float& DeltaTime )
{
}

//---------------------------.
// 描画.
//---------------------------.
void CComputer::Render()
{
	TransformUpdate();

	// プレイヤーの描画.
	for ( int i = 0; i < Mesh_Max; ++i ) {
		m_pSkinMesh[i]->Render( &m_TransformList[i] );
	}
}

//---------------------------.
// スプライト(3D)の描画.
//	_A：後に表示される / _B：先に表示される.
//---------------------------.
void CComputer::Sprite3DRender_A()
{
}

//---------------------------.
// 当たり判定関数.
//---------------------------.
void CComputer::Collision( CActor* pActor )
{
}

//---------------------------.
// トランスフォームの更新.
//---------------------------.
void CComputer::TransformUpdate()
{
	// プレイヤー本体のモデルの座標を設定する.
	m_pSkinMesh[Mesh_Body]->SetTransform( m_TransformList[Mesh_Body] );

	// ジェットパックの位置の更新.
	m_TransformList[Mesh_LJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( L_JETPACK_SETPOS_BONE );
	m_TransformList[Mesh_RJetPack].Position = m_pSkinMesh[Mesh_Body]->GetPosFromBone( R_JETPACK_SETPOS_BONE );

	// リュックの位置の更新.
	m_TransformList[Mesh_Bag].Position		= m_pSkinMesh[Mesh_Body]->GetPosFromBone( BAG_SETPOS_BONE );

//	// 落下している場合リュックを少し上に移動させる.
//	const float AddY = m_Transform.Position.y - m_TransformList[Mesh_Body].Position.y;
//	m_TransformList[Mesh_Bag].Position.y += sinf( Math::ToRadian( m_GetFoodAngCnt ) );
//	if ( AddY > 0.0f ) m_TransformList[Mesh_Bag].Position.y += AddY;

	// 外部で使用するトランスフォームをプレイヤー本体と同じにする.
	m_Transform	= m_TransformList[Mesh_Body];
}
