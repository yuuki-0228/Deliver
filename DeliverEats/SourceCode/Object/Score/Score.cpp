#include "Score.h"
#include "..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\Resource\FontResource\FontResource.h"
#include "Ranking\Ranking.h"

namespace {
	constexpr float FSCALE_WAGE				= 0.8f;	// �����̕����̑傫��.
	constexpr float FSCALE_WAGE_DETAILS		= 0.6f;	// ��������̕����̑傫��.
	constexpr char	SCORE_LIST_FILE_PATH[]	= "Data\\Parameter\\Main\\PlayData.csv";	// �X�R�A���X�g�f�[�^�̃t�@�C���p�X.

	enum enFontNo :unsigned char{
		Wage,			// ����.
		WageDetailStr,	// �����ڍׂ̕���.
		WageDetailNum,	// �����ڍׂ̐���.

		Font_Max
	}typedef EFontNo;
}

CScore::CScore ()
	: m_DeltaTime				( INIT_FLOAT )
	, m_TreatmentPay			( INIT_INT )
	, m_FinalWage				()
	, m_Alpha					( Color::ALPHA_MIN )
	, m_EndFlag					( false )
	, m_HomeFlag				( false )
	, m_FoodState				()
	, m_pWageDetailsFrame		( nullptr )
	, m_WageDetailsFrameState	()
	, m_IsDisp					( false )
	, m_ViewTime				( INIT_FLOAT )
	, m_AddScroll				( INIT_FLOAT )
	, m_IsReflect				( false )
	, m_pFont					( nullptr )
	, m_FontStateList			( Font_Max )

	, m_ViewFinalWage			( "0" )
	, m_PlayDataList			()
{
	// �v���C�f�[�^�̎擾.
	std::vector<std::string> Data = FileManager::TextLoad( SCORE_LIST_FILE_PATH );
	const int Size = static_cast<int>( Data.size() ) - 28;
	m_PlayDataList.resize( static_cast<int>( Size / 8 ) );
	for ( int i = 0; i < Size / 8; ++i ) {
		m_PlayDataList[i].EndScore		= std::stoi( Data[i * 8 + 28] );
		m_PlayDataList[i].GetScore		= std::stoi( Data[i * 8 + 29] );
		m_PlayDataList[i].HotBonus		= std::stoi( Data[i * 8 + 30] );
		m_PlayDataList[i].GoldenBonus	= std::stoi( Data[i * 8 + 31] );
		m_PlayDataList[i].DiscountMoney = std::stoi( Data[i * 8 + 32] );
		if (		Data[i * 8 + 33] != "" ) m_PlayDataList[i].EndNo = 0;
		else if (	Data[i * 8 + 34] != "" ) m_PlayDataList[i].EndNo = 1;
		else if (	Data[i * 8 + 35] != "" ) m_PlayDataList[i].EndNo = 2;
	}
}

CScore::~CScore ()
{
}

//---------------------------.
// ������ݒ�.
//---------------------------.
void CScore::SetScore( const SFoodState& Wage )
{
	CScore* pI = GetInstance();

	pI->m_FoodState = Wage;
	// �����̓����\������.
	pI->m_IsDisp	= true;
	// �����̓����\�����鎞�Ԃ�ݒ�.
	pI->m_ViewTime = 5.0f * 60.0f;

	pI->m_AddScroll = -130.0f;
	pI->m_IsReflect = false;
	// �����x��ݒ�.
	pI->m_Alpha		= Color::ALPHA_MIN;

	// ���������Z.
	pI->m_FinalWage.EndMoney		+= Wage.EndMoney;
	pI->m_FinalWage.HotBonus		+= Wage.HotBonus;
	pI->m_FinalWage.GoldenBonus		+= Wage.GoldenBonus;
	pI->m_FinalWage.DiscountMoney	+= Wage.DiscountMoney;

	// 3�����ƂɃJ���}������.
	pI->m_ViewFinalWage = std::to_string( pI->m_FinalWage.EndMoney );
	for ( int i = static_cast<int>(pI->m_ViewFinalWage.length()) - 3; i > 0; i -= 3 ) {
		pI->m_ViewFinalWage.insert( i, "," );
	}

	// �����L���O�̍X�V.
	CRanking::Update();
}

//---------------------------.
// ������.
//---------------------------.
void CScore::Init()
{
	CScore* pI = GetInstance();

	// ������.
	pI->m_TreatmentPay			= INIT_INT;
	pI->m_FinalWage				= SFoodState();
	pI->m_Alpha					= Color::ALPHA_MIN;
	pI->m_EndFlag				= false;
	pI->m_HomeFlag				= false;
	pI->m_FoodState				= SFoodState();
	pI->m_IsDisp				= false;
	pI->m_ViewTime				= INIT_FLOAT;
	pI->m_AddScroll				= INIT_FLOAT;
	pI->m_IsReflect				= false;
	pI->m_ViewFinalWage			= "0";

	// �t�H���g�̎擾.
	pI->m_pFont = CFontResource::GetFont( "NasuM" );
	for ( auto& f : pI->m_FontStateList ) {
		f = pI->m_pFont->GetRenderState();
		// �������E�����ɂ���.
		f.TextAlign = ETextAlign::Right;
	}

	pI->m_FontStateList[WageDetailStr].Color = Color4::Black;

	pI->m_FontStateList[WageDetailStr].RenderArea = { WND_W - 300.0f, 32.0f, 300.0f, 256.0f };
	pI->m_FontStateList[WageDetailNum].RenderArea = { WND_W - 300.0f, 32.0f, 300.0f, 256.0f };

	pI->m_FontStateList[WageDetailNum].IsOutLine = true;
	pI->m_FontStateList[WageDetailNum].OutLineColor = Color4::Black;
	pI->m_FontStateList[WageDetailNum].OutLineSize = 2.0f;

	// �\���̂̏�����.
	pI->m_FoodState = SFoodState();

	pI->m_pWageDetailsFrame = CSpriteResource::GetSprite( "WageDetailsFrame", &pI->m_WageDetailsFrameState );
	pI->m_WageDetailsFrameState.Transform.Position.x = WND_W - 310.0f;
	pI->m_WageDetailsFrameState.Transform.Position.y = INIT_FLOAT;

	pI->m_WageDetailsFrameState.RenderArea = { WND_W - 310.0f, 32.0f, 310.0f, 256.0f };
	
	// �����̓����\������.
	pI->m_IsDisp	= false;
	// �����̓����\�����鎞�Ԃ�ݒ�.
	pI->m_ViewTime	= 0.0f;
	pI->m_AddScroll = -130.0f;
	pI->m_IsReflect = false;

	// �����x��ݒ�.
	pI->m_Alpha		= Color::ALPHA_MAX;

	// �����I���t���O��������.
	pI->m_EndFlag	= false;
	pI->m_HomeFlag	= false;

	// ������������.
	pI->m_FinalWage.EndMoney = INIT_INT;
}

//---------------------------.
// �X�V.
//---------------------------.
void CScore::Update ( const float& DeltaTime )
{
	CScore* pI = GetInstance();

	pI->m_DeltaTime = DeltaTime;

	pI->WageDetailsScroll();
	
	// �����̕��������S�s�����Ȃ�Έȉ��̏����͍s��Ȃ�.
	if ( pI->m_Alpha == Color::ALPHA_MAX )return;
	pI->m_Alpha += 5.0f * pI->m_DeltaTime;
	if ( pI->m_Alpha >= Color::ALPHA_MAX )pI->m_Alpha = Color::ALPHA_MAX;
}

//---------------------------.
// �`��.
//---------------------------.
void CScore::Render()
{
	CScore* pI = GetInstance();

	// �����̓��󂪕\������Ă���ꍇ�A�����̓���̕\��.
	if ( pI->m_IsDisp ) pI->RenderWageDetails();
	
	// �����̕\��.
	pI->RenderWage();
}

//---------------------------.
// ���O�̏o��.
//---------------------------.
void CScore::SaveLog()
{
	CScore* pI = GetInstance();

	// �v���C�f�[�^�̒ǉ�.
	SPlayData NowData;
	if ( pI->m_EndFlag ) NowData.EndScore	= pI->m_FinalWage.EndMoney - 200000;
	else				 NowData.EndScore	= pI->m_FinalWage.EndMoney;
	NowData.GetScore		= pI->m_FinalWage.EndMoney;
	NowData.HotBonus		= pI->m_FinalWage.HotBonus;
	NowData.GoldenBonus		= pI->m_FinalWage.GoldenBonus;
	NowData.DiscountMoney	= pI->m_FinalWage.DiscountMoney;
	if (		pI->m_EndFlag  ) NowData.EndNo = 2;
	else if (	pI->m_HomeFlag ) NowData.EndNo = 1;
	else						 NowData.EndNo = 0;
	pI->m_PlayDataList.emplace_back( NowData );

	// �v���C�f�[�^�̕ۑ�.
	std::string ScoreText = 
		"�v���C��,=COUNT(A3:A100000),�N���A��,=COUNT(F3:F100000),�A���,=COUNT(G3:G100000),���̉�,=COUNT(H3:H100000),"
		"�ő�X�R�A,=MAX(A3:A100000),���σX�R�A,=AVEDEV(A3:A100000),���ώ擾�X�R�A,=AVEDEV(B3:B100000),"
		"���ω��x�{�[�i�X,=AVEDEV(C3:C100000),���σS�[���f���^�C���{�[�i�X,=AVEDEV(D3:D100000),���όX������,=AVEDEV(E3:E100000)\n"
		"�ŏI�X�R�A,�l���X�R�A,���x�{�[�i�X,�S�[���f���^�C���{�[�i�X,�X������,�N���A,�A��,����,";
	for ( auto& d : pI->m_PlayDataList ) {
		ScoreText += "\n" +
			std::to_string( d.EndScore ) + "," + std::to_string( d.GetScore ) + "," + std::to_string( d.HotBonus ) + "," +
			std::to_string( d.GoldenBonus ) + "," + std::to_string( d.DiscountMoney ) + ",";
		if (		d.EndNo == 0 ) ScoreText += "1,,,";
		else if (	d.EndNo == 1 ) ScoreText += ",1,,";
		else if (	d.EndNo == 2 ) ScoreText += ",,1,";
	}
	FileManager::TextSave( SCORE_LIST_FILE_PATH, ScoreText );
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CScore* CScore::GetInstance ()
{
	static std::unique_ptr<CScore> pInstance = std::make_unique<CScore>();
	return pInstance.get();
}

//---------------------------.
// �����̕`�揈��.
//---------------------------.
void CScore::RenderWage()
{
	CScore* pI = GetInstance();
	
	SFontRenderState* pState = &pI->m_FontStateList[Wage];
	pState->Color = Color4::Black;
	pState->Transform.Position.y = 5.0f;
	pState->Transform.Scale = { FSCALE_WAGE, FSCALE_WAGE, 1.0f };

	// �����̕`��.
	pState->Transform.Position.x = WND_W - 280.0f * FSCALE_WAGE;
	pI->m_pFont->RenderUI( "����:��", pState );

	// �����̕`��.
	pState->Color.w = pI->m_Alpha;
	pState->Transform.Position.x = WND_W - 5.0f;
	pI->m_pFont->RenderUI( pI->m_ViewFinalWage, pState );
}

//---------------------------.
// �����̓���̕`�揈��.
//---------------------------.
void CScore::RenderWageDetails()
{
	CScore* pI = GetInstance();

	pI->m_pWageDetailsFrame->RenderUI( &pI->m_WageDetailsFrameState );

	SFontRenderState* pStrState = &pI->m_FontStateList[WageDetailStr];
	SFontRenderState* pNumState = &pI->m_FontStateList[WageDetailNum];

	// �K�v�ȏ��𒊏o����.
	std::vector<int> WageDetails;
	WageDetails.resize( 5 );
	WageDetails[0] = static_cast<int>( pI->m_FoodState.Money );
	WageDetails[1] = static_cast<int>( pI->m_FoodState.DiscountMoney );
	WageDetails[2] = static_cast<int>( pI->m_FoodState.HotBonus );
	WageDetails[3] = static_cast<int>( pI->m_FoodState.GoldenBonus );
	WageDetails[4] = static_cast<int>( pI->m_FoodState.EndMoney );

	// �����̓���̖��̃��X�g.
	std::pair<std::string, D3DXCOLOR4> DetailsName[] = {
		{ "���i���l",				Color4::RGBA( 0.0f, 0.9f, 0.0f )	},
		{ "�X������",				Color4::Red							},
		{ "���x�{�[�i�X",				Color4::RGBA( 0.0f, 0.9f, 0.0f )	},
		{ "�S�[���f���^�C���{�[�i�X", Color4::RGBA( 0.0f, 0.9f, 0.0f )		},
		{ "���v"	,					Color4::White						}
	};

	// �S�[���f���^�C���{�[�i�X��\�����Ȃ���.
	pStrState->Transform.Scale = pNumState->Transform.Scale = { FSCALE_WAGE_DETAILS, FSCALE_WAGE_DETAILS, 0.0f };
	int WDSize = 5;
	if ( pI->m_FoodState.GoldenBonus == 0 ) {
		WDSize			= 4;
		WageDetails[3]	= static_cast<int>( pI->m_FoodState.EndMoney );
		DetailsName[3]	= { "���v", Color4::White };
	}

	// ����̐��������[�v����.
	for ( int i = 0; i < WDSize; ++i ) {
		pStrState->Transform.Position.y = pNumState->Transform.Position.y = (pI->m_AddScroll + i * 35.0f) * FSCALE_WAGE_DETAILS;

		// �����̕`��.
		pStrState->Transform.Position.x = WND_W - 175.0f * FSCALE_WAGE_DETAILS;
		pI->m_pFont->RenderUI( DetailsName[i].first + ":", pStrState );

		// �����̕`��.
		pNumState->Color = DetailsName[i].second;
		pNumState->Transform.Position.x = WND_W - 5.0f;
		pI->m_pFont->RenderUI( "��" + std::to_string( WageDetails[i] ), pNumState );
	}

	pI->m_WageDetailsFrameState.Transform.Position.y = (pI->m_AddScroll + 35.0f * WDSize) * FSCALE_WAGE_DETAILS;
}

//---------------------------.
// �X�N���[������.
//---------------------------.
void CScore::WageDetailsScroll()
{
	CScore* pI = GetInstance();

	//if ( pI->m_IsDisp == false )return;

	if ( pI->m_IsReflect == true && pI->m_AddScroll <= -130.0f )return;

	// ����������X�N���[������.
	if ( pI->m_IsReflect )pI->m_AddScroll -= 1.0f;
	else pI->m_AddScroll += 1.0f;

	// ���l�܂ł����Ȃ�ҋ@������.
	if ( pI->m_AddScroll >= 60.0f ) {
		pI->m_AddScroll = 60.0f;
		pI->m_ViewTime -= 2.0f * 60.0f * pI->m_DeltaTime;
	}

	// �ҋ@���Ԃ����l�𒴂����Ȃ�ҋ@�I��.
	if ( pI->m_ViewTime <= 0.0f ) pI->m_IsReflect = true;
}
