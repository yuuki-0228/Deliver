#pragma once
#include "..\SceneBase.h"
#include "..\..\Common\Sprite\Sprite.h"
#include "..\..\Common\Font\Font.h"

class CNormalCamera;
class CGameClearWidget;

namespace {
	// ���U���g�V�[���\�����Ǘ��񋓑�.
	enum class enShowState : unsigned char {
		HeatBonus,			// ���x�{�[�i�X.
		GoldenTimeBonus,	// �S�[���f���^�C���{�[�i�X.
		TiltDiscount,		// �X������.
		TreatmentPay,		// ���Ô�.

		OriginWage,			// �{�[�i�X�A�����𖳎��������z.

		FinalWage			// �ŏI�I�ȓ���.
	} typedef EShowState;

	// �e�l�̃p�����[�^�Ǘ��\����.
	struct stResultValue {
		bool				IsDisp;			// �\��/��\���̐؂�ւ�.
		int					Value;			// �l.
		float				Alpha;			// �����x.
		float				IntervalCount;	// ���̕���������/�\�������܂ł̊�.

		stResultValue()
			:stResultValue( false, 0, 0.0f, 0.0f )
		{}
		stResultValue( bool isdisp, int value, float alpha, float intervalcnt )
			: IsDisp		( isdisp )
			, Value			( value )
			, Alpha			( alpha )
			, IntervalCount	( intervalcnt )
		{}
	} typedef SResultValue;
}

/************************************************
*	�Q�[���N���A�N���X.
*		��J��.
**/
class CGameClear final
	: public CSceneBase
{
public:
	CGameClear();
	~CGameClear();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// ���f���̕`��.
	virtual void ModelRender() override;
	// �X�v���C�g(UI)�̕`��.
	virtual void SpriteUIRender() override;
	// �X�v���C�g(3D)/Effect�̕`��.
	//	_A�F��ɕ\������� / _B�F��ɕ\�������.
	virtual void Sprite3DRender_A() override;
	virtual void Sprite3DRender_B() override;

private:
	// �����̕`�揈��.
	void RenderOriginWage();
	// �ڍ׏��̕`�揈��.
	void RenderDetails();

	// �����\�����̏���.
	void ShowOriginWage();
	// �����\����̏���.
	void UpdateAfterShowTotalWage();

	// �e��{�[�i�X�\�����A����{�^���������ꂽ���̏���.
	void PushButton( int Type );
	// �ڍ׏��̒l�̓����x�𑝉������鏈��.
	void AddAlpha( int Type );
	// �{�[�i�X,����������̏���.
	void AfterAddDetails( int Type );

	// �ڍ׏��̏���.
	void ShowDetails( int Type );

	// �����̌����Đݒ肷��.
	void ResetWageDigit();

	// UI�̕s����������.
	void AddAlpha();

private:
	float								m_DeltaTime;

	// UI.
	std::unique_ptr<CGameClearWidget>	m_pGameClearWidget;

	std::vector<CSprite*>				m_pSpriteList;
	std::vector<SSpriteRenderState>		m_SpriteStateList;
	bool								m_IsStartDisp;
	bool								m_CanOperate;
	float								m_Alpha;
	int									m_Choice;

	// ���ݕ\�����Ă�����.
	EShowState							m_ShowState;

	// �ڍ׏��.
	std::vector<SResultValue>			m_ValueList;

	// �����̕\���Ɏg�p.
	std::vector<float>					m_PosX;				// 1�����������̂Ɏg�p.
	int									m_WageDigits;		// �����̌���.
	std::vector<float>					m_WageFinalXPos;	// �����Ƃ̍ŏI�I��X���W.

	// ���ʉ��Ɏg�p.
	std::vector<bool>					m_IsPlayDigitSE;	// ���ʉ����Đ�������(��).
	std::string							m_DigitSE;			// �Đ�������ʉ�(�����ƂɕύX).
	int									m_ThroughStringCnt;	// �ʉ߂������������J�E���g����.
	bool								m_IsPlayGetBonusSE;	// ���ʉ����Đ�������(�{�[�i�X�擾).

	// �t�H���g.
	CFont*								m_pFont;
	std::vector<SFontRenderState>		m_FontStateList;	// �t�H���g�̐ݒ�.
};