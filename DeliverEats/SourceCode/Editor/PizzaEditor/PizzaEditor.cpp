#include "PizzaEditor.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Resource\PizzaResource\PizzaResource.h"
#include "..\..\Scene\SceneManager\SceneManager.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\Random\Random.h"
#include "..\..\Utility\Easing\Easing.h"
#include "..\..\Utility\FileManager\DragAndDrop\DragAndDrop.h"

namespace {
	constexpr int	PAUSE_TEXT_NUM			= 4;													// �|�[�Y�e�L�X�g�̕\����.
	constexpr int	INGREDIENTS_MAX			= 10;													// ��ނ̐�.
	constexpr int	SAUCE_MAX				= 4;													// �\�[�X�̐�.
	constexpr int	GRAB_MAYONNAISE_NO		= -3;													// ��ނ̃}���l�[�Y�\�[�X��͂�ł��鎞��No.
	constexpr int	GRAB_TERIYAKI_NO		= -2;													// ��ނ̏Ƃ�Ă��\�[�X��͂�ł��鎞��No.
	constexpr int	NO_GRAB_NO				= -1;													// �����͂�ł��Ȃ�����No.
	constexpr int	NO_SAUCE_NO				= -1;													// �\�[�X��z�u���Ă��Ȃ�����No.
	constexpr float	TEXT_CHANGE_TIME		= 3.0f;													// �e�L�X�g��ύX���鎞��.
	constexpr float	CURSOR_SLOW_SPEED		= 2.0f;													// �J�[�\���̒x�����̎��̈ړ����x.
	constexpr float	CURSOR_NORMAL_SPEED		= 6.0f;													// �J�[�\���̒ʏ펞�̈ړ����x.
	constexpr float	CURSOR_FAST_SPEED		= 10.0f;												// �J�[�\���̑������̈ړ����x.
	constexpr float	ROT_SLOW_SPEED			= 2.0f;													// ��ނ̉�]�̒x�����̎��̈ړ����x.
	constexpr float	ROT_NORMAL_SPEED		= 5.0f;													// ��ނ̉�]�̒ʏ펞�̈ړ����x.
	constexpr float	ROT_FAST_SPEED			= 10.0f;												// ��ނ̉�]�̑������̈ړ����x.
	constexpr int	CHOICE_STARTCHOICE		= 0;													// �ŏ��̑I����.
	constexpr int	CHOICE_CENTERCHOICE		= 1;													// �^�񒆂̑I����.
	constexpr int	CHOICE_ENDCHOICE		= 2;													// �Ō�̑I����.
	constexpr char	SAVE_FILE_PATH[]		= "Data\\Parameter\\Pizza\\";							// �s�U�f�[�^�̕ۑ��ꏊ.
	constexpr char	SNAME_LIST_FILE_PATH[]	= "Data\\Parameter\\PizzaEditor\\SauceName.json";		// �\�[�X�̖��O���X�g�̃t�@�C���p�X.
	constexpr char	INAME_LIST_FILE_PATH[]	= "Data\\Parameter\\PizzaEditor\\IngredientName.json";	// ��ނ̖��O���X�g�̃t�@�C���p�X.
	constexpr char	IAREA_LIST_FILE_PATH[]	= "Data\\Parameter\\PizzaEditor\\IngredientArea.json";	// ��ނ͈̔̓��X�g�̃t�@�C���p�X.
	constexpr char	NORMAL_TEXT[]			= u8"�ۑ�/�ǂݍ��݃t�@�C�������L�����Ă�������";			// �ʏ펞�̃e�L�X�g.

	constexpr char	DECISION_SE[]			= "PizzaEditorDecision";								// ���莞��SE.
	constexpr char	OPEN_SE[]				= "Open";												// �|�[�Y���J������SE.
	constexpr char	CLOSE_SE[]				= "Close";												// �|�[�Y����鎞��SE.
	constexpr char	CURSOR_MOVE_SE[]		= "CursorMove";											// �J�[�\���ړ�����SE.
	constexpr char	BUTTON_SE[]				= "ImguiSE";											// �{�^��������������SE.
	constexpr char	CHEESE_GRAB_SE[]		= "CheeseGrab";											// �`�[�Y��͂񂾎���SE.
	constexpr char	CHEESE_GRAB_END_SE[]	= "CheeseGrabEnd";										// �`�[�Y�𗣂�������SE.
	constexpr char	INGREDIENT_GRAB_SE[]	= "IngredientGrab";										// ��ނ�͂񂾎���SE.
	constexpr char	INGREDIENT_GRAB_END_SE[]= "IngredientGrabEnd";									// ��ނ𗣂�������SE.
	constexpr char	SAUCE_SE[]				= "Sauce";												// �\�[�X��͂񂾂藣��������SE.
	constexpr char	FALL_SE[]				= "Fall";												// ��ނ�������������SE.
	constexpr char	TRASH_SE[]				= "Trash";												// ��ނȂǃS�~���G���A�Ɏ̂Ă�����SE.
	constexpr char	SPATULA_SE[]			= "Spatula";											// �w���̕\��/��\�������鎞��SE.
	constexpr char	COOK_PIZZA_SE[]			= "CookPizza";											// �s�U���Ă�SE.
	constexpr char	PIZZAKILN_SE[]			= "PizzaEditorKiln";									// �s�U�q�̃��[�vSE.
	constexpr char	BACK_SE[]				= "PizzaEditorBack";									// �����̃��[�vSE.

	// �J�[�\��No.
	enum enCursorNo : unsigned char {
		NormaCursorl,			// �ʏ펞.
		CanGrabCursor,			// �͂߂鎞.
		GrabCursor,				// �͂�ł鎞.
		SelectCursor,			// �I�ׂ鎞.
		MayonnaiseCursor,		// �}���l�[�Y�������Ă鎞.
		TeriyakiCursor,			// �Ƃ�Ă��\�[�X�������Ă鎞.
		SauceCursor,			// �\�[�X�������Ă鎞.
		CheeseCursor			// �`�[�Y�������Ă�Ƃ���.
	} typedef ECursorNo;

	// �㉺�{�^���̌�����No.
	enum enUpDownButtonImageNo : unsigned char {
		UpButton,				// ��{�^��.
		PushUpButton,			// ������Ă����{�^��.
		DownButton,				// ���{�^��.
		PushDownButton			// ������Ă��鉺�{�^��.
	} typedef EUpDownButtonImageNo;

	// ���E�{�^���̌�����No.
	enum enLeftRightButtonImageNo : unsigned char {
		LeftButton,				// ���{�^��.
		PushLeftButton,			// ������Ă��鍶�{�^��.
		RightButton,			// �E�{�^��.
		PushRightButton			// ������Ă���E�{�^��.
	} typedef ELeftRightButtonImageNo;

	// �e�L�X�g�{�^���̌�����No.
	enum enTextButtonImageNo : unsigned char {
		DecisionButton,			// ����{�^��.
		PushDecisionButton,		// ������Ă��錈��{�^��.
		PutInKilnButton,		// �q�ɓ����{�^��.
		PushPutInKilnButton,	// ������Ă���q�ɓ����{�^��.
		YesButton,				// �͂��{�^��.
		PushYesButton,			// ������Ă���͂��{�^��.
		NoButton,				// �������{�^��.
		PushNoButton,			// ������Ă��邢�����{�^��.
	} typedef ETextButtonImageNo;

	// �q�ɓ����A�j���[�V����No.
	enum enPutInKilnAnimNo : unsigned char {
		SpatulaDispAnim,		// �w����\��������A�j���[�V����.
		SpatulaBackAnim,		// �w����߂��A�j���[�V����.
		PutInKilnAnim,			// �q�ɓ����A�j���[�V����.
		CookPizzaAnim,			// �s�U���Ă��A�j���[�V����.
		RemoveFromKilnAnim,		// �q����o���A�j���[�V����.
	} typedef EPutInKilnAnimNo;

	// ��������̃e�L�X�gNo.
	enum enTextNo : unsigned char {
		PauseOpenText,			// �|�[�Y���J��.
		CursorMoveText,			// �J�[�\���̈ړ�.
		CursorMoveChangeText,	// �J�[�\���̈ړ����x�̕ύX.
		RotationChangeText,		// ��ނ̉�]���x�̕ύX.
		GrabText,				// ��ނ�͂�.
		GrabEndText,			// ��ނ𗣂�.
		GrabSauceText,			// �\�[�X��͂�.
		GrabEndSauceText,		// �\�[�X�𗣂�.
		CopyText,				// ��ނ̃R�s�[��z�u.
		Gray_CopyText,			// �D�F�̋�ނ̃R�s�[��z�u.
		ClockwiseText,			// ���v����ŉ�].
		Gray_ClockwiseText,		// �D�F�̎��v����ŉ�]. 
		AnticlockwiseText,		// �����v����ŉ�].
		Gray_AnticlockwiseText,	// �D�F�̔����v����ŉ�].
	} typedef ETextNo;
}

CPizzaEditor::CPizzaEditor()
	: m_pBack						( nullptr )
	, m_pPizzaBase					( nullptr )
	, m_pIngredient					( nullptr )
	, m_pIngredientSauce			( nullptr )
	, m_pIngredientArea				( nullptr )
	, m_pSauce						( nullptr )
	, m_pSauceArea					( nullptr )
	, m_pSpatula					( nullptr )
	, m_pKiln						( nullptr )
	, m_pCursor						( nullptr )
	, m_pTrashArea					( nullptr )
	, m_pUpDownButton				( nullptr )
	, m_pLeftRightButton			( nullptr )
	, m_pTextButton					( nullptr )
	, m_pPauseBack					( nullptr )
	, m_pPauseCursor				( nullptr )
	, m_pPauseText					( nullptr )
	, m_pText						( nullptr )
	, m_pSaveArea					( nullptr )
	, m_pFont						( nullptr )
	, m_pSauceMaskList				( 8 )
	, m_pSauceMaskNoMap				()
	, m_FontState					()
	, m_PizzaBaseState				()
	, m_SamplePizzaBaseState		()
	, m_IngredientStateBase			()
	, m_IngredientSauceState		()
	, m_IngredientAreaStateBase		()
	, m_SauceStateBase				()
	, m_SauceAreaStateBase			()
	, m_AnimSpatulaState			()
	, m_SpatulaState				()
	, m_KilnState					()
	, m_CursorState					()
	, m_PauseBackState				()
	, m_PauseCursorState			()
	, m_PauseTextStateBase			()
	, m_pSelectSaveAreaState		( nullptr )
	, m_TextButtonStateList			( 2 )
	, m_PauseTextStateList			( PAUSE_TEXT_NUM )
	, m_TrashAreaStateList			( 2 )
	, m_UpDownButtonStateList		( 2 )
	, m_LeftRightButtonStateList	( 2 )
	, m_IngredientStateList			()
	, m_IngredientAreaStateList		()
	, m_SauceStateList				()
	, m_CheeseStateList				()
	, m_SauceAreaStateList			()
	, m_TextStateList				( 8 )
	, m_SaveAreaStateList			( 6 )
	, m_SampleSauceStateList		()
	, m_SampleCheeseStateList		()
	, m_SampleIngredientStateList	()
	, m_GrabPosition				( INIT_FLOAT3 )
	, m_PutInKilnAnimVector			( INIT_FLOAT3 )
	, m_SauceNameList				()
	, m_IngredientNameList			()
	, m_IngredientTypeAreaList		()
	, m_SauceList					( 8 )
	, m_SelectObject				( "" )
	, m_CursorMoveSpeedList			( 3 )
	, m_IngredientRotSpeedList		( 3 )
	, m_PutInKilnAnimCnt			( INIT_FLOAT )
	, m_ButtonAnimCnt				( INIT_FLOAT )
	, m_CursorMoveSpeedNo			( 1 )
	, m_IngredientRotSpeedNo		( 1 )
	, m_SavePizzaData				()
	, m_AreaPageNo					( INIT_INT )
	, m_GrabSauceNo					( NO_GRAB_NO )
	, m_GrabIngredientNo			( NO_GRAB_NO )
	, m_PutInKilnAnimNo				( INIT_INT )
	, m_Choice						( INIT_INT )
	, m_SavePizzaNum				( INIT_INT )
	, m_SavePizzaPageNo				( INIT_INT )
	, m_IsBButtonAction				( false )
	, m_IsDispSauce					( true )
	, m_IsPutInKiln					( false )
	, m_IsPause						( false )
	, m_IsPauseAnim					( false )
	, m_IsPizzaSave					( false )
	, m_IsPizzaLoad					( false )
	, m_IsPizzaDelete				( false )
	, m_IsPizzaDownload				( false )
{
}

CPizzaEditor::~CPizzaEditor()
{
	// �h���b�N&�h���b�v�𖳌��ɂ���.
	CDragAndDrop::Close();

	// �T�E���h�̒�~.
	CSoundManager::StopSE( DECISION_SE				);
	CSoundManager::StopSE( OPEN_SE					);
	CSoundManager::StopSE( CLOSE_SE					);
	CSoundManager::StopSE( CURSOR_MOVE_SE			);
	CSoundManager::StopSE( BUTTON_SE				);
	CSoundManager::StopSE( CHEESE_GRAB_SE			);
	CSoundManager::StopSE( CHEESE_GRAB_END_SE		);
	CSoundManager::StopSE( INGREDIENT_GRAB_SE		);
	CSoundManager::StopSE( INGREDIENT_GRAB_END_SE	);
	CSoundManager::StopSE( SAUCE_SE					);
	CSoundManager::StopSE( FALL_SE					);
	CSoundManager::StopSE( TRASH_SE					);
	CSoundManager::StopSE( SPATULA_SE				);
	CSoundManager::StopSE( COOK_PIZZA_SE			);
	CSoundManager::StopSE( PIZZAKILN_SE				);
	CSoundManager::StopSE( BACK_SE					);
}

//---------------------------.
// ������.
//---------------------------.
bool CPizzaEditor::Init()
{
	m_pBack					= CSpriteResource::GetSprite( "PizzaEditorBack" );
	m_pPizzaBase			= CSpriteResource::GetSprite( "PizzaBase",					&m_PizzaBaseState );
	m_pIngredient			= CSpriteResource::GetSprite( "PizzaIngredient",			&m_IngredientStateBase );
	m_pIngredientSauce		= CSpriteResource::GetSprite( "PizzaIngredientSauce",		&m_IngredientSauceState );
	m_pIngredientArea		= CSpriteResource::GetSprite( "PizzaEditorIngredientArea",	&m_IngredientAreaStateBase );
	m_pSauce				= CSpriteResource::GetSprite( "PizzaSauce",					&m_SauceStateBase );
	m_pSauceArea			= CSpriteResource::GetSprite( "PizzaEditorSauceArea",		&m_SauceAreaStateBase );
	m_pSpatula				= CSpriteResource::GetSprite( "PizzaEditorSpatula",			&m_SpatulaState );
	m_pKiln					= CSpriteResource::GetSprite( "PizzaEditorKiln",			&m_KilnState );
	m_pCursor				= CSpriteResource::GetSprite( "PizzaEditorCursor",			&m_CursorState );
	m_pTrashArea			= CSpriteResource::GetSprite( "PizzaEditorTrashArea",		&m_TrashAreaStateList );
	m_pUpDownButton			= CSpriteResource::GetSprite( "UpDownButton",				&m_UpDownButtonStateList );
	m_pLeftRightButton		= CSpriteResource::GetSprite( "LeftRighrButton",			&m_LeftRightButtonStateList );
	m_pTextButton			= CSpriteResource::GetSprite( "TextButton",					&m_TextButtonStateList );
	m_pPauseBack			= CSpriteResource::GetSprite( "back",						&m_PauseBackState		);
	m_pPauseText			= CSpriteResource::GetSprite( "PizzaEditorPauseText",		&m_PauseTextStateBase	);
	m_pPauseCursor			= CSpriteResource::GetSprite( "Yubi",						&m_PauseCursorState		);
	m_pText					= CSpriteResource::GetSprite( "PizzaEditorText",			&m_TextStateList );
	m_pSaveArea				= CSpriteResource::GetSprite( "PizzaEditorSaveArea",		&m_SaveAreaStateList );
	m_pFont					= CFontResource::GetFont( "NasuM", &m_FontState );

	// ��������̐ݒ�.
	const int TSize = static_cast<int>( m_TextStateList.size() );
	for ( int i = 0; i < TSize; ++i ) {
		m_TextStateList[i].AnimState.IsSetAnimNumber	= true;
		m_TextStateList[i].Transform.Position.x			= 936.0f;
		m_TextStateList[i].Transform.Position.y			= 467.0f + 31.0f * i;
	}
	m_TextStateList[0].AnimState.AnimNumber = PauseOpenText;
	m_TextStateList[1].AnimState.AnimNumber = CursorMoveText;
	m_TextStateList[2].AnimState.AnimNumber = CursorMoveChangeText;
	m_TextStateList[3].AnimState.AnimNumber = RotationChangeText;
	m_TextStateList[4].AnimState.AnimNumber = GrabSauceText;
	m_TextStateList[5].AnimState.AnimNumber = CopyText;
	m_TextStateList[6].AnimState.AnimNumber = ClockwiseText;
	m_TextStateList[7].AnimState.AnimNumber = AnticlockwiseText;

	// �ۑ��G���A�̐ݒ�.
	const int	ASize		 = static_cast<int>( m_SaveAreaStateList.size() );
	const SSize SaveAreaSize = m_pSaveArea->GetSpriteState().Disp;
	for ( int i = 0; i < ASize; ++i ) {
		m_SaveAreaStateList[i].AnimState.IsSetAnimNumber	= true;
		m_SaveAreaStateList[i].Transform.Position.x			= 928.0f + SaveAreaSize.w * ( i % 2 );
		m_SaveAreaStateList[i].Transform.Position.y			= 6.0f	 + SaveAreaSize.h * ( i / 2 );
	}

	// �}�X�N�摜�̕ۑ�.
	for( int i = 0; i < 8; ++i ){
		m_pSauceMaskList[i].resize( 4 );
		for ( int No = 0; No < 4; ++No ) {
			m_pSauceMaskList[i][No] = CSpriteResource::GetSprite( "PizzaSauceMask_" + std::to_string( i ) + std::to_string( No ) )->GetTexture();
			m_pSauceMaskNoMap[m_pSauceMaskList[i][No]] = std::make_pair( i, No );
		}
	}

	// �|�[�Y��ʂ̐ݒ�.
	m_PauseTextStateBase.Transform.AttachParent( &m_PauseBackState.Transform );
	m_PauseCursorState.Transform.AttachParent(	 &m_PauseBackState.Transform );
	m_PauseBackState.Transform.Position.x				= FWND_W / 2.0f;
	m_PauseBackState.Transform.Position.y				= FWND_H / 2.0f;
	m_PauseBackState.Transform.Scale					= { 0.0f, 0.0f, 0.0f };
	m_PauseBackState.Color.w							= Color::ALPHA_MIN;
	m_PauseTextStateBase.AnimState.IsSetAnimNumber		= true;
	m_PauseTextStateBase.Transform.Position.y			= -166.0f;
	m_PauseTextStateBase.Color.w						= Color::ALPHA_MIN;
	for ( int i = 0; i < PAUSE_TEXT_NUM; ++i ) {
		m_PauseTextStateList[i]							= m_PauseTextStateBase;
		m_PauseTextStateList[i].Transform.Position.y	+= 110 * i;
		m_PauseTextStateList[i].AnimState.AnimNumber	= 2 * i;
	}

	// �|�[�Y�̃J�[�\���̐ݒ�.
	m_PauseCursorState.SceneName = "Menu";
	m_PauseCursorState.UIAnimState.StartTransform.Position		= m_PauseTextStateList[1].Transform.Position;
	m_PauseCursorState.UIAnimState.StartTransform.Position.x	= -250.0f;
	m_PauseCursorState.Transform.Rotation.z						= 0.0f;

	// �t���O�̐ݒ�.
	m_SauceStateBase.AnimState.IsSetAnimNumber				= true;
	m_IngredientStateBase.AnimState.IsSetAnimNumber			= true;
	m_IngredientAreaStateBase.AnimState.IsSetAnimNumber		= true;
	m_SauceAreaStateBase.AnimState.IsSetAnimNumber			= true;
	m_PizzaBaseState.AnimState.IsSetAnimNumber				= true;
	m_CursorState.AnimState.IsSetAnimNumber					= true;

	// �e�̐ݒ�.
	m_PizzaBaseState.Transform.AddParent(			&m_SpatulaState.Transform	);
	m_IngredientStateBase.Transform.AttachParent(	&m_PizzaBaseState.Transform );
	m_SauceStateBase.Transform.AttachParent(		&m_PizzaBaseState.Transform );

	// �T�C�Y�̎擾.
	const SSize& TrashAreaSize	= m_pTrashArea->GetSpriteState().Disp;
	const SSize& SpatulaSize	= m_pSpatula->GetSpriteState().Disp;

	// ���W�̐ݒ�.
	m_SauceStateBase.Transform.Position				= INIT_FLOAT3;
	m_SpatulaState.Transform.Position.x				= FWND_W / 2.0f;
	m_SpatulaState.Transform.Position.y				= FWND_H - 64.0f;
	m_PizzaBaseState.Transform.Position.x			= 0.0f;
	m_PizzaBaseState.Transform.Position.y			= -299.0f;
	m_PizzaBaseState.Transform.Scale				= { 2.0f, 2.0f, 2.0f };
	m_KilnState.Transform.Position.x				= FWND_W / 2.0f;
	m_CursorState.Transform.Position.x				= FWND_W / 2.0f;
	m_CursorState.Transform.Position.y				= FWND_H / 2.0f;
	m_CursorState.AnimState.AnimNumber				= NormaCursorl;
	m_TrashAreaStateList[1].Transform.Position.x	= FWND_W - TrashAreaSize.w;

	// �w���̐ݒ�.
	m_AnimSpatulaState								= m_SpatulaState;
	m_AnimSpatulaState.Transform.Position.y			= FWND_H + SpatulaSize.h;
	m_PutInKilnAnimVector							= m_SpatulaState.Transform.Position - m_AnimSpatulaState.Transform.Position;
	m_SpatulaState.IsDisp							= false;
	m_AnimSpatulaState.IsDisp						= false;

	// �S�~���G���A�̐ݒ�.
	for ( auto& a : m_TrashAreaStateList ) {
		a.Color.w					= Color::ALPHA_MIN;
		a.AnimState.IsSetAnimNumber = true;
	}

	// �{�^���̐ݒ�.
	for ( auto& b : m_UpDownButtonStateList ) b.AnimState.IsSetAnimNumber = true;
	m_UpDownButtonStateList[0].AnimState.AnimNumber		= UpButton;
	m_UpDownButtonStateList[0].Transform.Position.x		= 128.0f;
	m_UpDownButtonStateList[0].Transform.Position.y		= -4.0f;
	m_UpDownButtonStateList[1].AnimState.AnimNumber		= DownButton;
	m_UpDownButtonStateList[1].Transform.Position.x		= 128.0f;
	m_UpDownButtonStateList[1].Transform.Position.y		= 660.0f;
	for ( auto& b : m_LeftRightButtonStateList ) b.AnimState.IsSetAnimNumber = true;
	m_LeftRightButtonStateList[0].AnimState.AnimNumber	= LeftButton;
	m_LeftRightButtonStateList[0].Transform.Position.x	= 928.0f;
	m_LeftRightButtonStateList[0].Transform.Position.y	= 371.0f;
	m_LeftRightButtonStateList[1].AnimState.AnimNumber	= RightButton;
	m_LeftRightButtonStateList[1].Transform.Position.x	= 1215.0f;
	m_LeftRightButtonStateList[1].Transform.Position.y	= 370.0f;
	for( auto& t : m_TextButtonStateList ){
		t.AnimState.IsSetAnimNumber		= true;
		t.AnimState.AnimNumber			= DecisionButton;
		t.Transform.Position.x			= FWND_W / 2.0f;
		t.Transform.Position.y			= FWND_H - 39.0f;
	}
	m_TextButtonStateList[1].IsDisp		= false;

	// �t�H���g�̐ݒ�.
	m_FontState.Transform.Position.x	= FWND_W / 2.0f;
	m_FontState.Transform.Position.y	= 100.0f;
	m_FontState.TextAlign				= ETextAlign::Center;
	m_FontState.IsOutLine				= true;
	m_FontState.OutLineSize				= 2.0f;
	m_FontState.OutLineColor			= Color4::White;
	m_FontState.Color					= Color4::Black;

	// �J�[�\���̈ړ����x�̐ݒ�.
	m_CursorMoveSpeedList = {
		CURSOR_SLOW_SPEED,
		CURSOR_NORMAL_SPEED,
		CURSOR_FAST_SPEED
	};
	// ��ނ̉�]���x�̐ݒ�.
	m_IngredientRotSpeedList = {
		ROT_SLOW_SPEED,
		ROT_NORMAL_SPEED,
		ROT_FAST_SPEED
	};

	// �s�U�̉摜�T�C�Y�̎擾.
	const SSize& BaseSpriteSize			= m_pPizzaBase->GetSpriteState().Disp;
	const SSize& IngredientSpriteSize	= m_pIngredientArea->GetSpriteState().Disp;
	const SSize& SauceSpriteSize		= m_pSauceArea->GetSpriteState().Disp;

	// ��ރ��X�g�̐ݒ�.
	std::vector<std::string> INameList	= FileManager::JsonLoad( INAME_LIST_FILE_PATH )["TypeName"];
	std::vector<int>		 ISizeList	= FileManager::JsonLoad( IAREA_LIST_FILE_PATH )["TypeSize"];
	const int ISize = m_IngredientAreaStateBase.AnimState.AnimNumberMax - 1;
	m_IngredientNameList.resize( ISize );
	m_IngredientTypeAreaList.resize( ISize );
	int Start = 0;
	for ( int i = 0; i < ISize; ++i ) {
		const std::wstring& wString		= StringConversion::to_wString( INameList[i], ECodePage::UTF8 );
		m_IngredientNameList[i]			= StringConversion::to_String( wString );
		m_IngredientTypeAreaList[i]		= std::make_pair( Start, Start + ISizeList[i] - 1 );
		Start += ISizeList[i];
	}
	std::vector<std::string> SNameList	= FileManager::JsonLoad( SNAME_LIST_FILE_PATH )["TypeName"];
	const int SSize = m_SauceAreaStateBase.AnimState.AnimNumberMax - 1;
	m_SauceNameList.resize( SSize );
	for ( int i = 0; i < SSize; ++i ) {
		const std::wstring& wString		= StringConversion::to_wString( SNameList[i], ECodePage::UTF8 );
		m_SauceNameList[i]				= StringConversion::to_String( wString );
	}

	// ��ރG���A�̐ݒ�.
	m_IngredientAreaStateList.resize( INGREDIENTS_MAX, m_IngredientAreaStateBase );
	for ( int i = 0; i < INGREDIENTS_MAX; ++i ) {
		m_IngredientAreaStateList[i].Transform.Position.x = IngredientSpriteSize.w * ( i % 2 );
		m_IngredientAreaStateList[i].Transform.Position.y = IngredientSpriteSize.h * ( i / 2 ) + 60.0f;
		m_IngredientAreaStateList[i].AnimState.AnimNumber = i + 1;
	}

	// �\�[�X�G���A�̐ݒ�.
	m_SauceAreaStateList.resize( SAUCE_MAX, m_SauceAreaStateBase );
	for ( int i = 0; i < SAUCE_MAX; ++i ) {
		m_SauceAreaStateList[i].Transform.Position.x = 0.0f;
		m_SauceAreaStateList[i].Transform.Position.y = SauceSpriteSize.h * i + 60.0f;
		m_SauceAreaStateList[i].AnimState.AnimNumber = i + 1;
	}

	// �\�[�X�̏�ԃ��X�g�̏�����.
	for ( auto& [Sauce, Cheese] : m_SauceList ) {
		Sauce	= NO_SAUCE_NO;
		Cheese	= NO_SAUCE_NO;
	}

	// ��ނ̃\�[�X�̏�����.
	m_IngredientSauceState.AnimState.IsSetAnimNumber = true;
	m_IngredientSauceState.Transform.AttachParent( &m_PizzaBaseState.Transform );

	// �ۑ����Ă���s�U�̎擾.
	std::unordered_map<std::string, json> PizzaData = CPizzaResource::GetPizzaData();
	m_SavePizzaNum = static_cast<int>( PizzaData.size() );
	for ( auto& [Name, Data] : PizzaData ) {
		// �Â��s�U�f�[�^����V�����s�U�f�[�^�ɕϊ�����R���o�[�^�[.
#if 0
		json j;
		if ( Data["Base"] == 0 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 2;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
			}
		}
		else if ( Data["Base"] == 1 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 2;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
		}
		else if ( Data["Base"] == 2 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 3;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
			}
		}
		else if ( Data["Base"] == 3 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Sauce"][i]["Type"]		= 3;
				j["Sauce"][i]["MaskType"]	= i;
				j["Sauce"][i]["MaskNo"]		= 0;
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
		}
		else if ( Data["Base"] == 4 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
		}
		else if ( Data["Base"] == 5 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
			j["Ingredients"][0]["Position"]["x"] = 0.0f;
			j["Ingredients"][0]["Position"]["y"] = 0.0f;
			j["Ingredients"][0]["Rotation"]		 = 0.0f;
			j["Ingredients"][0]["Type"]		 = -4;
		}
		else if ( Data["Base"] == 6 ) {
			for ( int i = 0; i < 8; ++i ) {
				j["Cheese"][i]["Type"]		= 0;
				j["Cheese"][i]["MaskType"]	= i;
				j["Cheese"][i]["MaskNo"]	= 0;
			}
			j["Ingredients"][0]["Position"]["x"] = 0.0f;
			j["Ingredients"][0]["Position"]["y"] = 0.0f;
			j["Ingredients"][0]["Rotation"]		 = 0.0f;
			j["Ingredients"][0]["Type"]		 = -3;
		}
		
		for ( int i = 0; i < Data["Ingredients"].size(); ++i ) {
			j["Ingredients"][i]["Position"]["x"]	=  Data["Ingredients"][i]["Position"]["x"];
			j["Ingredients"][i]["Position"]["y"]	=  Data["Ingredients"][i]["Position"]["y"];
			j["Ingredients"][i]["Rotation"]			=  Data["Ingredients"][i]["Rotation"];
			if (  Data["Ingredients"][i]["Type"] <= 3 )  j["Ingredients"][i]["Type"] = 59;
			else if (  Data["Ingredients"][i]["Type"] <= 5 )  j["Ingredients"][i]["Type"] = 36;
			else if (  Data["Ingredients"][i]["Type"] <= 6 )  j["Ingredients"][i]["Type"] = 37;
			else if (  Data["Ingredients"][i]["Type"] <= 7 )  j["Ingredients"][i]["Type"] = 38;
			else if (  Data["Ingredients"][i]["Type"] <= 9 )  j["Ingredients"][i]["Type"] = 52;
			else if (  Data["Ingredients"][i]["Type"] <= 10 ) j["Ingredients"][i]["Type"] = 53;
			else if (  Data["Ingredients"][i]["Type"] <= 11 ) j["Ingredients"][i]["Type"] = 6*8-1;
			else if (  Data["Ingredients"][i]["Type"] <= 15 ) j["Ingredients"][i]["Type"] = 6*9+4-1;
			else if (  Data["Ingredients"][i]["Type"] <= 16 ) j["Ingredients"][i]["Type"] = 6*9+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 17 ) j["Ingredients"][i]["Type"] = 6*9+2-1;
			else if (  Data["Ingredients"][i]["Type"] <= 18 ) j["Ingredients"][i]["Type"] = 6*9+3-1;
			else if (  Data["Ingredients"][i]["Type"] <= 19 ) j["Ingredients"][i]["Type"] = 6*10+3-1;
			else if (  Data["Ingredients"][i]["Type"] <= 20 ) j["Ingredients"][i]["Type"] = 6*10+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 21 ) j["Ingredients"][i]["Type"] = 6*10+2-1;
			else if (  Data["Ingredients"][i]["Type"] <= 22 ) j["Ingredients"][i]["Type"] = 6*7+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 23 ) j["Ingredients"][i]["Type"] = 6*7+2-1;
			else if (  Data["Ingredients"][i]["Type"] <= 24 ) j["Ingredients"][i]["Type"] = 6*7+3-1;
			else if (  Data["Ingredients"][i]["Type"] <= 25 ) j["Ingredients"][i]["Type"] = 6*7+4-1;
			else if (  Data["Ingredients"][i]["Type"] <= 27 ) j["Ingredients"][i]["Type"] = 6*7+5-1;
			else if (  Data["Ingredients"][i]["Type"] <= 28 ) j["Ingredients"][i]["Type"] = 6*8+1-1;
			else if (  Data["Ingredients"][i]["Type"] <= 29 ) j["Ingredients"][i]["Type"] = 6*8+3-1;
			else  j["Ingredients"][i]["Type"] = 6*9+5-1;
		}
		FileManager::JsonSave("Data\\Parameter\\Pizza\\C\\" + Name + ".json", j);
#endif
		m_SavePizzaData.emplace_back( std::make_pair( Name, Data ) );
	}
	m_SamplePizzaBaseState = m_PizzaBaseState;
	m_SamplePizzaBaseState.Transform.pParent	= nullptr;
	m_SamplePizzaBaseState.Transform.Scale		= { 0.5f, 0.5f, 0.5f };
	m_SamplePizzaBaseState.AnimState.AnimNumber = 1;
	SavePizzaSampleInit();
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CPizzaEditor::Update( const float& DeltaTime )
{
	m_DeltaTime							= DeltaTime;
	m_IsBButtonAction					= false;

	if ( m_GrabSauceNo == NO_GRAB_NO && m_GrabIngredientNo == NO_GRAB_NO ) {
		m_CursorState.AnimState.AnimNumber = NormaCursorl;
	}

	if ( m_IsPause ) {
		PauseOpen();				// �|�[�Y��ʂ��J��.
		PauseUpdate();				// �|�[�Y���̍X�V.
	}
	else {
		CursorMove();				// �J�[�\���̈ړ�.
		NewSauce();					// �V�����\�[�X���擾.
		NewIngredient();			// �V������ނ��擾.
		IngredientGrabEnd();		// ��ނ݂̒͂��I������.
		IngredientSauceGrabEnd();	// ��ނ̃\�[�X�݂̒͂��I������.
		SauceGrabEnd();				// �\�[�X�݂̒͂��I������.
		GrabStart();				// ��ނ݂̒͂��J�n����.
		GradRotation();				// �͂�ł��ނ̉�].
		GradUpdate();				// �݂͂̍X�V.
		TrashAreaUpdate();			// �폜�G���A�̍X�V.
		IngredientDeleteUpdate();	// ��ނ̍폜�̍X�V.
		PushUpDownButton();			// �㉺�{�^�������������̍X�V.
		PushLeftRightButton();		// ���E�{�^�������������̍X�V.
		PushTextButton();			// �e�L�X�g�{�^�������������̍X�V.
		PutInKilnAnimUpdate();		// �q�ɓ����A�j���[�V�����̍X�V.

		PauseClose();				// �|�[�Y��ʂ����.
		MovePause();				// �|�[�Y�Ɉړ�.

		TextUpdate();				// ��������̍X�V.

		SaveAreaUpdate();			// �ۑ��G���A�̍X�V.
		SaveAreaButtonUpdate();		// �ۑ��G���A�̃{�^���̍X�V.

		DragAndDropUpdate();		// �h���b�N&�h���b�v�̍X�V.
	}

	// SE�����[�v�ōĐ�.
	CSoundManager::PlaySELoop( PIZZAKILN_SE, true );
	CSoundManager::PlaySELoop( BACK_SE,		 true );
}

//---------------------------.
// UI�̕`��.
//---------------------------.
void CPizzaEditor::UIRender()
{
	m_pBack->RenderUI();
	for( auto& t : m_TextButtonStateList ) m_pTextButton->RenderUI( &t );
	m_pSpatula->RenderUI( &m_AnimSpatulaState );
	m_pSpatula->RenderUI( &m_SpatulaState );
	m_pPizzaBase->RenderUI( &m_PizzaBaseState );
	if ( m_IsDispSauce ) {
		for ( auto& s : m_SauceAreaStateList		) m_pSauceArea->RenderUI( &s );
	}
	else {
		for ( auto& i : m_IngredientAreaStateList	) m_pIngredientArea->RenderUI( &i );
	}
	for ( auto& b : m_UpDownButtonStateList		) m_pUpDownButton->RenderUI( &b );
	for ( auto& b : m_LeftRightButtonStateList	) m_pLeftRightButton->RenderUI( &b );
	for ( auto& t : m_TextStateList				) m_pText->RenderUI( &t );
	for ( auto& s : m_SaveAreaStateList			) m_pSaveArea->RenderUI( &s );
	SavePizzaSampleRender();
	for ( auto& a : m_TrashAreaStateList		) m_pTrashArea->RenderUI( &a );
	for ( auto& s : m_SauceStateList			) m_pSauce->RenderUI( &s );
	for ( auto& c : m_CheeseStateList			) m_pSauce->RenderUI( &c );
	for ( auto& i : m_IngredientStateList ) {
		if ( i.AnimState.AnimNumber < 0 ) {
			m_IngredientSauceState.Transform			= i.Transform;
			m_IngredientSauceState.AnimState.AnimNumber = std::abs( i.AnimState.AnimNumber ) - 1;
			m_pIngredientSauce->RenderUI( &m_IngredientSauceState );
			continue;
		}
		m_pIngredient->RenderUI( &i );
	}
	m_pKiln->RenderUI( &m_KilnState );
	m_pCursor->RenderUI( &m_CursorState );
	if ( m_GrabIngredientNo == NO_GRAB_NO && m_GrabSauceNo == NO_GRAB_NO ) {
		m_pFont->RenderUI( &m_FontState );
	}

	// �|�[�Y�̕`��.
	if ( m_IsPauseAnim || m_IsPause ) {
		m_pPauseBack->RenderUI( &m_PauseBackState );
		for ( auto& t : m_PauseTextStateList ) m_pPauseText->RenderUI( &t );
		m_pPauseCursor->RenderUI( &m_PauseCursorState );
	}
}

//---------------------------.
// �J�[�\���̈ړ�.
//---------------------------.
void CPizzaEditor::CursorMove()
{
	if ( m_IsPutInKiln ) return;

	// �ړ����x�̕ύX.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_LEFT_THUMB ) ) {
		m_CursorMoveSpeedNo++;
		if ( m_CursorMoveSpeedNo >= 3 ) m_CursorMoveSpeedNo = 0;
	}

	// ���W�̎擾.
	D3DXPOSITION3* pPos = &m_CursorState.Transform.Position;

	// ���Ɉړ�.
	pPos->x += CXInput::GetLThumbX() * m_CursorMoveSpeedList[m_CursorMoveSpeedNo];
	pPos->y -= CXInput::GetLThumbY() * m_CursorMoveSpeedList[m_CursorMoveSpeedNo];

	// �J�[�\����͈͓��Ɏ��߂�.
	pPos->x = std::clamp( pPos->x, 0.0f, FWND_W );
	pPos->y = std::clamp( pPos->y, 0.0f, FWND_H );
}

//---------------------------.
// �V�����\�[�X�̎擾.
//---------------------------.
void CPizzaEditor::NewSauce()
{
	if ( m_IsPizzaSave 					) return;
	if ( m_IsPizzaLoad					) return;
	if ( m_IsPizzaDelete				) return;
	if ( m_IsPizzaDownload				) return;
	if ( m_IsDispSauce	== false		) return;
	if ( m_GrabSauceNo	!= NO_GRAB_NO	) return;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// ������.
	m_FontState.Text = "";

	// ��ރG���A�ɏd�Ȃ��Ă��邩.
	const SSize& Size = m_pSauceArea->GetSpriteState().Disp;
	for ( auto& i : m_SauceAreaStateList ) {
		if ( i.Transform.Position.x <= Pos.x && Pos.x <= i.Transform.Position.x + Size.w &&
			 i.Transform.Position.y <= Pos.y && Pos.y <= i.Transform.Position.y + Size.h )
		{
			// ��ނ̖��O�̐ݒ�.
			if ( i.AnimState.AnimNumber == 0 ) return;
			m_FontState.Text					= m_SauceNameList[i.AnimState.AnimNumber - 1];
			m_FontState.Transform.Position.x	= i.Transform.Position.x + Size.w / 2.0f;
			m_FontState.Transform.Position.y	= i.Transform.Position.y + 10.0f;

			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber	= CanGrabCursor;

			// B�{�^�����������u�Ԃ�.
			if ( m_IsBButtonAction								 ) return;
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;

			// �\�[�X��͂�.
			m_GrabSauceNo						= i.AnimState.AnimNumber;
			m_GrabPosition						= INIT_FLOAT3;
			m_IsBButtonAction					= true;

			// �J�[�\���̕ύX.
			if ( m_GrabSauceNo == 1 ) {
				m_CursorState.AnimState.AnimNumber = CheeseCursor;

				// ���ʉ��̍Đ�.
				CSoundManager::PlaySE( CHEESE_GRAB_SE );
			}
			else {
				m_CursorState.AnimState.AnimNumber = SauceCursor;

				// ���ʉ��̍Đ�.
				CSoundManager::PlaySE( SAUCE_SE );
			}
			return;
		}
	}
}

//---------------------------.
// �V������ނ̎擾.
//---------------------------.
void CPizzaEditor::NewIngredient()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// ������.
	m_FontState.Text					= "";

	// ��ރG���A�ɏd�Ȃ��Ă��邩.
	const SSize& Size = m_pIngredientArea->GetSpriteState().Disp;
	for ( auto& i : m_IngredientAreaStateList ) {
		if ( i.Transform.Position.x <= Pos.x && Pos.x <= i.Transform.Position.x + Size.w &&
			 i.Transform.Position.y <= Pos.y && Pos.y <= i.Transform.Position.y + Size.h )
		{
			// ��ނ̖��O�̐ݒ�.
			if ( i.AnimState.AnimNumber == 0 ) return;
			m_FontState.Text					= m_IngredientNameList[i.AnimState.AnimNumber - 1];
			m_FontState.Transform.Position.x	= i.Transform.Position.x + Size.w / 2.0f;
			m_FontState.Transform.Position.y	= i.Transform.Position.y + 10.0f;

			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber	= CanGrabCursor;

			// B�{�^�����������u�Ԃ�.
			if ( m_IsBButtonAction								 ) return;
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			m_IsBButtonAction = true;

			// ��ނ̃\�[�X�̑I��.
			if ( i.AnimState.AnimNumber == 20 ) {
				m_CursorState.AnimState.AnimNumber	= MayonnaiseCursor;
				m_GrabIngredientNo					= GRAB_MAYONNAISE_NO;
				return;
			}
			else if ( i.AnimState.AnimNumber == 21 ) {
				m_CursorState.AnimState.AnimNumber	= TeriyakiCursor;
				m_GrabIngredientNo					= GRAB_TERIYAKI_NO;
				return;
			}

			// �V������ނ̒ǉ�.
			m_GrabIngredientNo = static_cast<int>( m_IngredientStateList.size() );
			m_IngredientStateList.emplace_back( m_IngredientStateBase );
			m_IngredientStateList.back().AnimState.AnimNumber	= CRandom::GetRand( m_IngredientTypeAreaList[i.AnimState.AnimNumber - 1] );
			m_IngredientStateList.back().Transform.Position.x	= Pos.x;
			m_IngredientStateList.back().Transform.Position.y	= Pos.y;
			m_IngredientStateList.back().Transform.Rotation.z	= CRandom::GetRand( Math::RADIAN_MIN, Math::RADIAN_MAX );
			m_GrabPosition										= INIT_FLOAT3;

			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber					= GrabCursor;

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( INGREDIENT_GRAB_SE );
			return;
		}
	}
}

//---------------------------.
// ��ނ݂̒͂��J�n����.
//---------------------------.
void CPizzaEditor::GrabStart()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;
	
	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// �摜�T�C�Y�̎擾.
	const SSize& SpriteSize = m_pIngredient->GetSpriteState().Disp;

	// �}�E�X�ɏd�Ȃ��Ă��邩.
	float GrabDistance = 10000.0f;
	const int Size = static_cast<int>( m_IngredientStateList.size() );
	for ( int i = Size - 1; i >= 0; i-- ) {
		if ( m_IngredientStateList[i].UIAnimState.Flag_x ) continue;
		const D3DXPOSITION3& WorldScale	= m_IngredientStateList[i].Transform.GetWorldScale();
		const D3DXPOSITION3& WorldPos	= m_IngredientStateList[i].Transform.GetWorldPosition();
		const D3DXPOSITION3& LUPos		= WorldPos - D3DXPOSITION3( ( SpriteSize.w * WorldScale.x ) / 2.0f, ( SpriteSize.h * WorldScale.y ) / 2.0f, 0.0f );
		if ( LUPos.x <= Pos.x && Pos.x <= LUPos.x + SpriteSize.w * WorldScale.x &&
			 LUPos.y <= Pos.y && Pos.y <= LUPos.y + SpriteSize.h * WorldScale.y )
		{
			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = CanGrabCursor;

			// ���������߂�.
			if ( m_IsBButtonAction								 ) return;
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			const D3DXVECTOR3&	Vector	 = Pos - WorldPos;
			const float			Distance = D3DXVec3Length( &Vector );

			// �͂ދ�ނ̍X�V.
			if ( Distance >= GrabDistance ) continue;
			m_GrabIngredientNo = i;
			GrabDistance		= Distance;
			m_GrabPosition		= WorldPos - Pos;
		}
	}

	// �͂񂾋�ނ�z��̈�Ԍ��Ɉړ�������.
	if ( m_GrabIngredientNo == NO_GRAB_NO ) return;
	const SSpriteRenderState Grab = m_IngredientStateList[m_GrabIngredientNo];
	m_IngredientStateList.erase( m_IngredientStateList.begin() + m_GrabIngredientNo );
	m_IngredientStateList.emplace_back( Grab );

	// �͂�ł����ނ�ۑ�.
	m_GrabIngredientNo = Size - 1;
	m_IsBButtonAction	= true;

	// �J�[�\���̕ύX.
	m_CursorState.AnimState.AnimNumber = GrabCursor;

	// ���ʉ��̍Đ�.
	if ( Grab.AnimState.AnimNumber >= 0 )	CSoundManager::PlaySE( INGREDIENT_GRAB_SE  );
	else									CSoundManager::PlaySE( SAUCE_SE );
}

//---------------------------.
// ��ނ݂̒͂��I������.
//---------------------------.
void CPizzaEditor::IngredientGrabEnd()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo <= NO_GRAB_NO	) return;
	if ( m_IsBButtonAction					) return;

	// B�{�^�����������u�Ԃ�.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
	m_IsBButtonAction = true;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_IngredientStateList[m_GrabIngredientNo].Transform.GetWorldPosition();

	// ��ނ𗣂�.
	bool IsDelete = false;
	if ( Pos.x <= m_pTrashArea->GetSpriteState().Disp.w ||
		 Pos.x >= m_TrashAreaStateList[1].Transform.Position.x )
	{
		// �������ꏊ����ރG���A�̏ꍇ�폜����.
		m_IngredientStateList.erase( m_IngredientStateList.begin() + m_GrabIngredientNo );
		IsDelete = true;

		// ���ʉ��̍Đ�.
		CSoundManager::PlaySE( TRASH_SE );
	}
 
	// �܂Ȕ̏�ł͂Ȃ���.
	else if ( m_IngredientStateList[m_GrabIngredientNo].AnimState.AnimNumber >= 0 &&
				( 390.0f >= Pos.x || Pos.x >= 899.0f ||
				  107.0f >= Pos.y || Pos.y >= 608.0f ) )
	{
		m_IngredientStateList[m_GrabIngredientNo].UIAnimState.Flag_x = true;
		IsDelete = true;

		// ���ʉ��̍Đ�.
		CSoundManager::PlaySE( FALL_SE );
	}

	// �\�[�X��^�񒆂ɖ߂�.
	if ( IsDelete == false ) {
		if ( m_IngredientStateList[m_GrabIngredientNo].AnimState.AnimNumber < 0 ) {
			m_IngredientStateList[m_GrabIngredientNo].Transform.Position = INIT_FLOAT3;

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( SAUCE_SE );
		}
		else {
			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( INGREDIENT_GRAB_END_SE );
		}
	}

	// �͂�ł��ނ𗣂�.
	m_GrabIngredientNo = NO_GRAB_NO;

	// �J�[�\���̕ύX.
	m_CursorState.AnimState.AnimNumber = NormaCursorl;
}

//---------------------------.
// ��ނ̃\�[�X�݂̒͂��I������.
//---------------------------.
void CPizzaEditor::IngredientSauceGrabEnd()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo >= NO_GRAB_NO	) return;
	if ( m_IsBButtonAction					) return;

	// B�{�^�����������u�Ԃ�.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
	m_IsBButtonAction = true;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// ��ނ𗣂�.
	if ( Pos.x <= m_pTrashArea->GetSpriteState().Disp.w ||
		 Pos.x >= m_TrashAreaStateList[1].Transform.Position.x )
	{
		// �͂�ł��ނ𗣂�.
		m_GrabIngredientNo = NO_GRAB_NO;

		// �J�[�\���̕ύX.
		m_CursorState.AnimState.AnimNumber = NormaCursorl;
		return;
	}
 
	// ���n�̏ォ�m�F.
	const D3DXPOSITION3& CenterPos	= m_PizzaBaseState.Transform.GetWorldPosition();
	D3DXVECTOR3			 Vector		= Pos - CenterPos;
	const float			 Distance	= D3DXVec3Length( &Vector );
	if ( Distance >= 180.0f ) return;

	// ��ނ̃\�[�X��z�u.
	m_IngredientStateList.emplace_back( m_IngredientStateBase );
	if (		m_GrabIngredientNo == GRAB_MAYONNAISE_NO ) m_IngredientStateList.back().AnimState.AnimNumber = -1;
	else if (	m_GrabIngredientNo == GRAB_TERIYAKI_NO	 ) m_IngredientStateList.back().AnimState.AnimNumber = -2;
	m_IngredientStateList.back().Transform.Position.x = 0.0f;
	m_IngredientStateList.back().Transform.Position.y = 0.0f;
	m_IngredientStateList.back().Transform.Rotation.z = CRandom::GetRand( Math::RADIAN_MIN, Math::RADIAN_MAX );

	// ���ʉ��̍Đ�.
	CSoundManager::PlaySE( SAUCE_SE );
}

//---------------------------.
// �\�[�X�݂̒͂��I������.
//---------------------------.
void CPizzaEditor::SauceGrabEnd()
{
	if ( m_IsPizzaSave					) return;
	if ( m_IsPizzaLoad					) return;
	if ( m_IsPizzaDelete				) return;
	if ( m_IsPizzaDownload				) return;
	if ( m_IsDispSauce	== false		) return;
	if ( m_GrabSauceNo	== NO_GRAB_NO	) return;
	if ( m_IsBButtonAction				) return;

	// B�{�^�����������u�Ԃ�.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
	m_IsBButtonAction = true;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// ��ނ𗣂�.
	if ( Pos.x <= m_pTrashArea->GetSpriteState().Disp.w ||
		 Pos.x >= m_TrashAreaStateList[1].Transform.Position.x )
	{
		// �������ꏊ����ރG���A�̏ꍇ�폜����.
		m_GrabSauceNo = NO_GRAB_NO;

		// ���ʉ��̍Đ�.
		CSoundManager::PlaySE( TRASH_SE );
	}
 
	// ���n�̏ォ�m�F.
	const D3DXPOSITION3& CenterPos	= m_PizzaBaseState.Transform.GetWorldPosition();
	D3DXVECTOR3			 Vector		= Pos - CenterPos;
	const float			 Distance	= D3DXVec3Length( &Vector );
	if ( Distance >= 180.0f ) return;

	// �p�x���擾.
	float Ang = Math::ToDegree( atan2f( Pos.y - CenterPos.y, Pos.x - CenterPos.x ) );
	if ( Ang < Math::DEGREE_MIN ) Ang += Math::DEGREE_MAX;

	// ���łɃ`�[�Y���z�u����Ă��邩.
	const int AreaNo = static_cast<int>( Ang / 45.0f );
	if ( m_GrabSauceNo == 1 && m_SauceList[AreaNo].second != NO_SAUCE_NO ) return;

	// �`�[�Y��z�u���邩.
	const int SListNo = static_cast<int>( m_SauceStateList.size()  );
	const int CListNo = static_cast<int>( m_CheeseStateList.size() );
	if ( m_GrabSauceNo == 1 ) {
		m_CheeseStateList.emplace_back( m_SauceStateBase );
		m_CheeseStateList.back().AnimState.AnimNumber											= m_GrabSauceNo;
		m_SauceList[AreaNo].second																= CListNo;
		if ( m_SauceList[AreaNo].first == NO_SAUCE_NO ) m_CheeseStateList.back().pMaskTexture	= CRandom::GetRand( m_pSauceMaskList[AreaNo] );
		else											m_CheeseStateList.back().pMaskTexture	= m_SauceStateList[m_SauceList[AreaNo].first].pMaskTexture;

		// ���ʉ��̍Đ�.
		CSoundManager::PlaySE( CHEESE_GRAB_END_SE );
		return;
	}

	// ���łɃ\�[�X���h���Ă��邩.
	if ( m_SauceList[AreaNo].first != NO_SAUCE_NO ) {
		// ���łɂ���\�[�X����ԏ�Ɉړ�������.
		const SSpriteRenderState Sauce = m_SauceStateList[m_SauceList[AreaNo].first];
		m_SauceStateList.erase( m_SauceStateList.begin() + m_SauceList[AreaNo].first );
		m_SauceStateList.emplace_back( Sauce );
		for ( auto& [Sauce, Cheese] : m_SauceList ) {
			if ( Sauce  > m_SauceList[AreaNo].first ) Sauce--;
		}

		// �\�[�X��ύX����.
		m_SauceList[AreaNo].first											= SListNo - 1;
		m_SauceStateList[m_SauceList[AreaNo].first].AnimState.AnimNumber	= m_GrabSauceNo;
		m_SauceStateList[m_SauceList[AreaNo].first].pMaskTexture			= CRandom::GetRand( m_pSauceMaskList[AreaNo] );

		// �`�[�Y������ꍇ�B��邽�ߖ�����.
		if ( m_SauceList[AreaNo].second != NO_SAUCE_NO ) {
			m_CheeseStateList.erase( m_CheeseStateList.begin() + m_SauceList[AreaNo].second );
			for ( auto& [Sauce, Cheese] : m_SauceList ) {
				if ( Cheese > m_SauceList[AreaNo].second ) Cheese--;
			}
			m_SauceList[AreaNo].second = NO_SAUCE_NO;
		}

		// ���ʉ��̍Đ�.
		CSoundManager::PlaySE( SAUCE_SE );
		return;
	}

	// �\�[�X��h��.
	m_SauceStateList.emplace_back( m_SauceStateBase );
	m_SauceStateList.back().AnimState.AnimNumber = m_GrabSauceNo;
	m_SauceStateList.back().pMaskTexture		 = CRandom::GetRand( m_pSauceMaskList[AreaNo] );
	m_SauceList[AreaNo].first					 = SListNo;

	// �`�[�Y������ꍇ�B��邽�ߖ�����.
	if ( m_SauceList[AreaNo].second != NO_SAUCE_NO ) {
		m_CheeseStateList.erase( m_CheeseStateList.begin() + m_SauceList[AreaNo].second );
		for ( auto& [Sauce, Cheese] : m_SauceList ) {
			if ( Cheese > m_SauceList[AreaNo].second ) Cheese--;
		}
		m_SauceList[AreaNo].second = NO_SAUCE_NO;
	}

	// ���ʉ��̍Đ�.
	CSoundManager::PlaySE( SAUCE_SE );
}

//---------------------------.
// �݂͂̍X�V.
//---------------------------.
void CPizzaEditor::GradUpdate()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_GrabIngredientNo <= NO_GRAB_NO	) return;

	// ���W�̍X�V.
	m_IngredientStateList[m_GrabIngredientNo].Transform.SetWorldPosition( m_CursorState.Transform.Position + m_GrabPosition );

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_IngredientStateList[m_GrabIngredientNo].Transform.GetWorldPosition();

	// �R�s�[���邩.
	if ( m_IsDispSauce ) return;
	if ( Pos.x > m_pTrashArea->GetSpriteState().Disp.w &&
		 Pos.x < m_TrashAreaStateList[1].Transform.Position.x && 
		 CXInput::IsKeyDown( XINPUT_GAMEPAD_A ) )
	{
		if ( m_IngredientStateList[m_GrabIngredientNo].AnimState.AnimNumber < 0 ) {
			// ���n�̏ォ�m�F.
			const D3DXPOSITION3& CenterPos	= m_PizzaBaseState.Transform.GetWorldPosition();
			D3DXVECTOR3			 Vector		= Pos - CenterPos;
			const float			 Distance	= D3DXVec3Length( &Vector );
			if ( Distance >= 180.0f ) return;

			// ��ނ̃R�s�[�̔z�u.
			m_IngredientStateList.emplace_back( m_IngredientStateList[m_GrabIngredientNo] );
			m_IngredientStateList.back().Transform.Position = { 0.0f, 0.0f, 0.0f };

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( SAUCE_SE );
		}
		else {
			// ��ނ̃R�s�[�̔z�u.
			m_IngredientStateList.emplace_back( m_IngredientStateList[m_GrabIngredientNo] );

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( INGREDIENT_GRAB_END_SE );

			// �܂Ȕ̏�ł͂Ȃ���.
			if ( 390.0f >= Pos.x || Pos.x >= 899.0f ||
				 107.0f >= Pos.y || Pos.y >= 608.0f )
			{
				m_IngredientStateList.back().UIAnimState.Flag_x = true;

				// ���ʉ��̍Đ�.
				CSoundManager::PlaySE( FALL_SE );
			}
		}
	}
}

//---------------------------.
// �͂�ł��ނ̉�].
//---------------------------.
void CPizzaEditor::GradRotation()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsDispSauce						) return;
	if ( m_GrabIngredientNo <= NO_GRAB_NO	) return;

	// �ړ����x�̕ύX.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_RIGHT_THUMB ) ) {
		m_IngredientRotSpeedNo++;
		if ( m_IngredientRotSpeedNo >= 3 ) m_IngredientRotSpeedNo = 0;
	}

	// ��ނ̉�].
	m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z -= Math::ToRadian( m_IngredientRotSpeedList[m_IngredientRotSpeedNo] ) * CXInput::GetLTrigger();
	m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z += Math::ToRadian( m_IngredientRotSpeedList[m_IngredientRotSpeedNo] ) * CXInput::GetRTrigger();
	
	// �p�x��͈͓��Ɏ��߂�.
	if ( m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z < Math::RADIAN_MIN ) {
		m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z += Math::RADIAN_MAX;
	}
	if ( m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z > Math::RADIAN_MAX ) {
		m_IngredientStateList[m_GrabIngredientNo].Transform.Rotation.z -= Math::RADIAN_MAX;
	}
}

//---------------------------.
// �㉺�{�^�������������̍X�V.
//---------------------------.
void CPizzaEditor::PushUpDownButton()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsBButtonAction					) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;
	if ( m_GrabSauceNo		!= NO_GRAB_NO	) return;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// �{�^���ɐG��Ă��邩.
	const SSize& Size = m_pUpDownButton->GetSpriteState().Disp;
	for ( auto& b : m_UpDownButtonStateList ) {
		// �����Ă��Ӗ����Ȃ���.
		if ( m_IsDispSauce ) {
			if ( static_cast<int>( std::ceil( ( m_SauceAreaStateBase.AnimState.AnimNumberMax - 1 ) / SAUCE_MAX ) ) - 1 == 0 ) {
				if (	  b.AnimState.AnimNumber == UpButton	) b.AnimState.AnimNumber = PushUpButton;
				else if ( b.AnimState.AnimNumber == DownButton	) b.AnimState.AnimNumber = PushDownButton;
				continue;
			}
		}
		else {
			if ( static_cast<int>( std::ceil( m_IngredientAreaStateBase.AnimState.AnimNumberMax / INGREDIENTS_MAX ) ) == 0 ) {
				if (	  b.AnimState.AnimNumber == UpButton	) b.AnimState.AnimNumber = PushUpButton;
				else if ( b.AnimState.AnimNumber == DownButton	) b.AnimState.AnimNumber = PushDownButton;
				continue;
			}
		}

		// �{�^���̌����ڂ̕ύX.
		if ( CXInput::IsKeyUp( XINPUT_GAMEPAD_B ) ) {
			if (	  b.AnimState.AnimNumber == PushUpButton	) b.AnimState.AnimNumber = UpButton;
			else if ( b.AnimState.AnimNumber == PushDownButton	) b.AnimState.AnimNumber = DownButton;
		}

		// �{�^���ɐG��Ă��邩.
		if ( b.Transform.Position.x <= Pos.x - 32.0f && Pos.x <= b.Transform.Position.x + Size.w &&
			 b.Transform.Position.y <= Pos.y - 32.0f && Pos.y <= b.Transform.Position.y + Size.h )
		{
			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// B�{�^�����������u�Ԃ�.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			if ( b.AnimState.AnimNumber == 0 )	m_AreaPageNo--;
			else								m_AreaPageNo++;
			m_IsBButtonAction = true;
			b.AnimState.AnimNumber++;

			// ��ޒu����̃y�[�W�̏������.
			if ( m_IsDispSauce ) ChangeSauce();
			else				 ChangeIngredient();

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( BUTTON_SE );
		}
		else {
			// �{�^���̌����ڂ����Ƃɖ߂�.
			if (	  b.AnimState.AnimNumber == PushUpButton	) b.AnimState.AnimNumber = UpButton;
			else if ( b.AnimState.AnimNumber == PushDownButton	) b.AnimState.AnimNumber = DownButton;
		}
	}
}

//---------------------------.
// ���E�{�^�������������̍X�V.
//---------------------------.
void CPizzaEditor::PushLeftRightButton()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsBButtonAction					) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;
	if ( m_GrabSauceNo		!= NO_GRAB_NO	) return;

	// ���W�̎擾.
	const D3DXPOSITION3& Pos = m_CursorState.Transform.Position;

	// �{�^���ɐG��Ă��邩.
	const SSize& Size = m_pLeftRightButton->GetSpriteState().Disp;
	for ( auto& b : m_LeftRightButtonStateList ) {
		// �����Ă��Ӗ����Ȃ���.
		if ( static_cast<int>( std::ceilf( m_SavePizzaNum / 6.0f ) ) - 1 == 0 ) {
			if (	  b.AnimState.AnimNumber == LeftButton	) b.AnimState.AnimNumber = PushLeftButton;
			else if ( b.AnimState.AnimNumber == RightButton ) b.AnimState.AnimNumber = PushRightButton;
			continue;
		}

		// �{�^���̌����ڂ̕ύX.
		if ( CXInput::IsKeyUp( XINPUT_GAMEPAD_B ) ) {
			if (	  b.AnimState.AnimNumber == PushLeftButton	) b.AnimState.AnimNumber = LeftButton;
			else if ( b.AnimState.AnimNumber == PushRightButton	) b.AnimState.AnimNumber = RightButton;
		}

		// �{�^���ɐG��Ă��邩.
		if ( b.Transform.Position.x <= Pos.x - 32.0f && Pos.x <= b.Transform.Position.x + Size.w &&
			 b.Transform.Position.y <= Pos.y - 32.0f && Pos.y <= b.Transform.Position.y + Size.h )
		{
			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// B�{�^�����������u�Ԃ�.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			if ( b.AnimState.AnimNumber == 0 )	m_SavePizzaPageNo--;
			else								m_SavePizzaPageNo++;
			m_IsBButtonAction = true;
			b.AnimState.AnimNumber++;

			// �y�[�W�̏������.
			if ( m_SavePizzaPageNo < 0 ) {
				m_SavePizzaPageNo = static_cast<int>( std::ceilf( m_SavePizzaNum / 6.0f ) ) - 1;
			}
			else if ( m_SavePizzaPageNo * 6 >= m_SavePizzaNum ) {
				m_SavePizzaPageNo = 0;
			}

			// �ۑ����Ă���s�U�̃T���v���̏�����.
			SavePizzaSampleInit();

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( BUTTON_SE );
		}
		else {
			// �{�^���̌����ڂ����Ƃɖ߂�.
			if (	  b.AnimState.AnimNumber == PushLeftButton	) b.AnimState.AnimNumber = LeftButton;
			else if ( b.AnimState.AnimNumber == PushRightButton	) b.AnimState.AnimNumber = RightButton;
		}
	}
}

//---------------------------.
// �\�[�X�̕ύX.
//---------------------------.
void CPizzaEditor::ChangeSauce()
{
	// �y�[�W�̏������.
	if ( m_AreaPageNo < 0 ) {
		m_AreaPageNo = static_cast<int>( std::ceil( ( m_SauceAreaStateBase.AnimState.AnimNumberMax - 1 ) / SAUCE_MAX ) ) - 1;
	}
	else if ( m_AreaPageNo * SAUCE_MAX + 1 >= m_SauceAreaStateBase.AnimState.AnimNumberMax ) {
		m_AreaPageNo = 0;
	}

	// ��ޒu����̌����ڂ̕ύX.
	for ( int i = 0; i < SAUCE_MAX; ++i ) {
		m_SauceAreaStateList[i].AnimState.AnimNumber = m_AreaPageNo * SAUCE_MAX + i + 1;
		
		// �p�ӂ���Ă��Ȃ��ꍇ�󔠂ɕύX����.
		if ( m_SauceAreaStateList[i].AnimState.AnimNumber >= m_SauceAreaStateList[i].AnimState.AnimNumberMax ) {
			m_SauceAreaStateList[i].AnimState.AnimNumber = 0;
		}
	}
}

//---------------------------.
// ��ނ̕ύX.
//---------------------------.
void CPizzaEditor::ChangeIngredient()
{
	// �y�[�W�̏������.
	if ( m_AreaPageNo < 0 ) {
		m_AreaPageNo = static_cast<int>( std::ceil( m_IngredientAreaStateBase.AnimState.AnimNumberMax / INGREDIENTS_MAX ) );
	}
	else if ( m_AreaPageNo * INGREDIENTS_MAX + 1 >= m_IngredientAreaStateBase.AnimState.AnimNumberMax ) {
		m_AreaPageNo = 0;
	}

	// ��ޒu����̌����ڂ̕ύX.
	for ( int i = 0; i < INGREDIENTS_MAX; ++i ) {
		m_IngredientAreaStateList[i].AnimState.AnimNumber = m_AreaPageNo * INGREDIENTS_MAX + i + 1;
		
		// �p�ӂ���Ă��Ȃ��ꍇ�󔠂ɕύX����.
		if ( m_IngredientAreaStateList[i].AnimState.AnimNumber >= m_IngredientAreaStateList[i].AnimState.AnimNumberMax ) {
			m_IngredientAreaStateList[i].AnimState.AnimNumber = 0;
		}
	}
}

//---------------------------.
// �e�L�X�g�{�^�������������̍X�V.
//---------------------------.
void CPizzaEditor::PushTextButton()
{
	if ( m_IsPutInKiln								) return;
	if ( m_IsBButtonAction							) return;
	if ( m_GrabIngredientNo			!= NO_GRAB_NO	) return;
	if ( m_GrabSauceNo				!= NO_GRAB_NO	) return;

	for( auto& t : m_TextButtonStateList ){
		if ( t.IsDisp == false ) continue;

		// �{�^���̌����ڂ̕ύX.
		int* pButtonNo = &t.AnimState.AnimNumber;
		if ( CXInput::IsKeyUp( XINPUT_GAMEPAD_B ) ) {
			if ( *pButtonNo % 2 != 0 ) *pButtonNo -= 1;
		}

		// �{�^���ɐG��Ă��邩.
		const SSize&		 ButtonSize	= m_pTextButton->GetSpriteState().Disp;
		const float			 SizeAdjust = t.AnimState.AnimNumber >= YesButton ? 46.0f : 0.0f;
		const D3DXPOSITION3& ButtonPos	= t.Transform.Position;
		const D3DXPOSITION3& CursorPos	= m_CursorState.Transform.Position;
		if ( ButtonPos.x - ButtonSize.w / 2.0f + SizeAdjust + 24.0f <= CursorPos.x && CursorPos.x <= ButtonPos.x + ButtonSize.w / 2.0f - SizeAdjust + 24.0f &&
			 ButtonPos.y - ButtonSize.h / 2.0f				+ 24.0f	<= CursorPos.y && CursorPos.y <= ButtonPos.y + ButtonSize.h / 2.0f				+ 24.0f )
		{
			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// B�{�^�����������u�Ԃ�.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) return;
			*pButtonNo += 1;
			m_IsBButtonAction = true;

			// �{�^�����������Ƃ�.
			if ( *pButtonNo == PushDecisionButton ) {
				// ��ނ̐؂�ւ�.
				m_IsDispSauce	= false;
				*pButtonNo		= PushPutInKilnButton;
			}
			else if ( *pButtonNo == PushPutInKilnButton	) {
				// �q�ɓ����A�j���[�V�����̊J�n.
				m_IsPutInKiln				= true;
				m_AnimSpatulaState.IsDisp	= true;
				m_PutInKilnAnimNo			= SpatulaDispAnim;

				// �S�~���G���A�̐ݒ�.
				for ( auto& a : m_TrashAreaStateList ) {
					a.Color.w				= Color::ALPHA_MIN;
					a.AnimState.AnimNumber	= 1;
				}

				// SE�̍Đ�.
				CSoundManager::PlaySE( SPATULA_SE );
			}
			else {
				if (	  m_IsPizzaSave		) PizzaSave(	 *pButtonNo );
				else if ( m_IsPizzaLoad		) PizzaLoad(	 *pButtonNo );
				else if ( m_IsPizzaDelete	) PizzaDelete(	 *pButtonNo );
				else if ( m_IsPizzaDownload	) PizzaDownload( *pButtonNo );
			}

			// ���ʉ��̍Đ�.
			CSoundManager::PlaySE( BUTTON_SE );
		}
		else {
			// �{�^���̌����ڂ����Ƃɖ߂�.
			if ( *pButtonNo % 2 != 0 ) *pButtonNo -= 1;
		}
	}
}

//---------------------------.
// �s�U�̕ۑ�.
//---------------------------.
void CPizzaEditor::PizzaSave( const int ButtonNo )
{
	m_IsPizzaSave = false;

	// ��ނ̔z�u�ɖ߂�.
	m_TextButtonStateList[0].AnimState.AnimNumber	= PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// �s�U�f�[�^�̍쐬.
	if ( ButtonNo != PushYesButton ) return;
	const int SSize = static_cast<int>( m_SauceStateList.size()		 );
	const int CSize = static_cast<int>( m_CheeseStateList.size()	 );
	const int ISize = static_cast<int>( m_IngredientStateList.size() );
	json j;
	j["Sauce"] = {};
	for ( int i = 0; i < SSize; ++i ) {
		j["Sauce"][i]["Type"]					= m_SauceStateList[i].AnimState.AnimNumber;
		j["Sauce"][i]["MaskType"]				= m_pSauceMaskNoMap[m_SauceStateList[i].pMaskTexture].first;
		j["Sauce"][i]["MaskNo"]					= m_pSauceMaskNoMap[m_SauceStateList[i].pMaskTexture].second;
	}
	j["Cheese"] = {};
	for ( int i = 0; i < CSize; ++i ) {
		j["Cheese"][i]["Type"]					= m_CheeseStateList[i].AnimState.AnimNumber;
		j["Cheese"][i]["MaskType"]				= m_pSauceMaskNoMap[m_CheeseStateList[i].pMaskTexture].first;
		j["Cheese"][i]["MaskNo"]				= m_pSauceMaskNoMap[m_CheeseStateList[i].pMaskTexture].second;
	}
	j["Ingredients"] = {};
	for ( int i = 0; i < ISize; ++i ) {
		j["Ingredients"][i]["Position"]["x"]	= m_IngredientStateList[i].Transform.Position.x;
		j["Ingredients"][i]["Position"]["y"]	= m_IngredientStateList[i].Transform.Position.y;
		j["Ingredients"][i]["Rotation"]			= m_IngredientStateList[i].Transform.Rotation.z;
		j["Ingredients"][i]["Type"]				= m_IngredientStateList[i].AnimState.AnimNumber;
	}

	// �ۑ����Ă���s�U�̐������Z.
	m_SavePizzaNum++;

	// �s�U�̖��O�̍쐬.
	const std::vector<std::string>& PizzaList = CPizzaResource::GetPizzaList();
	std::string			FileName	= "Pizz_" + std::to_string( m_SavePizzaNum );
	bool				IsNameOK	= true;
	int					NameNo		= 0;
	const std::string	OldName		= FileName;
	do {
		IsNameOK = true;
		for ( auto& p : PizzaList ) {
			// �����̖��O�̃t�@�C�������邽�ߌ��ɂ�������t����.
			if ( p == FileName ) {
				NameNo++;
				FileName = OldName + "(" + std::to_string( NameNo ) + ")";

				// ��������t������ԂŖ��O�����Ԃ�Ȃ���������x�m�F����.
				IsNameOK = false;
				break;
			}
		}
	} while ( !IsNameOK );

	// �s�U�f�[�^��ۑ�.
	const std::string FilePath = SAVE_FILE_PATH + FileName + ".json";
	FileManager::JsonSave( FilePath, j );

	// �s�U�f�[�^�����\�[�X�ɒǉ�����.
	CPizzaResource::PushPizzaData( FileName, j );
	m_SavePizzaData.emplace_back( std::make_pair( FileName, j ) );

	// �ۑ����Ă���s�U�̃T���v���̍X�V.
	SavePizzaSampleInit();

	// SE�̍Đ�.
	CSoundManager::PlaySE( DECISION_SE );
}

//---------------------------.
// �s�U�̓ǂݍ���.
//---------------------------.
void CPizzaEditor::PizzaLoad( const int ButtonNo )
{
	// ������.
	m_IsDispSauce	= false;
	m_IsPizzaLoad	= false;
	m_ButtonAnimCnt	= INIT_FLOAT;

	// ��ނ̔z�u�ɖ߂�.
	m_TextButtonStateList[0].AnimState.AnimNumber	= PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// ���݂̃s�U���T���v���̃s�U�ŏ㏑������.
	if ( ButtonNo != PushYesButton ) return;
	const int PizzaNo = std::stoi( m_pSelectSaveAreaState->No ) - 6 * m_SavePizzaPageNo;
	m_PizzaBaseState.AnimState.AnimNumber	= 0;
	m_SauceStateList						= m_SampleSauceStateList[PizzaNo];
	m_CheeseStateList						= m_SampleCheeseStateList[PizzaNo];
	m_IngredientStateList					= m_SampleIngredientStateList[PizzaNo];
	for ( auto& s : m_SauceStateList		) s.Transform.pParent = &m_PizzaBaseState.Transform;
	for ( auto& c : m_CheeseStateList ) {
		c.Transform.pParent		= &m_PizzaBaseState.Transform;
		c.AnimState.AnimNumber	= 1;
	}
	for ( auto& i : m_IngredientStateList ) {
		i.Transform.pParent = &m_PizzaBaseState.Transform;
		if ( i.AnimState.AnimNumber < 0 ) {
			i.AnimState.AnimNumber += static_cast<int>( m_IngredientSauceState.AnimState.AnimNumberMax / 2.0f );
		}
		else {
			i.AnimState.AnimNumber -= static_cast<int>( i.AnimState.AnimNumberMax / 2.0f );;
		}
	}

	// �I�����Ă���Z�[�u�G���A�̏���j������.
	m_pSelectSaveAreaState = nullptr;

	// SE�̍Đ�.
	CSoundManager::PlaySE( DECISION_SE );
}

//---------------------------.
// �s�U�̍폜.
//---------------------------.
void CPizzaEditor::PizzaDelete( const int ButtonNo )
{
	// ������.
	m_IsPizzaDelete		= false;
	m_ButtonAnimCnt		= INIT_FLOAT;

	// ��ނ̔z�u�ɖ߂�.
	if ( m_IsDispSauce ) m_TextButtonStateList[0].AnimState.AnimNumber	= DecisionButton;
	else				 m_TextButtonStateList[0].AnimState.AnimNumber	= PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// �s�U���폜.
	if ( ButtonNo != PushYesButton ) return;
	const int PizzaNo = std::stoi( m_pSelectSaveAreaState->No );
	CPizzaResource::PizzaDelete( m_SavePizzaData[PizzaNo].first );
	m_SavePizzaData.erase( m_SavePizzaData.begin() + PizzaNo );

	// �ۑ����Ă���s�U�̐������炷.
	m_SavePizzaNum--;

	// �y�[�W������ۂɂȂ邩�m�F.
	if ( m_SavePizzaPageNo * 6 >= m_SavePizzaNum ) {
		m_SavePizzaPageNo = static_cast<int>( std::ceilf( m_SavePizzaNum / 6.0f ) ) - 1;
	}

	// �ۑ����Ă���s�U�̃T���v���̍X�V.
	SavePizzaSampleInit();

	// �I�����Ă���Z�[�u�G���A�̏���j������.
	m_pSelectSaveAreaState = nullptr;

	// SE�̍Đ�.
	CSoundManager::PlaySE( TRASH_SE );
}

//---------------------------.
// �s�U�̃_�E�����[�h.
//---------------------------.
void CPizzaEditor::PizzaDownload( const int ButtonNo )
{
	// ������.
	m_IsPizzaDownload	= false;
	m_ButtonAnimCnt		= INIT_FLOAT;

	// ��ނ̔z�u�ɖ߂�.
	if ( m_IsDispSauce ) m_TextButtonStateList[0].AnimState.AnimNumber = DecisionButton;
	else				 m_TextButtonStateList[0].AnimState.AnimNumber = PutInKilnButton;
	m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
	m_TextButtonStateList[1].IsDisp					= false;

	// ���[�U�[���̎擾.
	if ( ButtonNo != PushYesButton ) return;
	char UserName[256];
	DWORD Size = sizeof( UserName );
	if ( GetUserNameA( UserName, &Size ) == 0 ) {
		// ���[�U�[���̎擾�Ɏ��s����.
		ErrorMessage( "�s�U�̃_�E�����[�h�Ɏ��s���܂����B" );
		return;
	}

	// �_�E�����[�h�t�H���_�̐�΃p�X�̍쐬.
	const int PizzaNo = std::stoi( m_pSelectSaveAreaState->No );
	std::string Path = "C:\\Users\\" + std::string( UserName ) + "\\Downloads\\" + m_SavePizzaData[PizzaNo].first + ".json";

	// �s�U�f�[�^���o��.
	FileManager::JsonSave( Path, m_SavePizzaData[PizzaNo].second );

	// �I�����Ă���Z�[�u�G���A�̏���j������.
	m_pSelectSaveAreaState = nullptr;

	// SE�̍Đ�.
	CSoundManager::PlaySE( DECISION_SE );
}

//---------------------------.
// �폜�G���A�̍X�V.
//---------------------------.
void CPizzaEditor::TrashAreaUpdate()
{
	if ( m_IsPutInKiln		) return;
	if ( m_IsPizzaSave		) return;
	if ( m_IsPizzaLoad		) return;
	if ( m_IsPizzaDelete	) return;
	if ( m_IsPizzaDownload	) return;

	// �����ɂ��Ă���.
	if ( m_GrabIngredientNo == NO_GRAB_NO && m_GrabSauceNo == NO_GRAB_NO ) {
		if ( m_TrashAreaStateList[0].Color.w == Color::ALPHA_MIN ) return;
		for ( auto& a : m_TrashAreaStateList ) {
			a.Color.w -= Color::ALPHA_MAX * 3.0f * m_DeltaTime;
			if ( a.Color.w <= Color::ALPHA_MIN ) a.Color.w = Color::ALPHA_MIN;
		}
		return;
	}
	
	// �s�����ɂ��Ă���.
	if ( m_TrashAreaStateList[0].Color.w == Color::ALPHA_MAX ) return;
	for ( auto& a : m_TrashAreaStateList ) {
		a.AnimState.AnimNumber = 0;
		a.Color.w += Color::ALPHA_MAX * 3.0f * m_DeltaTime;
		if ( a.Color.w >= Color::ALPHA_MAX ) a.Color.w = Color::ALPHA_MAX;
	}
}

//---------------------------.
// ��ނ̍폜�̍X�V.
//---------------------------.
void CPizzaEditor::IngredientDeleteUpdate()
{
	int		No	= 0;
	auto	itr = m_IngredientStateList.begin();
	while ( itr != m_IngredientStateList.end() ) {
		if ( itr->UIAnimState.Flag_x ) {
			// ��ނ����������Ă���.
			float Scale = itr->Transform.Scale.x - 1.5f * m_DeltaTime;
			if ( Scale <= 0.0f ) Scale = 0.0f;
			itr->Transform.Scale		= { Scale, Scale, Scale };
			itr->Color.w				= Scale;
			itr->Transform.Rotation.z	+= Math::ToRadian( 90.0f ) * m_DeltaTime;

			// �����Ȃ��Ȃ�܂ŏ������Ȃ�����.
			if ( Scale == 0.0f ) {
				itr = m_IngredientStateList.erase( itr );
				if ( m_GrabIngredientNo > No ) m_GrabIngredientNo--;
			}
			else {
				itr++;
			}
		}
		else {
			itr++;
		}
		No++;
	}
}

//---------------------------.
// �q�ɓ����A�j���[�V�����̍X�V.
//---------------------------.
void CPizzaEditor::PutInKilnAnimUpdate()
{
	if ( m_IsPutInKiln == false ) return;

	switch ( m_PutInKilnAnimNo ) {
	case SpatulaDispAnim:		SpatulaDisp();		break;	// �w����\��.
	case SpatulaBackAnim:		SpatulaBack();		break;	// �w���̖߂�.
	case PutInKilnAnim:			PutInKiln();		break;	// �q�ɓ����.
	case CookPizzaAnim:			CookPizza();		break;	// �s�U���Ă�.
	case RemoveFromKilnAnim:	RemoveFromKiln();	break;	// �q����o��.
	default:										break;
	}
}

//---------------------------.
// �w����\��.
//---------------------------.
void CPizzaEditor::SpatulaDisp()
{
	const SSize& SpatulaSize = m_pSpatula->GetSpriteState().Disp;

	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_PutInKilnAnimCnt += m_DeltaTime;
	if ( m_PutInKilnAnimCnt >= 1.5f ) {
		// ���̃A�j���[�V�����̏���.
		m_AnimSpatulaState.Transform.Position.y = FWND_H - 64.0f;
		m_PutInKilnAnimCnt						= INIT_FLOAT;
		m_AnimSpatulaState.IsDisp				= false;
		m_SpatulaState.IsDisp					= true;
		m_PutInKilnAnimNo						= PutInKilnAnim;

		// �w���̊O�̋�ނ͍폜����.
		for ( auto& i : m_IngredientStateList ) {
			const D3DXPOSITION3& Pos = i.Transform.GetWorldPosition();
			if ( 428.0f >= Pos.x || Pos.x >= 858.0f ||
				 140.0f >= Pos.y || Pos.y >= 565.0f )
			{
				i.UIAnimState.Flag_x = true;
				i.Transform.DetachParent();
			}
		}
		return;
	}

	// �w�������o��.
	m_AnimSpatulaState.Transform.Position.y = FWND_H + SpatulaSize.h + m_PutInKilnAnimVector.y * CEasing::OutQuart( m_PutInKilnAnimCnt, 1.5f );
	
	// �e�L�X�g�{�^�����\���ɂ��Ă���.
	if ( m_TextButtonStateList[0].Color.w <= Color::ALPHA_MIN ) return;
	m_TextButtonStateList[0].Color.w = Color::ALPHA_MAX - m_PutInKilnAnimCnt;
	if ( m_TextButtonStateList[0].Color.w <= Color::ALPHA_MIN ) m_TextButtonStateList[0].Color.w = Color::ALPHA_MIN;
	for ( auto& a : m_TrashAreaStateList ) {
		a.Color.w = m_PutInKilnAnimCnt;
		if ( a.Color.w >= Color::ALPHA_MAX ) a.Color.w = Color::ALPHA_MAX;
	}
}

//---------------------------.
// �w����߂�.
//---------------------------.
void CPizzaEditor::SpatulaBack()
{
	const SSize& SpatulaSize = m_pSpatula->GetSpriteState().Disp;

	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_PutInKilnAnimCnt += m_DeltaTime;
	if ( m_PutInKilnAnimCnt >= 1.2f ) {
		// �A�j���[�V�����̏I��.
		m_AnimSpatulaState.Transform.Position.y = FWND_H + SpatulaSize.h;
		m_PutInKilnAnimCnt						= INIT_FLOAT;
		m_AnimSpatulaState.IsDisp				= false;
		m_SpatulaState.IsDisp					= false;
		m_IsPutInKiln							= false;
		m_IsPizzaSave							= true;
		return;
	}

	// �w�������o��.
	m_AnimSpatulaState.Transform.Position.y = FWND_H + SpatulaSize.h + m_PutInKilnAnimVector.y * ( 1.0f - CEasing::OutQuart( m_PutInKilnAnimCnt, 5.0f ) );

	// �e�L�X�g�{�^����\���ɂ��Ă���.
	if ( m_TextButtonStateList[0].Color.w >= Color::ALPHA_MAX ) return;
	for ( auto& t : m_TextButtonStateList ) {
		t.Color.w = m_PutInKilnAnimCnt;
		if ( t.Color.w >= Color::ALPHA_MAX ) t.Color.w = Color::ALPHA_MAX;
	}

	m_FontState.Color.w = m_PutInKilnAnimCnt;
	if ( m_FontState.Color.w >= Color::ALPHA_MAX ) {
		m_FontState.Color.w = Color::ALPHA_MAX;

		// SE�̍Đ�.
		CSoundManager::PlaySE( OPEN_SE );
	}
}

//---------------------------.
// �q�ɓ����.
//---------------------------.
void CPizzaEditor::PutInKiln()
{
	// �q�ɓ����.
	m_SpatulaState.Transform.Position.y -= 3.0f;
	if ( m_SpatulaState.Transform.Position.y <= 40.0f ) {
		// ���̃A�j���[�V�����̏���.
		m_SpatulaState.Transform.Position.y = 40.0f;
		m_PutInKilnAnimNo					= CookPizzaAnim;

		// SE�̍Đ�.
		CSoundManager::PlaySE( COOK_PIZZA_SE );
		return;
	};
}

//---------------------------.
// �s�U���Ă�.
//---------------------------.
void CPizzaEditor::CookPizza()
{
	// ��莞�ԑҋ@����.
	m_PutInKilnAnimCnt += m_DeltaTime;
	if ( m_PutInKilnAnimCnt > 5.0f ) {
		// ���n�̌����ڂ̕ύX.
		m_PizzaBaseState.AnimState.AnimNumber = 1;
		// �`�[�Y�̌����ڂ̕ύX.
		for ( auto& c : m_CheeseStateList ) {
			if ( c.AnimState.AnimNumber > 1 ) continue;
			c.AnimState.AnimNumber = 0;
		}

		// ��ނ̌����ڂ̕ύX.
		auto itr = m_IngredientStateList.begin();
		while ( itr != m_IngredientStateList.end() ) {
			// ���n�̏ォ�m�F.
			const float Distance = D3DXVec3Length( &itr->Transform.Position );
			if ( Distance >= 90.0f ) {
				// ���n�̊O�ɂ��邽�ߍ폜����.
				itr = m_IngredientStateList.erase( itr );
			}
			else{
				// �Ă��Ă�����.
				if ( itr->AnimState.AnimNumber >= static_cast<int>( itr->AnimState.AnimNumberMax / 2.0f ) ||
					 itr->AnimState.AnimNumber < -static_cast<int>( m_IngredientSauceState.AnimState.AnimNumberMax / 2.0f ) )
				{
					itr++;
					continue;
				}
				
				// ��ނ̃\�[�X�̌����ڂ̕ύX.
				if ( itr->AnimState.AnimNumber < 0 ) {
					itr->AnimState.AnimNumber -= static_cast<int>( m_IngredientSauceState.AnimState.AnimNumberMax / 2.0f );
					itr++;
					continue;
				}
				itr->AnimState.AnimNumber += static_cast<int>( itr->AnimState.AnimNumberMax / 2.0f );
				itr++;
			}
		}

		// ���̃A�j���[�V�����̏���.
		m_PutInKilnAnimCnt	= INIT_FLOAT;
		m_PutInKilnAnimNo	= RemoveFromKilnAnim;
		return;
	};
}

//---------------------------.
// �q����o��.
//---------------------------.
void CPizzaEditor::RemoveFromKiln()
{
	// �q����o��.
	m_SpatulaState.Transform.Position.y += 2.0f;
	if ( m_SpatulaState.Transform.Position.y >= FWND_H - 64.0f ) {
		// ���̃A�j���[�V�����̏���.
		m_SpatulaState.Transform.Position.y = FWND_H - 64.0f;
		m_PutInKilnAnimNo					= SpatulaBackAnim;
		m_AnimSpatulaState.IsDisp			= true;
		m_SpatulaState.IsDisp				= false;

		// �{�^���̏���������.
		m_TextButtonStateList[0].IsDisp					= true;
		m_TextButtonStateList[1].IsDisp					= true;
		m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
		m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
		m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
		m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
		m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
		m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

		// �e�L�X�g�̏���������.
		m_FontState.Text								= "�s�U��ۑ����܂����H";
		m_FontState.Transform.Position.x				= FWND_W / 2.0f;
		m_FontState.Transform.Position.y				= 130.0f;
		m_FontState.Color.w								= Color::ALPHA_MIN;

		// SE�̍Đ�.
		CSoundManager::PlaySE( SPATULA_SE );
		return;
	};
}

//---------------------------.
// �|�[�Y�Ɉړ�.
//---------------------------.
void CPizzaEditor::MovePause()
{
	if ( m_IsPizzaSave		) return;
	if ( m_IsPizzaLoad		) return;
	if ( m_IsPizzaDelete	) return;
	if ( m_IsPizzaDownload	) return;
	if ( m_IsPutInKiln		) return;

	// �|�[�Y���J��.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_START ) ) {
		CSoundManager::PlaySE( OPEN_SE );
		m_IsPause		= true;
		m_IsPauseAnim	= true;
	}
}

//---------------------------.
// �|�[�Y��ʂ��J��.
//---------------------------.
void CPizzaEditor::PauseOpen()
{
	if ( m_IsPauseAnim == false ) return;

	// �|�[�Y��ʂ��g�債�Ȃ���t�F�[�h�C��������.
	float Scale = m_PauseBackState.Transform.Scale.x;
	Scale += 3.0f * m_DeltaTime;
	if ( Scale >= STransform::NORMAL_SCALE ) {
		Scale			= STransform::NORMAL_SCALE;
		m_IsPauseAnim	= false;
	}

	// �ݒ�.
	m_PauseBackState.Transform.Scale	= { Scale, Scale, Scale };
	m_PauseBackState.Color.w			= Scale;
	m_PauseCursorState.Color.w			= Scale;
	for ( auto& t : m_PauseTextStateList ) t.Color.w = Scale;
}

//---------------------------.
// �|�[�Y��ʂ����.
//---------------------------.
void CPizzaEditor::PauseClose()
{
	if ( m_IsPauseAnim == false ) return;

	// �|�[�Y��ʂ��k�����Ȃ���t�F�[�h�A�E�g������.
	float Scale = m_PauseBackState.Transform.Scale.x;
	Scale -= 3.0f * m_DeltaTime;
	if ( Scale <= 0.0f ) {
		m_Choice		= 0;
		Scale			= 0.0f;
		m_IsPauseAnim	= false;
	}

	// �ݒ�.
	m_PauseBackState.Transform.Scale	= { Scale, Scale, Scale };
	m_PauseBackState.Color.w			= Scale;
	m_PauseCursorState.Color.w			= Scale;
	for ( auto& t : m_PauseTextStateList ) t.Color.w = Scale;
}

//---------------------------.
// �|�[�Y���̍X�V.
//---------------------------.
void CPizzaEditor::PauseUpdate()
{
	// �|�[�Y�����.
	if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_START ) ) {
		CSoundManager::PlaySE( CLOSE_SE );
		m_IsPause		= false;
		m_IsPauseAnim	= true;
	}

	// �I���ӏ��̈ړ�.
	else if ( CInput::IsKeyDown( "Up" ) ) {
		// �J�[�\�����グ��.
		m_Choice--;
		if ( m_Choice < CHOICE_STARTCHOICE ) m_Choice = CHOICE_ENDCHOICE;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}
	else if ( CInput::IsKeyDown( "Down" ) ) {
		// �J�[�\����������.
		m_Choice++;
		if ( m_Choice > CHOICE_ENDCHOICE ) m_Choice = CHOICE_STARTCHOICE;

		// SE�̍Đ�.
		CSoundManager::PlaySE( CURSOR_MOVE_SE );
	}

	// �I�����̌���.
	else if ( CInput::IsKeyDown( "Decision" ) ) {
		switch ( m_Choice ) {
		// �u�s�U���ɖ߂�v��I�������Ƃ�.
		case CHOICE_STARTCHOICE:
			CSoundManager::PlaySE( CLOSE_SE );
			m_IsPause		= false;
			m_IsPauseAnim	= true;
			break;
		// �u�s�U������������v��I�������Ƃ�.
		case CHOICE_CENTERCHOICE:
			m_SauceStateList.clear();
			m_CheeseStateList.clear();
			m_IngredientStateList.clear();
			m_AreaPageNo									= INIT_INT;
			m_GrabIngredientNo								= NO_GRAB_NO;
			m_GrabSauceNo									= NO_GRAB_NO;
			m_IsDispSauce									= true;
			m_PizzaBaseState.AnimState.AnimNumber			= 0;
			m_TextButtonStateList[0].AnimState.AnimNumber	= PushDecisionButton;
			m_TextButtonStateList[1].IsDisp					= false;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f;
			for ( auto& [Sauce, Cheese] : m_SauceList ) {
				Sauce	= NO_SAUCE_NO;
				Cheese	= NO_SAUCE_NO;
			}

			m_IsPause								= false;
			m_IsPauseAnim							= true;

			// SE�̍Đ�.
			CSoundManager::PlaySE( TRASH_SE );
			break;
		// �u�s�U�����I������v��I�������Ƃ�.
		case CHOICE_ENDCHOICE:
			CSoundManager::PlaySE( "Decision" );
			CSceneManager::SceneChange( ESceneList::Title, EFadeType::MaskFade );
			break;
		}
	}

	// �J�[�\���̈ʒu�̐ݒ�.
	m_PauseCursorState.Transform.Position.y = m_PauseTextStateList[m_Choice + 1].Transform.Position.y;

	// �����ڂ̕ύX.
	switch ( m_Choice ) {
	case 0:
		m_PauseTextStateList[1].AnimState.AnimNumber = 2;
		m_PauseTextStateList[2].AnimState.AnimNumber = 3;
		m_PauseTextStateList[3].AnimState.AnimNumber = 5;
		break;
	case 1:
		m_PauseTextStateList[1].AnimState.AnimNumber = 1;
		m_PauseTextStateList[2].AnimState.AnimNumber = 4;
		m_PauseTextStateList[3].AnimState.AnimNumber = 5;
		break;
	case 2:
		m_PauseTextStateList[1].AnimState.AnimNumber = 1;
		m_PauseTextStateList[2].AnimState.AnimNumber = 3;
		m_PauseTextStateList[3].AnimState.AnimNumber = 6;
		break;
	default:
		break;
	};
}

//---------------------------.
// ��������̍X�V.
//---------------------------.
void CPizzaEditor::TextUpdate()
{
	// �\�[�X��͂�/�h��.
	if ( m_IsDispSauce ) {
		if ( m_GrabSauceNo		== NO_GRAB_NO ) m_TextStateList[4].AnimState.AnimNumber = GrabSauceText;
		else									m_TextStateList[4].AnimState.AnimNumber = GrabEndSauceText;
	}
	// ��ނ�͂�/����.
	else {
		if ( m_GrabIngredientNo	== NO_GRAB_NO ) m_TextStateList[4].AnimState.AnimNumber = GrabText;
		else									m_TextStateList[4].AnimState.AnimNumber = GrabEndText;
	}

	// ��ނ�͂�ł��邩.
	if ( m_GrabIngredientNo == NO_GRAB_NO ) {
		m_TextStateList[5].AnimState.AnimNumber = Gray_CopyText;
		m_TextStateList[6].AnimState.AnimNumber = Gray_ClockwiseText;
		m_TextStateList[7].AnimState.AnimNumber = Gray_AnticlockwiseText;
	}
	else {
		m_TextStateList[5].AnimState.AnimNumber = CopyText;
		m_TextStateList[6].AnimState.AnimNumber = ClockwiseText;
		m_TextStateList[7].AnimState.AnimNumber = AnticlockwiseText;
	}
}

//---------------------------.
// �Z�[�u�G���A�̍X�V.
//---------------------------.
void CPizzaEditor::SaveAreaUpdate()
{
	if ( m_IsPizzaSave						) return;
	if ( m_IsPizzaLoad						) return;
	if ( m_IsPizzaDelete					) return;
	if ( m_IsPizzaDownload					) return;
	if ( m_IsPutInKiln						) return;
	if ( m_IsBButtonAction					) return;
	if ( m_GrabSauceNo		!= NO_GRAB_NO	) return;
	if ( m_GrabIngredientNo != NO_GRAB_NO	) return;

	// �J�[�\���̈ʒu���擾.
	const D3DXPOSITION3& CursorPos = m_CursorState.Transform.Position;

	// �ۑ��G���A�̃{�^���̍X�V.
	for ( auto& s : m_SaveAreaStateList ) {
		s.AnimState.AnimNumber = 0;
		if ( s.No == "" ) {
			s.AnimState.AnimNumber = 2;
			continue;
		}

		// �G���A�̈ʒu���擾.
		const D3DXPOSITION3& Pos = s.Transform.Position;

		// �s�U�����{�^���̏ォ.
		if ( Pos.x + 135.0f + 7.0f <= CursorPos.x && CursorPos.x <= Pos.x + 175.0f	+ 25.0f &&
			 Pos.y + 1.0f	+ 7.0f <= CursorPos.y && CursorPos.y <= Pos.y + 41.0f	+ 25.0f )
		{
			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// B�{�^������������.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) continue;
			m_IsBButtonAction	= true;
			m_IsPizzaLoad		= true;

			// �S�~���G���A�̐ݒ�.
			for ( auto& a : m_TrashAreaStateList ) {
				a.Color.w				= Color::ALPHA_MIN;
				a.AnimState.AnimNumber	= 1;
			}

			// �{�^���̏���������.
			m_ButtonAnimCnt									= INIT_FLOAT;
			m_TextButtonStateList[0].IsDisp					= true;
			m_TextButtonStateList[1].IsDisp					= true;
			m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
			m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
			m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

			// �e�L�X�g�̏���������.
			m_FontState.Text								= "�s�U��ǂݍ��݂܂����H";
			m_FontState.Transform.Position.x				= FWND_W / 2.0f;
			m_FontState.Transform.Position.y				= 130.0f;
			m_FontState.Color.w								= Color::ALPHA_MIN;

			// �Z�[�u�G���A�̏����擾.
			m_pSelectSaveAreaState = &s;

			// SE�̍Đ�.
			CSoundManager::PlaySE( OPEN_SE );
			break;
		}

		// �s�U���폜�{�^���̏ォ.
		else if (	Pos.x + 135.0f	+ 7.0f <= CursorPos.x && CursorPos.x <= Pos.x + 175.0f	+ 25.0f &&
					Pos.y + 42.0f	+ 7.0f <= CursorPos.y && CursorPos.y <= Pos.y + 82.0f	+ 25.0f)
		{
			if ( m_SavePizzaNum <= 1 ) break;

			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// B�{�^������������.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) continue;
			m_IsBButtonAction	= true;
			m_IsPizzaDelete		= true;

			// �S�~���G���A�̐ݒ�.
			for ( auto& a : m_TrashAreaStateList ) {
				a.Color.w				= Color::ALPHA_MIN;
				a.AnimState.AnimNumber	= 1;
			}

			// �{�^���̏���������.
			m_ButtonAnimCnt									= INIT_FLOAT;
			m_TextButtonStateList[0].IsDisp					= true;
			m_TextButtonStateList[1].IsDisp					= true;
			m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
			m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
			m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

			// �e�L�X�g�̏���������.
			m_FontState.Text								= "�s�U���폜���܂����H";
			m_FontState.Transform.Position.x				= FWND_W / 2.0f;
			m_FontState.Transform.Position.y				= 130.0f;
			m_FontState.Color.w								= Color::ALPHA_MIN;

			// �Z�[�u�G���A�̏����擾.
			m_pSelectSaveAreaState = &s;

			// SE�̍Đ�.
			CSoundManager::PlaySE( OPEN_SE );
			break;
		}

		// �s�U���_�E�����[�h�{�^���̏ォ.
		else if (	Pos.x + 135.0f	+ 7.0f <= CursorPos.x && CursorPos.x <= Pos.x + 175.0f	+ 25.0f &&
					Pos.y + 83.0f	+ 7.0f <= CursorPos.y && CursorPos.y <= Pos.y + 123.0f	+ 25.0f )
		{
			// �J�[�\���̕ύX.
			m_CursorState.AnimState.AnimNumber = SelectCursor;

			// B�{�^������������.
			if ( CXInput::IsKeyDown( XINPUT_GAMEPAD_B ) == false ) continue;
			m_IsBButtonAction	= true;
			m_IsPizzaDownload	= true;

			// �S�~���G���A�̐ݒ�.
			for ( auto& a : m_TrashAreaStateList ) {
				a.Color.w				= Color::ALPHA_MIN;
				a.AnimState.AnimNumber	= 1;
			}

			// �{�^���̏���������.
			m_ButtonAnimCnt									= INIT_FLOAT;
			m_TextButtonStateList[0].IsDisp					= true;
			m_TextButtonStateList[1].IsDisp					= true;
			m_TextButtonStateList[0].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[1].Color.w				= Color::ALPHA_MIN;
			m_TextButtonStateList[0].AnimState.AnimNumber	= YesButton;
			m_TextButtonStateList[1].AnimState.AnimNumber	= NoButton;
			m_TextButtonStateList[0].Transform.Position.x	= FWND_W / 2.0f - 140.0f;
			m_TextButtonStateList[1].Transform.Position.x	= FWND_W / 2.0f + 140.0f;

			// �e�L�X�g�̏���������.
			m_FontState.Text								= "�s�U���_�E�����[�h���܂����H";
			m_FontState.Transform.Position.x				= FWND_W / 2.0f;
			m_FontState.Transform.Position.y				= 130.0f;
			m_FontState.Color.w								= Color::ALPHA_MIN;

			// �Z�[�u�G���A�̏����擾.
			m_pSelectSaveAreaState = &s;

			// SE�̍Đ�.
			CSoundManager::PlaySE( OPEN_SE );
			break;
		}
	}

	// �s�U��������Ȃ��Ƃ���ԏ��߂̌����ڂ�ύX����.
	if ( m_SavePizzaNum <= 1 ) {
		m_SaveAreaStateList[0].AnimState.AnimNumber = 1;
	}
}

//---------------------------.
// �Z�[�u�G���A�̃{�^���̍X�V.
//---------------------------.
void CPizzaEditor::SaveAreaButtonUpdate()
{
	if ( m_IsPizzaLoad		== false &&
		 m_IsPizzaDelete	== false &&
		 m_IsPizzaDownload	== false ) return;
	if ( m_TrashAreaStateList[0].Color.w >= Color::ALPHA_MAX ) return;

	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_ButtonAnimCnt += m_DeltaTime;

	// �폜�G���A��\�����Ă���.
	for ( auto& a : m_TrashAreaStateList ) {
		a.Color.w = m_ButtonAnimCnt;
		if ( a.Color.w >= Color::ALPHA_MAX ) a.Color.w = Color::ALPHA_MAX;
	}

	// �e�L�X�g�{�^����\�����Ă���.
	for ( auto& t : m_TextButtonStateList ) {
		t.Color.w = m_ButtonAnimCnt;
		if ( t.Color.w >= Color::ALPHA_MAX ) t.Color.w = Color::ALPHA_MAX;
	}

	// �e�L�X�g��\�����Ă���.
	m_FontState.Color.w = m_ButtonAnimCnt;
	if ( m_FontState.Color.w >= Color::ALPHA_MAX ) m_FontState.Color.w = Color::ALPHA_MAX;
}

//---------------------------.
// �ۑ����Ă���s�U�̃T���v���̏�����.
//---------------------------.
void CPizzaEditor::SavePizzaSampleInit()
{
	// �s�U�f�[�^�̊J�n�ʒu�̎擾.
	const int DataStartNo = 6 * m_SavePizzaPageNo;

	// �\�����̎擾.
	int RenderNum = m_SavePizzaNum - DataStartNo;
	if ( RenderNum > 6 ) RenderNum = 6;

	// �T���v���̕\�����̐ݒ�.
	m_SampleSauceStateList.resize( RenderNum );
	m_SampleCheeseStateList.resize( RenderNum );
	m_SampleIngredientStateList.resize( RenderNum );

	// �s�U���̏�����.
	for ( auto& a : m_SaveAreaStateList ) a.No = "";

	// �T���v���̏�����.
	for ( int i = 0; i < RenderNum; ++i ) {
		// �s�U�̏��̎擾.
		m_SaveAreaStateList[i].No	= std::to_string( DataStartNo + i );
		json PizzaData				= m_SavePizzaData[DataStartNo + i].second;

		// �s�U�̏�����.
		const int SSize = static_cast<int>( PizzaData["Sauce"].size()		);
		const int CSize = static_cast<int>( PizzaData["Cheese"].size()		);
		const int ISize = static_cast<int>( PizzaData["Ingredients"].size() );
		m_SampleSauceStateList[i].resize(		SSize, m_SauceStateBase );
		m_SampleCheeseStateList[i].resize(		CSize, m_SauceStateBase );
		m_SampleIngredientStateList[i].resize(	ISize, m_IngredientStateBase );

		// �\�[�X�̐ݒ�.
		for ( int SauceNo = 0; SauceNo < SSize; ++SauceNo ) {
			json SauceData = PizzaData["Sauce"][SauceNo];
			m_SampleSauceStateList[i][SauceNo].AnimState.AnimNumber			= SauceData["Type"];
			m_SampleSauceStateList[i][SauceNo].pMaskTexture					= m_pSauceMaskList[SauceData["MaskType"]][SauceData["MaskNo"]];
			m_SampleSauceStateList[i][SauceNo].Transform.pParent			= &m_SamplePizzaBaseState.Transform;
		}

		// �`�[�Y�̐ݒ�.
		for ( int CheeseNo = 0; CheeseNo < CSize; ++CheeseNo ) {
			json CheeseData = PizzaData["Cheese"][CheeseNo];
			m_SampleCheeseStateList[i][CheeseNo].AnimState.AnimNumber		= CheeseData["Type"];
			m_SampleCheeseStateList[i][CheeseNo].pMaskTexture				= m_pSauceMaskList[CheeseData["MaskType"]][CheeseData["MaskNo"]];
			m_SampleCheeseStateList[i][CheeseNo].Transform.pParent			= &m_SamplePizzaBaseState.Transform;
		}

		// ��ނ̐ݒ�.
		for ( int IngredientNo = 0; IngredientNo < ISize; ++IngredientNo ) {
			json IngredientData = PizzaData["Ingredients"][IngredientNo];
			m_SampleIngredientStateList[i][IngredientNo].AnimState.AnimNumber		= IngredientData["Type"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.Position.x		= IngredientData["Position"]["x"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.Position.y		= IngredientData["Position"]["y"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.Rotation.z		= IngredientData["Rotation"];
			m_SampleIngredientStateList[i][IngredientNo].Transform.pParent			= &m_SamplePizzaBaseState.Transform;
		}
	}
}

//---------------------------.
// �ۑ����Ă���s�U�̃T���v���̕`��.
//---------------------------.
void CPizzaEditor::SavePizzaSampleRender()
{
	// �\��������̎擾.
	const int RenderNum = static_cast<int>( m_SampleSauceStateList.size() );

	// �T���v���̕\��.
	for ( int i = 0; i < RenderNum; ++i ) {
		// �s�U���n�̕`��.
		m_SamplePizzaBaseState.Transform.Position.x = 996.0f + 177.0f * ( i % 2 );
		m_SamplePizzaBaseState.Transform.Position.y = 68.0f	 + 125.0f * ( i / 2 );
		m_pPizzaBase->RenderUI( &m_SamplePizzaBaseState );

		// ��ނ̕`��.
		for ( auto& s : m_SampleSauceStateList[i]	) m_pSauce->RenderUI( &s );
		for ( auto& c : m_SampleCheeseStateList[i]	) m_pSauce->RenderUI( &c );
		for ( auto& i : m_SampleIngredientStateList[i] ) {
			if ( i.AnimState.AnimNumber < 0 ) {
				m_IngredientSauceState.Transform			= i.Transform;
				m_IngredientSauceState.AnimState.AnimNumber = std::abs( i.AnimState.AnimNumber ) - 1;
				m_pIngredientSauce->RenderUI( &m_IngredientSauceState );
				continue;
			}
			m_pIngredient->RenderUI( &i );
		}
	}
}

//---------------------------.
// �h���b�N&�h���b�v�̍X�V.
//---------------------------.
void CPizzaEditor::DragAndDropUpdate()
{
	if ( m_IsPizzaSave		== false &&
		 m_IsPizzaLoad		== false &&
		 m_IsPizzaDelete	== false &&
		 m_IsPizzaDownload	== false &&
		 m_IsPutInKiln		== false &&
		 m_IsPause			== false )
	{
		// �h���b�N&�h���b�v��L���ɂ���.
		CDragAndDrop::Open();
	}
	else {
		// �h���b�N&�h���b�v�𖳌��ɂ���.
		CDragAndDrop::Close();
	}

	// �h���b�N&�h���b�v���ꂽ��.
	if ( CDragAndDrop::GetIsDrop() == false ) return;

	// �h���b�N&�h���b�v�����t�@�C�����X�g���擾.
	std::vector<std::string> FileList = CDragAndDrop::GetFilePath();
	for ( auto& f : FileList ) {
		// json�t�@�C����.
		const std::string Extension = f.substr( f.length() - 4 );
		if ( Extension != "json" && Extension != "JSON" ) continue;

		// �s�U�f�[�^��.
		json j = FileManager::JsonLoad( f );
		if ( j.find( "Ingredients" ) == j.end() ) continue;

		// �t�@�C�������擾.
		std::string FileName = "";
		std::string::size_type Spos = f.rfind( "\\" );
		if ( Spos != std::string::npos ) {
			std::string::size_type Epos = f.find( ".", Spos + 1 );
			if ( Epos != std::string::npos ) {
				FileName = f.substr( Spos + 1, Epos - Spos - 1 );
			}
		}

		// ���O�������t�H���_���������m�F.
		const std::vector<std::string>& PizzaList = CPizzaResource::GetPizzaList();
		bool				IsNameOK	= true;
		int					NameNo		= 0;
		const std::string	OldName		= FileName;
		do {
			IsNameOK = true;
			for ( auto& p : PizzaList ) {
				// �����̖��O�̃t�@�C�������邽�ߌ��ɂ�������t����.
				if ( p == FileName ) {
					NameNo++;
					FileName = OldName + "(" + std::to_string( NameNo ) + ")";

					// ��������t������ԂŖ��O�����Ԃ�Ȃ���������x�m�F����.
					IsNameOK = false;
					break;
				}
			}
		} while ( !IsNameOK );

		// �ړ���̃t�@�C���p�X�̍쐬.
		const std::string NewFilePath = SAVE_FILE_PATH + FileName + ".json";

		// �t�@�C�����ړ�������.
		std::filesystem::rename( f, NewFilePath );

		// ���\�[�X�ɒǉ�.
		CPizzaResource::PushPizzaData( FileName, j );
		m_SavePizzaData.emplace_back( std::make_pair( FileName, j ) );
		m_SavePizzaNum++;
	}

	// �h���b�N&�h���b�v�I��.
	CDragAndDrop::End();

	// �ۑ����Ă���s�U�̃T���v���̍X�V.
	SavePizzaSampleInit();
}
