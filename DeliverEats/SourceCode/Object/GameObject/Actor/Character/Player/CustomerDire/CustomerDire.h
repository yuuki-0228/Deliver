#pragma once
#include "..\..\..\Actor.h"

class CPlayer;
class CCustomer;

/************************************************
*	渡しに行くお客さんの方向クラス.
**/
class CCustomerDire final
	: public CActor
{
public:
	CCustomerDire();
	virtual ~CCustomerDire();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;
	// 描画.
	virtual void Render() override;

	// プレイヤーの設定.
	void SetPlayer( CPlayer* pPlayer ) { m_pPlayer = pPlayer; }

private:
	CCollisionMesh*				m_pCustomerDire;		// お客さんの方向メッシュ.
	CPlayer*					m_pPlayer;				// プレイヤー.
	CCustomer*					m_pGoHandingCustomer;	// 渡しに行くお客さん.
	D3DXMATRIX					m_mCustomerDireRot;		// お客さんの向き行列.
	std::vector<STransform>		m_TransformList;		// トランスフォームリスト.
	float						m_AnimCnt;				// アニメーション用カウント.
	float						m_DecAlpha;				// 減らすアルファ値.

private:
	// 外部データの定数.
	const ConstantStruct::SCustomerDire& CONSTANT = CConstant::GetCustomerDire();
};
