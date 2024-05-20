#pragma once
#include "..\..\Global.h"
#include <random>
#include <wincrypt.h>

/************************************************
*	�����_���N���X.
*		���c�F�P.
**/
class CRandom final
{
public:
	CRandom();
	~CRandom();

	// �C���X�^���X�̎擾.
	static CRandom* GetInstance();

	// �����̏�����.
	static HRESULT Init();

	// �������Œ肷�邩.
	static void IsRandLock( bool Flag ) { GetInstance()->m_RandLock = Flag; }

	// �����_���Ȑ��l( float�^ )���擾.
	//	Min�ȏ�AMax������Ԃ�.
	static float GetRand( float Min, float Max );
	static float GetRand( std::pair<float, float> Rand );
	// �����_���Ȑ��l( int�^ )���擾.
	//	Min�ȏ�AMax�ȉ���Ԃ�.
	static int	 GetRand( int	Min,  int  Max );
	static int	 GetRand( std::pair<int, int> Rand );
	// �����_���ȗv�f��Ԃ�.
	template<class T>
	static T GetRand( const std::vector<T>& Vector );

	// �m��( ���� or �Z/�Z ).
	static bool Probability( int	Molecule, int	Denominator );
	static bool Probability( float	Molecule, float Denominator );
	static bool Probability( float	Ratio );

private:
	// �����_���̐ݒ�.
	std::random_device	m_rnd;
	std::mt19937		m_mt;

	// �������Œ肷�邩.
	bool				m_RandLock;

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CRandom( const CRandom & )				= delete;
	CRandom& operator = ( const CRandom & ) = delete;
	CRandom( CRandom && )					= delete;
	CRandom& operator = ( CRandom && )		= delete;
};


//---------------------------.
// �����_���ȗv�f��Ԃ�.
//---------------------------.
template<class T>
inline T CRandom::GetRand( const std::vector<T>& Vector ) {
	// �z�񂪋�.
	if ( Vector.empty() ) {
		ErrorMessage( "�z�񂪋�ł�" );
	}

	// �����_���ŕԂ��v�f��I��.
	const int Max = static_cast<int>( Vector.size() ) - 1;
	return Vector[GetRand( 0, Max )];
}
