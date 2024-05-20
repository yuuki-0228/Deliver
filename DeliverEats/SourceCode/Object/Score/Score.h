#pragma once
#include "..\..\Object\GameObject\Actor\RuckSack\RuckSack.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Common\Font\Font.h"

/************************************************
*	�X�R�A/�����Ǘ��N���X.
*		��J��.
**/
class CScore final
{
public:
	CScore();
	~CScore();

	// ������ݒ�.
	static void SetScore ( const SFoodState& Wage );

	// �������擾.
	static SFoodState GetScore() { return CScore::GetInstance()->m_FinalWage; }

	// ������.
	static void Init();

	// �X�V.
	static void Update( const float& DeltaTime );

	// �`��.
	static void Render();

	// �����I�ɃQ�[�����C�����I�������ۂɌĂяo��.
	static void SetGameEnd() { CScore::GetInstance()->m_EndFlag  = true; }
	static void SetHomeEnd() { CScore::GetInstance()->m_HomeFlag = true; }
	static bool GetGameEnd() { return CScore::GetInstance()->m_EndFlag; }

	// ���O�̏o��.
	static void SaveLog();

private:
	// �C���X�^���X�̎擾.
	static CScore* GetInstance();

	// �����̕`�揈��.
	void RenderWage();

	// �����̓���̕`�揈��.
	void RenderWageDetails();

	// �X�N���[������.
	void WageDetailsScroll();

private:
	float				m_DeltaTime;

	// �X�R�A(����).
	int					m_TreatmentPay;
	SFoodState			m_FinalWage;
	float				m_Alpha;	// ���ߓx.
	bool				m_EndFlag;	// �Q�[�����C���������I���ɂȂ��true�ɂȂ�.
	bool				m_HomeFlag;	// �A����true�ɂȂ�.

	// �X�R�A�̏ڍ�.
	SFoodState			m_FoodState;

	CSprite*			m_pWageDetailsFrame;
	SSpriteRenderState	m_WageDetailsFrameState;

	// �X�R�A�̓���\������.
	bool					m_IsDisp;	// �\������?.
	float					m_ViewTime;	// �\������.
	float					m_AddScroll;// �X�N���[���ŉ��Z�����l.
	bool					m_IsReflect;// �����̏ڍׂ��܂�Ԃ�����.

	CFont*					m_pFont;
	SFontRenderStateList	m_FontStateList;

	std::string				m_ViewFinalWage;

private:
	// �v���C�f�[�^�\����.
	struct stPlayData {
		int			EndScore;		// �I���X�R�A.
		int			GetScore;		// �擾�X�R�A.
		int			HotBonus;		// ���x�{�[�i�X.
		int			GoldenBonus;	// �S�[���f���^�C���{�[�i�X.
		int			DiscountMoney;	// �X������.
		int			EndNo;			// �I���^�C�v.
	} typedef SPlayData;
	std::vector<SPlayData> m_PlayDataList;		// �v���C�f�[�^.
};