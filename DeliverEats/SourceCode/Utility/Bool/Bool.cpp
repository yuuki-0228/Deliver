#include "Bool.h"

void CBool::operator=( const bool Flag )
{
	if ( Flag == true )
		CFlagManager::OnBitFlag( m_Handle );
	else
		CFlagManager::OffBitFlag( m_Handle );
}
void CBool::operator=( const char* sFlag )
{
	if ( sFlag == "true" )
		CFlagManager::OnBitFlag( m_Handle );
	else if ( sFlag == "false" )
		CFlagManager::OffBitFlag( m_Handle );
}
void CBool::operator=( const int iFlag )
{
	if ( iFlag != 0 )
		CFlagManager::OnBitFlag( m_Handle );
	else
		CFlagManager::OffBitFlag( m_Handle );
}

bool CBool::operator==( const bool Flag )
{
	return Flag == CFlagManager::IsBitFlag( m_Handle );
}

bool CBool::operator!=( const bool Flag )
{
	return Flag != CFlagManager::IsBitFlag( m_Handle );
}

bool CBool::operator!()
{
	return !CFlagManager::IsBitFlag( m_Handle );
}

CBool::CBool( bool Flag, std::string Name, std::string Group )
	: m_Handle( 0 )
{
	// フラグの作成.
	m_Handle = CFlagManager::CreateFlag( Name, Group );
	operator=( Flag );
}

CBool::~CBool()
{
	// このフラグは使わなくなったためフラグを削除する.
	CFlagManager::DeleteFlag( m_Handle );
	m_Handle = 0;
}
