#pragma once
#include "..\SceneBase.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Common\Font\Font.h"

/************************************************
*	ゲームクラス.
*		﨑田友輝.
**/
class CMiniGame final
	: public CSceneBase
{
public:
	using SortCloudList = std::vector<std::pair<float, SSpriteRenderState*>>;

public:
	CMiniGame();
	~CMiniGame();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// モデルの描画.
	virtual void ModelRender() override {};
	// スプライト(UI)の描画.
	virtual void SpriteUIRender() override;
	// スプライト(3D)/Effectの描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override {};
	virtual void Sprite3DRender_B() override {};

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

	// ポーズのボタンのテキストの更新.
	void PauseButtonUpdate();
	// サウンドの更新.
	void SoundUpdate();
	// スコアの更新.
	void ScoreUpdate();
	// テキストの更新.
	void TextUpdate();
	// クールタイムの更新.
	void CoolTimeUpdate();

	// 自動操作の更新.
	void AutoPlayUpdate();
	// 雲のソートリストの作成.
	void CreateSortCloudList();
	// 自動操作の行動の更新.
	void AutoPlayActionUpdate();
	// 上に移動できるか判断.
	bool UpMoveCheck( const int CloudNo );
	// 下に移動できるか判断.
	bool DownMoveCheck( const int CloudNo );
	// 自動操作の出力の更新.
	void AutoPlayOutUpdate();

	// ポーズに移動.
	void MovePause();
	// ポーズ画面を開く.
	void PauseOpen();
	// ポーズ画面を閉じる.
	void PauseClose();
	// ポーズ中の更新.
	void PauseUpdate();

private:
	CSprite*					m_pText;				// テキスト画像.
	CSprite*					m_pPlayer;				// プレイヤー画像.
	CSprite*					m_pPlayerFire;			// プレイヤーの炎画像.
	CSprite*					m_pPlayerIcon;			// プレイヤーアイコン画像.
	CSprite*					m_pCloud;				// 雲画像.
	CSprite*					m_pLRTrigger;			// LRトリガーのテキスト画像.
	CSprite*					m_pPauseButton;			// ポーズのボタンのテキスト画像.
	CSprite*					m_pPauseBack;			// ポーズの背景画像.
	CSprite*					m_pPauseText;			// ポーズ時のテキスト画像.
	CSprite*					m_pPauseCursor;			// ポーズ時のカーソル画像.
	CFont*						m_pFont;				// フォント.
	SFontRenderStateList		m_FontStateList;		// フォントの情報.
	SSpriteRenderStateList		m_TextStateList;		// テキストの情報リスト.
	SSpriteRenderStateList		m_CloudStateList;		// 雲の情報リスト.
	SSpriteRenderStateList		m_PauseTextStateList;	// ポーズ時のテキストの情報リスト.
	SSpriteRenderState			m_PauseTextStateBase;	// ポーズ時のテキストの情報ベース.
	SSpriteRenderState			m_CloudStateBase;		// 雲の情報ベース.
	SSpriteRenderState			m_PlayerState;			// プレイヤーの情報.
	SSpriteRenderState			m_PlayerFireState;		// プレイヤーの炎情報.
	SSpriteRenderState			m_PlayerIconState;		// プレイヤーアイコンの情報.
	SSpriteRenderState			m_LRTriggerState;		// LRトリガーのテキストの情報.
	SSpriteRenderState			m_PauseButtonState;		// ポーズのボタンのテキストの情報.
	SSpriteRenderState			m_PauseBackState;		// ポーズの背景の情報.
	SSpriteRenderState			m_PauseCursorState;		// ポーズのカーソルの情報.
	SortCloudList				m_SortCloudList;		// 雲のソートリスト.
	std::vector<int>			m_PlayData;				// プレイデータリスト.
	float						m_DeltaTime;			// デルタタイム.
	float						m_AnimTimeCnt;			// アニメーションの時間用カウント.
	float						m_Power;				// プレイヤーのジャンプや落下の力.
	float						m_CloudMoveSpeed;		// 雲の移動速度.
	float						m_Score;				// スコア.
	float						m_PlayerFireCnt;		// プレイヤーの炎のアニメーション用カウント.
	float						m_AutoPlay;				// 自動プレイ.
	int							m_PreviousScore;		// 前回のスコア.
	int							m_BestScore;			// ベストスコア.
	int							m_CloudTimeCnt;			// 雲の出現時間用のカウント.
	int							m_TextAnimNo;			// アニメーションするテキストのNo.
	int							m_CoolTime;				// 当たり判定のクールタイム.
	int							m_AutoPlayAction;		// 自動操作の行動.
	int							m_Choice;				// 選択箇所.
	bool						m_IsPlay;				// 操作中か.
	bool						m_IsCloudHit;			// 雲に当たったか.
	bool						m_LeftRotation;			// 左回転させるか.
	bool						m_IsPause;				// ポーズ中か.
	bool						m_IsPauseAnim;			// ポーズのアニメーション中か.
};