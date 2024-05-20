#pragma once
#include "..\..\..\..\Actor.h"

class CEmployee;
class CSmokeManager;
class CPizzaKilnFire;
class CStageIcon;

/************************************************
*	マンションタイプのお店クラス.
*		﨑田友輝.
**/
class CApartmentShop final
	: public CActor
{
public:
	CApartmentShop();
	virtual ~CApartmentShop();

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

	// お店のタイプの取得.
	int GetShopNo() const { return m_ShopNo; }
	// お店のタイプの指定.
	void SetShopNo( const int No ) { m_ShopNo = No; }

	// 表示するか.
	void SetIsDisp( const bool Flag ) { m_FloorIsDisp = Flag; }
	// アルファの設定.
	void SetAlpha( const float Alpha );

private:
	// 部屋の向きを取得.
	EDirection GetRoomDirection();

	// アイコンの更新.
	void IconUpdate();

	// 店員の更新.
	void EmployeeUpdate();
	// 店員の回転.
	void EmployeeRotation( const float EndRot );

	// レジに移動.
	void GoResistor();
	// 冷蔵庫に移動.
	void GoRefrigerator();
	// 窯に移動.
	void GoKiln();

	// 煙の再生.
	void PlaySmoke();

	// 店員がプレイヤーの方を見るように回転.
	void EmployeePlayerLook();

private:
	CSkinMesh*						m_pShopMesh;			// 部屋メッシュ.
	std::unique_ptr<CStageIcon>		m_pIcon;				// アイコン.
	std::unique_ptr<CPizzaKilnFire>	m_pFire;				// ピザ窯の炎.
	std::unique_ptr<CEmployee>		m_pEmployee;			// 店員.
	std::unique_ptr<CSmokeManager>	m_pSmokeManager;		// 煙マネージャー.
	SMeshPointLight					m_PointLight;			// ポイントライト.
	STransform						m_EmployeeTransform;	// 店員のトランスフォーム.
	D3DXROTATION3					m_EmployeeAddRotation;	// 店員の追加の角度.
	D3DXPOSITION3					m_SmokePlayPosition;	// 煙の再生位置.
	std::vector<D3DXPOSITION3>		m_EmployeeSetPosition;	// 店員の設定する座標.
	std::vector<D3DXVECTOR3>		m_EmployeeMoveVectir;	// 店員の移動ベクトル.
	float							m_Alpha;				// アルファ値.
	float							m_MoveCnt;				// 移動用カウント.
	int								m_ShopNo;				// お店のタイプ.
	int								m_AnimationNo;			// アニメーションNo.
	bool							m_FloorIsDisp;			// 1フロア単位の表示するかどうか.
	bool							m_RotationEnd;			// 回転が終了したか.
	bool							m_IsSmoke;				// 煙を出すか.
	bool							m_OldSEBGMPlay;			// 前回BGMを再生したか.

private:
	// 外部データの定数.
	const ConstantStruct::SApartment& CONSTANT = CConstant::GetApartment();
};
