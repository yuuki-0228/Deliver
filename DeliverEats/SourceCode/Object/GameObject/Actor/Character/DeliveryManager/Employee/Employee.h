#pragma once
#include "..\..\Character.h"
#include "..\Linking\Linking.h"

class CCustomer;
class CPlayer;
class CEffectManager;

/************************************************
*	XõNX.
*		ú±cFP.
**/
class CEmployee final
	: public CCharacter
{
public:
	CEmployee( const EObjectTag TypeTag, int ShopNo );
	~CEmployee();

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

	// ½è»èÖ.
	virtual void Collision( CActor* pActor ) override;
	
	// ÅáÀàç¦é¨àÌæ¾.
	inline int GetPizzaMoney() const { return m_Money; }

	// vC[ªß­É¢é©æ¾.
	inline bool GetIsShortly() const { return m_IsShortly; }
	// RÃ¯nhÌæ¾.
	inline CLinking::Handle GetHandle() const { return m_Handle; }
	// RÃ¯³êÄ¢é©æ¾.
	inline bool GetIsLink() const { return m_Handle != NO_LINK_HANDLE; }
	// Ò¿Ô©æ¾.
	inline bool GetIsWait() const { return m_DetachCoolTime > 0.0f; }
	// ¤iðàç¤Xõ©æ¾.
	inline bool GetIsGetFoodEmployee() const { return m_IsGetFoodEmployee; }
	// sUÌæ¾Aj[VªI¹µ½©æ¾.
	inline bool GetIsPizzaPassedAnimEnd() const { return m_IsPizzaPassedAnimEnd; }

	// ó¯æèQ[WÌæ¾.
	inline float GetGauge() const { return m_Gauge; }
	// Q[WíJEgÌæ¾.
	inline float GetDeleteTimeCnt() const { return m_GaugeDeleteTimeCnt; }
	// FÌæ¾.
	inline D3DXCOLOR3 GetLinkColor() const { return CLinking::GetLinkColor( m_Handle ); }

	// ü«ÌÝè.
	inline void SetDirection( const EDirection Dire ) { m_Direction = Dire; }
	// ÇÁÌ¨àÌÝè.
	inline void SetAddMoney( const float AddMoney ) { m_AddMoney = AddMoney; }
	// At@ÌÝè.
	inline void SetAlpha( const float Alpha ) { m_Alpha = Alpha; }
	// bV|CgCgÌÝè.
	inline void SetMeshPointLight( const SMeshPointLight& Light ) { m_PointLight = Light; }
	// vC[ÌÙ¤ð©é©Ýè.
	inline void SetIsLookPlayer( const bool Flag ) { m_IsLookPlayer = Flag; }

	// RÃ¯ÌÝè.
	void AttachLink( const CLinking::Handle& handle );
	// RÃ¯Ìjü.
	void DetachLink();

	// ÊíÉÏX.
	void ChangeNormal();
	// èðUéAj[VÉÏX.
	void ChangeWaveHands();
	// sU ðÂAj[VÉÏX.
	void ChangeHavePizaBox();

protected:
	// ½è»èÌú».
	//	InitÖÌÅãÉÄÔ.
	virtual void InitCollision() override;
	// ½è»èÌXV.
	//	UpdateÖÌÅãÉÄÔ.
	virtual void UpdateCollision() override;

private:
	// vC[ªß­É¢é©Ì½è»è.
	void PlayerShortlyCollision( CActor* pActor );
	// ¤iðn·pÌ½è»è.
	void HandOverFoodCollision( CActor* pActor );

	// gXtH[ÌXV.
	void TransformUpdate();
	// Aj[VÌXV.
	void AnimationUpdate();

	// liðæ¾.
	int GetMoney();

	// A±ÅRÃ¯³êÈ¢æ¤É·éN[^CÌXV.
	void DetachCoolTimeUpdate();

	// sUÌæ¾Aj[VÉÏX.
	void ChangeGetPizza();

	// ÊíÉÏXÌXV.
	void ChangeNormalAnimUpdate();
	// èðUéAj[VÉÏXÌXV.
	void ChangeWaveHandsAnimUpdate();
	// èðUéAj[VÌXV.
	void WaveHandsAnimUpdate();
	// sUðn·Aj[VÉÏXÌXV.
	void ChangeGetPizzaAnimUpdate();
	// sUðn·Aj[VÌXV.
	void PizzaPassedAnimUpdate();
	// sUðnµIíÁ½Aj[VÌXV.
	void PizzaPassedAnimEndUpdate();
	// sU ðÂAj[VÌXV.
	void ChangeHavePizzaAnimUpdate();

private:
	std::vector<CSkinMesh*>			m_pSkinMesh;			// XLbV.
	std::vector<STransform>			m_TransformList;		// gXtH[Xg.
	std::unique_ptr<CEffectManager>	m_pHandOverEffect;		// ¤iðnµ½Æ«ÌGtFNg.
	CSkinMesh*						m_pPizzaBox;			// sU .
	CPlayer*						m_pPlayer;				// vC[.
	SMeshPointLight					m_PointLight;			// |CgCg.
	STransform						m_PizzaBoxTransform;	// sU ÌgXtH[.
	CLinking::Handle				m_Handle;				// RÃ¯nh.
	D3DXVECTOR3						m_RightHandMoveVector;	// EèÌÚ®xNg.
	D3DXVECTOR3						m_LeftHandMoveVector;	// ¶èÌÚ®xNg.
	D3DXPOSITION3					m_RightHandsStartPos;	// EèÌAj[VÌJnÊu.
	D3DXPOSITION3					m_LeftHandsStartPos;	// ¶èÌAj[VÌJnÊu.
	D3DXMATRIX						m_HeadRotMatrix;		// ªÌñ]sñ.
	EObjectTag						m_Type;					// ^CvÌ^O( Apartment, IslandÌÝgp ).
	EDirection						m_Direction;			// ü«.
	float							m_AddMoney;				// ÇÁ·é¨à.
	float							m_Gauge;				// ó¯æèQ[W( 0.0f ~ 1.0f ).
	float							m_GaugeDeleteTimeCnt;	// Q[WÌíJEg.
	float							m_Alpha;				// At@l.
	float							m_DetachCoolTime;		// A±ÅRÃ¯³êÈ¢æ¤É·éN[^C.
	float							m_PizzaBoxAlpha;		// sU ÌAt@l.
	float							m_AnimCnt;				// Aj[VpÌJEg.
	int								m_AnimationNo;			// Aj[VNo.
	int								m_ShopNo;				// ¨XÌ^Cv.
	int								m_Money;				// ÅáÀàç¦é¨à.
	int								m_HeadNo;				// ªÌNo.
	int								m_PizzaBoxNo;			// sU ÌNo.
	bool							m_IsHitPlayer;			// vC[É½Á½©.
	bool							m_IsShortly;			// vC[ªß­É¢é©.
	bool							m_IsPassed;				// nµ½©.
	bool							m_IsGetFoodEmployee;	// ¤iðàç¤Xõ©.
	bool							m_IsWaveHandsMoveUp;	// èðUéÆ«ÉãÉ °é©.
	bool							m_IsPizzaPassedAnimEnd;	// sUðn·Aj[VªI¹µ½©.
	bool							m_IsPizzaBoxAlpha;		// sU ÉAt@ðÝè·é©.
	bool							m_IsLookPlayer;			// vC[Ìûð©é©.

private:
	// ftHgRXgN^ðÖ~·é.
	CEmployee() = delete;
	CEmployee( const CEmployee& ) = delete;
	CEmployee& operator = ( const CEmployee& rhs ) = default;

private:
	// Of[^Ìè.
	const ConstantStruct::SDeliveryCharacter& CONSTANT = CConstant::GetDeliveryCharacter();
};