#pragma once
#include "..\..\..\Actor.h"

class CSignboard;
class CEmployee;
class CSmokeManager;
class CStageIcon;

/************************************************
*	ÌXNX.
*		ú±cFP.
**/
class CIslandShop final
	: public CActor
{
public:
	CIslandShop();
	virtual ~CIslandShop();

	// ú».
	virtual bool Init() override;

	// XV.
	virtual void Update( const float& DeltaTime ) override;
	// ½è»èI¹ãÄÑo³êéXV.
	virtual void LateUpdate( const float& DeltaTime ) override;

	// `æ.
	virtual void Render() override;
	// XvCg(3D)Ì`æ.
	//	_AFãÉ\¦³êé / _BFæÉ\¦³êé.
	virtual void Sprite3DRender_A() override;

	// ¨XÌ^CvÌæ¾.
	int GetShopNo() const { return m_ShopNo; }
	// ¨XÌ^CvÌwè.
	void SetShopNo( const int No ) { m_ShopNo = No; }

	// ÅÂÌÀWÌÝè.
	void SetSignboardPosition( const D3DXPOSITION3& Pos );

	// ½è»èðÇÁ·é.
	void PushCollisionManager();

protected:
	// ½è»èÌú».
	//	InitÖÌÅãÉÄÔ.
	virtual void InitCollision() override;
	// ½è»èÌXV.
	//	UpdateÖÌÅãÉÄÔ.
	virtual void UpdateCollision() override;

private:
	// JÉæéAt@lÌXV.
	void CameraAlphaUpdate();

	// ACRÌXV.
	void IconUpdate();

	// ÌÄ¶.
	void PlaySmoke();

	// Xõð\¦·é½ßÌÚ®.
	void EmployeeDispMove();
	// XõªvC[Ìûð©éæ¤Éñ].
	void EmployeePlayerLook();
	// Xõðñ\¦É·é½ßÌÚ®.
	void EmployeeHideMove();
	// XõÌñ].
	void EmployeeRotation();

	// àðJ¯é.
	void DoorOpen();
	// àðÂ¶½©.
	void DoorClose();

private:
	CSkinMesh*						m_pShopMesh;			// ®bV.
	CSkinMesh*						m_pDoorMesh;			// àbV.
	CCollisionMesh*					m_pCollisionMesh;		// ½è»èpbV.
	std::unique_ptr<CStageIcon>		m_pIcon;				// ACR.
	std::unique_ptr<CSignboard>		m_pSignboard;			// ÅÂ.
	std::unique_ptr<CEmployee>		m_pEmployee;			// Xõ.
	std::unique_ptr<CSmokeManager>	m_pSmokeManager;		// }l[W[.
	SMeshPointLight					m_PointLight;			// |CgCg.
	SMeshPointLight					m_EmployeeLight;		// XõÌ|CgCgðú»·ép.
	STransform						m_EmployeeTransform;	// XõÌgXtH[.
	STransform						m_DoorTransform;		// àÌgXtH[.
	D3DXROTATION3					m_EmployeeAddRotation;	// XõÌÇÁÌpx
	D3DXPOSITION3					m_SmokePlayPosition;	// ÌÄ¶Êu.
	D3DXPOSITION3					m_StartPos;				// JnÀW.
	D3DXPOSITION3					m_EndPos;				// I¹ÀW.
	D3DXVECTOR3						m_MoveVector;			// Ú®xNg.
	D3DXPOSITION3					m_DoorStartPos;			// àÌJnÀW.
	D3DXPOSITION3					m_DoorEndPos;			// àÌI¹ÀW.
	D3DXVECTOR3						m_DoorMoveVector;		// àÌÚ®xNg.
	float							m_MoveCnt;				// Ú®pÌJEg.
	float							m_DoorMoveCnt;			// àÚ®pÌJEg.
	float							m_Alpha;				// At@l.
	int								m_ShopNo;				// ¨XÌ^Cv.
	bool							m_IsRotEnd;				// ñ]I¹µ½©.
	bool							m_IsOpenEnd;			// ðJ¯½©.
	bool							m_IsCloseEnd;			// ðÂ¶½©.
	bool							m_IsSmoke;				// ðo·©.
	bool							m_IsOpenSE;				// SEðÄ¶µ½©.

private:
	// Of[^Ìè.
	const ConstantStruct::SIsland& CONSTANT = CConstant::GetIsland();
};