#pragma once
#include "..\..\..\Utility\FileManager\FileManager.h"
#include "..\..\..\Common\Sprite\Sprite.h"
#include "..\..\..\Common\Font\Font.h"

namespace {
	// ���ʗ񋓑�.
	enum class enGrade : unsigned char {
		First,				// 1��.
		Second,				// 2��.
		Third,				// 3��.
		Fourth,				// 4��.
		Fifth,				// 5��.

		Now,				// ���݂̃X�R�A.

		Max,				// �v�f��.

		Rank_Max = Fifth + 1// ���ʂ̐�.
	} typedef EGrade;
}

/************************************************
*	�����L���O�N���X.
*		��J��.
**/
class CRanking final
{
public:
	using RankInt = std::vector<std::pair<int, EGrade>>;

public:
	CRanking();
	~CRanking();

	// �t�@�C���̓ǂݍ���.
	static void Load();

	// �t�@�C���̏�������.
	static void Save();

	// ���Z�b�g.
	static void Reset();

	// ����������.
	static void Init();

	// �����L���O�̍X�V.
	static void Update();

	// �����L���O�̕`��.
	static void RankingRender( const float AdjustX = 0.0f, const float AdjustY = 0.0f );

	// ���Ô�̉��Z.
	static void AddTreatmentPay( const int TreatmentPay );

private:
	// �C���X�^���X�̎擾.
	static CRanking* GetInstance();

private:
	RankInt				m_Ranking;				// �����L���O.
	RankInt				m_ProvisionalRanking;	// �������L���O.
	
	CFont*				m_pFont;
	SFontRenderState	m_FontState;

	int					m_No;					// ���݂̏���.
};