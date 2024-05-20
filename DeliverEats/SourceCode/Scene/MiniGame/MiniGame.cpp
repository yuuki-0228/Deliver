#include "MiniGame.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Utility\Input\Input.h"
#include "..\..\Utility\Random\Random.h"
#include "..\..\Utility\FileManager\FileManager.h"

namespace{
	constexpr int	PAUSE_TEXT_NUM				= 4;											// �|�[�Y�e�L�X�g�̕\����.
	constexpr int	TEXT_NUM					= 8;											// �e�L�X�g�̕\����.
	constexpr int	TEXT_MAX					= 7;											// �e�L�X�g�̏��.
	constexpr float	TEXT_GAP					= 20.0f;										// �e�L�X�g�̌���.
	constexpr float	TEXT_JUMP_Y					= 20.0f;										// �e�L�X�g�̃W�����v�����鍂��.
	constexpr float	TEXT_ANIM_SPEED				= 5.0f;											// �e�L�X�g�̃A�j���[�V�����̑��x.
	constexpr int	CLOUD_SPOWN_TIME			= 50;											// �_���������Ă�������.
	constexpr float	CLOUD_SPOWN_POS_X			= 20.0f;										// �_����������X���W.
	constexpr float	CLOUD_DELETE_POS_X			= FWND_W - CLOUD_SPOWN_POS_X;					// �_���폜����X���W.
	constexpr float	CLOUD_SPOWN_POS_Y_MIN		= 50.0f;										// �_����������Y���W�̍ŏ��l.
	constexpr float	CLOUD_SPOWN_POS_Y_MAX		= 620.0f;										// �_����������Y���W�̍ő�l.
	constexpr float	CLOUD_START_MOVE_SPPED		= 5.0f;											// �_�̊J�n���̈ړ����x.
	constexpr float	CLOUD_CENTER_MOVE_SPPED		= 15.0f;										// �_�̒��Ԃ̈ړ����x.
	constexpr float	CLOUD_END_MOVE_SPPED		= 18.0f;										// �_�̍ŏI�̈ړ����x.
	constexpr float	CLOUD_RADIUS				= 8.0f;											// �_�̓����蔻��̔��a.
	constexpr float	CLOUD_AVERT_RADIUS			= 23.0f;										// �_�̃M���M��������������ׂ铖���蔻��̔��a.
	constexpr float GRAVITY_POWER				= 0.4f;											// �d�͂̋���.
	constexpr float	PLAYER_POS_X				= 1000.0f;										// �v���C���[��X���W.
	constexpr float	PLAYER_FIRE_POS_X			= 37.0f;										// �v���C���[�̉���X���W.
	constexpr float	PLAYER_FIRE_POS_Y			= 11.0f;										// �v���C���[�̉���Y���W.
	constexpr float	PLAYER_RADIUS				= 23.0f;										// �v���C���[�̓����蔻��̔��a.
	constexpr float	PLAYER_AVERT_RADIUS			= 38.0f;										// �v���C���[�̃M���M��������������ׂ铖���蔻��̔��a.
	constexpr float	PLAYER_ROT_SPEED			= 6.0f;											// �v���C���[�̉_�ɓ����������̃A�j���[�V�����̉�]���x.
	constexpr int	COOL_TIME					= 10;											// �����蔻��̃N�[���^�C��.
	constexpr float	CEILING_POS_Y				= 100.0f;										// �V���Y���W.
	constexpr float	GROUND_POS_Y				= 550.0f;										// �n�ʂ�Y���W.
	constexpr float	FLIGHT_ANIM_SIZE			= 20.0f;										// ��s�A�j���[�V�����p�̃T�C�Y.
	constexpr int	AVERT_ADD_SCORE				= 15;											// �M���M������������ɒǉ�����X�R�A.
	constexpr int	CHOICE_STARTCHOICE			= 0;											// �ŏ��̑I����.
	constexpr int	CHOICE_CENTERCHOICE			= 1;											// �^�񒆂̑I����.
	constexpr int	CHOICE_ENDCHOICE			= 2;											// �Ō�̑I����.
	constexpr char	HIT_SE[]					= "MiniGameHit";								// �_�ɓ�����������SE.
	constexpr char	AVERT_SE[]					= "MiniGameAvert";								// �M���M�������������SE.
	constexpr char	SCORE_1000M_SE[]			= "MiniGameScore1000";							// 1000m���Ƃɖ��SE.
	constexpr char	SCORE_5000M_SE[]			= "MiniGameScore5000";							// 5000m���Ƃɖ��SE.
	constexpr char	SCORE_10000M_SE[]			= "MiniGameScore10000";							// 10000m���Ƃɖ��SE.
	constexpr char	BESTSCORE_UPDATE_SE[]		= "MiniGameBestScoreUpdate";					// �x�X�g�X�R�A���X�V�����u�Ԃɖ�SE.
	constexpr char	END_BESTSCORE_UPDATE_SE[]	= "MiniGameBestScoreUpdateEnd";					// �Ԃ��������Ƀx�X�g�X�R�A���X�V���Ă������ɖ�SE.
	constexpr char	DATA_FILE_PATH[]			= "Data\\Parameter\\MiniGame\\MiniGame.bin";	// �X�R�A�f�[�^�̃t�@�C���p�X.
	constexpr char	SCORE_LIST_FILE_PATH[]		= "Data\\Parameter\\MiniGame\\PlayData.csv";	// �X�R�A���X�g�f�[�^�̃t�@�C���p�X.
	constexpr char	OPEN_SE[]					= "Open";										// �|�[�Y���J������SE.
	constexpr char	CLOSE_SE[]					= "Close";										// �|�[�Y����鎞��SE.
	constexpr char	CURSOR_MOVE_SE[]			= "CursorMove";									// �J�[�\���ړ�����SE.
	constexpr char	TRASH_SE[]					= "Trash";										// �x�X�g�X�R�A�̍폜����SE.

	// �X�R�ANo.
	enum enScoreNo : unsigned char {
		BestScore,		// �x�X�g�X�R�A.
		PreviousScore,	// �O��̃X�R�A.

		Score_Max
	} typedef ScoreNo;

	// ��������̍s��No
	enum enAutoPlayAction : unsigned char {
		MoveUp,			// �㏸.
		MoveUpSmall,	// �����㏸.
		MoveWait,		// ���̏�őҋ@.
		MoveDownSmall,	// �������~.
		MoveDown		// ���~.
	} typedef EAutoPlayAction;
}

CMiniGame::CMiniGame()
	: m_pText				( nullptr )
	, m_pPlayer				( nullptr )
	, m_pPlayerFire			( nullptr )
	, m_pPlayerIcon			( nullptr )
	, m_pCloud				( nullptr )
	, m_pLRTrigger			( nullptr )
	, m_pPauseButton		( nullptr )
	, m_pPauseBack			( nullptr )
	, m_pPauseText			( nullptr )
	, m_pPauseCursor		( nullptr )
	, m_pFont				( nullptr )
	, m_FontStateList		( Score_Max )
	, m_TextStateList		( TEXT_NUM )
	, m_CloudStateList		()
	, m_PauseTextStateList	( PAUSE_TEXT_NUM )
	, m_PauseTextStateBase	()
	, m_CloudStateBase		()
	, m_PlayerState			()
	, m_PlayerFireState		()
	, m_PlayerIconState		()
	, m_LRTriggerState		()
	, m_PauseButtonState	()
	, m_PauseBackState		()
	, m_PauseCursorState	()
	, m_SortCloudList		()
	, m_PlayData			()
	, m_DeltaTime			( INIT_FLOAT )
	, m_AnimTimeCnt			( INIT_FLOAT )
	, m_Power				( INIT_FLOAT )
	, m_CloudMoveSpeed		( CLOUD_START_MOVE_SPPED )
	, m_Score				( INIT_FLOAT )
	, m_PlayerFireCnt		( INIT_FLOAT )
	, m_AutoPlay			( INIT_FLOAT )
	, m_PreviousScore		( INIT_INT )
	, m_BestScore			( INIT_INT )
	, m_CloudTimeCnt		( INIT_INT )
	, m_TextAnimNo			( INIT_INT )
	, m_CoolTime			( INIT_INT )
	, m_Choice				( INIT_INT )
	, m_AutoPlayAction		( MoveWait )
	, m_IsPlay				( false )
	, m_IsCloudHit			( false )
	, m_LeftRotation		( false )
	, m_IsPause				( false )
	, m_IsPauseAnim			( false )
{
}

CMiniGame::~CMiniGame()
{
	// �X�R�A�̕ۑ�.
	int Data[3];
	Data[0] = m_BestScore;
	Data[1] = CRandom::GetRand( 0, 360 );
	Data[2] = static_cast<int>( m_BestScore * 2.319845f - 194350.726071f * sinf( static_cast<float>( Data[1] ) ) );
	FileManager::BinarySave( DATA_FILE_PATH, Data );

	// �w�i�̐F�̏�����.
	CDirectX11::InitBackColor();

	// �v���C�f�[�^�̕ۑ�.
	std::string ScoreText = "�v���C��,=COUNT(A2:A100000),�ő�X�R�A,=MAX(A2:A100000),����,=AVERAGE(A2:A100000)";
	for ( auto& d : m_PlayData ) ScoreText += "\n" + std::to_string( d );
	FileManager::TextSave( SCORE_LIST_FILE_PATH, ScoreText );
}

//---------------------------.
// ������.
//---------------------------.
bool CMiniGame::Init()
{
	// �摜�̎擾.
	m_pText			= CSpriteResource::GetSprite( "LoadText"	);
	m_pPlayer		= CSpriteResource::GetSprite( "LoadPlayer",			&m_PlayerState			);
	m_pPlayerFire	= CSpriteResource::GetSprite( "LoadPlayerFire",		&m_PlayerFireState		);
	m_pPlayerIcon	= CSpriteResource::GetSprite( "LoadPlayerIcon",		&m_PlayerIconState		);
	m_pCloud		= CSpriteResource::GetSprite( "LoadCloud",			&m_CloudStateBase		);
	m_pLRTrigger	= CSpriteResource::GetSprite( "LoadLRTrigger",		&m_LRTriggerState		);
	m_pPauseButton	= CSpriteResource::GetSprite( "LoadPauseButton",	&m_PauseButtonState		);
	m_pPauseBack	= CSpriteResource::GetSprite( "back",				&m_PauseBackState		);
	m_pPauseText	= CSpriteResource::GetSprite( "LoadPauseText",		&m_PauseTextStateBase	);
	m_pPauseCursor	= CSpriteResource::GetSprite( "Yubi",				&m_PauseCursorState		);
	m_pFont			= CFontResource::GetFont(	  "NasuM",				&m_FontStateList		);

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

	// �X�R�A�̎擾.
	int Out[3];
	FileManager::BinaryLoad( DATA_FILE_PATH, Out );
	const int c = static_cast<int>( Out[0] * 2.319845f - 194350.726071f * sinf( static_cast<float>( Out[1] ) ) );
	if ( c == Out[2] ) m_BestScore = Out[0];

	// �t�H���g�̐ݒ�.
	m_FontStateList[BestScore].TextAlign				= ETextAlign::Left;
	m_FontStateList[BestScore].Transform.Position.x		= TEXT_GAP;
	m_FontStateList[BestScore].Transform.Position.y		= TEXT_GAP;
	m_FontStateList[BestScore].IsOutLine				= true;
	m_FontStateList[BestScore].OutLineColor				= Color4::Black;
	m_FontStateList[BestScore].OutLineSize				= 2.5f;
	m_FontStateList[PreviousScore].TextAlign			= ETextAlign::Right;
	m_FontStateList[PreviousScore].Transform.Position.x = FWND_W - TEXT_GAP;
	m_FontStateList[PreviousScore].Transform.Position.y = TEXT_GAP;
	m_FontStateList[PreviousScore].IsOutLine			= true;
	m_FontStateList[PreviousScore].OutLineColor			= Color4::Black;
	m_FontStateList[PreviousScore].OutLineSize			= 2.5f;

	// �����̐ݒ�.
	const SSize&				TextSize	= m_pText->GetSpriteState().Disp;
	const SSpriteRenderState&	TextState	= m_pText->GetRenderState();
	for ( int i = 0; i < TEXT_NUM; ++i ) {
		const int No = 11 + ( i > TEXT_MAX ? TEXT_MAX : i );
		m_TextStateList[i] = TextState;
		m_TextStateList[i].AnimState.IsSetAnimNumber	= true;
		m_TextStateList[i].AnimState.AnimNumber			= No;
		m_TextStateList[i].Transform.Position.x			= FWND_W - TextSize.w * ( TEXT_NUM - i ) - TEXT_GAP;
		m_TextStateList[i].Transform.Position.y			= FWND_H - TextSize.h - TEXT_GAP;
	}

	// ��������̃e�L�X�g�̈ʒu�̐ݒ�.
	const SSize LRTriggerSize = m_pLRTrigger->GetSpriteState().Disp;
	m_LRTriggerState.Transform.Position.x			= TEXT_GAP;
	m_LRTriggerState.Transform.Position.y			= FWND_H - TEXT_GAP;
	m_PauseButtonState.Transform.Position.x			= m_LRTriggerState.Transform.Position.x + LRTriggerSize.w + 40.0f;
	m_PauseButtonState.Transform.Position.y			= FWND_H - TEXT_GAP;
	m_PauseButtonState.AnimState.IsSetAnimNumber	= true;

	// �_�̐ݒ�.
	const SSize& CloudSize = m_pCloud->GetSpriteState().Disp;
	m_CloudStateBase.AnimState.IsSetAnimNumber = true;
	m_CloudStateBase.RenderArea = {
		CLOUD_SPOWN_POS_X + CloudSize.w / 2.0f,
		0.0f,
		CLOUD_DELETE_POS_X - CLOUD_SPOWN_POS_X - CloudSize.w,
		FWND_H
	};

	// �v���C���[�̐ݒ�.
	m_PlayerState.Transform.Position.x		= PLAYER_POS_X;
	m_PlayerState.Transform.Position.y		= GROUND_POS_Y;
	m_PlayerFireState.Transform.AddParent( &m_PlayerState.Transform );
	m_PlayerFireState.Transform.Position.x	= PLAYER_FIRE_POS_X;
	m_PlayerFireState.Transform.Position.y	= PLAYER_FIRE_POS_Y;
	
	// �v���C���[�̃A�C�R���̐ݒ�.
	m_PlayerIconState.Transform.Position.x = TEXT_GAP;
	m_PlayerIconState.Transform.Position.y = m_PlayerState.Transform.Position.y;

	// BGM���̐ݒ�.
	m_BGMName = "MiniGame";

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// �w�i�̐F�̐ݒ�.
	CDirectX11::SetBackColor( Color4::Black );

	// �v���C�f�[�^�̎擾.
	std::vector<std::string> Data = FileManager::TextLoad( SCORE_LIST_FILE_PATH );
	const int Size = static_cast<int>( Data.size() ) - 6;
	m_PlayData.resize( Size );
	for ( int i = 0; i < Size; ++i ) m_PlayData[i] = std::stoi( Data[i + 6] );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CMiniGame::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	if ( m_IsPause ) {
		PauseOpen();				// �|�[�Y��ʂ��J��.
		PauseUpdate();				// �|�[�Y���̍X�V.
	}
	else {
		CloudSpown();				// �_�̏���.
		CloudMove();				// �_�̈ړ�.
		CloudDelete();				// �g�p���Ă��Ȃ��_�̍폜.
		TextAnimation();			// �e�L�X�g�̃A�j���[�V����.
		PlayerJump();				// �v���C���[�̃W�����v.
		PlayerFall();				// �v���C���[�̗���.
		GroundCollision();			// �n�ʂ̓����蔻��.
		CeilingCollision();			// �V��̓����蔻��.
		CloudCollision();			// �_�̓����蔻��.
		PlayerCloudHitAnimation();	// �v���C���[�̉_�ɓ����������̃A�j���[�V����.
		PlayerPositionUpdate();		// �v���C���[�̍��W�̍X�V.
		PlayerFireUpdate();			// �v���C���[�̉��̍X�V.
		ScoreUpdate();				// �X�R�A�̍X�V.
		TextUpdate();				// �e�L�X�g�̍X�V.
		PauseButtonUpdate();		// �|�[�Y�̃{�^���̍X�V.
		SoundUpdate();				// �T�E���h�̍X�V.
		CoolTimeUpdate();			// �N�[���^�C���̍X�V.
		AutoPlayUpdate();			// ��������̍X�V.

		PauseClose();				// �|�[�Y��ʂ����.
		MovePause();				// �|�[�Y�Ɉړ�.
	}
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CMiniGame::SpriteUIRender()
{
	for ( auto& t : m_TextStateList		) m_pText->RenderUI( &t );
	m_pLRTrigger->RenderUI( &m_LRTriggerState );
	m_pPauseButton->RenderUI( &m_PauseButtonState );
	m_pPlayer->RenderUI( &m_PlayerState );
	m_pPlayerFire->RenderUI( &m_PlayerFireState );
	m_pPlayerIcon->RenderUI( &m_PlayerIconState );
	for ( auto& c : m_CloudStateList	) m_pCloud->RenderUI( &c );
	for ( auto& t : m_FontStateList		) m_pFont->RenderUI( &t );

	// �|�[�Y�̕`��.
	if ( m_IsPauseAnim || m_IsPause ) {
		m_pPauseBack->RenderUI( &m_PauseBackState );
		for ( auto& t : m_PauseTextStateList ) m_pPauseText->RenderUI( &t );
		m_pPauseCursor->RenderUI( &m_PauseCursorState );
	}
}

//---------------------------.
// �_�̏���.
//---------------------------.
void CMiniGame::CloudSpown()
{
	// �_���������邩.
	m_CloudTimeCnt++;
	if ( m_CloudTimeCnt <= CLOUD_SPOWN_TIME / ( m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED ) ) return;
	m_CloudTimeCnt = 0;

	// �_����������.
	const int NoMax		= m_CloudStateBase.AnimState.AnimNumberMax - 1;
	const int SpownMax	= CRandom::Probability( 1, 2 ) ? 2 : CRandom::GetRand( 3, 4 );
	const int SpownNum	= CRandom::GetRand( 2, SpownMax );
	for ( int i = 0; i < SpownNum; ++i ) {
		// �g�p���Ă��Ȃ��̂�����Ύg�p����.
		for ( auto& c : m_CloudStateList ) {
			if ( c.IsDisp ) continue;

			// �_����������.
			c.IsDisp				= true;
			c.Color					= Color4::White;
			c.Transform.Position.x	= CLOUD_SPOWN_POS_X;
			c.Transform.Position.y	= CRandom::GetRand( CLOUD_SPOWN_POS_Y_MIN, CLOUD_SPOWN_POS_Y_MAX );
			c.AnimState.AnimNumber	= CRandom::GetRand( 0, NoMax );
			return;
		}

		// �V�����쐬.
		m_CloudStateList.emplace_back( m_CloudStateBase );
		m_CloudStateList.back().Transform.Position.x	= CLOUD_SPOWN_POS_X;
		m_CloudStateList.back().Transform.Position.y	= CRandom::GetRand( CLOUD_SPOWN_POS_Y_MIN, CLOUD_SPOWN_POS_Y_MAX );
		m_CloudStateList.back().AnimState.AnimNumber	= CRandom::GetRand( 0, NoMax );
	}
}

//---------------------------.
// �_�̈ړ�.
//---------------------------.
void CMiniGame::CloudMove()
{
	// �_�̈ړ�.
	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// �_�̈ړ�.
		c.Transform.Position.x += m_CloudMoveSpeed;
		c.Color					= Color4::White;

		// �ړ����I��������.
		if ( c.Transform.Position.x >= CLOUD_DELETE_POS_X ) {
			c.IsDisp = false;
		}
	}

	// �_�̈ړ����x���グ��.
	if ( m_IsPlay == false ) return;
	if ( m_CloudMoveSpeed < CLOUD_CENTER_MOVE_SPPED ) {
		m_CloudMoveSpeed += 0.01f;
		if ( m_CloudMoveSpeed >= CLOUD_CENTER_MOVE_SPPED ) 
			m_CloudMoveSpeed = CLOUD_CENTER_MOVE_SPPED;
	}
	else if ( m_CloudMoveSpeed < CLOUD_END_MOVE_SPPED ) {
		if ( static_cast<int>( m_Score ) % 100 != 0 ) return;
		m_CloudMoveSpeed += 0.025f;
		if ( m_CloudMoveSpeed >= CLOUD_END_MOVE_SPPED ) m_CloudMoveSpeed = CLOUD_END_MOVE_SPPED;
	}
}

//---------------------------.
// �g�p���Ă��Ȃ��_�̍폜.
//---------------------------.
void CMiniGame::CloudDelete()
{
	const int Size = static_cast<int>( m_CloudStateList.size() );
	for ( int i = Size - 1; i >= 0; i-- ) {
		if ( m_CloudStateList[i].IsDisp ) break;
		m_CloudStateList.pop_back();
	}
}

//---------------------------.
// �e�L�X�g�̃A�j���[�V����.
//---------------------------.
void CMiniGame::TextAnimation()
{
	// �����̃A�j���[�V�����̍X�V.
	m_AnimTimeCnt += TEXT_ANIM_SPEED * ( m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED );

	// �ʒu�̍X�V.
	const SSize Size = m_pText->GetSpriteState().Disp;
	m_TextStateList[m_TextAnimNo].Transform.Position.y = 
		FWND_H - Size.h - TEXT_GAP - TEXT_JUMP_Y * sinf( Math::ToRadian( m_AnimTimeCnt ) );
	 
	// ���̕����Ɉړ�.
	if ( m_AnimTimeCnt >= Math::DEGREE_MID ) {
		m_AnimTimeCnt = 0.0f;
		m_TextAnimNo++;
		if ( m_TextAnimNo >= TEXT_NUM ) m_TextAnimNo = 0;
	}
}

//---------------------------.
// �v���C���[�̃W�����v.
//---------------------------.
void CMiniGame::PlayerJump()
{
	if ( m_IsCloudHit ) return;

	// �v���C���[�̃W�����v.
	if ( m_PlayerState.Transform.Position.y <= CEILING_POS_Y + FLIGHT_ANIM_SIZE ) return;

	// �g���K�[�̉�����̎擾.
	const float LTrigger = CXInput::GetLTrigger();
	const float RTrigger = CXInput::GetRTrigger();

	// �v���C���[�̃p���[�̍X�V.
	m_Power -= ( LTrigger + RTrigger + m_AutoPlay + 1.1f ) / 5.0f;

	// ���쒆�ɕύX.
	if ( m_IsPlay ) return;
	if ( LTrigger > 0.0f || RTrigger > 0.0f ) {
		m_IsPlay	= true;
		m_AutoPlay	= INIT_FLOAT;

		// �X�R�A�̏�����.
		m_PreviousScore = static_cast<int>( std::ceil( m_Score ) );
		m_Score			= INIT_FLOAT;
	}
}

//---------------------------.
// �v���C���[�̗���.
//---------------------------.
void CMiniGame::PlayerFall()
{
	// �v���C���[�̏d��.
	if ( m_PlayerState.Transform.Position.y >= GROUND_POS_Y - FLIGHT_ANIM_SIZE ) return;
	
	// �v���C���[�𗎉������Ă���.
	m_Power += GRAVITY_POWER;
}

//---------------------------.
// �v���C���[�̉_�ɓ����������̃A�j���[�V����.
//---------------------------.
void CMiniGame::PlayerCloudHitAnimation()
{
	if ( m_IsCloudHit == false ) return;

	// �v���C���[����]������.
	const float RotSpeed = m_LeftRotation ? -PLAYER_ROT_SPEED : PLAYER_ROT_SPEED;
	m_PlayerState.Transform.Rotation.z += Math::ToRadian( RotSpeed );

	// �A�j���[�V�����I��.
	if ( ( m_PlayerState.Transform.Rotation.z <= -Math::RADIAN_MAX ) || 
		 ( m_PlayerState.Transform.Rotation.z >=  Math::RADIAN_MAX ) )
	{
		m_PlayerState.Transform.Rotation.z	= 0.0f;
		m_IsCloudHit						= false;
		m_LeftRotation						= false;
		m_CoolTime							= COOL_TIME;
	}
}

//---------------------------.
// �v���C���[�̍��W�̍X�V.
//---------------------------.
void CMiniGame::PlayerPositionUpdate()
{
	m_PlayerState.Transform.Position.y		+= m_Power;
	m_PlayerIconState.Transform.Position.y	 = m_PlayerState.Transform.Position.y;
}

//---------------------------.
// �v���C���[�̉��̍X�V.
//---------------------------.
void CMiniGame::PlayerFireUpdate()
{
	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_PlayerFireCnt += Math::ToRadian( 3.0f );
	if ( m_PlayerFireCnt >= Math::RADIAN_MID ) m_PlayerFireCnt = 0.0f;

	// ���̊g�k.
	const float Scale = STransform::NORMAL_SCALE - 0.3f * sinf( m_PlayerFireCnt );
	m_PlayerFireState.Transform.Scale = { Scale, Scale, Scale };

	// �F�̐ݒ�.
	if ( m_IsPlay ) {
		m_PlayerState.Color		= Color4::White;
		m_PlayerFireState.Color = Color4::White;
	}
	else {
		m_PlayerState.Color		= Color4::Gray;
		m_PlayerFireState.Color = Color4::Darkgray;
	}
}

//---------------------------.
// �n�ʂ̓����蔻��.
//---------------------------.
void CMiniGame::GroundCollision()
{
	if ( m_PlayerState.Transform.Position.y < GROUND_POS_Y ) return;
	
	// �n�ʂ̈ʒu�Ɏ~�߂�.
	m_PlayerState.Transform.Position.y	= GROUND_POS_Y;
	if ( m_Power > 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// �V��̓����蔻��.
//---------------------------.
void CMiniGame::CeilingCollision()
{
	if ( m_PlayerState.Transform.Position.y > CEILING_POS_Y ) return;

	// �V��̈ʒu�Ŏ~�߂�.
	m_PlayerState.Transform.Position.y = CEILING_POS_Y;
	if ( m_Power < 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// �_�̓����蔻��.
//---------------------------.
void CMiniGame::CloudCollision()
{
	if ( m_IsCloudHit		) return;
	if ( m_CoolTime > 0		) return;

	// �v���C���[�̍��W�̎擾.
	const D3DXPOSITION3 PPos = m_PlayerState.Transform.Position;

	// �_�Ɠ������Ă��邩���ׂ�.
	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// �_�̍��W�̎擾.
		const D3DXPOSITION3& CPos = c.Transform.Position;

		// �~�ʂ����������Ă��邩���ׂ�.
		const float dx	= PPos.x - CPos.x;
		const float dy	= PPos.y - CPos.y;
		const float dr	= dx * dx + dy * dy;
		const float ar	= PLAYER_RADIUS			+ CLOUD_RADIUS;
		const float br	= PLAYER_AVERT_RADIUS	+ CLOUD_AVERT_RADIUS;
		const float dla = ar * ar;
		const float dlb = br * br;

		// �������Ă��邩.
		if ( dr < dla ) {
			m_IsCloudHit = true;
			if ( CPos.y >= PPos.y ) m_LeftRotation = true;

			// �v���C�f�[�^�̒ǉ�.
			const int CheckScore = static_cast<int>( std::ceil( m_Score ) );
			if ( m_IsPlay ) m_PlayData.emplace_back( CheckScore );

			// ���쒆�ł͖�����.
			m_IsPlay	= false;
			m_AutoPlay	= 0.0f;

			// �_�̑��x��x������.
			m_CloudMoveSpeed	= CLOUD_START_MOVE_SPPED;

			// �x�X�g�X�R�A���X�V������.
			if ( CheckScore > m_BestScore ) {
				m_BestScore = CheckScore;

				// �X�R�A�̕ۑ�.
				int Data[1];
				Data[0] = m_BestScore;
				FileManager::BinarySave( DATA_FILE_PATH, Data );

				// SE�̍Đ�.
				CSoundManager::PlaySE( END_BESTSCORE_UPDATE_SE );
			}

			// SE�̍Đ�.
			CSoundManager::PlaySE( HIT_SE );
			return;
		}

		// �M���M�����������.
		if ( dr < dlb && m_IsPlay ) {
			// �X�R�A���������₷.
			m_Score += AVERT_ADD_SCORE;

			// SE�̍Đ�.
			CSoundManager::PlaySE( AVERT_SE, 0.05f );
		}
	}
}

//---------------------------.
// �|�[�Y�̃{�^���̃e�L�X�g�̍X�V.
//---------------------------.
void CMiniGame::PauseButtonUpdate()
{
	// �e�L�X�g�̌����ڂ̕ύX.
	m_PauseButtonState.AnimState.AnimNumber = m_IsPlay ? 1 : 0;
}

//---------------------------.
// �T�E���h�̍X�V.
//---------------------------.
void CMiniGame::SoundUpdate()
{
	// BGM�̍Đ�.
	CSoundManager::PlayBGM( m_BGMName );
}

//---------------------------.
// �X�R�A�̍X�V.
//---------------------------.
void CMiniGame::ScoreUpdate()
{
	if ( m_IsPlay		== false) return;
	if ( m_IsCloudHit	== true ) return;
	if ( m_CoolTime		> 0		) return;

	// �X�R�A�̉��Z.
	const int	OldCheckScore		= static_cast<int>( std::ceil( m_Score ) );
	const int	OldScore10000Cnt	= static_cast<int>( OldCheckScore / 10000.0f );
	const int	OldScore5000Cnt		= static_cast<int>( OldCheckScore / 5000.0f  );
	const int	OldScore1000Cnt		= static_cast<int>( OldCheckScore / 1000.0f  );
	const bool	OldBestscoreUpdate	= OldCheckScore > m_BestScore;
	m_Score += m_CloudMoveSpeed / CLOUD_START_MOVE_SPPED;

	// �x�X�g�X�R�A���X�V������.
	const int	NewCheckScore		= static_cast<int>( std::ceil( m_Score ) );
	const bool	NewBestscoreUpdate	= NewCheckScore > m_BestScore;
	if ( OldBestscoreUpdate == false && NewBestscoreUpdate ) {
		CSoundManager::PlaySE( BESTSCORE_UPDATE_SE );
	}

	// SE�̍Đ�.
	const int NewScore10000Cnt	= static_cast<int>( NewCheckScore / 10000.0f );
	const int NewScore5000Cnt	= static_cast<int>( NewCheckScore / 5000.0f  );
	const int NewScore1000Cnt	= static_cast<int>( NewCheckScore / 1000.0f  );
	if ( OldScore10000Cnt != NewScore10000Cnt ) {
		CSoundManager::PlaySE( SCORE_10000M_SE );
		return;
	}
	if ( OldScore5000Cnt != NewScore5000Cnt ) {
		CSoundManager::PlaySE( SCORE_5000M_SE );
		return;
	}
	if ( OldScore1000Cnt != NewScore1000Cnt ) {
		CSoundManager::PlaySE( SCORE_1000M_SE );
		return;
	}
}

//---------------------------.
// �e�L�X�g�̍X�V.
//---------------------------.
void CMiniGame::TextUpdate()
{
	// �X�R�A�̃e�L�X�g�̍쐬.
	std::string BestScoreText		= std::to_string( m_BestScore );
	std::string PreviousScoreText	= std::to_string( m_PreviousScore );
	std::string NowScoreText		= std::to_string( static_cast<int>( std::ceil( m_Score ) ) );

	// �e�L�X�g�̒����̎擾.
	const int PTextLength	= static_cast<int>( PreviousScoreText.length()	);
	const int NTextLength	= static_cast<int>( NowScoreText.length()		);

	// �e�L�X�g�̒���.
	const size_t TextLength = PTextLength > NTextLength ? PTextLength : NTextLength;
	PreviousScoreText		= StringConversion::to_LeftPaddingString( PreviousScoreText, static_cast<int>( TextLength ) );
	NowScoreText			= StringConversion::to_LeftPaddingString( NowScoreText,		 static_cast<int>( TextLength ) );

	// �e�L�X�g�̍X�V.
	m_FontStateList[BestScore].Text =
		"�x�X�g�X�R�A:" + BestScoreText + "m";
	m_FontStateList[PreviousScore].Text =
		"�O��̃X�R�A:" + PreviousScoreText + "m\n"
		"���݂̃X�R�A:" + NowScoreText + "m";

	// ��������̏����e�L�X�g�ɒǉ�.
#ifdef _DEBUG
	m_FontStateList[BestScore].Text +=
		"\n-------------\n"
		"�p���[:" + std::to_string( m_Power ) + "\n"
		"���@��:" + std::to_string( m_AutoPlay ) + "\n"
		"�s�@��:" + std::to_string( m_AutoPlayAction );
#endif // #ifdef _DEBUG.
}

//---------------------------.
// �N�[���^�C���̍X�V.
//---------------------------.
void CMiniGame::CoolTimeUpdate()
{
	if ( m_CoolTime <= 0 ) return;

	// �N�[���^�C�������炷.
	m_CoolTime--;
	if ( m_CoolTime <= 0 ) m_CoolTime = 0;
}

//---------------------------.
// ��������̍X�V.
//---------------------------.
void CMiniGame::AutoPlayUpdate()
{
	if ( m_IsPlay					) return;
	if ( m_IsCloudHit				) return;
	if ( m_CloudStateList.empty()	) return;
	
	CreateSortCloudList();		// �_�̃\�[�g���X�g�̍쐬.
	AutoPlayActionUpdate();		// ��������̍s���̍X�V.
	AutoPlayOutUpdate();		// ��������̏o�͂̍X�V.
}

//---------------------------.
// �_�̃\�[�g���X�g�̍쐬.
//---------------------------.
void CMiniGame::CreateSortCloudList()
{
	// �v���C���[�̍��W�̎擾.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// �_���v���C���[�ɋ߂����ɕ��ׂ�.
	m_SortCloudList.clear();
	for ( auto& c : m_CloudStateList ) {
		if ( c.Transform.Position.x >= PlayerPos.x - PLAYER_RADIUS * 1.5f ) continue;
		const D3DXVECTOR3&	Vector		= c.Transform.Position - PlayerPos;
		const float			Distance	= D3DXVec3Length( &Vector );
		m_SortCloudList.emplace_back( std::make_pair( Distance, &c ) );
	}
	std::sort( m_SortCloudList.begin(), m_SortCloudList.end() );
}

//---------------------------.
// ��������̍s���̍X�V.
//---------------------------.
void CMiniGame::AutoPlayActionUpdate()
{
	// �v���C���[�̍��W�̎擾.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// �]������_�̏��̎擾.
	const D3DXPOSITION3& Pos		= m_SortCloudList[0].second->Transform.Position;
	const float			 CheckUY	= Pos.y - CLOUD_RADIUS - PLAYER_RADIUS * 1.5f;
	const float			 CheckDY	= Pos.y + CLOUD_RADIUS + PLAYER_RADIUS * 1.5f;
	bool				 IsUp		= PlayerPos.y <= CheckUY ? true : false;

	// �]������_��ԐF�ɂ���.
#ifdef _DEBUG
	m_SortCloudList[0].second->Color = Color4::Red;
#endif	// #ifdef _DEBUG.

	// �㉺�ɕ����ĕ]������.
	bool IsMoveEnd = false;
	for ( int i = 0; i < 2; ++i ) {
		// ��Ɉړ��o��������.
		if ( IsUp ) {
			const bool IsMoveOK = UpMoveCheck( 0 );

			// ��Ɉړ�.
			if ( IsMoveOK ) {
				if (	  PlayerPos.y > CheckUY - PLAYER_RADIUS			) m_AutoPlayAction = MoveUp;
				else if ( PlayerPos.y < CheckUY - PLAYER_RADIUS * 2.0f	) m_AutoPlayAction = MoveWait;
				else													  m_AutoPlayAction = MoveUpSmall;
				IsMoveEnd = true;
			}
		}

		// ���Ɉړ��o��������.
		if ( IsUp == false ) {
			const bool IsMoveOK = DownMoveCheck( 0 );

			// ���Ɉړ�.
			if ( IsMoveOK ) {
				if ( PlayerPos.y < CheckDY + PLAYER_RADIUS * 2.0f ) m_AutoPlayAction = MoveDown;
				else												m_AutoPlayAction = MoveDownSmall;
				IsMoveEnd = true;;
			}
		}

		// �ړ������𔽓]�����čĕ]��.
		if ( IsMoveEnd ) break;
		IsUp = !IsUp;
	}
}

//---------------------------.
// ��Ɉړ��ł��邩���f.
//---------------------------.
bool CMiniGame::UpMoveCheck( const int CloudNo )
{
	// �ʒu�̎擾.
	const D3DXPOSITION3& Pos	= m_SortCloudList[CloudNo].second->Transform.Position;
	const float			 CheckY	= Pos.y - CLOUD_RADIUS - PLAYER_RADIUS * 1.5f;

	// �v���C���[�̍��W�̎擾.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// ��Ɉړ��ł��邩.
	const float DeadCeiling = CEILING_POS_Y + PLAYER_RADIUS;
	if ( CheckY <= DeadCeiling ) {
#ifdef _DEBUG
		m_SortCloudList[CloudNo].second->Color.w = 0.5f;
#endif	// #ifdef _DEBUG.
		return false;
	}

	// ���̉_�ɋ߂����Ȃ����m�F.
	const int ListSize	= static_cast<int>( m_SortCloudList.size() );
	for ( int CheckNo = 0; CheckNo < ListSize; ++CheckNo ) {
		if ( CloudNo == CheckNo												) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.x != Pos.x	) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.y  > Pos.y	) continue;

		// ���������߂�.
		const float Distance = std::abs( m_SortCloudList[CheckNo].second->Transform.Position.y - Pos.y );
		if ( Distance > ( CLOUD_RADIUS + PLAYER_RADIUS * 2.5f ) * 2.0f ) continue;

		// �]������_�����F�ɂ���.
#ifdef _DEBUG
		m_SortCloudList[CheckNo].second->Color = Color4::Yellow;
#endif	// #ifdef _DEBUG.

		// ��̉_�̏�͈ړ��ł��邩.
		return UpMoveCheck( CheckNo );
	}
	return true;
}

//---------------------------.
// ���Ɉړ��ł��邩���f.
//---------------------------.
bool CMiniGame::DownMoveCheck( const int CloudNo )
{
	// �ʒu�̎擾.
	const D3DXPOSITION3& Pos	= m_SortCloudList[CloudNo].second->Transform.Position;
	const float			 CheckY	= Pos.y + CLOUD_RADIUS + PLAYER_RADIUS * 1.5f;

	// �v���C���[�̍��W�̎擾.
	const D3DXPOSITION3& PlayerPos = m_PlayerState.Transform.Position;

	// ���Ɉړ��ł��邩.
	const float DeadGround = GROUND_POS_Y - PLAYER_RADIUS * 2.0f;
	if ( CheckY >= DeadGround ) {
#ifdef _DEBUG
		m_SortCloudList[CloudNo].second->Color.w = 0.5f;
#endif	// #ifdef _DEBUG.
	return false;
}
	// ���̉_�ɋ߂����Ȃ����m�F.
	const int ListSize	= static_cast<int>( m_SortCloudList.size() );
	for ( int CheckNo = 0; CheckNo < ListSize; ++CheckNo ) {
		if ( CloudNo == CheckNo												) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.x != Pos.x	) continue;
		if ( m_SortCloudList[CheckNo].second->Transform.Position.y <= Pos.y	) continue;

		// ���������߂�.
		const float Distance = std::abs( m_SortCloudList[CheckNo].second->Transform.Position.y - Pos.y );
		if ( Distance > ( CLOUD_RADIUS + PLAYER_RADIUS * 2.5f ) * 2.0f ) continue;

		// �]������_�����F�ɂ���.
#ifdef _DEBUG
		m_SortCloudList[CheckNo].second->Color = Color4::Yellow;
#endif	// #ifdef _DEBUG.

		// ���̉_�̉��͈ړ��ł��邩.
		return DownMoveCheck( CheckNo );
	}
	return true;
}

//---------------------------.
// ��������̏o�͂̍X�V.
//---------------------------.
void CMiniGame::AutoPlayOutUpdate()
{
	switch ( m_AutoPlayAction ) {
	case MoveUp:
		// �p���[�� -6 �ɂȂ�悤�ɒ�������.
		if ( m_Power == -6.0f ) break;
		if ( m_Power > -6.0f )	m_AutoPlay = m_Power > -4.0f ? 2.0f : m_Power + 6.0f;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveUpSmall:
		// �p���[�� -2 �ɂȂ�悤�ɒ�������.
		if ( m_Power == -2.0f ) break;
		if ( m_Power > -2.0f )	m_AutoPlay = m_Power > 0.0f ? 2.0f : m_Power + 2.0f;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveWait:
		// �p���[��0�ɂȂ�悤�ɒ�������.
		if ( m_Power == 0.0f ) break;
		if ( m_Power > 0.0f )	m_AutoPlay = m_Power > 2.0f ? 2.0f : m_Power;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveDownSmall:
		// �p���[�� 2 �ɂȂ�悤�ɒ�������.
		if ( m_Power == 2.0f ) break;
		if ( m_Power > 2.0f )	m_AutoPlay = m_Power > 4.0f ? 2.0f : m_Power - 2.0f;
		else					m_AutoPlay = 0.0f;
		break;
	case MoveDown:
		// �p���[�� 6 �ɂȂ�悤�ɒ�������.
		if ( m_Power == 6.0f ) break;
		if ( m_Power > 6.0f )	m_AutoPlay = m_Power > 8.0f ? 2.0f : m_Power - 6.0f;
		else					m_AutoPlay = 0.0f;
		break;
	default:
		break;
	}
}

//---------------------------.
// �|�[�Y�Ɉړ�.
//---------------------------.
void CMiniGame::MovePause()
{
	if ( m_IsPlay ) return;

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
void CMiniGame::PauseOpen()
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
void CMiniGame::PauseClose()
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
void CMiniGame::PauseUpdate()
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
		// �u�Q�[���ɖ߂�v��I�������Ƃ�.
		case CHOICE_STARTCHOICE:
			CSoundManager::PlaySE( CLOSE_SE );
			m_IsPause		= false;
			m_IsPauseAnim	= true;
			break;
		// �u�x�X�g�X�R�A���폜����v��I�������Ƃ�.
		case CHOICE_CENTERCHOICE:
			FileManager::BinarySave( DATA_FILE_PATH, 0 );
			m_BestScore		= 0;
			m_IsPause		= false;
			m_IsPauseAnim	= true;

			// SE�̍Đ�.
			CSoundManager::PlaySE( TRASH_SE );
			CSoundManager::PlaySE( CLOSE_SE );
			break;
		// �u�W�F�b�g���x�����I������v��I�������Ƃ�.
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
