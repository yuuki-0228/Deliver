#pragma once
#include "..\..\Actor.h"

class CPlayer;
class CIslandPlayerHouse;
class CIslandHouse;
class CIslandShop;

/************************************************
*	NX.
*		ϊ±cFP.
**/
class CIsland final
	: public CActor
{
public:
	CIsland();
	virtual ~CIsland();

	// ϊ».
	virtual bool Init() override;

	// XV.
	virtual void Update( const float& DeltaTime ) override;
	// ½θ»θIΉγΔΡo³κιXV.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// `ζ.
	virtual void Render() override;
	// XvCg(3D)Μ`ζ.
	//	_AFγΙ\¦³κι / _BFζΙ\¦³κι.
	virtual void Sprite3DRender_A() override;

	// Xe[WGfB^ΜImGuiΜ`ζ.
	void StageEditorImGuiRender();
	// ΜξρπImGuiΕ`ζ.
	void IslandStateImGuiRender();

	// Μf[^ΜζΎ.
	json GetIslandData();

	// Μμ¬.
	void Create( json j );

	// vC[πJnΚuΙέθ.
	bool SetPlayerStartPos( CPlayer* pPlayer );

protected:
	// ½θ»θΜϊ».
	//	InitΦΜΕγΙΔΤ.
	virtual void InitCollision() override;
	// ½θ»θΜXV.
	//	UpdateΦΜΕγΙΔΤ.
	virtual void UpdateCollision() override;

private:
	// JΙζιAt@lΜXV.
	void CameraAlphaUpdate();

private:
	std::unique_ptr<CIslandPlayerHouse>	m_pPlayerHouse;		// vC[ΜΖ.
	std::unique_ptr<CIslandHouse>		m_pHouse;			// Ζ.
	std::unique_ptr<CIslandShop>		m_pShop;			// ¨X.
	CSkinMesh*							m_pIslandMash;		// f.
	CCollisionMesh*						m_pCollisionMesh;	// ½θ»θpbV.
	float								m_Alpha;			// At@l.

private:
	// Of[^Μθ.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};
