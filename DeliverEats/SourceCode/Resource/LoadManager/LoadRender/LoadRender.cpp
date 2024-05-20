#include "LoadRender.h"
#include "..\..\..\Common\DirectX\DirectX11.h"
#include "..\..\..\Common\SoundManeger\SoundManeger.h"
#include "..\..\..\Utility\FrameRate\FrameRate.h"
#include "..\..\..\Utility\Input\Input.h"
#include "..\..\..\Utility\Random\Random.h"

namespace{
	constexpr int	TEXT_NUM					= 13;							// �e�L�X�g�̕\����.
	constexpr int	TEXT_MAX					= 10;							// �e�L�X�g�̏��.
	constexpr float	TEXT_GAP					= 20.0f;						// �e�L�X�g�̌���.
	constexpr float	TEXT_JUMP_Y					= 20.0f;						// �e�L�X�g�̃W�����v�����鍂��.
	constexpr float	TEXT_ANIM_SPEED				= 5.0f;							// �e�L�X�g�̃A�j���[�V�����̑��x.
	constexpr int	CLOUD_SPOWN_TIME			= 50;							// �_���������Ă�������.
	constexpr float	CLOUD_SPOWN_POS_X			= 20.0f;						// �_����������X���W.
	constexpr float	CLOUD_DELETE_POS_X			= FWND_W - CLOUD_SPOWN_POS_X;	// �_���폜����X���W.
	constexpr float	CLOUD_SPOWN_POS_Y_MIN		= 50.0f;						// �_����������Y���W�̍ŏ��l.
	constexpr float	CLOUD_SPOWN_POS_Y_MAX		= 620.0f;						// �_����������Y���W�̍ő�l.
	constexpr float	CLOUD_START_MOVE_SPPED		= 5.0f;							// �_�̊J�n���̈ړ����x.
	constexpr float	CLOUD_END_MOVE_SPPED		= 14.0f;						// �_�̍ŏI�̈ړ����x.
	constexpr float	CLOUD_RADIUS				= 8.0f;							// �_�𓖂��蔻��̔��a.
	constexpr float	CLOUD_AVERT_RADIUS			= 23.0f;						// �_�̃M���M��������������ׂ铖���蔻��̔��a.
	constexpr float GRAVITY_POWER				= 0.4f;							// �d�͂̋���.
	constexpr float	PLAYER_POS_X				= 1000.0f;						// �v���C���[��X���W.
	constexpr float	PLAYER_FIRE_POS_X			= 37.0f;						// �v���C���[�̉���X���W.
	constexpr float	PLAYER_FIRE_POS_Y			= 11.0f;						// �v���C���[�̉���Y���W.
	constexpr float	PLAYER_RADIUS				= 23.0f;						// �v���C���[�̓����蔻��̔��a.
	constexpr float	PLAYER_AVERT_RADIUS			= 38.0f;						// �v���C���[�̃M���M��������������ׂ铖���蔻��̔��a.
	constexpr float	PLAYER_ROT_SPEED			= 6.0f;							// �v���C���[�̉_�ɓ����������̃A�j���[�V�����̉�]���x.
	constexpr int	COOL_TIME					= 10;							// �����蔻��̃N�[���^�C��.
	constexpr float	CEILING_POS_Y				= 100.0f;						// �V���Y���W.
	constexpr float	GROUND_POS_Y				= 550.0f;						// �n�ʂ�Y���W.
	constexpr float	FLIGHT_ANIM_SIZE			= 20.0f;						// ��s�A�j���[�V�����p�̃T�C�Y.
	constexpr char	HIT_SE[]					= "MiniGameHit";				// �_�ɓ�����������SE.
	constexpr char	AVERT_SE[]					= "MiniGameAvert";				// �M���M�������������SE.
}

CLoadRender::CLoadRender()
	: m_pText				( nullptr )
	, m_pPlayer				( nullptr )
	, m_pPlayerFire			( nullptr )
	, m_pPlayerIcon			( nullptr )
	, m_pCloud				( nullptr )
	, m_pLRTrigger			( nullptr )
	, m_TextStateList		( TEXT_NUM )
	, m_CloudStateList		()
	, m_CloudStateBase		()
	, m_PlayerState			()
	, m_PlayerFireState		()
	, m_PlayerIconState		()
	, m_LRTriggerState		()
	, m_AnimTimeCnt			( INIT_FLOAT )
	, m_Power				( INIT_FLOAT )
	, m_PlayerFireCnt		( INIT_FLOAT )
	, m_CloudMoveSpeed		( CLOUD_START_MOVE_SPPED )
	, m_CloudTimeCnt		( INIT_INT )
	, m_TextAnimNo			( INIT_INT )
	, m_CoolTime			( INIT_INT )
	, m_IsCloudHit			( false )
	, m_LeftRotation		( false )
{
	m_pText			= std::make_unique<CSprite>();
	m_pPlayer		= std::make_unique<CSprite>();
	m_pPlayerFire	= std::make_unique<CSprite>();
	m_pPlayerIcon	= std::make_unique<CSprite>();
	m_pCloud		= std::make_unique<CSprite>();
	m_pLRTrigger	= std::make_unique<CSprite>();
}

CLoadRender::~CLoadRender()
{
}

//---------------------------.
// �ǂݍ���.
//---------------------------.
HRESULT CLoadRender::Load()
{
	if ( m_pText->Init(			"Data\\Sprite\\Scene\\Loading\\LoadText.png"		) ) return E_FAIL;
	if ( m_pPlayer->Init(		"Data\\Sprite\\Scene\\Loading\\LoadPlayer.png"		) ) return E_FAIL;
	if ( m_pPlayerFire->Init(	"Data\\Sprite\\Scene\\Loading\\LoadPlayerFire.png"	) ) return E_FAIL;
	if ( m_pPlayerIcon->Init(	"Data\\Sprite\\Scene\\Loading\\LoadPlayerIcon.png"	) ) return E_FAIL;
	if ( m_pCloud->Init(		"Data\\Sprite\\Scene\\Loading\\LoadCloud.png"		) ) return E_FAIL;
	if ( m_pLRTrigger->Init(	"Data\\Sprite\\Scene\\Loading\\LoadLRTrigger.png"	) ) return E_FAIL;

	// �����̐ݒ�.
	const SSize&				TextSize	= m_pText->GetSpriteState().Disp;
	const SSpriteRenderState&	TextState	= m_pText->GetRenderState();
	for ( int i = 0; i < TEXT_NUM; ++i ) {
		const int No = i > TEXT_MAX ? TEXT_MAX : i;
		m_TextStateList[i] = TextState;
		m_TextStateList[i].AnimState.IsSetAnimNumber	= true;
		m_TextStateList[i].AnimState.AnimNumber			= No;
		m_TextStateList[i].Transform.Position.x			= FWND_W - TextSize.w * ( TEXT_NUM - i ) - TEXT_GAP;
		m_TextStateList[i].Transform.Position.y			= FWND_H - TextSize.h - TEXT_GAP;
	}

	// ��������̃e�L�X�g�̈ʒu�̐ݒ�.
	m_LRTriggerState = m_pLRTrigger->GetRenderState();
	m_LRTriggerState.Transform.Position.x = TEXT_GAP;
	m_LRTriggerState.Transform.Position.y = FWND_H - TEXT_GAP;

	// �_�̐ݒ�.
	const SSize& CloudSize = m_pCloud->GetSpriteState().Disp;
	m_CloudStateBase								= m_pCloud->GetRenderState();
	m_CloudStateBase.AnimState.IsSetAnimNumber		= true;
	m_CloudStateBase.RenderArea = {
		CLOUD_SPOWN_POS_X + CloudSize.w / 2.0f,
		0.0f,
		CLOUD_DELETE_POS_X - CLOUD_SPOWN_POS_X - CloudSize.w,
		FWND_H
	};

	// �v���C���[�̐ݒ�.
	m_PlayerState = m_pPlayer->GetRenderState();
	m_PlayerState.Transform.Position.x		= PLAYER_POS_X;
	m_PlayerState.Transform.Position.y		= GROUND_POS_Y;
	m_PlayerFireState.Transform.AddParent( &m_PlayerState.Transform );
	m_PlayerFireState.Transform.Position.x	= PLAYER_FIRE_POS_X;
	m_PlayerFireState.Transform.Position.y	= PLAYER_FIRE_POS_Y;

	// �v���C���[�̃A�C�R���̐ݒ�.
	m_PlayerIconState.Transform.Position.x = TEXT_GAP;
	m_PlayerIconState.Transform.Position.y = m_PlayerState.Transform.Position.y;

	// �w�i�̐F�̐ݒ�.
	CDirectX11::SetBackColor( { 0.0f, 0.0f, 0.0f, 0.0f } );
	return S_OK;
}

//---------------------------.
// �X�V.
//---------------------------.
void CLoadRender::Update( const float& DeltaTime )
{
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
	CoolTimeUpdate();			// �N�[���^�C���̍X�V.

	// BGM�̍Đ�.
	CSoundManager::PlayBGM( "MiniGame" );
}

//---------------------------.
// �`��.
//---------------------------.
void CLoadRender::Render()
{
	CDirectX11::SetDepth( false );
	for( auto& t : m_TextStateList	) m_pText->RenderUI( &t );
	m_pLRTrigger->RenderUI( &m_LRTriggerState );
	m_pPlayer->RenderUI( &m_PlayerState );
	m_pPlayerFire->RenderUI( &m_PlayerFireState );
	m_pPlayerIcon->RenderUI( &m_PlayerIconState );
	for( auto& c : m_CloudStateList ) m_pCloud->RenderUI( &c );
	CDirectX11::SetDepth( true );
}

//---------------------------.
// �_�̏���.
//---------------------------.
void CLoadRender::CloudSpown()
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
void CLoadRender::CloudMove()
{
	// �_�̈ړ����x���グ��.
	if ( m_CloudMoveSpeed < CLOUD_END_MOVE_SPPED ) {
		m_CloudMoveSpeed += 0.01f;
		if ( m_CloudMoveSpeed >= CLOUD_END_MOVE_SPPED ) m_CloudMoveSpeed = CLOUD_END_MOVE_SPPED;
	}

	for ( auto& c : m_CloudStateList ) {
		if ( c.IsDisp == false ) continue;

		// �_�̈ړ�.
		c.Transform.Position.x += m_CloudMoveSpeed;

		// �ړ����I��������.
		if ( c.Transform.Position.x >= CLOUD_DELETE_POS_X ) {
			c.IsDisp = false;
		}
	}
}

//---------------------------.
// �g�p���Ă��Ȃ��_�̍폜.
//---------------------------.
void CLoadRender::CloudDelete()
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
void CLoadRender::TextAnimation()
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
void CLoadRender::PlayerJump()
{
	if ( m_IsCloudHit ) return;

	// �v���C���[�̃W�����v.
	if ( m_PlayerState.Transform.Position.y <= CEILING_POS_Y + FLIGHT_ANIM_SIZE ) return;

	// �g���K�[�̉�����̎擾.
	const float LTrigger = CXInput::GetLTrigger();
	const float RTrigger = CXInput::GetRTrigger();

	// �v���C���[�̃p���[�̍X�V.
	m_Power -= ( LTrigger + RTrigger + 1.1f ) / 5.0f;
}

//---------------------------.
// �v���C���[�̗���.
//---------------------------.
void CLoadRender::PlayerFall()
{
	// �v���C���[�̏d��.
	if ( m_PlayerState.Transform.Position.y >= GROUND_POS_Y - FLIGHT_ANIM_SIZE ) return;
	
	// �v���C���[�𗎉������Ă���.
	m_Power += GRAVITY_POWER;
}

//---------------------------.
// �v���C���[�̉_�ɓ����������̃A�j���[�V����.
//---------------------------.
void CLoadRender::PlayerCloudHitAnimation()
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
void CLoadRender::PlayerPositionUpdate()
{
	m_PlayerState.Transform.Position.y		+= m_Power;
	m_PlayerIconState.Transform.Position.y	 = m_PlayerState.Transform.Position.y;
}

//---------------------------.
// �v���C���[�̉��̍X�V.
//---------------------------.
void CLoadRender::PlayerFireUpdate()
{
	// �A�j���[�V�����p�̃J�E���g�̉��Z.
	m_PlayerFireCnt += Math::ToRadian( 3.0f );
	if ( m_PlayerFireCnt >= Math::RADIAN_MID ) m_PlayerFireCnt = 0.0f;

	// ���̊g�k.
	const float Scale = STransform::NORMAL_SCALE - 0.3f * sinf( m_PlayerFireCnt );
	m_PlayerFireState.Transform.Scale = { Scale, Scale, Scale };
}

//---------------------------.
// �n�ʂ̓����蔻��.
//---------------------------.
void CLoadRender::GroundCollision()
{
	if ( m_PlayerState.Transform.Position.y < GROUND_POS_Y ) return;
	
	// �n�ʂ̈ʒu�Ɏ~�߂�.
	m_PlayerState.Transform.Position.y	= GROUND_POS_Y;
	if ( m_Power > 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// �V��̓����蔻��.
//---------------------------.
void CLoadRender::CeilingCollision()
{
	if ( m_PlayerState.Transform.Position.y > CEILING_POS_Y ) return;

	// �V��̈ʒu�Ŏ~�߂�.
	m_PlayerState.Transform.Position.y = CEILING_POS_Y;
	if ( m_Power < 0.0f ) m_Power = 0.0f;
}

//---------------------------.
// �_�̓����蔻��.
//---------------------------.
void CLoadRender::CloudCollision()
{
	if ( m_IsCloudHit	) return;
	if ( m_CoolTime > 0 ) return;

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
		const float ar	= PLAYER_RADIUS + CLOUD_RADIUS;
		const float br	= PLAYER_AVERT_RADIUS + CLOUD_AVERT_RADIUS;
		const float dla = ar * ar;
		const float dlb = br * br;

		// �������Ă��邩.
		if ( dr < dla ) {
			m_IsCloudHit = true;
			if ( CPos.y >= PPos.y ) m_LeftRotation = true;

			// �_�̑��x��x������.
			if ( m_CloudMoveSpeed > CLOUD_START_MOVE_SPPED ) {
				m_CloudMoveSpeed /= 2.0f;
				if ( m_CloudMoveSpeed <= CLOUD_START_MOVE_SPPED ) m_CloudMoveSpeed = CLOUD_START_MOVE_SPPED;
			}

			// SE�̍Đ�.
			CSoundManager::PlaySE( HIT_SE );
			return;
		}

		// �M���M�����������.
		if ( dr < dlb ) {
			// SE�̍Đ�.
			CSoundManager::PlaySE( AVERT_SE, 0.05f );
		}
	}
}

//---------------------------.
// �N�[���^�C���̍X�V.
//---------------------------.
void CLoadRender::CoolTimeUpdate()
{
	if ( m_CoolTime <= 0 ) return;

	// �N�[���^�C�������炷.
	m_CoolTime--;
	if ( m_CoolTime <= 0 ) m_CoolTime = 0;
}
