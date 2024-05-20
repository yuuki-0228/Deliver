#include "MiniMapUI.h"
#include "..\..\..\..\Actor\Actor.h"
#include "..\..\..\..\Actor\Character\Character.h"
#include "..\..\..\..\Actor\Character\Player\Player.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\DeliveryManager.h"
#include "..\..\..\..\Actor\Character\DeliveryManager\Employee\Employee.h"
#include "..\..\..\..\..\Time\Time.h"
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\..\..\..\..\Resource\SpriteResource\SpriteResource.h"

namespace {
	// アイコンの背景の開始時のy座標のUVの位置.
	constexpr float ICON_BACK_START_UV_Y = 0.5f;

	// お店の方向のアイコンの数.
	constexpr int SHOP_DIRE_ICON_NUM = 3;
}

CMiniMapUI::CMiniMapUI()
	: m_pPlayer							( nullptr )
	, m_pBack							( nullptr )
	, m_pIconBack						( nullptr )
	, m_pIcon							( nullptr )
	, m_pIslandIcon						( nullptr )
	, m_pATypeApartmentIcon				( nullptr )
	, m_pBTypeApartmentIcon				( nullptr )
	, m_pDireIcon						( nullptr )
	, m_pGrid							( nullptr )
	, m_pStageOut						( nullptr )
	, m_IconBackState					()
	, m_DireIconState					()
	, m_GridState						()
	, m_StageOutState					()
	, m_IconState						()
	, m_IslandIconStateBase				()
	, m_ATypeApartmentIconStateBase		()
	, m_BTypeApartmentIconStateBase		()
	, m_IslandIconStateList				()
	, m_OldIslandIconStateList			()
	, m_ATypeApartmentIconStateList		()
	, m_OldATypeApartmentIconStateList	()
	, m_BTypeApartmentIconStateList		()
	, m_OldBTypeApartmentIconStateList	()
	, m_IconStateList					()
	, m_OldIconStateList				()
	, m_PlayerHouseIconState			()
	, m_TagImageNo						()
	, m_DireImageNo						()
	, m_DireMask						()
	, m_pGoHandingCustomer				( nullptr )
	, m_IsDisp							( false )
{
}

CMiniMapUI::~CMiniMapUI()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CMiniMapUI* CMiniMapUI::GetInstance()
{
	static std::unique_ptr<CMiniMapUI> pInstance = std::make_unique<CMiniMapUI>();
	return pInstance.get();
}

//---------------------------.
// 初期化.
//---------------------------.
void CMiniMapUI::Init()
{
	CMiniMapUI* pI = GetInstance();

	// 初期化.
	pI->m_IconStateList.clear();
	pI->m_OldIconStateList.clear();
	pI->m_IslandIconStateList.clear();
	pI->m_OldIslandIconStateList.clear();
	pI->m_ATypeApartmentIconStateList.clear();
	pI->m_OldATypeApartmentIconStateList.clear();
	pI->m_BTypeApartmentIconStateList.clear();
	pI->m_OldBTypeApartmentIconStateList.clear();

	// ミニマップを表示する.
	pI->m_IsDisp		= true;
	
	// 画像の設定.
	pI->m_pBack					= CSpriteResource::GetSprite( "Screen" );
	pI->m_pIconBack				= CSpriteResource::GetSprite( "MapIconGauge",			&pI->m_IconBackState );
	pI->m_pIcon					= CSpriteResource::GetSprite( "MapIcon",				&pI->m_IconState );
	pI->m_pIslandIcon			= CSpriteResource::GetSprite( "MapIslandIcon",			&pI->m_IslandIconStateBase );
	pI->m_pATypeApartmentIcon	= CSpriteResource::GetSprite( "MapATypeApartmentIcon",	&pI->m_ATypeApartmentIconStateBase );
	pI->m_pBTypeApartmentIcon	= CSpriteResource::GetSprite( "MapBTypeApartmentIcon",	&pI->m_BTypeApartmentIconStateBase );
	pI->m_pStageOut				= CSpriteResource::GetSprite( "StageOut",				&pI->m_StageOutState );
	pI->m_pDireIcon				= CSpriteResource::GetSprite( "DireIcon",				&pI->m_DireIconState );
	pI->m_pGrid					= CSpriteResource::GetSprite( "Grid",					&pI->m_GridState );
	pI->m_pBack->InitSceneSpriteState( "GameMain",	0 );
	pI->m_IconState.AnimState.IsSetAnimNumber = true;

	// 描画する範囲の設定.
	const D3DXVECTOR3&	MiniMapPos		= pI->m_pBack->GetRenderPos();			// ミニマップの中心座標.
	const float			MiniMapSizeW	= pI->m_pBack->GetSpriteState().Disp.w;	// ミニマップのサイズ.
	const float			MiniMapSizeH	= pI->m_pBack->GetSpriteState().Disp.h;	// ミニマップのサイズ.
	pI->m_IconState.RenderArea						= { MiniMapPos.x - MiniMapSizeW / 2.0f, MiniMapPos.y - MiniMapSizeH / 2.0f, MiniMapSizeW, MiniMapSizeH };
	pI->m_StageOutState.RenderArea					= { MiniMapPos.x - MiniMapSizeW / 2.0f, MiniMapPos.y - MiniMapSizeH / 2.0f, MiniMapSizeW, MiniMapSizeH };
	pI->m_IconBackState.RenderArea					= { MiniMapPos.x - MiniMapSizeW / 2.0f, MiniMapPos.y - MiniMapSizeH / 2.0f, MiniMapSizeW, MiniMapSizeH };
	pI->m_IslandIconStateBase.RenderArea			= { MiniMapPos.x - MiniMapSizeW / 2.0f, MiniMapPos.y - MiniMapSizeH / 2.0f, MiniMapSizeW, MiniMapSizeH };
	pI->m_ATypeApartmentIconStateBase.RenderArea	= { MiniMapPos.x - MiniMapSizeW / 2.0f, MiniMapPos.y - MiniMapSizeH / 2.0f, MiniMapSizeW, MiniMapSizeH };
	pI->m_BTypeApartmentIconStateBase.RenderArea	= { MiniMapPos.x - MiniMapSizeW / 2.0f, MiniMapPos.y - MiniMapSizeH / 2.0f, MiniMapSizeW, MiniMapSizeH };

	// タグに対応した画像のパターンNoを設定.
	pI->m_TagImageNo[EObjectTag::Player]			= 8;
	pI->m_TagImageNo[EObjectTag::PlayerIsland]		= 7;
	pI->m_TagImageNo[EObjectTag::GoHandingCustomer]	= 6;
	pI->m_TagImageNo[EObjectTag::Employee]			= 5;
	pI->m_TagImageNo[EObjectTag::Customer]			= 4;

	// 方向に対応した画像のパターンNoを設定.
	pI->m_DireImageNo[EDirection::Left]				= 9;
	pI->m_DireImageNo[EDirection::Right]			= 10;
	pI->m_DireImageNo[EDirection::Down]				= 11;
	pI->m_DireImageNo[EDirection::Up]				= 12;

	// 方向に対応したマスクテクスチャを設定.
	pI->m_DireMask[EDirection::Left]				= CSpriteResource::GetSprite( "MapIconMaskLeft" )->GetTexture();
	pI->m_DireMask[EDirection::Right]				= CSpriteResource::GetSprite( "MapIconMaskRight" )->GetTexture();
	pI->m_DireMask[EDirection::Down]				= CSpriteResource::GetSprite( "MapIconMaskDown" )->GetTexture();
	pI->m_DireMask[EDirection::Up]					= CSpriteResource::GetSprite( "MapIconMaskUp" )->GetTexture();
}

//---------------------------.
// 描画.
//---------------------------.
void CMiniMapUI::Render()
{
	CMiniMapUI*	pI = GetInstance();

	if ( pI->m_pPlayer == nullptr ) return;

	// 画面を停止しているか.
	if ( CSceneManager::GetIsSceneStop() || CFadeManager::GetIsFade() ) {
		StopRender();
		return;
	}

	GridRender();		// グリッド線の描画.
	IconRender();		// アイコンの描画.
	PlayerIconRender();	// プレイヤーのアイコンの描画.
	DireRender();		// 渡しに行くお客さんの方向のアイコンの表示.
	ShopDireRender();	// お店の方向のアイコンの表示.
	StageOutRender();	// ステージの範囲の描画.

	// 今のリストを保存して削除する.
	pI->m_OldIconStateList					= pI->m_IconStateList;
	pI->m_OldIslandIconStateList			= pI->m_IslandIconStateList;
	pI->m_OldATypeApartmentIconStateList	= pI->m_ATypeApartmentIconStateList;
	pI->m_OldBTypeApartmentIconStateList	= pI->m_BTypeApartmentIconStateList;
	pI->m_IconStateList.clear();
	pI->m_IslandIconStateList.clear();
	pI->m_ATypeApartmentIconStateList.clear();
	pI->m_BTypeApartmentIconStateList.clear();
}

//---------------------------.
// キャラクターのアイコンの追加.
//---------------------------.
void CMiniMapUI::PushCharacterIcon( CActor* pActor, const EDirection Dire, const CLinking::Handle Handle, const EObjectTag Tag )
{
	CMiniMapUI* pI = GetInstance();

	if ( pI->m_IsDisp == false ) return;

	const EObjectTag	ObjTag			= Tag == EObjectTag::None ? pActor->GetObjectTag() : Tag;	// タグ.
	const D3DXVECTOR3&	MiniMapPos		= pI->m_pBack->GetRenderPos();								// ミニマップの中心座標.
	const D3DXVECTOR3&	IconPos			= pActor->GetPosition() - pI->m_pPlayer->GetPosition();		// アイコンのプレイヤーから位置を取得.
	const float			IconSize		= pI->m_pIcon->GetSpriteState().Stride.w / 2;				// アイコンのサイズ.
	const float			MiniMapSizeW	= pI->m_pBack->GetSpriteState().Disp.w / 2;					// ミニマップのサイズ.
	const float			MiniMapSizeH	= pI->m_pBack->GetSpriteState().Disp.h / 2;					// ミニマップのサイズ.

	// 表示する位置を設定.
	D3DXPOSITION3 DispPos = INIT_FLOAT3;
	DispPos.x = MiniMapPos.x - IconPos.x * pI->CONSTANT.MAGNIFICATION;
	DispPos.y = MiniMapPos.y + IconPos.z * pI->CONSTANT.MAGNIFICATION;

	// ミニマップのアイコン設定.
	SIconState NewIcon;
	NewIcon.Pos			= DispPos;
	NewIcon.Color		= CLinking::GetLinkColor( Handle );
	NewIcon.Pattern		= pI->m_TagImageNo[ObjTag];
	NewIcon.Dire		= Dire;
	NewIcon.Handle		= Handle;

	// ミニマップからはみ出るか.
	bool IsIconChange	= false;
	if ( NewIcon.Pos.x < MiniMapPos.x - MiniMapSizeW ) {
		NewIcon.Pos.x	= MiniMapPos.x - MiniMapSizeW;
		IsIconChange	= true;
	}
	else if ( NewIcon.Pos.x > MiniMapPos.x + MiniMapSizeW ) {
		NewIcon.Pos.x	= MiniMapPos.x + MiniMapSizeW;
		IsIconChange	= true;
	}
	if ( NewIcon.Pos.y < MiniMapPos.y - MiniMapSizeH ) {
		NewIcon.Pos.y	= MiniMapPos.y - MiniMapSizeH;
		IsIconChange	= true;
	}
	else if ( NewIcon.Pos.y > MiniMapPos.y + MiniMapSizeH ) {
		NewIcon.Pos.y	= MiniMapPos.y + MiniMapSizeH;
		IsIconChange	= true;
	}

	// アイコンの変更.
	if ( IsIconChange ) {
		NewIcon.Pattern -= pI->CONSTANT.ICON_CHANGE_MOVE_NO;
		NewIcon.IsBack	= false;
	}

	// アイコン情報を追加.
	pI->m_IconStateList.emplace_back( NewIcon );

	// アイコンを描画並び変える.
	std::vector<std::pair<int, int>> LengthList;
	const int Size = static_cast<int>( pI->m_IconStateList.size() );
	for ( int i = 0; i < Size; ++i ) {
		const int No = pI->m_IconStateList[i].Pattern;
		LengthList.emplace_back( std::make_pair( No, i ) );
	}
	std::sort( LengthList.begin(), LengthList.end() );

	// 並び変えた順番に変更する.
	std::vector<SIconState> outList;
	for ( auto& l : LengthList )
		outList.emplace_back( pI->m_IconStateList[l.second] );
	pI->m_IconStateList = outList;

	// 渡しに行くお客さんの場合情報を保存する.
	if ( ObjTag != EObjectTag::GoHandingCustomer ) return;
	pI->m_pGoHandingCustomer	= pActor;
	pI->m_DireIconState.Color	= Color4::RGBA( NewIcon.Color );
}

//---------------------------.
// Aタイプマンションのアイコンの追加.
//---------------------------.
void CMiniMapUI::PushApartmentAIcon( CActor* pActor )
{
	CMiniMapUI* pI = GetInstance();

	if ( pI->m_pPlayer == nullptr	) return;
	if ( pI->m_IsDisp  == false		) return;

	const D3DXVECTOR3&	MiniMapPos		= pI->m_pBack->GetRenderPos();								// ミニマップの中心座標.
	const D3DXVECTOR3&	IconPos			= pActor->GetPosition() - pI->m_pPlayer->GetPosition();		// アイコンのプレイヤーから位置を取得.
	const float			IconSize		= pI->m_pATypeApartmentIcon->GetSpriteState().Stride.w / 2;	// アイコンのサイズ.
	const float			MiniMapSizeW	= pI->m_pBack->GetSpriteState().Disp.w / 2;					// ミニマップのサイズ.
	const float			MiniMapSizeH	= pI->m_pBack->GetSpriteState().Disp.h / 2;					// ミニマップのサイズ.

	// 表示する位置を設定.
	D3DXPOSITION3 DispPos = INIT_FLOAT3;
	DispPos.x = MiniMapPos.x - IconPos.x * pI->CONSTANT.MAGNIFICATION;
	DispPos.y = MiniMapPos.y + IconPos.z * pI->CONSTANT.MAGNIFICATION;

	// スマホ外のアイコンは表示しない.
	if ( DispPos.x + IconSize < MiniMapPos.x - MiniMapSizeW ) return;
	if ( DispPos.x - IconSize > MiniMapPos.x + MiniMapSizeW ) return;
	if ( DispPos.y + IconSize < MiniMapPos.y - MiniMapSizeH ) return;
	if ( DispPos.y - IconSize > MiniMapPos.y + MiniMapSizeH ) return;

	// アイコン情報を追加.
	pI->m_ATypeApartmentIconStateList.emplace_back( pI->m_ATypeApartmentIconStateBase );
	pI->m_ATypeApartmentIconStateList.back().Transform.Position = DispPos;
}

//---------------------------.
// Bタイプマンションのアイコンの追加.
//---------------------------.
void CMiniMapUI::PushApartmentBIcon( CActor* pActor )
{
	CMiniMapUI* pI = GetInstance();

	if ( pI->m_pPlayer == nullptr	) return;
	if ( pI->m_IsDisp  == false		) return;

	const D3DXVECTOR3&	MiniMapPos		= pI->m_pBack->GetRenderPos();								// ミニマップの中心座標.
	const D3DXVECTOR3&	IconPos			= pActor->GetPosition() - pI->m_pPlayer->GetPosition();		// アイコンのプレイヤーから位置を取得.
	const float			IconSize		= pI->m_pBTypeApartmentIcon->GetSpriteState().Stride.w / 2;	// アイコンのサイズ.
	const float			MiniMapSizeW	= pI->m_pBack->GetSpriteState().Disp.w / 2;					// ミニマップのサイズ.
	const float			MiniMapSizeH	= pI->m_pBack->GetSpriteState().Disp.h / 2;					// ミニマップのサイズ.
	
	// 表示する位置を設定.
	D3DXPOSITION3 DispPos = INIT_FLOAT3;
	DispPos.x = MiniMapPos.x - IconPos.x * pI->CONSTANT.MAGNIFICATION;
	DispPos.y = MiniMapPos.y + IconPos.z * pI->CONSTANT.MAGNIFICATION;

	// スマホ外のアイコンは表示しない.
	if ( DispPos.x + IconSize < MiniMapPos.x - MiniMapSizeW ) return;
	if ( DispPos.x - IconSize > MiniMapPos.x + MiniMapSizeW ) return;
	if ( DispPos.y + IconSize < MiniMapPos.y - MiniMapSizeH ) return;
	if ( DispPos.y - IconSize > MiniMapPos.y + MiniMapSizeH ) return;

	// アイコン情報を追加.
	pI->m_BTypeApartmentIconStateList.emplace_back( pI->m_BTypeApartmentIconStateBase );
	pI->m_BTypeApartmentIconStateList.back().Transform.Position = DispPos;
}

//---------------------------.
// 浮島のアイコンの追加.
//---------------------------.
void CMiniMapUI::PushIslandIcon( CActor* pActor )
{
	CMiniMapUI* pI = GetInstance();

	if ( pI->m_pPlayer == nullptr	) return;
	if ( pI->m_IsDisp  == false		) return;

	// アイコンの設定.
	const D3DXVECTOR3&	MiniMapPos		= pI->m_pBack->GetRenderPos();								// ミニマップの中心座標.
	const D3DXVECTOR3&	IconPos			= pActor->GetPosition() - pI->m_pPlayer->GetPosition();		// アイコンのプレイヤーから位置を取得.
	const float			IconSize		= pI->m_pIslandIcon->GetSpriteState().Stride.w / 2;			// アイコンのサイズ.
	const float			MiniMapSizeW	= pI->m_pBack->GetSpriteState().Disp.w / 2;					// ミニマップのサイズ.
	const float			MiniMapSizeH	= pI->m_pBack->GetSpriteState().Disp.h / 2;					// ミニマップのサイズ.

	// 表示する位置を設定.
	D3DXPOSITION3 DispPos = INIT_FLOAT3;
	DispPos.x = MiniMapPos.x - IconPos.x * pI->CONSTANT.MAGNIFICATION;
	DispPos.y = MiniMapPos.y + IconPos.z * pI->CONSTANT.MAGNIFICATION;

	// プレイヤーの家のアイコンの追加.
	const EObjectTag Tag = pActor->GetObjectTag();
	if ( Tag == EObjectTag::PlayerIsland ) PushPlayerHouse( DispPos, MiniMapPos, MiniMapSizeW, MiniMapSizeH );

	// スマホ外のアイコンは表示しない.
	if ( DispPos.x + IconSize < MiniMapPos.x - MiniMapSizeW ) return;
	if ( DispPos.x - IconSize > MiniMapPos.x + MiniMapSizeW ) return;
	if ( DispPos.y + IconSize < MiniMapPos.y - MiniMapSizeH ) return;
	if ( DispPos.y - IconSize > MiniMapPos.y + MiniMapSizeH ) return;

	// アイコン情報を追加.
	pI->m_IslandIconStateList.emplace_back( pI->m_IslandIconStateBase );
	pI->m_IslandIconStateList.back().Transform.Position = DispPos;
}

//---------------------------.
// 画面を停止している時の描画.
//---------------------------.
void CMiniMapUI::StopRender()
{
	CMiniMapUI* pI = GetInstance();

	GridRender();		// グリッド線の描画.
	OldIconRender();	// ミニマップの描画.
	PlayerIconRender();	// プレイヤーのアイコンの描画.
	DireRender();		// 渡しに行くお客さんの方向のアイコンの表示.
	ShopDireRender();	// お店の方向のアイコンの表示.
	StageOutRender();	// ステージの範囲の描画.
}

//---------------------------.
// プレイヤーの家の追加.
//---------------------------.
void CMiniMapUI::PushPlayerHouse( const D3DXPOSITION3& DispPos, const D3DXVECTOR3& MiniMapPos, const float MiniMapSizeW, const float MiniMapSizeH )
{
	if ( CTime::GetIsGameEnd() == false ) return;

	CMiniMapUI* pI = GetInstance();

	// アイコンの設定.
	pI->m_PlayerHouseIconState.Pos		= DispPos;
	pI->m_PlayerHouseIconState.Color	= Color::Dodgerblue;
	pI->m_PlayerHouseIconState.Pattern	= pI->m_TagImageNo[EObjectTag::PlayerIsland];
	pI->m_PlayerHouseIconState.Dire		= EDirection::Down;
	pI->m_PlayerHouseIconState.IsBack	= true;

	// ミニマップからはみ出るか.
	bool IsIconChange	= false;
	if ( pI->m_PlayerHouseIconState.Pos.x < MiniMapPos.x - MiniMapSizeW ) {
		pI->m_PlayerHouseIconState.Pos.x	= MiniMapPos.x - MiniMapSizeW;
		IsIconChange						= true;
	}
	else if ( pI->m_PlayerHouseIconState.Pos.x > MiniMapPos.x + MiniMapSizeW ) {
		pI->m_PlayerHouseIconState.Pos.x	= MiniMapPos.x + MiniMapSizeW;
		IsIconChange						= true;
	}
	if ( pI->m_PlayerHouseIconState.Pos.y < MiniMapPos.y - MiniMapSizeH ) {
		pI->m_PlayerHouseIconState.Pos.y	= MiniMapPos.y - MiniMapSizeH;
		IsIconChange						= true;
	}
	else if ( pI->m_PlayerHouseIconState.Pos.y > MiniMapPos.y + MiniMapSizeH ) {
		pI->m_PlayerHouseIconState.Pos.y	= MiniMapPos.y + MiniMapSizeH;
		IsIconChange						= true;
	}

	// アイコンの変更.
	if ( IsIconChange ) {
		pI->m_PlayerHouseIconState.Pattern -= pI->CONSTANT.ICON_CHANGE_MOVE_NO;
		pI->m_PlayerHouseIconState.IsBack	= false;
	}
}

//---------------------------.
// グリッド線の描画.
//---------------------------.
void CMiniMapUI::GridRender()
{
	CMiniMapUI* pI = GetInstance();

	const D3DXVECTOR3&	MiniMapPos		= pI->m_pBack->GetRenderPos();				// ミニマップの中心座標.
	const D3DXVECTOR3&	PlayerPos		= pI->m_pPlayer->GetPosition();				// プレイヤーの座標.
	const float			MiniMapSizeW	= pI->m_pBack->GetSpriteState().Disp.w / 2;	// ミニマップのサイズ.
	const float			MiniMapSizeH	= pI->m_pBack->GetSpriteState().Disp.h / 2;	// ミニマップのサイズ.
	const float			StartPosX		= std::fmod( PlayerPos.x, pI->CONSTANT.GRID_SPACE );
	const float			StartPosY		= std::fmod( PlayerPos.z, pI->CONSTANT.GRID_SPACE );
	
	// 上方向の横線の描画.
	const int DispNumUH = static_cast<int>( ( MiniMapSizeH - StartPosY ) / pI->CONSTANT.GRID_SPACE ) + 1;
	for ( int i = 0; i < DispNumUH; ++i ) {
		D3DXVECTOR3* Pos		= &pI->m_GridState.Transform.Position;
		D3DXVECTOR3* Scale		= &pI->m_GridState.Transform.Scale;
		Pos->x					= MiniMapPos.x;
		Pos->y					= MiniMapPos.y - StartPosY - pI->CONSTANT.GRID_SPACE * i;
		Scale->x				= MiniMapSizeW * 2;
		Scale->y				= STransform::NORMAL_SCALE;
		pI->m_GridState.Color	= Color4::Lightgray;
		pI->m_pGrid->RenderUI( &pI->m_GridState );
	}

	// 下方向の横線の描画.
	const int DispNumDH = static_cast<int>( ( MiniMapSizeH + StartPosY ) / pI->CONSTANT.GRID_SPACE ) + 1;
	for ( int i = 0; i < DispNumDH; ++i ) {
		D3DXVECTOR3* Pos		= &pI->m_GridState.Transform.Position;
		D3DXVECTOR3* Scale		= &pI->m_GridState.Transform.Scale;
		Pos->x					= MiniMapPos.x;
		Pos->y					= MiniMapPos.y - StartPosY + pI->CONSTANT.GRID_SPACE * i;
		Scale->x				= MiniMapSizeW * 2;
		Scale->y				= STransform::NORMAL_SCALE;
		pI->m_GridState.Color	= Color4::Lightgray;
		pI->m_pGrid->RenderUI( &pI->m_GridState );
	}

	// 左方向の縦線の描画.
	const int DispNumLW = static_cast<int>( ( MiniMapSizeW + StartPosX ) / pI->CONSTANT.GRID_SPACE ) + 1;
	for ( int i = 0; i < DispNumLW; ++i ) {
		D3DXVECTOR3* Pos		= &pI->m_GridState.Transform.Position;
		D3DXVECTOR3* Scale		= &pI->m_GridState.Transform.Scale;
		Pos->x					= MiniMapPos.x + StartPosX - pI->CONSTANT.GRID_SPACE * i;
		Pos->y					= MiniMapPos.y;
		Scale->x				= STransform::NORMAL_SCALE;
		Scale->y				= MiniMapSizeH * 2;
		pI->m_GridState.Color	= Color4::Lightgray;
		pI->m_pGrid->RenderUI( &pI->m_GridState );
	}

	// 右方向の縦線の描画.
	const int DispNumRW = static_cast<int>( ( MiniMapSizeW - StartPosX ) / pI->CONSTANT.GRID_SPACE ) + 1;
	for ( int i = 0; i < DispNumRW; ++i ) {
		D3DXVECTOR3* Pos		= &pI->m_GridState.Transform.Position;
		D3DXVECTOR3* Scale		= &pI->m_GridState.Transform.Scale;
		Pos->x					= MiniMapPos.x + StartPosX + pI->CONSTANT.GRID_SPACE * i;
		Pos->y					= MiniMapPos.y;
		Scale->x				= STransform::NORMAL_SCALE;
		Scale->y				= MiniMapSizeH * 2;
		pI->m_GridState.Color	= Color4::Lightgray;
		pI->m_pGrid->RenderUI( &pI->m_GridState );
	}
}

//---------------------------.
// アイコンの描画.
//---------------------------.
void CMiniMapUI::IconRender()
{
	CMiniMapUI* pI = GetInstance();

	// 浮島のアイコンの描画.
	if ( pI->m_IslandIconStateList.empty() == false ) {
		for ( auto& i : pI->m_IslandIconStateList ) pI->m_pIslandIcon->RenderUI( &i );
	}
		
	// Aタイプマンションのアイコンの描画.
	if ( pI->m_ATypeApartmentIconStateList.empty() == false ) {
		for ( auto& a : pI->m_ATypeApartmentIconStateList ) pI->m_pATypeApartmentIcon->RenderUI( &a );
	}

	// Bタイプマンションのアイコンの描画.
	if ( pI->m_BTypeApartmentIconStateList.empty() == false ) {
		for ( auto& b : pI->m_BTypeApartmentIconStateList ) pI->m_pBTypeApartmentIcon->RenderUI( &b );
	}

	// ミニマップの描画.
	for ( auto& s : pI->m_IconStateList ) {
		const float Ratio = CLinking::GetLinkEndTimeCnt( s.Handle ) / CLinking::GetLinkEndTime( s.Handle );

		// 背景の描画.
		if ( s.IsBack ) {
			pI->m_IconBackState.Transform.Position	= s.Pos;
			pI->m_IconBackState.AnimState.UV.y		= ICON_BACK_START_UV_Y - ICON_BACK_START_UV_Y * Ratio;
			pI->m_IconBackState.Color				= Color4::RGBA( s.Color );
			pI->m_IconBackState.pMaskTexture		= pI->m_DireMask[s.Dire];
			pI->m_pIconBack->RenderUI( &pI->m_IconBackState );

			// 背景を少し白っぽくする.
			pI->m_IconBackState.AnimState.UV.y		= ICON_BACK_START_UV_Y;
			pI->m_IconBackState.Color				= Color4::RGBA( Color::White, 0.3f );
			pI->m_pIconBack->RenderUI( &pI->m_IconBackState );
		}

		// アイコンの描画.
		pI->m_IconState.Transform.Position		= s.Pos;
		pI->m_IconState.Transform.Rotation		= INIT_FLOAT3;
		pI->m_IconState.AnimState.AnimNumber	= s.Pattern;
		pI->m_IconState.Color					= Color4::RGBA( s.Color );
		pI->m_pIcon->RenderUI( &pI->m_IconState );

		// アイコンを黒っぽくする.
		pI->m_IconState.Color = Color4::RGBA( Color::Black, 0.1f );
		pI->m_pIcon->RenderUI( &pI->m_IconState );

		// フレームの描画.
		if ( s.IsBack ) {
			pI->m_IconState.Transform.Position		= s.Pos;
			pI->m_IconState.Transform.Rotation		= INIT_FLOAT3;
			pI->m_IconState.AnimState.AnimNumber	= pI->m_DireImageNo[s.Dire];
			pI->m_IconState.Color					= Color4::White;
			pI->m_pIcon->RenderUI( &pI->m_IconState );
		}
	}

	// プレイヤーの家のアイコンの表示.
	if ( CTime::GetIsGameEnd() == false ) return;

	// 背景の描画.
	if ( pI->m_PlayerHouseIconState.IsBack ) {
		pI->m_IconBackState.Transform.Position	= pI->m_PlayerHouseIconState.Pos;
		pI->m_IconBackState.AnimState.UV.y		= ICON_BACK_START_UV_Y;
		pI->m_IconBackState.Color				= Color4::RGBA( pI->m_PlayerHouseIconState.Color );
		pI->m_IconBackState.pMaskTexture		= pI->m_DireMask[pI->m_PlayerHouseIconState.Dire];
		pI->m_pIconBack->RenderUI( &pI->m_IconBackState );

		// 背景を少し白っぽくする.
		pI->m_IconBackState.AnimState.UV.y		= ICON_BACK_START_UV_Y;
		pI->m_IconBackState.Color				= Color4::RGBA( Color::White, 0.5f );
		pI->m_pIconBack->RenderUI( &pI->m_IconBackState );
	}

	// アイコンの描画.
	pI->m_IconState.Transform.Position			= pI->m_PlayerHouseIconState.Pos;
	pI->m_IconState.Transform.Rotation			= INIT_FLOAT3;
	pI->m_IconState.AnimState.AnimNumber		= pI->m_PlayerHouseIconState.Pattern;
	pI->m_IconState.Color						= Color4::RGBA( pI->m_PlayerHouseIconState.Color );
	pI->m_pIcon->RenderUI( &pI->m_IconState );

	// アイコンを黒っぽくする.
	pI->m_IconState.Color						= Color4::RGBA( Color::Black, 0.1f );
	pI->m_pIcon->RenderUI( &pI->m_IconState );

	// フレームの描画.
	if ( pI->m_PlayerHouseIconState.IsBack ) {
		pI->m_IconState.Transform.Position		= pI->m_PlayerHouseIconState.Pos;
		pI->m_IconState.Transform.Rotation		= INIT_FLOAT3;
		pI->m_IconState.AnimState.AnimNumber	= pI->m_DireImageNo[pI->m_PlayerHouseIconState.Dire];
		pI->m_IconState.Color					= Color4::White;
		pI->m_pIcon->RenderUI( &pI->m_IconState );
	}
}

//---------------------------.
// 前のフレームのアイコンの描画.
//---------------------------.
void CMiniMapUI::OldIconRender()
{
	CMiniMapUI* pI = GetInstance();

	// 浮島のアイコンの描画.
	if ( pI->m_OldIslandIconStateList.empty() == false ) {
		for ( auto& i : pI->m_OldIslandIconStateList ) pI->m_pIslandIcon->RenderUI( &i );
	}
		
	// Aタイプマンションのアイコンの描画.
	if ( pI->m_OldATypeApartmentIconStateList.empty() == false ) {
		for ( auto& a : pI->m_OldATypeApartmentIconStateList ) pI->m_pATypeApartmentIcon->RenderUI( &a );
	}

	// Bタイプマンションのアイコンの描画.
	if ( pI->m_OldBTypeApartmentIconStateList.empty() == false ) {
		for ( auto& b : pI->m_OldBTypeApartmentIconStateList ) pI->m_pBTypeApartmentIcon->RenderUI( &b );
	}

	// ミニマップの描画.
	for ( auto& s : pI->m_OldIconStateList ) {
		const float Ratio = CLinking::GetLinkEndTimeCnt( s.Handle ) / CLinking::GetLinkEndTime( s.Handle );

		// 背景の描画.
		if ( s.IsBack ) {
			pI->m_IconBackState.Transform.Position	= s.Pos;
			pI->m_IconBackState.AnimState.UV.y		= ICON_BACK_START_UV_Y - ICON_BACK_START_UV_Y * Ratio;
			pI->m_IconBackState.Color				= Color4::RGBA( s.Color );
			pI->m_IconBackState.pMaskTexture		= pI->m_DireMask[s.Dire];
			pI->m_pIconBack->RenderUI( &pI->m_IconBackState );

			// 背景を少し白っぽくする.
			pI->m_IconBackState.AnimState.UV.y		= ICON_BACK_START_UV_Y;
			pI->m_IconBackState.Color				= Color4::RGBA( Color::White, 0.5f );
			pI->m_pIconBack->RenderUI( &pI->m_IconBackState );
		}

		// アイコンの描画.
		pI->m_IconState.Transform.Position		= s.Pos;
		pI->m_IconState.Transform.Rotation		= INIT_FLOAT3;
		pI->m_IconState.AnimState.AnimNumber	= s.Pattern;
		pI->m_IconState.Color					= Color4::RGBA( s.Color );
		pI->m_pIcon->RenderUI( &pI->m_IconState );

		// フレームの描画.
		if ( s.IsBack ) {
			pI->m_IconState.Transform.Position = s.Pos;
			pI->m_IconState.Transform.Rotation = INIT_FLOAT3;
			pI->m_IconState.AnimState.AnimNumber = pI->m_DireImageNo[s.Dire];
			pI->m_IconState.Color = Color4::White;
			pI->m_pIcon->RenderUI( &pI->m_IconState );
		}
	}
}

//---------------------------.
// プレイヤーのアイコンの描画.
//---------------------------.
void CMiniMapUI::PlayerIconRender()
{
	CMiniMapUI* pI = GetInstance();

	pI->m_IconState.Transform.Position		= pI->m_pBack->GetRenderPos();
	pI->m_IconState.Transform.Rotation.z	= pI->m_pPlayer->GetRotation().y;
	pI->m_IconState.Color					= Color4::White;
	pI->m_IconState.AnimState.AnimNumber	= pI->m_TagImageNo[EObjectTag::Player];
	pI->m_pIcon->RenderUI( &pI->m_IconState );
}

//---------------------------.
// 渡しに行くお客さんの方向のアイコンの描画.
//---------------------------.
void CMiniMapUI::DireRender()
{
	CMiniMapUI* pI = GetInstance();

	const bool IsEndDire = CTime::GetIsGameEnd() && pI->m_pPlayer->GetIsFoodHave() == false;
	if ( pI->m_pGoHandingCustomer == nullptr && IsEndDire == false ) return;

	// ミニマップの中心座標.
	const D3DXVECTOR3& MiniMapPos	= pI->m_pBack->GetRenderPos();
	const D3DXVECTOR3& PlayerPos	= pI->m_pPlayer->GetPosition();
	D3DXVECTOR3 LookPos = INIT_FLOAT3;
	if ( IsEndDire ) LookPos = pI->m_pPlayer->GetStartPosition();
	else			 LookPos = pI->m_pGoHandingCustomer->GetPosition();

	// 矢印に色を付ける.
	if ( IsEndDire )  pI->m_DireIconState.Color = Color4::Dodgerblue;

	// 向きベクトルの作成.
	//	y座標を使用しないため無くす.
	D3DXVECTOR3 Vector		= PlayerPos - LookPos;
	Vector.y				= INIT_FLOAT;
	const float Distance	= D3DXVec3Length( &Vector );
	D3DXVec3Normalize( &Vector, &Vector );

	// アイコンの表示する位置の設定.
	pI->m_DireIconState.Transform.Position.x = MiniMapPos.x + Vector.x * pI->CONSTANT.DIRE_ICON_DISTANCE;
	pI->m_DireIconState.Transform.Position.y = MiniMapPos.y - Vector.z * pI->CONSTANT.DIRE_ICON_DISTANCE;

	// 渡しに行くお客さんの方向に回転させる.
	pI->m_DireIconState.Transform.Rotation.z = atan2f( PlayerPos.x - LookPos.x, PlayerPos.z - LookPos.z );

	// 近づくごとに透明にしていく.
	if ( Distance < pI->CONSTANT.DIRE_ICON_ALPHA_START_DISTANCE ) {
		const float OverDistance	= pI->CONSTANT.DIRE_ICON_ALPHA_START_DISTANCE - Distance;
		pI->m_DireIconState.Color.w = Color::ALPHA_MAX - OverDistance / pI->CONSTANT.DIRE_ICON_ALPHA_DEC_DISTANCE / 10.0f;
		if ( pI->m_DireIconState.Color.w < Color::ALPHA_MIN ) pI->m_DireIconState.Color.w = Color::ALPHA_MIN;
	}

	// 渡しに行くお客さんの方向のアイコンの描画.
	pI->m_pDireIcon->RenderUI( &pI->m_DireIconState );
	pI->m_pGoHandingCustomer = nullptr;
}

//---------------------------.
// お店の方向のアイコンの描画.
//---------------------------.
void CMiniMapUI::ShopDireRender()
{
	CMiniMapUI* pI = GetInstance();

	if ( pI->m_pPlayer->GetIsFoodHave() ) return;

	// ミニマップの中心座標.
	const D3DXVECTOR3&				MiniMapPos		= pI->m_pBack->GetRenderPos();
	const D3DXVECTOR3&				PlayerPos		= pI->m_pPlayer->GetPosition();
	const std::vector<CEmployee*>	pEmployeeList	= CDeliveryManager::GetLinkSortEmployee();

	// 表示する個数の取得.
	const int	Size = static_cast<int>( pEmployeeList.size() );
	int			Max  = SHOP_DIRE_ICON_NUM;
	if ( Size < Max ) Max = Size;

	// 近くのお店の矢印の描画.
	if ( Max <= 0 ) return;
	for ( int i = Max - 1; i >= 0; --i ) {
		CEmployee*	pEmployee	= pEmployeeList[i];
		D3DXVECTOR3 LookPos		= pEmployee->GetPosition();

		// 向きベクトルの作成.
		//	y座標を使用しないため無くす.
		D3DXVECTOR3 Vector		= PlayerPos - LookPos;
		Vector.y				= INIT_FLOAT;
		const float Distance	= D3DXVec3Length( &Vector );
		D3DXVec3Normalize( &Vector, &Vector );

		// アイコンの表示する位置の設定.
		pI->m_DireIconState.Transform.Position.x = MiniMapPos.x + Vector.x * pI->CONSTANT.DIRE_ICON_DISTANCE;
		pI->m_DireIconState.Transform.Position.y = MiniMapPos.y - Vector.z * pI->CONSTANT.DIRE_ICON_DISTANCE;

		// 渡しに行くお客さんの方向に回転させる.
		pI->m_DireIconState.Transform.Rotation.z = atan2f( PlayerPos.x - LookPos.x, PlayerPos.z - LookPos.z );

		// 色の変更.
		const CLinking::Handle Handle = pEmployee->GetHandle();
		pI->m_DireIconState.Color = Color4::RGBA( CLinking::GetLinkColor( Handle ) );

		// 近づくごとに透明にしていく.
		if ( Distance < pI->CONSTANT.DIRE_ICON_ALPHA_START_DISTANCE ) {
			const float OverDistance	= pI->CONSTANT.DIRE_ICON_ALPHA_START_DISTANCE - Distance;
			pI->m_DireIconState.Color.w = Color::ALPHA_MAX - OverDistance / pI->CONSTANT.DIRE_ICON_ALPHA_DEC_DISTANCE / 10.0f;
			if ( pI->m_DireIconState.Color.w < Color::ALPHA_MIN ) pI->m_DireIconState.Color.w = Color::ALPHA_MIN;
		}

		// 渡しに行くお客さんの方向のアイコンの描画.
		pI->m_pDireIcon->RenderUI( &pI->m_DireIconState );
	}
}

//---------------------------.
// ステージの範囲の描画.
//---------------------------.
void CMiniMapUI::StageOutRender()
{
	CMiniMapUI* pI = GetInstance();

	const D3DXVECTOR3& MiniMapPos	=  pI->m_pBack->GetRenderPos();		// ミニマップの中心座標.
	const D3DXVECTOR3& IconPos		= -pI->m_pPlayer->GetPosition();	// アイコンのプレイヤーから位置を取得.

	// 表示位置の設定.
	pI->m_StageOutState.Transform.Position.x = MiniMapPos.x - IconPos.x * pI->CONSTANT.MAGNIFICATION;
	pI->m_StageOutState.Transform.Position.y = MiniMapPos.y + IconPos.z * pI->CONSTANT.MAGNIFICATION;

	// ステージの範囲の描画.
	pI->m_pStageOut->RenderUI( &pI->m_StageOutState );
}