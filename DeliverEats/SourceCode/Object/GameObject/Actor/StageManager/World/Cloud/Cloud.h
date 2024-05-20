#pragma once
#include "..\..\..\Actor.h"

/************************************************
*	雲クラス.
*		﨑田友輝.
**/
class CCloud final
	: public CActor
{
public:
	CCloud();
	virtual ~CCloud();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;

private:
	std::vector<CSprite*>	m_pCloud;				// 雲.
	SSpriteRenderStateList	m_CloudState;			// 雲の情報.

private:
	// 外部データの定数.
	const ConstantStruct::SStage& CONSTANT = CConstant::GetStage();
};