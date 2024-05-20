#pragma once
#include "..\Character.h"

/************************************************
*	コンピュータクラス.
**/
class CComputer final
	: public CCharacter
{
public:
	CComputer();
	virtual ~CComputer();

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

private:
	// トランスフォームの更新.
	void TransformUpdate();

private:
	std::vector<CSkinMesh*>	m_pSkinMesh;		// スキンメッシュ.
	std::vector<STransform>	m_TransformList;	// トランスフォーム.

};
