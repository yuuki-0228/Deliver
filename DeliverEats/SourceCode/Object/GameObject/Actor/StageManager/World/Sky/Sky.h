#pragma once
#include "..\..\..\Actor.h"

class CLensFlare;
class CSkyBoxMesh;

/************************************************
*	空クラス.
*		﨑田友輝.
**/
class CSky final
	: public CActor
{
public:
	CSky();
	virtual ~CSky();

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
	CSkyBoxMesh*				m_pColorMesh;			// 空の色用のメッシュ.
	CSkyBoxMesh*				m_pSunAndMonn;			// 太陽と月メッシュ.
	CSkyBoxMesh*				m_pCloudMesh;			// 雲メッシュ.
	STransform					m_CloudTransform;		// 雲のトランスフォーム.
	STransform					m_SunAndMonnTransform;	// 太陽と月のトランスフォーム.
	D3DXCOLOR3					m_CloudColor;			// 雲の色.
	D3DXCOLOR3					m_SkyColor;				// 空の色.
	std::vector<D3DXVECTOR3>	m_SkyColorVector;		// 空の色の変化用.

private:
	// 外部データの定数.
	const ConstantStruct::STime& CONSTANT = CConstant::GetTime();
};