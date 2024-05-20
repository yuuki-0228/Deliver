#pragma once
#include "..\..\Actor.h"

/************************************************
*	煙クラス.
**/
class CSmoke final
	: public CActor
{
public:
	CSmoke();
	virtual ~CSmoke();

	// 初期化.
	virtual bool Init() override;
	// 更新.
	virtual void Update( const float& deltaTime ) override;
	// 描画.
	virtual void Render() override;

	// アニメーションの開始.
	void AnimPlay( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& AddMoveVec );
	// アニメーションが終了したか.
	inline bool IsAnimEnd() const { return m_AnimEnd; }

private:
	CCollisionMesh*	m_pSmokeMesh;	// 煙モデル.
	D3DXVECTOR3		m_MoveVec;		// 移動ベクトル.
	D3DXVECTOR3		m_AddMoveVec;	// 追加の移動ベクトル.
	float			m_DAng;			// 拡縮アニメーション用角度.
	float			m_ScaleSpeed;	// 拡縮アニメーション速度.
	float			m_Alpha;		// アルファ値.
	bool			m_AnimEnd;		// アニメーションが終了したか.

private:
	// 外部データの定数.
	const ConstantStruct::SSmoke& CONSTANT = CConstant::GetSmoke();
};