#include "WindUI.h"
#include "..\..\..\..\Actor\Wind\Wind.h"
#include "..\..\..\..\..\..\Utility\Random\Random.h"

CWindUI::CWindUI()
	: m_pWindLine				( nullptr )
	, m_WindLineStateBase		()
	, m_WindLineStateList		()
	, m_DispWindLineTimeCnt		( INIT_FLOAT )
{
}

CWindUI::~CWindUI()
{
}

//---------------------------.
// ������.
//---------------------------.
bool CWindUI::Init()
{
	// ���̐��̎擾.
	m_pWindLine = CSpriteResource::GetSprite( "WindLine", &m_WindLineStateBase );
	return true;
}

//---------------------------.
// �X�V.
//---------------------------.
void CWindUI::Update( const float& DeltaTime )
{
	m_DeltaTime = DeltaTime;

	WindLineMove();		// ���̐��̈ړ�.
	ListDelete();		// ���X�g�̍폜.
	SpownWindLine();	// ���̐�����������.
}

//---------------------------.
// �`��.
//---------------------------.
void CWindUI::Render()
{
	for ( auto& w : m_WindLineStateList ) m_pWindLine->RenderUI( &w );
}

//---------------------------.
// ���̐��̈ړ�.
//---------------------------.
void CWindUI::WindLineMove()
{
	// ���̏��̎擾.
	const bool  IsWindLeft	= CWind::GetWindDirection() == "Left";
	const float WindSpeed	= CRandom::GetRand( CONSTANT.WIND_LINE_SPPED_MIN, CONSTANT.WIND_LINE_SPPED_MAX ) + 10.0f * CWind::GetWindPower();
	const float AddSpeed	= CONSTANT.WIND_LINE_WIND_POWER_ADD_SPPED * CWind::GetWindPower();

	// �_�̈ړ�.
	for ( auto& w : m_WindLineStateList ) {
		if ( w.IsDisp == false ) continue;
		if( IsWindLeft ) w.Transform.Position.x -= WindSpeed + AddSpeed;
		else			 w.Transform.Position.x += WindSpeed + AddSpeed;

		// ��ʊO�Ɉړ�������.
		if ( w.Transform.Position.x <= -w.Transform.Scale.x || w.Transform.Position.x >= FWND_W ) {
			w.IsDisp = false;
		}
	}
}

//---------------------------.
// ���̐�������.
//---------------------------.
void CWindUI::SpownWindLine()
{
	if ( CWind::GetIsWind() == false ) return;

	// �_���������邩.
	m_DispWindLineTimeCnt += m_DeltaTime;
	if ( m_DispWindLineTimeCnt < CONSTANT.WIND_LINE_DISP_SPEED ) return;
	m_DispWindLineTimeCnt = INIT_FLOAT;

	// �T�C�Y�̃����_���ŕύX.
	const float ScaleX = CRandom::GetRand( CONSTANT.WIND_LINE_SCALE_MIN, CONSTANT.WIND_LINE_SCALE_MAX );

	// �\���ʒu�̎擾.
	D3DXPOSITION3	SpownPos	= INIT_FLOAT3;
	if ( CWind::GetWindDirection() == "Left" )	SpownPos.x =  FWND_W;
	else										SpownPos.x = -ScaleX;
	SpownPos.y = CRandom::GetRand( 0.0f, FWND_H );

	// �A���t�@�l�������_���őI��.
	const float Alpha = CRandom::GetRand( CONSTANT.WIND_LINE_ALPHA_MIN, CONSTANT.WIND_LINE_ALPHA_MAX );

	// �g�p���Ă��Ȃ��̂�����Ύg�p����.
	for ( auto& w : m_WindLineStateList ) {
		if ( w.IsDisp ) continue;

		// �A�j���[�V�������J�n������.
		w = m_WindLineStateBase;
		w.Transform.Position	= SpownPos;
		w.Transform.Scale.x		= ScaleX;
		w.Color.w				= Alpha;
		return;
	}

	// �V�����쐬.
	m_WindLineStateList.emplace_back( m_WindLineStateBase );
	m_WindLineStateList.back().Transform.Position	= SpownPos;
	m_WindLineStateList.back().Transform.Scale.x	= ScaleX;
	m_WindLineStateList.back().Color.w				= Alpha;
}

//---------------------------.
// �_���X�g�̍폜.
//---------------------------.
void CWindUI::ListDelete()
{
	// ���̐��̍폜.
	for ( int i = static_cast<int>( m_WindLineStateList.size() ) - 1; i >= 0; i-- ) {
		if ( m_WindLineStateList[i].IsDisp ) break;
		m_WindLineStateList.pop_back();
	}
}