#pragma once
#include "..\..\Character.h"
#include "..\Linking\Linking.h"

class CCustomer;
class CPlayer;
class CEffectManager;

/************************************************
*	店員クラス.
*		﨑田友輝.
**/
class CEmployee final
	: public CCharacter
{
public:
	CEmployee( const EObjectTag TypeTag, int ShopNo );
	~CEmployee();

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
	
	// 最低限もらえるお金の取得.
	inline int GetPizzaMoney() const { return m_Money; }

	// プレイヤーが近くにいるか取得.
	inline bool GetIsShortly() const { return m_IsShortly; }
	// 紐づけハンドルの取得.
	inline CLinking::Handle GetHandle() const { return m_Handle; }
	// 紐づけされているか取得.
	inline bool GetIsLink() const { return m_Handle != NO_LINK_HANDLE; }
	// 待ち時間か取得.
	inline bool GetIsWait() const { return m_DetachCoolTime > 0.0f; }
	// 商品をもらう店員か取得.
	inline bool GetIsGetFoodEmployee() const { return m_IsGetFoodEmployee; }
	// ピザの取得アニメーションが終了したか取得.
	inline bool GetIsPizzaPassedAnimEnd() const { return m_IsPizzaPassedAnimEnd; }

	// 受け取りゲージの取得.
	inline float GetGauge() const { return m_Gauge; }
	// ゲージ削除カウントの取得.
	inline float GetDeleteTimeCnt() const { return m_GaugeDeleteTimeCnt; }
	// 色の取得.
	inline D3DXCOLOR3 GetLinkColor() const { return CLinking::GetLinkColor( m_Handle ); }

	// 向きの設定.
	inline void SetDirection( const EDirection Dire ) { m_Direction = Dire; }
	// 追加のお金の設定.
	inline void SetAddMoney( const float AddMoney ) { m_AddMoney = AddMoney; }
	// アルファの設定.
	inline void SetAlpha( const float Alpha ) { m_Alpha = Alpha; }
	// メッシュポイントライトの設定.
	inline void SetMeshPointLight( const SMeshPointLight& Light ) { m_PointLight = Light; }
	// プレイヤーのほうを見るか設定.
	inline void SetIsLookPlayer( const bool Flag ) { m_IsLookPlayer = Flag; }

	// 紐づけの設定.
	void AttachLink( const CLinking::Handle& handle );
	// 紐づけの破棄.
	void DetachLink();

	// 通常時に変更.
	void ChangeNormal();
	// 手を振るアニメーションに変更.
	void ChangeWaveHands();
	// ピザ箱を持つアニメーションに変更.
	void ChangeHavePizaBox();

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
	// 商品を渡す用の当たり判定.
	void HandOverFoodCollision( CActor* pActor );

	// トランスフォームの更新.
	void TransformUpdate();
	// アニメーションの更新.
	void AnimationUpdate();

	// 値段を取得.
	int GetMoney();

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
	// ピザを渡すアニメーションに変更の更新.
	void ChangeGetPizzaAnimUpdate();
	// ピザを渡すアニメーションの更新.
	void PizzaPassedAnimUpdate();
	// ピザを渡し終わったアニメーションの更新.
	void PizzaPassedAnimEndUpdate();
	// ピザ箱を持つアニメーションの更新.
	void ChangeHavePizzaAnimUpdate();

private:
	std::vector<CSkinMesh*>			m_pSkinMesh;			// スキンメッシュ.
	std::vector<STransform>			m_TransformList;		// トランスフォームリスト.
	std::unique_ptr<CEffectManager>	m_pHandOverEffect;		// 商品を渡したときのエフェクト.
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
	int								m_ShopNo;				// お店のタイプ.
	int								m_Money;				// 最低限もらえるお金.
	int								m_HeadNo;				// 頭のNo.
	int								m_PizzaBoxNo;			// ピザ箱のNo.
	bool							m_IsHitPlayer;			// プレイヤーに当たったか.
	bool							m_IsShortly;			// プレイヤーが近くにいるか.
	bool							m_IsPassed;				// 渡したか.
	bool							m_IsGetFoodEmployee;	// 商品をもらう店員か.
	bool							m_IsWaveHandsMoveUp;	// 手を振るときに上にあげるか.
	bool							m_IsPizzaPassedAnimEnd;	// ピザを渡すアニメーションが終了したか.
	bool							m_IsPizzaBoxAlpha;		// ピザ箱にアルファを設定するか.
	bool							m_IsLookPlayer;			// プレイヤーの方を見るか.

private:
	// デフォルトコンストラクタを禁止する.
	CEmployee() = delete;
	CEmployee( const CEmployee& ) = delete;
	CEmployee& operator = ( const CEmployee& rhs ) = default;

private:
	// 外部データの定数.
	const ConstantStruct::SDeliveryCharacter& CONSTANT = CConstant::GetDeliveryCharacter();
};