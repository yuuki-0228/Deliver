#pragma once
#include "..\Character.h"
#include "..\..\RuckSack\RuckSack.h"

class CMainCamera;
class CGazingCamera;
class CEffectManager;
class CCustomer;
class CEmployee;
class CCustomerDire;

/**************************************************
*	プレイヤークラス.
*		﨑田友輝.
**/
class CPlayer final
	: public CCharacter
{
public:
	using Effect_List = std::vector<std::unique_ptr<CEffectManager>>;

public:
	CPlayer();
	virtual ~CPlayer();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 当たり判定終了後呼び出される更新.
	virtual void LateUpdate( const float& DeltaTime ) override;
	// デバックの更新.
	virtual void DebugUpdate( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// 影の描画.
	//	プレイヤーが隠れた時に描画する.
	void ShadowRender();

	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;

	// 当たり判定関数.
	virtual void Collision( CActor* pActor ) override;

	// 商品を渡す.
	void FoodPassed();

	// 向きベクトルの取得.
	D3DXVECTOR3 GetDireVector();

	// 商品を持っているか取得.
	inline bool GetIsFoodHave() const { return m_pRuckSack->GetIsFoodHave(); }
	// 現在の商品情報を取得.
	inline SFoodState GetFoodState() const { return m_pRuckSack->GetFoodState(); }
	// 左右のゲージの位置を取得( -1.0f ~ 1.0f ).
	inline float GetRuckSackLRGauge() const { return m_pRuckSack->GetLRGauge(); }
	// 上下のゲージの位置を取得( -1.0f ~ 1.0f ).
	inline float GetRuckSackUDGauge() const { return m_pRuckSack->GetUDGauge(); }

	// 店員/お客さんが見る位置を取得.
	D3DXPOSITION3 GetLookPos();

	// 開始時の位置を取得.
	D3DXPOSITION3 GetStartPosition() const { return m_StartPosition; }

	// 着地しているかを取得.
	bool GetIsLanding();
	// 浮島の近くにいるか取得.
	bool GetIsIsland();
	// 注視カメラを使用しているかを取得.
	bool GetIsActiveGazingCam();

	// ベクトルの追加.
	inline void PushVector( const D3DXVECTOR3& Vec ) { m_MoveVectorList.emplace_back( Vec ); }

	// 開始位置に設定.
	void SetStartPosition( const D3DXPOSITION3& Pos );

	//　商品を受け取る.
	void SetNewFood( const int Money, int ShopNo );
	// 風の設定.
	void SetWind( const D3DXVECTOR3& Vec );

protected:
	// 当たり判定の初期化.
	//	Init関数の最後に呼ぶ.
	virtual void InitCollision() override;
	// 当たり判定の更新.
	//	Update関数の最後に呼ぶ.
	virtual void UpdateCollision() override;

private:
	// 浮島の当たり判定.
	void IslandCollision( CActor* pActor );
	// プレイヤーの家の着地の当たり判定.
	void IsPlayerHouseCollision( CActor* pActor );
	// ジェットパックのエフェクトの当たり判定.
	void JetPackEffectCollision( CActor* pActor );
	// 地面の当たり判定.
	void GroundCollision( CActor* pActor );
	// 地面に近いかの当たり判定.
	void GroundCloseCollision( CActor* pActor );
	// 影の当たり判定.
	void ShadowCollision( CActor* pActor );
	// 天井の当たり判定.
	void CeilingCollision( CActor* pActor );
	// 壁の当たり判定.
	void WallCollision( CActor* pActor );

	// カメラの更新.
	void CameraControl();
	// 注視カメラからメインカメラに切り替える.
	void SwitchToMain();
	// メインカメラから注視カメラに切り替える.
	void SwitchToGazing(CActor& pCustmer);

	// トランスフォームの更新.
	void TransformUpdate();
	// リュックの色の設定.
	void SetRuckSackColor();

	// ジェットパックの噴射.
	void JetPackJetControl();
	// ジェットパックの停止.
	void JetPackStop();
	// ジェットパックの停止の修復.
	void JetPackRepair();
	// ジェットパックの回転.
	void JetPackRotationControl();
	// プレイヤーの移動.
	void PlayerMove();
	// プレイヤーの回転.
	void PlayerRotation();
	// プレイヤーの傾きの更新.
	void PlayerTiltUpdate();
	
	// 移動ベクトルの作成.
	void CreateMoveVec();
	// 左右のジェットパックの回転ベクトルの作成.
	void CreateRotVec();

	// 重力の追加.
	void AddGravity();
	// 加速度の上限/下限の設定.
	void AccelerationClamp();
	// 摩擦
	void Friction( const float Value );

	// 着地エフェクトの再生.
	void LandingEffectPlay( const D3DXPOSITION3& Pos );
	// 着地エフェクトの更新.
	void LandingEffectUpdate();
	// 商品取得アニメーションの更新.
	void GetFoodAnimUpdate();

	// ステージ外か確認.
	void OffStageCheck();
	// ステージ外に移動した時の更新.
	void StageOutUpdate();

	// 移動のSEの再生.
	void PlayMoveSE();

protected:
	std::unique_ptr<CMainCamera>				m_pMainCamera;				// メインカメラ.
	std::unique_ptr<CGazingCamera>				m_pGazingCamera;			// 注視カメラ.
	std::vector<std::unique_ptr<CRay>>			m_pRayList;					// レイリスト.
	std::unique_ptr<CRuckSack>					m_pRuckSack;				// リュック.
	std::unique_ptr<CCustomerDire>				m_pCustomerDire;			// 渡しに行くお客さんの方向.
	std::vector<CSkinMesh*>						m_pSkinMesh;				// スキンメッシュ.
	std::vector<STransform>						m_TransformList;			// トランスフォーム.
	std::vector<D3DXVECTOR3>					m_MoveVectorList;			// 移動ベクトルリスト.
	std::vector<D3DXVECTOR3>					m_VectorList;				// ベクトルリスト.
	std::vector<float>							m_LJetPackInputList;		// 左ジェットパックの入力リスト.
	std::vector<float>							m_RJetPackInputList;		// 右ジェットパックの入力リスト.
	std::vector<float>							m_CntList;					// カウントリスト.
	std::vector<float>							m_SEVolumeList;				// SEの音量リスト.
	std::vector<bool>							m_FlagList;					// フラグリスト.
	CSprite*									m_pShadow;					// 影.
	SSpriteRenderState							m_ShadowState;				// 影の情報.
	Effect_List									m_pEffectList;				// エフェクトリスト.
	Effect_List									m_pLandingEffectList;		// 着地エフェクト.
	CCustomer*									m_pOldCustomer;				// 1フレーム前のお客さんの情報(渡した瞬間を取得するのに必要).
	SAnimationController						m_AC;						// アニメーションコントローラー.
	D3DXPOSITION3								m_LandingEndPos;			// 着地の最後の座標.
	D3DXPOSITION3								m_StartPosition;			// 開始時の位置.
	std::pair<float, float>						m_ShadowPosY;				// 影の位置<距離, Y座標>.
	float										m_Alpha;					// アルファ値.
	float										m_SideMoveSpeed;			// 横方向に対しての移動速度.
	float										m_VerticalMoveSpeed;		// 縦方向に対しての移動速度.

private:
	// 外部データの定数.
	const ConstantStruct::SPlayer& CONSTANT = CConstant::GetPlayer();
};
