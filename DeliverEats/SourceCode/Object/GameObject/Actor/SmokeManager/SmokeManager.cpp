#include "SmokeManager.h"
#include "Smoke\Smoke.h"
#include "..\..\..\Camera\CameraManager\CameraManager.h"

CSmokeManager::CSmokeManager()
	: m_pSmokeList	()
	, m_TimeCnt		( 0.0f )
{
	Init();
}

CSmokeManager::~CSmokeManager()
{
}

//------------------------------------.
// ������.
//------------------------------------.
bool CSmokeManager::Init()
{
	m_TimeCnt = 0.0f;
	return true;
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CSmokeManager::Update( const float& deltaTime )
{
	m_DeltaTime = deltaTime;

	for ( auto& s : m_pSmokeList ) s->Update( deltaTime );

	RenderOrder();		// �`�揇�Ԃ̕��ѕς�.
	SmokeListDelete();	// �����X�g�̍폜.
	CoolTimeUpdate();	// �N�[���^�C���̍X�V.
}

//------------------------------------.
// �X�V.
//------------------------------------.
void CSmokeManager::Render()
{
	for ( auto& s : m_pSmokeList ) s->Render();
}

//------------------------------------.
// �A�j���[�V�����̊J�n.
//	�҂����Ԃ�����ꍇ�������s��Ȃ�.
//------------------------------------.
void CSmokeManager::PlaySmoke( const D3DXPOSITION3& Pos, const D3DXVECTOR3& MoveVec, const D3DXVECTOR3& pAddMoveVec )
{
	if ( m_TimeCnt > 0.0f	) return;

	m_TimeCnt = CONSTANT.COOL_TIME;

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& s : m_pSmokeList ) {
		if ( s->IsAnimEnd() == false ) continue;

		// �A�j���[�V�������J�n������.
		s->AnimPlay( Pos, MoveVec, pAddMoveVec );
		return;
	}

	// �V�����쐬.
	m_pSmokeList.emplace_back( std::make_unique<CSmoke>() );

	// �A�j���[�V�������J�n������.
	m_pSmokeList.back()->AnimPlay( Pos, MoveVec, pAddMoveVec );
}

//------------------------------------.
// �`�揇�Ԃ���ѕς���.
//------------------------------------.
void CSmokeManager::RenderOrder()
{
	// �J�����̍��W���擾.
	const D3DXVECTOR3& CamPos = CCameraManager::GetPosition();

	// �G�t�F�N�g�ƃJ�����Ƃ̋����𒲂ׂ�.
	std::vector<std::pair<float, std::unique_ptr<CSmoke>>> LengthList;
	const int Size = static_cast<int>( m_pSmokeList.size() );
	for ( int i = 0; i < Size; i++ ){
		const D3DXVECTOR3 vLength = CamPos - m_pSmokeList[i]->GetPosition();
		LengthList.emplace_back( std::make_pair( D3DXVec3Length( &vLength ), std::move( m_pSmokeList[i] ) ) );
	}
	// �J�����ɉ������ɕ��בւ���.
	std::sort( LengthList.begin(), LengthList.end(), std::greater<>() );

	// �G�t�F�N�g����ѕς���.
	for ( int i = 0; i < Size; i++ )
		m_pSmokeList[i] = std::move( LengthList[i].second );
}

//------------------------------------.
// �����X�g�̍폜.
//------------------------------------.
void CSmokeManager::SmokeListDelete()
{
	for ( int i = static_cast<int>( m_pSmokeList.size() ) - 1; i >= 0; i-- ) {
		if ( m_pSmokeList[i]->IsAnimEnd() == false ) break;
		m_pSmokeList.pop_back();
	}
}

//------------------------------------.
// �N�[���^�C���̍X�V.
//	�҂����Ԃ��Ȃ��ꍇ�������s��Ȃ�.
//------------------------------------.
void CSmokeManager::CoolTimeUpdate()
{
	if ( m_TimeCnt == 0.0f ) return;
	m_TimeCnt -= m_DeltaTime;
	if ( m_TimeCnt <= 0.0f )
		m_TimeCnt = 0.0f;
}