#include "Ranking.h"
#include "..\..\..\Resource\SpriteResource\SpriteResource.h"
#include "..\..\..\Resource\FontResource\FontResource.h"
#include "..\..\..\Object\GameObject\Widget\UIWidget\GameMain\NotificationUI\NotificationUI.h"
#include "..\Score.h"
#include "..\..\..\Scene\SceneManager\SceneManager.h"

namespace {
	constexpr char	FILE_PATH[]			= "Data\\Parameter\\Main\\Ranking.bin";
	constexpr int	GRADE_MAX			= static_cast<int>( EGrade::Max );
	constexpr int	GRADE_RANK_MAX		= static_cast<int>( EGrade::Rank_Max );
	constexpr int	GRADE_FIRST			= static_cast<int>( EGrade::First );
	constexpr int	GRADE_SECOND		= static_cast<int>( EGrade::Second );
	constexpr int	GRADE_THIRD			= static_cast<int>( EGrade::Third );
	constexpr int	GRADE_FOURTH		= static_cast<int>( EGrade::Fourth );
	constexpr int	GRADE_FIFTH			= static_cast<int>( EGrade::Fifth );
	constexpr int	GRADE_NOW			= static_cast<int>( EGrade::Now );
}

CRanking::CRanking()
	: m_Ranking		( GRADE_MAX )
	, m_pFont		( nullptr )
	, m_FontState	()
	, m_No			( GRADE_NOW )
{
	// �t�H���g�̎擾.
	m_pFont		= CFontResource::GetFont( "NasuM", &m_FontState );
	m_FontState.Transform.Scale = { 0.6f, 0.6f, 0.6f };
	m_FontState.IsBold = true;
	m_FontState.OutLineSize = 1.0f;
}

CRanking::~CRanking()
{
}

//---------------------------.
// �C���X�^���X�̎擾.
//---------------------------.
CRanking* CRanking::GetInstance()
{
	static std::unique_ptr<CRanking> pInstance = std::make_unique<CRanking>();
	return pInstance.get();
}

//---------------------------.
// �t�@�C���̓ǂݍ���.
//---------------------------.
void CRanking::Load()
{
	CRanking* pI = GetInstance();

	// �X�R�A��ǂݍ���.
	int Data[GRADE_RANK_MAX];
	FileManager::BinaryLoad( FILE_PATH, Data );

	// �����L���O�̕ۑ�.
	pI->m_Ranking.resize( GRADE_MAX );
	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		pI->m_Ranking[i] = std::make_pair( Data[i], static_cast<EGrade>( i ) );
	}
	pI->m_Ranking[GRADE_NOW] = std::make_pair( 0, EGrade::Now );

	// �������L���O�������L���O�Ɠ����ɂ��Ă���.
	pI->m_ProvisionalRanking	= pI->m_Ranking;
}

//---------------------------.
// �t�@�C���̏�������.
//---------------------------.
void CRanking::Save()
{
	CRanking* pI = GetInstance();

	// ����̌��ʂ��m�肳����.
	pI->m_Ranking = pI->m_ProvisionalRanking;

	// �X�R�A����������.
	int Out[GRADE_RANK_MAX];
	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		Out[i] = pI->m_Ranking[i].first;
	}
	FileManager::BinarySave( FILE_PATH, Out );
}

//---------------------------.
// ���Z�b�g.
//---------------------------.
void CRanking::Reset()
{
	CRanking* pI = GetInstance();

	// �X�R�A����������.
	int Out[GRADE_RANK_MAX];
	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		pI->m_Ranking[i] = std::make_pair( 0, static_cast<EGrade>( i ) );
		Out[i]			 = 0;
	}
	pI->m_Ranking[GRADE_NOW] = std::make_pair( 0, EGrade::Now );
	FileManager::BinarySave( FILE_PATH, Out );

	// �������L���O�������L���O�Ɠ����ɂ��Ă���.
	pI->m_ProvisionalRanking = pI->m_Ranking;
}

//---------------------------.
// ����������.
//---------------------------.
void CRanking::Init()
{
	CRanking* pI = GetInstance();

	for ( int i = 0; i < GRADE_RANK_MAX; ++i ) {
		// ���ʂ�ݒ肷��.
		pI->m_Ranking[i].second = static_cast<EGrade>(i);
	}
	// ���݂̃X�R�A�͏���������.
	pI->m_Ranking[GRADE_NOW] = std::make_pair( 0, EGrade::Now );

	// �������L���O�������L���O�Ɠ����ɂ��Ă���.
	pI->m_ProvisionalRanking = pI->m_Ranking;
}

//---------------------------.
// �����L���O�̍X�V.
//---------------------------.
void CRanking::Update()
{
	CRanking* pI = GetInstance();

	// ���̃����L���O�ɕۑ����Ă���.
	pI->m_ProvisionalRanking	= pI->m_Ranking;
	const int NowScore			= CScore::GetScore().EndMoney;
	pI->m_ProvisionalRanking[GRADE_NOW].first	= NowScore;
	pI->m_ProvisionalRanking[GRADE_NOW].second	= static_cast<EGrade>( EGrade::Now );

	// �����L���O���X�V����Ȃ����,�Ȍ�̏����͍s��Ȃ�.
	if ( NowScore <= pI->m_ProvisionalRanking[GRADE_FIFTH].first ) return;
	// �\�[�g���s��.
	std::sort( pI->m_ProvisionalRanking.rbegin (), pI->m_ProvisionalRanking.rend () );

	// ���݂̏��ʂ�ۑ�����.
	const int OldNo = pI->m_No;
	// ���ʂ̐��������[�v����.
	for ( int i = 0; i < GRADE_MAX; ++i ) {
		if ( pI->m_ProvisionalRanking[i].second != EGrade::Now ) continue;
		pI->m_No = i;
		break;
	}
	if ( pI->m_No >= OldNo )return;

	// �����L���O�̍X�V�����ꂽ���Ƃ�ʒm����.
	std::string Message = "�����L���O�X�V!!\n" + std::to_string( pI->m_No + 1 ) + "�ʂɃ����N�C��!!";
	CNotificationUI::PushMessage( EMessageType::RankingUpdate, Message );
}

//---------------------------.
// �����L���O�̕`��.
//---------------------------.
void CRanking::RankingRender( const float AdjustX, const float AdjustY )
{
	CRanking* pI = GetInstance();

	// �薼�̕\��.
	pI->m_FontState.TextAlign				= ETextAlign::Left;
	pI->m_FontState.Color					= Color4::Black;
	pI->m_FontState.Transform.Position.x	= WND_W - 220.0f + AdjustX;
	pI->m_FontState.Transform.Position.y	= WND_H - 370.0f + AdjustY;
	pI->m_pFont->RenderUI( "���������L���O", &pI->m_FontState );

	int s = 0;
	for ( auto& [num, No] : pI->m_ProvisionalRanking ) {
		if ( No == EGrade::Now ) {
			s = num;
			break;
		}
	}

	bool IsViewNowScore = false;	// �����̏ꍇ,1�ԏゾ���Ԃ�����̂ɕK�v.
	std::string RendStr;
	// 1�ʂ���5�ʂ܂ŕ`��.
	for ( int No = GRADE_FIRST; No < GRADE_RANK_MAX; ++No ) {
		pI->m_FontState.Transform.Position.y = WND_H - 290.0f + No * 35.0f + AdjustY;

		// �����̕`��.
		pI->m_FontState.TextAlign				= ETextAlign::Left;
		pI->m_FontState.Color = Color4::Black;
		pI->m_FontState.Transform.Position.x	= WND_W - 220.0f + AdjustX;
		RendStr = std::to_string( No + 1 ) + "�ʁF��";
		pI->m_pFont->RenderUI( RendStr, &pI->m_FontState );

		if ( CSceneManager::GetNowScene() != ESceneList::Title &&
			IsViewNowScore == false &&
			pI->m_ProvisionalRanking[No].first == s ) {
			pI->m_FontState.Color = Color4::Red;

			// �Ȍ�A�����͍��F�ŕ\�������.
			IsViewNowScore = true;
		}

		// �X�R�A��3�����ƂɃJ���}������.
		std::string Text = std::to_string( pI->m_ProvisionalRanking[No].first );
		for ( int i = static_cast<int>( Text.length() ) - 3; i > 0; i -= 3 ) {
			Text.insert( i, "," );
		}

		// �����̕`��.
		pI->m_FontState.TextAlign				= ETextAlign::Right;
		pI->m_FontState.Transform.Position.x	= WND_W - 35.0f + AdjustX;
		pI->m_pFont->RenderUI( Text, &pI->m_FontState );
	}
}

// ���Ô�̉��Z.
void CRanking::AddTreatmentPay( const int TreatmentPay )
{
	CRanking* pI = GetInstance();

	pI->m_ProvisionalRanking[pI->m_No].first += TreatmentPay;

	// �\�[�g���s��.
	std::sort( pI->m_ProvisionalRanking.rbegin(), pI->m_ProvisionalRanking.rend() );
}
