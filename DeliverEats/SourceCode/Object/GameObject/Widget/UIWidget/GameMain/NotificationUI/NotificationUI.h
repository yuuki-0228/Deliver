#pragma once
#include "..\..\..\..\..\..\Global.h"
#include "..\..\..\..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\..\..\..\Common\Font\Font.h"
#include "..\..\..\..\..\..\Utility\Constant\Constant.h"

// ���b�Z�[�W�̎��.
enum class enMessageType : unsigned char {
	None,

	// ���ɍs���قǏd�v�x������.
	Evaluation,			// ���q���񂩂�̕]��.
	RankingUpdate,		// �����L���O�X�V.
	WindIsGenerated,	// ��������.
	GoldenTime,			// �S�[���f���^�C���֌W.
	TimeLeft,			// �c�莞�Ԋ֌W.
	Warning,			// �G���A�O�Ȃǂ̒ʒm.

	Max
} typedef EMessageType;

/************************************
*	�ʒm�N���X.
**/
class CNotificationUI final
{
private:
	// ���b�Z�[�W�\����.
	struct stMessage {
		EMessageType	Type;			// ���b�Z�[�W�^�C�v.
		std::string		Text;			// �e�L�X�g.
		float			ElapsedTime;	// �ʒm����������Q�[�����̕�.

		stMessage()
			: stMessage( EMessageType::None, "", 0.0f )
		{
		}
		stMessage( const EMessageType type, const std::string& text, const float time )
			: Type			( type )
			, Text			( text )
			, ElapsedTime	( time )
		{
		}
	} typedef SMessage;

public:
	using MessageList		= std::vector<SMessage>;
	using FontList			= std::vector<SFontRenderStateList>;
	using IconNoMap			= std::unordered_map<EMessageType, int>;
	using IconNameMap		= std::unordered_map<int, std::string>;
	using IconSEMap			= std::unordered_map<int, std::string>;

public:
	CNotificationUI();
	~CNotificationUI();

	// �C���X�^���X�̎擾.
	static CNotificationUI* GetInstance();

	// ������.
	static bool Init();
	// �X�V.
	static void Update( const float& deltaTime );
	// �`��.
	static void Render();

	// �����̒ǉ�.
	static void PushMessage( const EMessageType& Type, const std::string& Text );

private:
	// �\������.
	static void OpenUpdate( const int Type );
	// ���鏈��.
	static void CloseUpdate();
	// �ύX�ɂ����鏈��.
	static void ChangeCloseUpdate();
	// �ʒm�̕ύX�I������.
	static void ChangeUpdate();
	// �����ŕ��鏈��.
	static void AutoCloseUpdate( const float& deltaTime );

	// �\���ݒ�.
	static void OpenSetting( const int Type );
	// �ύX�ݒ�.
	static void ChangeSetting();

	// ���b�Z�[�W����ёւ���.
	static void MessageSort();

private:
	CSprite*				m_pBackSprite;		// �w�i�摜.
	CSprite*				m_pIconSprite;		// �A�C�R���摜.
	CFont*					m_pFont;			// �t�H���g.
	SSpriteRenderStateList	m_BackState;		// �w�i�̏��.
	SSpriteRenderStateList	m_IconState;		// �A�C�R���̏��.
	FontList				m_FontState;		// �t�H���g�̏��.
	IconNoMap				m_IconNoList;		// �A�C�R���̔ԍ����X�g( EMessageType -> AnimNumber ).
	IconNameMap				m_IconNameList;		// �A�C�R���̖��O���X�g( AnimNumber -> �A�C�R���� ).
	IconSEMap				m_IconSEList;		// �A�C�R����SE���X�g( AnimNumber -> SE�� ).
	MessageList				m_MessageList;		// ���b�Z�[�W���X�g.
	MessageList				m_Message;			// ���݂̃��b�Z�[�W.
	std::vector<bool>		m_IsOpen;			// �ʒm��\�������邩.
	std::vector<D3DXSCALE3>	m_MessageScale;		// ���b�Z�[�W�̊g�k.
	std::vector<float>		m_MessageAlpha;		// ���b�Z�[�W�̃A���t�@�l.
	bool					m_IsClose;			// �ʒm����邩.
	bool					m_IsChangeClose;	// �ʒm�̕ύX�ŕ��邩.
	bool					m_IsChange;			// �ʒm��ύX���邩.
	bool					m_IsDisp;			// �ʒm��\�����Ă��邩.
	float					m_DispTimeCnt;		// �\�����ԗp�̃J�E���g.

private:
	// �O���f�[�^�̒萔.
	const ConstantStruct::SNotification& CONSTANT = CConstant::GetNotification();
};