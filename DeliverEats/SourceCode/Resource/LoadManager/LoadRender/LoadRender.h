#pragma once
#include "..\..\..\Global.h"
#include "..\..\..\Common\Sprite\Sprite.h"

/************************************************
*	ロード中の描画クラス.
*		﨑田友輝.
**/
class CLoadRender final
{
public:
	CLoadRender();
	~CLoadRender();

	// 読み込み.
	HRESULT Load();

	// 更新.
	void Update( const float& DeltaTime );
	// 描画.
	void Render();

private:
	// 雲の召喚.
	void CloudSpown();
	// 雲の移動.
	void CloudMove();
	// 使用していない雲の削除.
	void CloudDelete();

	// テキストのアニメーション.
	void TextAnimation();

	// プレイヤーのジャンプ.
	void PlayerJump();
	// プレイヤーの落下.
	void PlayerFall();
	// プレイヤーの雲に当たった時のアニメーション.
	void PlayerCloudHitAnimation();
	// プレイヤーの座標の更新.
	void PlayerPositionUpdate();
	// プレイヤーの炎の更新.
	void PlayerFireUpdate();

	// 地面の当たり判定.
	void GroundCollision();
	// 天井の当たり判定.
	void CeilingCollision();
	// 雲の当たり判定.
	void CloudCollision();

	// クールタイムの更新.
	void CoolTimeUpdate();

private:
	std::unique_ptr<CSprite>	m_pText;			// テキスト画像.
	std::unique_ptr<CSprite>	m_pPlayer;			// プレイヤー画像.
	std::unique_ptr<CSprite>	m_pPlayerFire;		// プレイヤーの炎画像.
	std::unique_ptr<CSprite>	m_pPlayerIcon;		// プレイヤーアイコン画像.
	std::unique_ptr<CSprite>	m_pCloud;			// 雲画像.
	std::unique_ptr<CSprite>	m_pLRTrigger;		// LRトリガーのテキスト画像.
	SSpriteRenderStateList		m_TextStateList;	// テキストの情報リスト.
	SSpriteRenderStateList		m_CloudStateList;	// 雲の情報リスト.
	SSpriteRenderState			m_CloudStateBase;	// 雲の情報ベース.
	SSpriteRenderState			m_PlayerState;		// プレイヤーの情報.
	SSpriteRenderState			m_PlayerFireState;	// プレイヤーの炎情報.
	SSpriteRenderState			m_PlayerIconState;	// プレイヤーアイコンの情報.
	SSpriteRenderState			m_LRTriggerState;	// LRトリガーのテキストの情報.
	float						m_AnimTimeCnt;		// アニメーションの時間用カウント.
	float						m_Power;			// プレイヤーのジャンプや落下の力.
	float						m_CloudMoveSpeed;	// 雲の移動速度.
	float						m_PlayerFireCnt;	// プレイヤーの炎のアニメーション用カウント.
	int							m_CloudTimeCnt;		// 雲の出現時間用のカウント.
	int							m_TextAnimNo;		// アニメーションするテキストのNo.
	int							m_CoolTime;			// 当たり判定のクールタイム.
	bool						m_IsCloudHit;		// 雲に当たったか.
	bool						m_LeftRotation;		// 左回転させるか.
};
