#pragma once
#include "..\..\..\..\..\..\Global.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\Widget.h"

class CActor;
class CPlayer;

/************************************************
*	ミニマップUIクラス.
*		﨑田友輝.
**/
class CMiniMapUI final
{
public:
	using TagIconNo_map = std::unordered_map<EObjectTag, SHORT>;
	using DireIcon_map	= std::unordered_map<EDirection, SHORT>;
	using DireMask_map	= std::unordered_map<EDirection, ID3D11ShaderResourceView*>;

private:
	// アイコン情報.
	struct stIconState {
		D3DXVECTOR3			Pos;		// 座標.
		D3DXCOLOR3			Color;		// カラー.
		SHORT				Pattern;	// アイコンパターン.
		EDirection			Dire;		// 向き.
		CLinking::Handle	Handle;		// 紐づけハンドル.
		bool				IsBack;		// 背景を表示するか.

		stIconState()
			: Pos		( INIT_FLOAT3 )
			, Color		( Color::White )
			, Pattern	( 0 )
			, Dire		( EDirection::None )
			, Handle	( 0 )
			, IsBack	( true )
		{}

	} typedef SIconState;

public:
	CMiniMapUI();
	~CMiniMapUI();

	// インスタンスの取得.
	static CMiniMapUI* GetInstance();

	// 初期化.
	static void Init();

	// 描画.
	static void Render();

	// キャラクターのアイコンの追加.
	static void PushCharacterIcon( CActor* pActor, const EDirection Dire, const CLinking::Handle Handle, const EObjectTag Tag = EObjectTag::None );
	// Aタイプマンションのアイコンの追加.
	static void PushApartmentAIcon( CActor* pActor );
	// Bタイプマンションのアイコンの追加.
	static void PushApartmentBIcon( CActor* pActor );
	// 浮島のアイコンの追加.
	static void PushIslandIcon( CActor* pActor );

	// スマホの画面の中央位置の取得.
	static D3DXPOSITION3 GetScreenPos() { return GetInstance()->m_pBack->GetRenderPos(); }
	// スマホの画面の大きさの取得.
	static SSize GetScreenSize() { return GetInstance()->m_pBack->GetSpriteState().Disp; }

	// プレイヤーの設定.
	static void SetPlayer( CPlayer* pPlayer ) { GetInstance()->m_pPlayer = pPlayer; }

private:
	// 画面を停止している時の描画.
	static void StopRender();

	// プレイヤーの家の追加.
	static void PushPlayerHouse( const D3DXPOSITION3& DispPos, const D3DXVECTOR3& MiniMapPos, const float MiniMapSizeW, const float MiniMapSizeH );

	// グリッド線の描画.
	static void GridRender();
	// アイコンの描画.
	static void IconRender();
	// 前のフレームのアイコンの描画.
	static void OldIconRender();
	// プレイヤーのアイコンの描画.
	static void PlayerIconRender();
	// 渡しに行くお客さんの方向のアイコンの描画.
	static void DireRender();
	// お店の方向のアイコンの描画.
	static void ShopDireRender();
	// ステージの範囲の描画.
	static void StageOutRender();

private:
	CPlayer*					m_pPlayer;							// プレイヤー.
	CSprite*					m_pBack;							// 背景.
	CSprite*					m_pIconBack;						// アイコンの背景.
	CSprite*					m_pIcon;							// アイコン.
	CSprite*					m_pIslandIcon;						// 浮島のアイコン.
	CSprite*					m_pATypeApartmentIcon;				// Aタイプのマンションのアイコン.
	CSprite*					m_pBTypeApartmentIcon;				// Bタイプのマンションのアイコン.
	CSprite*					m_pDireIcon;						// 渡しに行くお客さんの向きアイコン.
	CSprite*					m_pGrid;							// グリッド線.
	CSprite*					m_pStageOut;						// ステージの範囲.
	SSpriteRenderState			m_IconBackState;					// アイコンの背景の情報.
	SSpriteRenderState			m_DireIconState;					// 渡しに行くお客さんの向きアイコンの情報.
	SSpriteRenderState			m_GridState;						// グリッド線の情報.
	SSpriteRenderState			m_StageOutState;					// ステージの範囲の情報.
	SSpriteRenderState			m_IconState;						// アイコンの情報.
	SSpriteRenderState			m_IslandIconStateBase;				// 浮島のアイコンの情報ベース.
	SSpriteRenderState			m_ATypeApartmentIconStateBase;		// Aタイプのマンションのアイコンの情報ベース.
	SSpriteRenderState			m_BTypeApartmentIconStateBase;		// Bタイプのマンションのアイコンの情報ベース.
	SSpriteRenderStateList		m_IslandIconStateList;				// 浮島のアイコンの情報リスト.
	SSpriteRenderStateList		m_OldIslandIconStateList;			// 前の浮島のアイコンの情報リスト.
	SSpriteRenderStateList		m_ATypeApartmentIconStateList;		// Aタイプのマンションのアイコンの情報リスト.
	SSpriteRenderStateList		m_OldATypeApartmentIconStateList;	// 前のフレームのAタイプのマンションのアイコンの情報リスト.
	SSpriteRenderStateList		m_BTypeApartmentIconStateList;		// Bタイプのマンションのアイコンの情報リスト.
	SSpriteRenderStateList		m_OldBTypeApartmentIconStateList;	// 前のフレームのBタイプのマンションのアイコンの情報リスト.
	std::vector<SIconState>		m_IconStateList;					// アイコンの情報リスト.
	std::vector<SIconState>		m_OldIconStateList;					// 前のフレームのアイコンの情報リスト.
	SIconState					m_PlayerHouseIconState;				// プレイヤーの家のアイコンの情報.
	TagIconNo_map				m_TagImageNo;						// タグごとの画像No.
	DireIcon_map				m_DireImageNo;						// 方向ごとの画像No.
	DireMask_map				m_DireMask;							// 方向ごとのマスクテクスチャ.
	CActor*						m_pGoHandingCustomer;				// 渡しに行くお客さん.
	bool						m_IsDisp;							// 表示するか.

private:
	// 外部データの定数.
	const ConstantStruct::SMiniMap& CONSTANT = CConstant::GetMiniMap();
};
