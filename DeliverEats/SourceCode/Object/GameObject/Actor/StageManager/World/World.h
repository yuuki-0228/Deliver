#pragma once
#include "..\..\Actor.h"
#include "..\..\..\..\..\Common\Sprite\Sprite.h"

class CSky;
class CCloud;

/************************************************
*	ワールドクラス.
*		﨑田友輝.
**/
class CWorld final
	: public CActor
{
public:
	CWorld();
	virtual ~CWorld();

	// 初期化.
	virtual bool Init() override;

	// 更新.
	virtual void Update( const float& DeltaTime ) override;

	// 描画.
	virtual void Render() override;
	// スプライト(3D)の描画.
	//	_A：後に表示される / _B：先に表示される.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	std::unique_ptr<CSky>	m_pSky;		// 空.
	std::unique_ptr<CCloud>	m_pCloud;	// 雲.
};