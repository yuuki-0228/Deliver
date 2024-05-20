#pragma once
#include "..\EditorBase.h"
#include "..\..\Common\Font\Font.h"

class CStageManager;

/************************************************
*	�s�U�G�f�B�^.
*		���c�F�P.
**/
class CPizzaEditor
	: public CEditorBase
{
public:
	using MaskList			= std::vector<std::vector<ID3D11ShaderResourceView*>>;
	using MaskNoMap			= std::unordered_map<ID3D11ShaderResourceView*, std::pair<int, int>>;
	using SavePizzaDataList = std::vector<std::pair<std::string, json>>;

public:
	CPizzaEditor();
	~CPizzaEditor();

	// ������.
	virtual bool Init() override;

	// �X�V.
	virtual void Update( const float& DeltaTime ) override;

	// ImGui�`��.
	virtual void ImGuiRender() override {};
	// ���f���̕`��.
	virtual void ModelRender() override {};
	// UI�̕`��.
	virtual void UIRender() override;
	// �G�t�F�N�g�`��.
	virtual void EffectRneder() override {};

private:
	// �J�[�\���̈ړ�.
	void CursorMove();

	// �V�����\�[�X�̎擾.
	void NewSauce();
	// �V������ނ̎擾.
	void NewIngredient();

	// ��ނ݂̒͂��J�n����.
	void GrabStart();
	// ��ނ݂̒͂��I������.
	void IngredientGrabEnd();
	// ��ނ̃\�[�X�݂̒͂��I������.
	void IngredientSauceGrabEnd();
	// �\�[�X�݂̒͂��I������.
	void SauceGrabEnd();

	// �݂͂̍X�V.
	void GradUpdate();
	// �͂�ł��ނ̉�].
	void GradRotation();

	// �㉺�{�^�������������̍X�V.
	void PushUpDownButton();
	// ���E�{�^�������������̍X�V.
	void PushLeftRightButton();
	// �\�[�X�̕ύX.
	void ChangeSauce();
	// ��ނ̕ύX.
	void ChangeIngredient();
	// �e�L�X�g�{�^�������������̍X�V.
	void PushTextButton();
	// �s�U�̕ۑ�.
	void PizzaSave( const int ButtonNo );
	// �s�U�̓ǂݍ���.
	void PizzaLoad( const int ButtonNo );
	// �s�U�̍폜.
	void PizzaDelete( const int ButtonNo );
	// �s�U�̃_�E�����[�h.
	void PizzaDownload( const int ButtonNo );

	// �폜�G���A�̍X�V.
	void TrashAreaUpdate();
	// ��ނ̍폜�̍X�V.
	void IngredientDeleteUpdate();

	// �q�ɓ����A�j���[�V�����̍X�V.
	void PutInKilnAnimUpdate();
	// �w����\��.
	void SpatulaDisp();
	// �w����߂�.
	void SpatulaBack();
	// �q�ɓ����.
	void PutInKiln();
	// �s�U���Ă�.
	void CookPizza();
	// �q����o��.
	void RemoveFromKiln();

	// �|�[�Y�Ɉړ�.
	void MovePause();
	// �|�[�Y��ʂ��J��.
	void PauseOpen();
	// �|�[�Y��ʂ����.
	void PauseClose();
	// �|�[�Y���̍X�V.
	void PauseUpdate();

	// ��������̍X�V.
	void TextUpdate();

	// �Z�[�u�G���A�̍X�V.
	void SaveAreaUpdate();
	// �Z�[�u�G���A�̃{�^���̍X�V.
	void SaveAreaButtonUpdate();
	// �ۑ����Ă���s�U�̃T���v���̏�����.
	void SavePizzaSampleInit();
	// �ۑ����Ă���s�U�̃T���v���̕`��.
	void SavePizzaSampleRender();

	// �h���b�N&�h���b�v�̍X�V.
	void DragAndDropUpdate();

private:
	CSprite*							m_pBack;						// �w�i.
	CSprite*							m_pPizzaBase;					// ���n.
	CSprite*							m_pIngredient;					// ���.
	CSprite*							m_pIngredientSauce;				// ��ނ̃\�[�X.
	CSprite*							m_pIngredientArea;				// ��ރG���A.
	CSprite*							m_pSauce;						// �\�[�X.
	CSprite*							m_pSauceArea;					// �\�[�X�G���A.
	CSprite*							m_pSpatula;						// �w��.
	CSprite*							m_pKiln;						// �s�U�q.
	CSprite*							m_pCursor;						// �J�[�\��.
	CSprite*							m_pTrashArea;					// �폜�G���A.
	CSprite*							m_pUpDownButton;				// �㉺�{�^��.
	CSprite*							m_pLeftRightButton;				// ���E�{�^��.
	CSprite*							m_pTextButton;					// �e�L�X�g�{�^��.
	CSprite*							m_pPauseBack;					// �|�[�Y�̔w�i�摜.
	CSprite*							m_pPauseCursor;					// �|�[�Y���̃J�[�\���摜.
	CSprite*							m_pPauseText;					// �|�[�Y���̃e�L�X�g�摜.
	CSprite*							m_pText;						// ��������e�L�X�g�摜.
	CSprite*							m_pSaveArea;					// �ۑ��G���A�摜.
	CFont*								m_pFont;						// �t�H���g.
	MaskList							m_pSauceMaskList;				// �\�[�X�̃}�X�N�摜���X�g.
	MaskNoMap							m_pSauceMaskNoMap;				// �\�[�X�̃}�X�N�摜��No���擾�p.
	SFontRenderState					m_FontState;					// �t�H���g�̏��.
	SSpriteRenderState					m_PizzaBaseState;				// �s�U���n�̏��.
	SSpriteRenderState					m_SamplePizzaBaseState;			// �s�U���n�̏��.
	SSpriteRenderState					m_IngredientStateBase;			// ��ނ̏��x�[�X.
	SSpriteRenderState					m_IngredientSauceState;			// ��ނ̃\�[�X�̏�񃊃X�g.
	SSpriteRenderState					m_IngredientAreaStateBase;		// ��ރG���A�̏��x�[�X.
	SSpriteRenderState					m_SauceStateBase;				// �\�[�X�̏��x�[�X.
	SSpriteRenderState					m_SauceAreaStateBase;			// �\�[�X�G���A�̏��x�[�X.
	SSpriteRenderState					m_AnimSpatulaState;				// �A�j���[�V�����p�̃w���̏��.
	SSpriteRenderState					m_SpatulaState;					// �w���̏��.
	SSpriteRenderState					m_KilnState;					// �s�U�q�̏��.
	SSpriteRenderState					m_CursorState;					// �J�[�\���̏��.
	SSpriteRenderState					m_PauseBackState;				// �|�[�Y�̔w�i�̏��.
	SSpriteRenderState					m_PauseCursorState;				// �|�[�Y�̃J�[�\���̏��.
	SSpriteRenderState					m_PauseTextStateBase;			// �|�[�Y���̃e�L�X�g�̏��x�[�X.
	SSpriteRenderState*					m_pSelectSaveAreaState;			// �I�񂾃Z�[�u�G���A�̏��.
	SSpriteRenderStateList				m_TextButtonStateList;			// �e�L�X�g�{�^���̏�񃊃X�g.
	SSpriteRenderStateList				m_PauseTextStateList;			// �|�[�Y���̃e�L�X�g�̏�񃊃X�g.
	SSpriteRenderStateList				m_TrashAreaStateList;			// �폜�G���A��񃊃X�g.
	SSpriteRenderStateList				m_UpDownButtonStateList;		// �㉺�{�^����񃊃X�g.
	SSpriteRenderStateList				m_LeftRightButtonStateList;		// ���E�{�^����񃊃X�g.
	SSpriteRenderStateList				m_IngredientStateList;			// ��ނ̏�񃊃X�g.
	SSpriteRenderStateList				m_IngredientAreaStateList;		// ��ރG���A�̏�񃊃X�g.
	SSpriteRenderStateList				m_SauceStateList;				// �\�[�X�̏�񃊃X�g.
	SSpriteRenderStateList				m_CheeseStateList;				// �`�[�Y�̏�񃊃X�g.
	SSpriteRenderStateList				m_SauceAreaStateList;			// �\�[�X�G���A�̏�񃊃X�g.
	SSpriteRenderStateList				m_TextStateList;				// ��������̃e�L�X�g�̏�񃊃X�g.
	SSpriteRenderStateList				m_SaveAreaStateList;			// �ۑ��G���A�̏�񃊃X�g.
	std::vector<SSpriteRenderStateList>	m_SampleSauceStateList;			// �ۑ����Ă���s�U�̃T���v���p�̃\�[�X�̏�񃊃X�g.
	std::vector<SSpriteRenderStateList>	m_SampleCheeseStateList;		// �ۑ����Ă���s�U�̃T���v���p�̃`�[�Y�̏�񃊃X�g.
	std::vector<SSpriteRenderStateList>	m_SampleIngredientStateList;	// �ۑ����Ă���s�U�̃T���v���p�̋�ނ̏�񃊃X�g.
	D3DXPOSITION3						m_GrabPosition;					// �͂񂾏ꏊ.
	D3DXVECTOR3							m_PutInKilnAnimVector;			// �q�ɓ����A�j���[�V�����p�̃x�N�g��.
	std::vector<std::string>			m_SauceNameList;				// �\�[�X�̖��O���X�g.
	std::vector<std::string>			m_IngredientNameList;			// ��ނ̖��O���X�g.
	std::vector<std::pair<int, int>>	m_IngredientTypeAreaList;		// ��ނ̎�ނ͈̔̓��X�g<�J�n, �I��>.
	std::vector<std::pair<int, int>>	m_SauceList;					// �\�[�X�̏�ԃ��X�g<�\�[�X�̃��X�gNo, �`�[�Y�̃��X�gNo>.
	std::string							m_SelectObject;					// �I�����Ă���I�u�W�F�N�g.
	std::vector<float>					m_CursorMoveSpeedList;			// �J�[�\���̈ړ����x���X�g.
	std::vector<float>					m_IngredientRotSpeedList;		// ��ނ̉�]���x���X�g.
	SavePizzaDataList					m_SavePizzaData;				// �ۑ����Ă���s�U�f�[�^���X�g<���O, json>.
	float								m_PutInKilnAnimCnt;				// �q�ɓ����A�j���[�V�����p�̃J�E���g.
	float								m_ButtonAnimCnt;				// �{�^���̃A�j���[�V�����p�̃J�E���g.
	int									m_CursorMoveSpeedNo;			// �J�[�\���̈ړ����xNo.
	int									m_IngredientRotSpeedNo;			// ��ނ̉�]���xNo.
	int									m_AreaPageNo;					// ��ޒu����̃y�[�WNo.
	int									m_GrabSauceNo;					// �͂�ł�\�[�XNo.
	int									m_GrabIngredientNo;				// �͂�ł���No.
	int									m_PutInKilnAnimNo;				// �q�ɓ����A�j���[�V����No.
	int									m_Choice;						// �I���ӏ�.
	int									m_SavePizzaNum;					// �ۑ����Ă���s�U�̐�.
	int									m_SavePizzaPageNo;				// �ۑ����Ă���s�U�̃y�[�WNo.
	bool								m_IsBButtonAction;				// B�{�^���ōs���s�����s������.
	bool								m_IsDispSauce;					// �\�[�X��\�����Ă��邩.
	bool								m_IsPutInKiln;					// �q�ɓ����.
	bool								m_IsPause;						// �|�[�Y����.
	bool								m_IsPauseAnim;					// �|�[�Y�̃A�j���[�V��������.
	bool								m_IsPizzaSave;					// �s�U��ۑ����邩.
	bool								m_IsPizzaLoad;					// �s�U��ǂݍ��ނ�.
	bool								m_IsPizzaDelete;				// �s�U���폜���邩.
	bool								m_IsPizzaDownload;				// �s�U���_�E�����[�h���邩.
};