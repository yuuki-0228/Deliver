#pragma once
#include "..\..\Character\DeliveryManager\Linking\Linking.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	ステージのアイコンクラス.
**/
class CStageIcon final
{
public:
	CStageIcon();
	~CStageIcon();

	// アイコンの初期化.
	bool InitIcon( const EObjectTag Tag );

	// アイコンの更新.
	void IconUpdate( const CLinking::Handle Handle );

	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	void Sprite3DRender_A();

	// タグの設定.
	void SetTag( const EObjectTag Tag );
	// 座標の設定.
	void SetPosition( const D3DXPOSITION3& Pos ) { m_Transform.Position = Pos; }

private:
	STransform			m_Transform;		// トランスフォーム.
	CSprite*			m_pIcon;			// アイコン.
	CSprite*			m_pIconBack;		// アイコンの背景.
	SSpriteRenderState	m_IconState;		// アイコンの情報.
	SSpriteRenderState	m_IconFrameState;	// アイコンのフレーム情報.
	SSpriteRenderState	m_IconBackState;	// アイコンの背景の情報.
};