#include "GameClear.h"
#include "..\..\Object\GameObject\Widget\SceneWidget\GameClearWidget\GameClearWidget.h"
#include "..\..\Object\Score\Score.h"
#include "..\..\Object\Score\Ranking\Ranking.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "..\..\Utility\Constant\Constant.h"
#include "..\..\Utility\Random\Random.h"

namespace {
	constexpr int HEAT_BONUS			= static_cast<int>(EShowState::HeatBonus);
	constexpr int GOLDEN_TIME_BONUS		= static_cast<int>(EShowState::GoldenTimeBonus);
	constexpr int TILT_DISCOUNT			= static_cast<int>(EShowState::TiltDiscount);
	constexpr int TREATMENT_PAY			= static_cast<int>(EShowState::TreatmentPay);
	constexpr int ORIGIN_WAGE			= static_cast<int>(EShowState::OriginWage);
	constexpr int DETAIL_MAX			= 5;

	constexpr int TREATMENT_PAY_VALUE	= -200000;

	constexpr float SCALE				= 3.0f;
	constexpr float	INTERVAL_MAX		= 1.0f;
	constexpr float	ADJUST_X			= 440.0f;
	constexpr float	ADJUST_Y			= 130.0f;
	constexpr float STRING_SIZE_X		= 50.0f;
	constexpr float STRING_SIZE_Y		= 24.0f;

	constexpr float WAGE_SPEED			= 1536.0f;
	constexpr float ALPHA_SPEED			= 3.0f;
	constexpr float INTERVAL_SPEED		= 3.0f;

	enum enSpriteList :unsigned char {
		BackGround,		// ������\������w�i.
		Frame,			// �����̏�ɕ\������w�i.
		Yubi,			// �J�[�\��.
		Choices_1,		// �I����1.
		Choices_2,		// �I����2.

		Max
	}typedef ESpriteList;

	enum enFontNo :unsigned char {
		WageNumber,		// �����̐���.
		WageYen,		// �Ō�ɕ\������u���v.
		DetailString,	// �����ڍׂ̕���.
		DetailNumber,	// �����ڍׂ̐���.

		Font_Max
	}typedef EFontNo;

	// ���ʉ�.
	constexpr int	DIGIT_SE_MAX		= 7;
	constexpr char	DIGIT_1_SE[]		= "Digit_1";
	constexpr char	DIGIT_10_SE[]		= "Digit_10";
	constexpr char	DIGIT_100_SE[]		= "Digit_100";
	constexpr char	DIGIT_1000_SE[]		= "Digit_1000";
	constexpr char	DIGIT_10000_SE[]	= "Digit_10000";
	constexpr char	DIGIT_100000_SE[]	= "Digit_100000";
	constexpr char	DIGIT_1000000_SE[]	= "Digit_1000000";

	constexpr char	DRUMROLL_SE[]		= "DrumRoll";
	constexpr char	CASH_REGISTER_SE[]	= "CashRegister";
	constexpr char	GET_BONUS_SE[]		= "GetBonus";
	constexpr char	GET_PENALTY_SE[]	= "GetPenalty";

	constexpr char	CURSOR_MOVE_SE[]	= "CursorMove";
}

CGameClear::CGameClear()
	: m_DeltaTime			()
	, m_pGameClearWidget	( nullptr )
	, m_pSpriteList			( Max )
	, m_SpriteStateList		( Max )
	, m_IsStartDisp			( false )
	, m_CanOperate			( false )
	, m_Alpha				( INIT_FLOAT )
	, m_Choice				( INIT_INT )

	, m_ShowState			( EShowState::OriginWage )
	, m_ValueList			( DETAIL_MAX )
	, m_PosX				()
	, m_WageDigits			( INIT_INT )
	, m_WageFinalXPos		()
	, m_IsPlayDigitSE		()
	, m_DigitSE				()
	, m_ThroughStringCnt	()
	, m_IsPlayGetBonusSE	()
	, m_pFont				( nullptr )
	, m_FontStateList		( Font_Max )
{
}

CGameClear::~CGameClear()
{
}

//---------------------------.
// �ǂݍ���.
//---------------------------.
bool CGameClear::Init()
{
	// �萔�̏�����.
	CConstant::GameClearLoad();

	m_pGameClearWidget = std::make_unique<CGameClearWidget>();
	m_pGameClearWidget->Init();

	m_IsStartDisp	= false;
	m_CanOperate	= false;
	m_Alpha			= INIT_FLOAT;
	m_Choice		= INIT_INT;

	m_pSpriteList[BackGround] = CSpriteResource::GetSprite( "result_back_1110", &m_SpriteStateList[BackGround]	);
	std::string FrameFileName = "";
	if ( CScore::GetGameEnd() ) {
		FrameFileName = CRandom::Probability( 1, 4 ) ? "result_screen2_2" : "result_screen2_1";
	}
	else {
		FrameFileName = "result_screen1_1";
	}
	m_pSpriteList[Frame] =		CSpriteResource::GetSprite( FrameFileName,		&m_SpriteStateList[Frame]		);

	m_pSpriteList[Yubi] =		CSpriteResource::GetSprite( "Yubi",				&m_SpriteStateList[Yubi]		);
	m_pSpriteList[Choices_1] =	CSpriteResource::GetSprite( "result_txt_1109",	&m_SpriteStateList[Choices_1]	);
	m_pSpriteList[Choices_2] =	CSpriteResource::GetSprite( "result_txt_1109",	&m_SpriteStateList[Choices_2]	);

	m_SpriteStateList[BackGround].Transform.Position =	{ 171.0f, 136.0f, 0.0f };
	m_SpriteStateList[Yubi].Transform.Position =		{ 400.0f, 590.0f, 0.0f };
	const SSize ChoicesSize = m_pSpriteList[Choices_1]->GetSpriteState().Disp;
	m_SpriteStateList[Choices_1].Transform.Position =	{ 430.0f, 590.0f - (ChoicesSize.h / 2.0f), 0.0f };
	m_SpriteStateList[Choices_1].AnimState.IsSetAnimNumber = true;
	m_SpriteStateList[Choices_1].AnimState.AnimNumber =	0;
	m_SpriteStateList[Choices_2].Transform.Position =	{ 430.0f, 680.0f - (ChoicesSize.h / 2.0f), 0.0f };
	m_SpriteStateList[Choices_2].AnimState.IsSetAnimNumber = true;
	m_SpriteStateList[Choices_2].AnimState.AnimNumber = 2;

	m_SpriteStateList[Yubi].Transform.Scale = { 1.0f, 1.0f, 1.0f };

	m_SpriteStateList[Yubi].Color.w			= m_Alpha;
	m_SpriteStateList[Choices_1].Color.w	= m_Alpha;
	m_SpriteStateList[Choices_2].Color.w	= m_Alpha;

	// �J�����̐ݒ�.
	m_pCameras = std::make_unique<CCameras>();
	m_pCameras->InitCamera( ECameraType::NormalCamera );
	CCameraManager::SetCamera( m_pCameras->GetCamera<CNormalCamera>() );

	// BGM���̐ݒ�.
	m_BGMName = "GameClear";

	// �Q�[���I�����̓������擾.
	const SFoodState FinalWage = CScore::GetScore();
	m_ValueList[ORIGIN_WAGE].Value =
		FinalWage.EndMoney - FinalWage.HotBonus -
		FinalWage.GoldenBonus + FinalWage.DiscountMoney;
	m_ValueList[HEAT_BONUS].Value =
		FinalWage.HotBonus;
	m_ValueList[GOLDEN_TIME_BONUS].Value =
		FinalWage.GoldenBonus;
	m_ValueList[TILT_DISCOUNT].Value =
		-FinalWage.DiscountMoney;
	m_ValueList[TREATMENT_PAY].Value =
		TREATMENT_PAY_VALUE;

	m_WageDigits = static_cast<int>(std::to_string(
		m_ValueList[ORIGIN_WAGE].Value ).length());

	m_WageDigits += (m_WageDigits - 1) / 3;
	// 3�����ƂɃJ���}������.
	m_PosX.resize( m_WageDigits );
	for ( auto& p : m_PosX ) p = -30.0f;

	m_WageFinalXPos.resize( m_WageDigits );
	for ( int i = 0; i < m_WageDigits; ++i ) {
		m_WageFinalXPos[i] =
			850.0f - (((m_WageDigits - 1) - i) * STRING_SIZE_X);
	}

	for (auto& r : m_ValueList) {
		r.Alpha = Color::ALPHA_MIN;
	}
	m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MAX;

	// ���ʉ��̐ݒ�.
	m_IsPlayDigitSE.resize( DIGIT_SE_MAX );
	m_DigitSE = DIGIT_1_SE;
	m_ThroughStringCnt = 0;

	// �t�H���g���擾����.
	m_pFont = CFontResource::GetFont("NasuM");
	for (auto& f : m_FontStateList) {
		// �t�H���g�̕`������擾����.
		f = m_pFont->GetRenderState();
		// �������E�����ɂ���.
		f.TextAlign = ETextAlign::Right;
		// �����̐F�����F�ɂ���.
		f.Color = Color4::Black;
	}

	m_FontStateList[WageNumber].Transform.Position.x	= 0.0f;
	m_FontStateList[WageNumber].Transform.Position.y	= 150.0f;
	m_FontStateList[WageNumber].Transform.Scale			= { 2.0f, SCALE, 1.0f };

	m_FontStateList[WageYen].Text						= "��";
	m_FontStateList[WageYen].Transform.Position.x		= -30.0f;
	m_FontStateList[WageYen].Transform.Position.y		= 150.0f;
	m_FontStateList[WageYen].Transform.Scale			= { 2.0f, SCALE, 1.0f };

	m_FontStateList[DetailString].TextAlign				= ETextAlign::Left;
	m_FontStateList[DetailString].Transform.Position.x	= 200.0f;

	m_FontStateList[DetailNumber].Transform.Position.x	= 850.0f;
	m_FontStateList[DetailNumber].Transform.Scale		= { 1.0f, 2.0f, 1.0f };

	// �����L���O�̕ۑ�.
	if ( CScore::GetGameEnd() ) {
		CRanking::AddTreatmentPay( TREATMENT_PAY_VALUE );
	}
	CScore::SaveLog();
	CRanking::Save();

	// SE�̍Đ�.
	CSoundManager::PlaySE( DRUMROLL_SE );

	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CGameClear::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	// BGM�̍Đ�.
	CSoundManager::PlayBGM( m_BGMName );

	// UI�̍X�V.
	m_pGameClearWidget->Update( DeltaTime );

	if ( m_IsStartDisp ) {
		// �s�����ɂ��Ă���.
		AddAlpha();

		if ( m_CanOperate ) {
			// �J�[�\���̈ړ�����.
			m_SpriteStateList[Yubi].Transform.Position.y =
				590.0f + m_Choice * 90.0f;

			// �I��.
			if ( CInput::IsKeyDown( "Up" ) ) {
				// �J�[�\�����グ��.
				--m_Choice;
				if ( m_Choice < 0 )m_Choice = 1;

				// SE�̍Đ�.
				CSoundManager::PlaySE( CURSOR_MOVE_SE );
			}
			else if ( CInput::IsKeyDown( "Down" ) ) {
				// �J�[�\����������.
				++m_Choice;
				if ( m_Choice > 1 )m_Choice = 0;

				// SE�̍Đ�.
				CSoundManager::PlaySE( CURSOR_MOVE_SE );
			}
			// ����.
			else if ( CInput::IsKeyDown( "Decision" ) ) {
				CSoundManager::PlaySE( "Decision" );
				// �I�������擾����.
				switch ( m_Choice ) {
				case 0:
					// �Q�[�����C���ɑJ�ڂ���.
					CSceneManager::SceneChange( ESceneList::GameMain, EFadeType::MaskFade );
					// �Q�[���Ŏg�p���郉���L���O������������.
					CRanking::Init();
					break;
				case 1:
					// �^�C�g����ʂɖ߂�.
					CSceneManager::SceneChange( ESceneList::Title, EFadeType::MaskFade );
					break;
				default:
					break;
				}
			}
		}

		return;
	}

	const int State = static_cast<int>(m_ShowState);
	switch ( m_ShowState ) {
	case EShowState::OriginWage:		// �����\�����̏���.
		ShowOriginWage();
		break;
	case EShowState::HeatBonus:			// ���x�{�[�i�X�\�����̏���.
	case EShowState::GoldenTimeBonus:	// �S�[���f���^�C���{�[�i�X�\�����̏���.
	case EShowState::TiltDiscount:		// �X�������\�����̏���.
	case EShowState::TreatmentPay:		// ���Ô�\�����̏���.
		ShowDetails( State );
		break;
	case EShowState::FinalWage:			// �����\����̏���.
		UpdateAfterShowTotalWage();
		break;	
	default:
		break;
	}

	m_ValueList[ORIGIN_WAGE].Alpha += ALPHA_SPEED * m_DeltaTime;
	if ( m_ValueList[ORIGIN_WAGE].Alpha < Color::ALPHA_MAX ) return;
	m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MAX;
}

//---------------------------.
// ���f���̕`��.
//---------------------------.
void CGameClear::ModelRender()
{
}

//---------------------------.
// �X�v���C�g(UI)�̕`��.
//---------------------------.
void CGameClear::SpriteUIRender()
{
	// UI�̕`��.
	m_pGameClearWidget->Render();

	m_pSpriteList[BackGround]->RenderUI( &m_SpriteStateList[BackGround] );

	// �����̕\��.
	RenderOriginWage();

	// �u���v�̕\��.
	m_pFont->RenderUI( &m_FontStateList[WageYen] );

	// �ڍ׏��̕\��.
	RenderDetails();

	m_SpriteStateList[Choices_1].AnimState.AnimNumber = m_Choice;
	m_SpriteStateList[Choices_2].AnimState.AnimNumber = 3 - m_Choice;

	m_pSpriteList[Frame]->RenderUI( &m_SpriteStateList[Frame] );
	for ( int i = Yubi; i < Max; ++i ) {
		m_SpriteStateList[i].Color.w = m_Alpha;
		m_pSpriteList[i]->RenderUI( &m_SpriteStateList[i] );
	}

	// �����L���O�̕`��.
	CRanking::RankingRender(-80.0f);
}

//---------------------------.
// �X�v���C�g(3D)/Effect�̕`��.
//	_A�F��ɕ\������� / _B�F��ɕ\�������.
//---------------------------.
void CGameClear::Sprite3DRender_A()
{
}
void CGameClear::Sprite3DRender_B()
{
}

//---------------------------.
// �����̕`�揈��.
//---------------------------.
void CGameClear::RenderOriginWage()
{
	// �J���}�}������.
	std::string ValueStr = std::to_string( m_ValueList[ORIGIN_WAGE].Value );
	const int Size = static_cast<int>(ValueStr.length());

	// �u-�v��������΂���͌����Ɋ܂߂Ȃ�.
	int DeleteDigit = 0;
	if ( ValueStr.find( "-" ) != std::string::npos ) {
		++DeleteDigit;
	}
	int ConmaNumber = 0;
	// 3�����ƂɃJ���}������.
	for ( int i = Size - DeleteDigit - 3; i > 0; i -= 3 ) {
		const int InsertPos = i + DeleteDigit;
		ValueStr.insert( InsertPos, "," );
		++ConmaNumber;
	}

	// �������J��Ԃ�.
	for ( int Digit = Size + ConmaNumber - 1; Digit >= 0; --Digit ) {
		// �������Digit�Ԗڂ̗v�f���i�[.
		m_FontStateList[WageNumber].Text = ValueStr.substr( Digit, 1 );

		// �\�����W�����ɍ��킹�Ă��炷.
		m_FontStateList[WageNumber].Transform.Position.x = m_PosX[Digit];
		// ���ߏ���.
		m_FontStateList[WageNumber].Color.w = m_ValueList[ORIGIN_WAGE].Alpha;

		// �����̕`��.
		m_pFont->RenderUI( &m_FontStateList[WageNumber] );
	}
}

//---------------------------.
// �ڍ׏��̕`�揈��.
//---------------------------.
void CGameClear::RenderDetails()
{
	// ���@��������Ă��Ȃ��Ƃ��A�������m�̊Ԋu��������.
	const float AdjustY =
		CScore::GetGameEnd() == false ? STRING_SIZE_Y / 2.0f : 0.0f;

	for ( int i = 0; i < DETAIL_MAX; ++i ) {
		// ��\���Ȃ珈���𒆒f����.
		if ( m_ValueList[i].IsDisp == false ) return;

		float ScaleX = 2.0f;
		const float ScaleY = 2.0f;
		// �\���ʒu�̕ύX.
		m_FontStateList[DetailString].Transform.Position.y =
			m_FontStateList[DetailNumber].Transform.Position.y =
			300.0f + (50.0f + AdjustY * ScaleY) * i;

		// �����̑傫���̕ύX.
		if ( i == GOLDEN_TIME_BONUS )ScaleX /= 2.0f;
		m_FontStateList[DetailString].Transform.Scale = { ScaleX, ScaleY, 1.0f };
		// �\�����镶���̐ݒ�.
		std::string str = "";
		switch ( i ) {
		case HEAT_BONUS:		str = "���x�{�[�i�X";				break;
		case GOLDEN_TIME_BONUS:	str = "�S�[���f���^�C���{�[�i�X"; break;
		case TILT_DISCOUNT:		str = "�X������";				break;
		case TREATMENT_PAY:		str = "���@��";					break;
		default:												break;
		}
		m_FontStateList[DetailString].Text = str;
		m_FontStateList[DetailNumber].Color.w = m_ValueList[i].Alpha;
		// �����̕\��.
		m_pFont->RenderUI( &m_FontStateList[DetailString] );

		// �J���}�}������.
		std::string ValueStr = std::to_string( m_ValueList[i].Value );
		int Size = static_cast<int>(ValueStr.length());
		// �u-�v��������΂���͌����Ɋ܂߂Ȃ�.
		int DeleteDigit = 0;
		if ( ValueStr.find( "-" ) != std::string::npos ) {
			++DeleteDigit;
		}
		// 3�����ƂɃJ���}������.
		for ( int InsertNo = Size - DeleteDigit - 3; InsertNo > 0; InsertNo -= 3 ) {
			const int InsertPos = InsertNo + DeleteDigit;
			ValueStr.insert( InsertPos, "," );
		}

		// �\�����镶���̐ݒ�.
		m_FontStateList[DetailNumber].Text = ValueStr;
		// �����̕\��.
		m_pFont->RenderUI( &m_FontStateList[DetailNumber] );
	}
}

//---------------------------.
// �����\�����̏���.
//---------------------------.
void CGameClear::ShowOriginWage()
{
	// �w�肵�����̐������E�Ɉړ�������.
	const int DigitsNo = m_WageDigits - 1;
	m_PosX[DigitsNo] += WAGE_SPEED * m_DeltaTime;

	// �������w��ʒu�ɓ������Ă��Ȃ��A������{�^����������Ă��Ȃ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_PosX[DigitsNo] < m_WageFinalXPos[DigitsNo] && CInput::IsKeyDown( "Decision" ) == false ) return;
	m_PosX[DigitsNo] = m_WageFinalXPos[DigitsNo];
	m_ValueList[ORIGIN_WAGE].IntervalCount += INTERVAL_SPEED * m_DeltaTime;

	// �\�����Ă��镶�����R���}�ł͂Ȃ��A�܂�SE���Đ����Ă��Ȃ��Ȃ�.
	if ( m_ThroughStringCnt != 3 && m_ThroughStringCnt != 7 && m_IsPlayDigitSE[DigitsNo] == false ) {
		// SE���Đ�.
		CSoundManager::PlaySE( m_DigitSE );
		// �Đ�����SE��ύX����.
		m_DigitSE += "0";
		// �Đ��ς�.
		m_IsPlayDigitSE[DigitsNo] = true;
	}

	// �\����̑҂����Ԃ��I�����Ă��Ȃ��A������{�^����������Ă��Ȃ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_ValueList[ORIGIN_WAGE].IntervalCount < INTERVAL_MAX && CInput::IsKeyDown( "Decision" ) == false ) return;
	m_ValueList[ORIGIN_WAGE].IntervalCount = 0.0f;
	// �ړ������錅��1�����炷.
	m_WageDigits--;

	m_ThroughStringCnt++;

	// �܂��\�������錅���c���Ă���Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_WageDigits > 0 ) return;
	m_ValueList[HEAT_BONUS].IsDisp = true;
	m_ShowState = EShowState::HeatBonus;
}

//---------------------------.
// �����\����̏���.
//---------------------------.
void CGameClear::UpdateAfterShowTotalWage()
{
	const float EndPosX = m_WageFinalXPos[0] - STRING_SIZE_X * 1.5f;
	float* YenPosX = &m_FontStateList[WageYen].Transform.Position.x;
	// �u���v���E�Ɉړ�������.
	*YenPosX += WAGE_SPEED * m_DeltaTime;

	// �u���v���w��ʒu�ɓ������Ă��炸,����{�^���������Ă��Ȃ��Ȃ珈���𒆒f.
	if ( *YenPosX < EndPosX && CInput::IsKeyDown( "Decision" ) == false )return;

	// �u���v���w��ʒu�ɓ������邩,����{�^������������.
	// �ړ�������̍��W�ɂ���.
	*YenPosX = EndPosX;

	// SE�̍Đ�.
	CSoundManager::PlaySE( CASH_REGISTER_SE );

	// �I������\������.
	m_IsStartDisp = true;
}

//---------------------------.
// �e��{�[�i�X�\�����A����{�^���������ꂽ���̏���.
//---------------------------.
void CGameClear::PushButton( int Type )
{
	// ����{�^����������Ă��Ȃ��Ȃ珈�����s��Ȃ�.
	if ( CInput::IsKeyDown( "Decision" ) == false )return;

	// ����{�^�����������Ƃ�.
	// �ڍ׏��̕��������S�ɕ\������Ă��Ȃ��Ȃ�.
	if ( m_ValueList[Type].Alpha != Color::ALPHA_MAX ) {
		m_ValueList[Type].Alpha = Color::ALPHA_MAX;

		// �����𓧖��ɂ���.
		m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MIN;
		// �����Ƀ{�[�i�X,������������.
		m_ValueList[ORIGIN_WAGE].Value += m_ValueList[Type].Value;
		// �����̌����Đݒ肷��.
		ResetWageDigit();

		// �܂�SE���Đ����Ă��Ȃ��Ȃ�.
		if ( m_IsPlayGetBonusSE == false ) {
			// �\�����ꂽ�ڍ׏�񂪉��x�{�[�i�X,�S�[���f���^�C���{�[�i�X�Ȃ�.
			if ( Type == HEAT_BONUS || Type == GOLDEN_TIME_BONUS ) {
				// SE���Đ�����.
				CSoundManager::PlaySE( GET_BONUS_SE );
			}
			// �\�����ꂽ�ڍ׏�񂪌X������,���Ô�Ȃ�.
			else {
				// SE���Đ�����.
				CSoundManager::PlaySE( GET_PENALTY_SE );
			}
			// �Đ���.
			m_IsPlayGetBonusSE = true;
		}
	}
	// �ڍ׏��̕����\����A���������S�ɕ\������Ă��Ȃ��Ȃ�.
	else if ( m_ValueList[ORIGIN_WAGE].Alpha != Color::ALPHA_MAX ) {
		m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MAX;
	}
	// ���̏ڍ׏��ւ̑ҋ@���Ԃ��I�����Ă��Ȃ��Ȃ�.
	else if ( m_ValueList[Type].IntervalCount != INTERVAL_MAX ) {
		m_ValueList[Type].IntervalCount = INIT_FLOAT;
		const int Next = Type + 1;

		// �\�����Ă���ڍׂɉ����ď�����؂�ւ���.
		switch ( Type ) {
		case HEAT_BONUS:		// ���x�{�[�i�X.
		case GOLDEN_TIME_BONUS:	// �S�[���f���^�C���{�[�i�X.
			m_ValueList[Next].IsDisp = true;
			m_ShowState = static_cast<EShowState>(Next);
			break;
		case TILT_DISCOUNT:		// �X������.
			// �����A���Ô���݂���Ȃ�.
			if ( CScore::GetGameEnd() ) {
				m_ValueList[TREATMENT_PAY].IsDisp = true;
				m_ShowState = EShowState::TreatmentPay;
				break;
			}

			m_ShowState = EShowState::FinalWage;
			break;
		case TREATMENT_PAY:		// ���Ô�.
			m_ShowState = EShowState::FinalWage;
			break;
		default:
			break;
		}

		m_IsPlayGetBonusSE = false;
	}
}

//---------------------------.
// �ڍ׏��̒l�̓����x�𑝉������鏈��.
//---------------------------.
void CGameClear::AddAlpha( int Type )
{
	// ���łɏڍ׏��̒l���s�����Ȃ珈�����s��Ȃ�.
	if ( m_ValueList[Type].Alpha >= Color::ALPHA_MAX )return;
	//  �ڍ׏��̒l�̓����x�𑝉�������.
	m_ValueList[Type].Alpha += ALPHA_SPEED * m_DeltaTime;

	//  �ڍ׏��̒l���s�����łȂ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_ValueList[Type].Alpha < Color::ALPHA_MAX ) return;
	m_ValueList[Type].Alpha = Color::ALPHA_MAX;

	// �����𓧖��ɂ���.
	m_ValueList[ORIGIN_WAGE].Alpha = Color::ALPHA_MIN;
	// �����ɏڍ׏��̒l�����Z����.
	m_ValueList[ORIGIN_WAGE].Value += m_ValueList[Type].Value;
	// �����̌����Đݒ肷��.
	ResetWageDigit();

	// �܂�SE���Đ����Ă��Ȃ��Ȃ�.
	if ( m_IsPlayGetBonusSE == false ) {
		// �\�����ꂽ�ڍ׏�񂪉��x�{�[�i�X,�S�[���f���^�C���{�[�i�X�Ȃ�.
		if ( Type == HEAT_BONUS || Type == GOLDEN_TIME_BONUS ) {
			// SE���Đ�����.
			CSoundManager::PlaySE( GET_BONUS_SE );
		}
		// �\�����ꂽ�ڍ׏�񂪌X������,���Ô�Ȃ�.
		else {
			// SE���Đ�����.
			CSoundManager::PlaySE( GET_PENALTY_SE );
		}
		// �Đ���.
		m_IsPlayGetBonusSE = true;
	}
}

//---------------------------.
// �{�[�i�X,����������̏���.
//---------------------------.
void CGameClear::AfterAddDetails( int Type )
{
	// �ڍ׏��̒l�̐������s�����łȂ��A�������͓������s�����łȂ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_ValueList[Type].Alpha < Color::ALPHA_MAX || m_ValueList[ORIGIN_WAGE].Alpha != Color::ALPHA_MAX ) return;
	m_ValueList[Type].IntervalCount += INTERVAL_SPEED * m_DeltaTime;

	// �҂����Ԃ��I����Ă��Ȃ��Ȃ�Ȍ�̏����͍s��Ȃ�.
	if ( m_ValueList[Type].IntervalCount < INTERVAL_MAX ) return;
	m_ValueList[Type].IntervalCount = INIT_FLOAT;
	const int Next = Type + 1;

	// �\�����Ă���ڍׂɉ����ď�����؂�ւ���.
	switch ( Type ) {
	case HEAT_BONUS:		// ���x�{�[�i�X.
	case GOLDEN_TIME_BONUS:	// �S�[���f���^�C���{�[�i�X.
		m_ValueList[Next].IsDisp = true;
		m_ShowState = static_cast<EShowState>(Next);
		break;
	case TILT_DISCOUNT:		// �X������.
		// �����A���Ô���݂���Ȃ�.
		if ( CScore::GetGameEnd() ) {
			m_ValueList[TREATMENT_PAY].IsDisp = true;
			m_ShowState = EShowState::TreatmentPay;
			break;
		}

		m_ShowState = EShowState::FinalWage;
		break;
	case TREATMENT_PAY:		// ���Ô�.
		m_ShowState = EShowState::FinalWage;
		break;
	default:
		break;
	}

	m_IsPlayGetBonusSE = false;
}

//---------------------------.
// �ڍ׏��̏���.
//---------------------------.
void CGameClear::ShowDetails( int Type )
{
	PushButton( Type );
	AddAlpha( Type );
	AfterAddDetails( Type );
}

//---------------------------.
// �����̌����Đݒ肷��.
//---------------------------.
void CGameClear::ResetWageDigit()
{
	const std::string ValueStr = std::to_string( m_ValueList[ORIGIN_WAGE].Value );
	m_WageDigits = static_cast<int>(ValueStr.length());
	// �u-�v��������΂���͌����Ɋ܂߂Ȃ�.
	int DeleteDigit = 0;
	if ( ValueStr.find( "-" ) != std::string::npos ) {
		++DeleteDigit;
	}

	m_WageDigits += (m_WageDigits - DeleteDigit - 1) / 3;
	m_PosX.resize( m_WageDigits );
	m_WageFinalXPos.resize( m_WageDigits );
	for ( int i = 0; i < m_WageDigits; ++i ) {
		m_WageFinalXPos[i] = 850.0f - (((m_WageDigits - 1) - i) * STRING_SIZE_X);
		m_PosX[i] = m_WageFinalXPos[i];
	}
}

//---------------------------.
// UI�̕s����������.
//---------------------------.
void CGameClear::AddAlpha()
{
	if ( m_CanOperate )return;
	if ( m_IsStartDisp == false )return;

	m_Alpha += m_DeltaTime;
	if ( m_Alpha >= Color::ALPHA_MAX ) {
		m_Alpha = Color::ALPHA_MAX;

		m_CanOperate = true;
	}
}
