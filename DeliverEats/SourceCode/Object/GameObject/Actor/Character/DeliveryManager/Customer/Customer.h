#pragma once
#include "..\Linking\Linking.h"
#include "..\..\Character.h"
#include "..\..\..\RuckSack\RuckSack.h"

class CEmployee;
class CPlayer;
class CEffectManager;

/************************************************
*	お客さんクラス.
*		﨑田友輝.
**/
class CCustomer final
	: public CCharacter
{
public:
	CCustomer( const EObjectTag TypeTag );
	~CCustomer();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;

	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// 紐づけハンドルの取得.
	inline CLinking::Handle GetHandle() const { return m_Handle; }
	// 色の取得.
	inline D3DXCOLOR3 GetLinkColor() const { return CLinking::GetLinkColor( m_Handle ); }

	// プレイヤーが近くにいるか取得.
	inline bool GetIsShortly() const { return m_IsShortly; }
	// 紐づけされているか取得.
	inline bool GetIsLink() const { return m_Handle != NO_LINK_HANDLE; }
	// 待ち時間か取得.
	inline bool GetIsWait() const { return m_DetachCoolTime > 0.0f; }
	// 渡しに行くお客さんか取得.
	inline bool GetIsGoHandingCustomer() const { return m_IsGoHandingCustomer; }
	// ピザの取得アニメーションが終了したか取得.
	inline bool GetIsGetPizzaAnimEnd() const { return m_IsGetPizzaAnimEnd; }

	// 受け取りゲージの取得.
	inline float GetGauge() const { return m_Gauge; }
	// ゲージ削除カウントの取得.
	inline float GetDeleteTimeCnt() const { return m_GaugeDeleteTimeCnt; }
	// 追加するお金の取得.
	inline float GetAddMoney() const { return m_AddMoney; }

	// 向きの設定.
	inline void SetDirection( const EDirection Dire ) { m_Direction = Dire; }
	// 追加のお金の設定.
	inline void SetAddMoney( const float AddMoney ) { m_AddMoney = AddMoney; }
	// アルファの設定.
	inline void SetAlpha( const float Alpha ) { m_Alpha = Alpha; }
	// メッシュポイントライトの設定.
	inline void SetMeshPointLight( const SMeshPointLight& Light ) { m_PointLight = Light; }
	// 渡しに行くお客さんに設定.
	void SetGoHandingCustomer();

	// お店の設定.
	void AttachLink( const CLinking::Handle& handle );
	// 紐づけの破棄.
	void DetachLink();

	// 通常時に変更.
	void ChangeNormal();
	// 手を振るアニメーションに変更.
	void ChangeWaveHands();

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

private:
	// プレイヤーが近くにいるかの当たり判定.
	void PlayerShortlyCollision( CActor* pActor );
	// 商品の受け取り用の当たり判定.
	void GetFoodCollision( CActor* pActor );

	// トランスフォームの更新.
	void TransformUpdate();
	// アニメーションの更新.
	void AnimationUpdate();

	// 通知を送る.
	void PushNotification( const SFoodState& FoodState );

	// 連続で紐づけされないようにするクールタイムの更新.
	void DetachCoolTimeUpdate();

	// ピザの取得アニメーションに変更.
	void ChangeGetPizza();

	// 通常時に変更の更新.
	void ChangeNormalAnimUpdate();
	// 手を振るアニメーションに変更の更新.
	void ChangeWaveHandsAnimUpdate();
	// 手を振るアニメーションの更新.
	void WaveHandsAnimUpdate();
	// ピザの取得アニメーションに変更の更新.
	void ChangeGetPizzaAnimUpdate();
	// ピザの取得アニメーションの更新.
	void GetPizzaAnimUpdate();
	// 通常に戻すアニメーションの更新.
	void GetPizzaAnimEndUpdate();

private:
	std::vector<CSkinMesh*>			m_pSkinMesh;			// スキンメッシュ.
	std::vector<STransform>			m_TransformList;		// トランスフォームリスト.
	std::unique_ptr<CEffectManager>	m_pGetEffect;			// 商品をもらった時のエフェクト.
	CSkinMesh*						m_pPizzaBox;			// ピザ箱.
	CPlayer*						m_pPlayer;				// プレイヤー.
	SMeshPointLight					m_PointLight;			// ポイントライト.
	STransform						m_PizzaBoxTransform;	// ピザ箱のトランスフォーム.
	CLinking::Handle				m_Handle;				// 紐づけハンドル.
	D3DXVECTOR3						m_RightHandMoveVector;	// 右手の移動ベクトル.
	D3DXVECTOR3						m_LeftHandMoveVector;	// 左手の移動ベクトル.
	D3DXPOSITION3					m_RightHandsStartPos;	// 右手のアニメーションの開始位置.
	D3DXPOSITION3					m_LeftHandsStartPos;	// 左手のアニメーションの開始位置.
	D3DXMATRIX						m_HeadRotMatrix;		// 頭の回転行列.
	EObjectTag						m_Type;					// タイプのタグ( Apartment, Islandのみ使用 ).
	EDirection						m_Direction;			// 向き.
	float							m_AddMoney;				// 追加するお金.
	float							m_Gauge;				// 受け取りゲージ( 0.0f ~ 1.0f ).
	float							m_GaugeDeleteTimeCnt;	// ゲージの削除カウント.
	float							m_Alpha;				// アルファ値.
	float							m_DetachCoolTime;		// 連続で紐づけされないようにするクールタイム.
	float							m_PizzaBoxAlpha;		// ピザ箱のアルファ値.
	float							m_AnimCnt;				// アニメーション用のカウント.
	int								m_AnimationNo;			// アニメーションNo.
	int								m_BodyNo;				// 体のNo.
	int								m_HeadNo;				// 頭のNo.
	int								m_PizzaBoxNo;			// ピザ箱のNo.
	bool							m_IsHitPlayer;			// プレイヤーに当たったか.
	bool							m_IsShortly;			// プレイヤーが近くにいるか.
	bool							m_IsGoHandingCustomer;	// 渡しに行くお客さんか.
	bool							m_IsWaveHandsMoveUp;	// 手を振るときに上にあげるか.
	bool							m_IsGetPizzaAnimEnd;	// ピザの取得アニメーションが終了したか.

private:
	// デフォルトコンストラクタを禁止する.
	CCustomer() = delete;
	CCustomer( const CCustomer& ) = delete;
	CCustomer& operator = ( const CCustomer& rhs ) = default;

private:
	// 外部データの定数.
	const ConstantStruct::SDeliveryCharacter& CONSTANT = CConstant::GetDeliveryCharacter();
};