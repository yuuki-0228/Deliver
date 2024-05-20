#include "SoundManeger.h"
#include "Sound\Sound.h"
#include "..\..\Object\Object.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\ImGuiManager\MessageWindow\MessageWindow.h"

namespace {
	// �T�E���h�t�@�C���p�X.
	constexpr char FILE_PATH[] = "Data\\Sound";

	// 3D�T�E���h�Ŏg�p���鉹�ʂ������Ă�������.
	constexpr float VOLUME_DOWN_LENGTH = 0.1f;

	// �ʏ펞�̉���.
	constexpr int NORMAL_VOLUME = 1000;
}

CSoundManager::CSoundManager()
	: m_pListener		( nullptr )
	, m_BGMList			()
	, m_SEList			()
	, m_BGMNames		()
	, m_SENames			()
	, m_NowBGMName		( "" )
	, m_Mutex			()
	, m_IsLoadEnd		( false )
	, m_LoopStopFlag	( false )
	, m_SelectNo		( -1 )
{
}

CSoundManager::~CSoundManager()
{
	m_BGMList.clear();
	m_SEList.clear();
}

//----------------------------.
// �C���X�^���X�̎擾.
//----------------------------.
CSoundManager* CSoundManager::GetInstance()
{
	static std::unique_ptr<CSoundManager> pInstance = std::make_unique<CSoundManager>();
	return pInstance.get();
}

//----------------------------.
// �T�E���h�̓ǂݍ���.
//----------------------------.
HRESULT CSoundManager::SoundLoad( HWND hWnd )
{
	CSoundManager* pI = GetInstance();
	std::unique_lock<std::mutex> Lock( pI->m_Mutex );

	auto SoundLoad = [&]( const std::filesystem::directory_entry& Entry )
	{
		const std::string Extension = Entry.path().extension().string();	// �g���q.
		const std::string FilePath	= Entry.path().string();				// �t�@�C���p�X.
		const std::string FileName	= Entry.path().stem().string();			// �t�@�C����.
		const std::string LoadMsg	= FilePath + " �ǂݍ��� : ����";			// ���炩���߃��[�h�������b�Z�[�W��ݒ肷��.

		// �g���q�� ".midi" / ".mp3" / ".wav" �ł͂Ȃ��ꍇ�ǂݍ��܂Ȃ�.
		if ( Extension != ".midi" && Extension != ".MIDI" &&
			 Extension != ".mp3"  && Extension != ".MP3"  &&
			 Extension != ".wav"  && Extension != ".WAV" ) return;

		// �������O��json�t�@�C���̓ǂݍ���.
		std::string TextPath = FilePath;
		TextPath.erase( TextPath.find( "." ), TextPath.size() ) += ".json";
		json j = FileManager::JsonLoad( TextPath );
		// json�t�@�C���̍쐬.
		if ( j.empty() ){
			CLog::PushLog( TextPath + " ���������ߍ쐬���܂��B" );
			j[".Comment"]	= {
				"----------------------------------------------",
				u8"[MaxVolume] : �ő�̉���( 0 ~ 1000 )",
				u8"[PlayMax]   : ���d�Đ��\�Ȑ�",
				u8"[SiteName]  : �_�E�����[�h���Ă����T�C�g��",
				u8"[SiteURL]   : �_�E�����[�h���Ă����T�C�g��URL",
				"----------------------------------------------"
			};
			j["MaxVolume"]	= 1000;
			j["PlayMax"]	= 1;
			j["SiteName"]	= "";
			j["SiteURL"]	= "";
			FileManager::JsonSave( TextPath, j );
		}

		// �t�@�C���p�X�� "BGM" ���Ȃ�����SE�Ƃ��ĕۑ�.
		const std::wstring wFilePath = StringConversion::to_wString( FilePath );
		if ( FilePath.find( "BGM" ) == std::string::npos ) {
			// �T�E���h�t�@�C���̓ǂݍ���.
			OpenSound( &pI->m_SEList[FileName], hWnd, j["PlayMax"], j["MaxVolume"], FileName, wFilePath);
			pI->m_SENames.emplace_back( FileName );
		}
		// �t�@�C���p�X�� "BGM" �����邽��BGM�Ƃ��ĕۑ�.
		else{
			// �T�E���h�t�@�C���̓ǂݍ���.
			OpenSound( &pI->m_BGMList[FileName], hWnd, j["PlayMax"], j["MaxVolume"], FileName, wFilePath );
			pI->m_BGMNames.emplace_back( FileName );
		}
		CLog::PushLog( LoadMsg.c_str() );
	};

	CLog::PushLog( "------ �T�E���h�ǂݍ��݊J�n -------" );
	try {
		std::filesystem::recursive_directory_iterator Dir_itr( FILE_PATH ), End_itr;
		std::for_each( Dir_itr, End_itr, SoundLoad );
	}
	catch ( const std::filesystem::filesystem_error& e ) {
		// �G���[���b�Z�[�W��\��.
		ErrorMessage( e.path1().string().c_str() );
		return E_FAIL;
	}
	CLog::PushLog( "------ �T�E���h�ǂݍ��ݏI�� -------" );

	// �ǂݍ��ݏI��.
	pI->m_IsLoadEnd = true;
	return S_OK;
}

//----------------------------.
// SE���Đ�����.
//	�w�肵���Ԋu���Đ��Ԋu���Z���ꍇ�͏����͍s��Ȃ�.
//----------------------------.
void CSoundManager::PlaySE( std::string Name, float Interval, bool IsStopPlay, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	// �w�肵���Ԋu(�~���b�ɕϊ�)�ƁA���݂̎����ƑO�ɍĐ����������̊Ԋu���r����.
	if ( Interval * 1000 > static_cast<float>( GetTickCount64() - pI->m_PlayTimeList[Name] ) ) return;

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		// SE�̍Đ�.
		pI->m_SEList[Name][No]->ResetVolume();
		pI->m_SEList[Name][No]->PlaySE( IsStopPlay );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���Đ����܂���", Color4::Cyan );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		if ( IsStopPlay == false && s->IsStopped() == false ) continue;

		// SE�̍Đ�.
		s->ResetVolume();
		s->PlaySE( IsStopPlay );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���Đ����܂���", Color4::Cyan );
		return;
	}

	if ( IsStopPlay ) return;
	pI->m_SEList[Name][0]->ResetVolume();
	pI->m_SEList[Name][0]->PlaySE( IsStopPlay );
	pI->m_PlayTimeList[Name] = GetTickCount64();
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���Đ����܂���", Color4::Cyan );
}

//----------------------------.
// SE���Đ�����.
//	���[�v���čĐ�����.
//----------------------------.
void CSoundManager::PlaySELoop( std::string Name, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		// SE���Đ�����.
		pI->m_SEList[Name][No]->ResetVolume();
		if ( pI->m_SEList[Name][No]->PlayLoop() == false ) return;
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ) {
		if ( s->IsStopped() == false ) continue;

		// SE���Đ�����.
		s->ResetVolume();
		if ( s->PlayLoop()	== false ) return;
		if ( IsMsgStop		== false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
		break;
	}
}

//----------------------------.
// 3D��SE���Đ�����.
//	�w�肵���Ԋu���Đ��Ԋu���Z���ꍇ�͏����͍s��Ȃ�.
//----------------------------.
void CSoundManager::PlaySE3D( std::string Name, D3DXPOSITION3 PlayPos, float Interval, bool IsStopPlay, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	// �w�肵���Ԋu(�~���b�ɕϊ�)�ƁA���݂̎����ƑO�ɍĐ����������̊Ԋu���r����.
	if ( Interval * 1000 > static_cast<float>( GetTickCount64() - pI->m_PlayTimeList[Name] ) ) return;

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
		const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
		const float Length = D3DXVec3Length( &ListenerVec );

		// ����邲�Ƃɉ��ʂ������Ă���.
		const int MaxVolume = pI->m_SEList[Name][No]->GetMaxVolume();
		pI->m_SEList[Name][No]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

		// SE�̍Đ�.
		pI->m_SEList[Name][No]->PlaySE( IsStopPlay );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v��3D�ōĐ����܂���", Color4::Cyan );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		if ( IsStopPlay == false && s->IsStopped() == false ) continue;

		// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
		const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
		const float Length = D3DXVec3Length( &ListenerVec );

		// ����邲�Ƃɉ��ʂ������Ă���.
		const int MaxVolume = s->GetMaxVolume();
		s->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

		// SE�̍Đ�.
		s->PlaySE( IsStopPlay );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v��3D�ōĐ����܂���", Color4::Cyan );
		break;
	}

	if ( IsStopPlay ) return;
	const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
	const float Length = D3DXVec3Length( &ListenerVec );

	// ����邲�Ƃɉ��ʂ������Ă���.
	const int MaxVolume = pI->m_SEList[Name][0]->GetMaxVolume();
	pI->m_SEList[Name][0]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

	// SE�̍Đ�.
	pI->m_SEList[Name][0]->PlaySE( IsStopPlay );
	pI->m_PlayTimeList[Name] = GetTickCount64();
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v��3D�ōĐ����܂���", Color4::Cyan );
}

//----------------------------.
// 3D��SE���Đ�����.
//	���[�v���Ďw�肵���Ԋu���Đ��Ԋu���Z���ꍇ�͏����͍s��Ȃ�.
//----------------------------.
void CSoundManager::PlaySELoop3D( std::string Name, D3DXPOSITION3 PlayPos, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
	const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
	const float Length = D3DXVec3Length( &ListenerVec );

	// ����邲�Ƃɉ��ʂ������Ă���.
	const int MaxVolume = pI->m_SEList[Name][0]->GetMaxVolume();
	pI->m_SEList[Name][0]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

	// �T�E���h�̍Đ�.
	if ( pI->m_SEList[Name][0]->PlayLoop() == false ) return;
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// BGM���Đ�����.
//	���[�v��~���̏ꍇ�����͍s��Ȃ�.
//----------------------------.
void CSoundManager::PlayBGM( std::string Name, bool OldBGMStop, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �����Ă���BGM���~�߂�.
	if ( OldBGMStop && pI->m_NowBGMName != "" && pI->m_NowBGMName != Name ) {
		pI->m_BGMList[pI->m_NowBGMName][0]->Stop();
	}

	// BGM���Đ�����.
	pI->m_BGMList[Name][0]->ResetVolume();
	if ( pI->m_BGMList[Name][0]->PlayLoop() == false ) return;
	if ( OldBGMStop ) pI->m_NowBGMName = Name;
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// BGM���t�F�[�h�Ő؂�ւ��čĐ�����.
//	���[�v��~���̏ꍇ�����͍s��Ȃ�.
//----------------------------.
void CSoundManager::PlayFadeBGM( std::string Name, int FadeSpeed, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �����Ă���BGM�����X�Ɏ~�߂�.
	if ( pI->m_NowBGMName != "" && pI->m_NowBGMName != Name ) {
		pI->m_BGMList[pI->m_NowBGMName][0]->AddVolume( -FadeSpeed );
		pI->m_BGMList[Name][0]->SetVolume( 
			pI->m_BGMList[Name][0]->GetMaxVolume() - pI->m_BGMList[Name][0]->GetMaxVolume() * (
			pI->m_BGMList[pI->m_NowBGMName][0]->GetVolume() / pI->m_BGMList[pI->m_NowBGMName][0]->GetMaxVolume() 
		) );

		// BGM���~�߂�.
		if ( pI->m_BGMList[pI->m_NowBGMName][0]->GetVolume() <= 0 ) {
			pI->m_BGMList[pI->m_NowBGMName][0]->ResetVolume();
			pI->m_BGMList[pI->m_NowBGMName][0]->Stop();
			pI->m_BGMList[Name][0]->ResetVolume();
			pI->m_NowBGMName = Name;
		}
	}

	// BGM���Đ�����.
	if ( pI->m_BGMList[Name][0]->PlayLoop() == false ) return;
	if ( pI->m_NowBGMName	== ""		) pI->m_NowBGMName = Name;
	if ( IsMsgStop			== false	) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// 3D��BGM���Đ�����.
//	���[�v��~���̏ꍇ�����͍s��Ȃ�.
//----------------------------.
void CSoundManager::PlayBGM3D( std::string Name, D3DXPOSITION3 PlayPos, bool OldBGMStop, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �����Ă���BGM���~�߂�.
	if ( OldBGMStop && pI->m_NowBGMName != "" && pI->m_NowBGMName != Name ) pI->m_BGMList[pI->m_NowBGMName][0]->Stop();

	// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
	const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
	const float Length = D3DXVec3Length( &ListenerVec );

	// ����邲�Ƃɉ��ʂ������Ă���.
	const int MaxVolume = pI->m_BGMList[Name][0]->GetMaxVolume();
	pI->m_BGMList[Name][0]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

	// �T�E���h�̍Đ�.
	if ( pI->m_BGMList[Name][0]->PlayLoop() == false ) return;
	if ( OldBGMStop ) pI->m_NowBGMName = Name;
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// SE���t�Đ�����.
//	�w�肵���Ԋu���Đ��Ԋu���Z���ꍇ�͏����͍s��Ȃ�.
//----------------------------.
void CSoundManager::ReversePlaySE( std::string Name, float Interval, bool IsStopPlay, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	// �w�肵���Ԋu(�~���b�ɕϊ�)�ƁA���݂̎����ƑO�ɍĐ����������̊Ԋu���r����.
	if ( Interval * 1000 > static_cast<float>( GetTickCount64() - pI->m_PlayTimeList[Name] ) ) return;

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		// SE�̍Đ�.
		pI->m_SEList[Name][No]->ResetVolume();
		pI->m_SEList[Name][No]->PlaySE( IsStopPlay, true );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���t�Đ����܂���", Color4::Cyan );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		if ( IsStopPlay == false && s->IsStopped() == false ) continue;

		// SE�̍Đ�.
		s->ResetVolume();
		s->PlaySE( IsStopPlay, true );
		pI->m_PlayTimeList[Name] = GetTickCount64();

		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���t�Đ����܂���", Color4::Cyan );
		break;
	}

	if ( IsStopPlay ) return;
	pI->m_SEList[Name][0]->ResetVolume();
	pI->m_SEList[Name][0]->PlaySE( IsStopPlay, true );
	pI->m_PlayTimeList[Name] = GetTickCount64();

	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���t�Đ����܂���", Color4::Cyan );
}

//----------------------------.
// 3D��SE���t�Đ�����.
//	�w�肵���Ԋu���Đ��Ԋu���Z���ꍇ�͏����͍s��Ȃ�.
//----------------------------.
void CSoundManager::ReversePlaySE3D( std::string Name, D3DXPOSITION3 PlayPos, float Interval, bool IsStopPlay, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	// �w�肵���Ԋu(�~���b�ɕϊ�)�ƁA���݂̎����ƑO�ɍĐ����������̊Ԋu���r����.
	if ( Interval * 1000 > static_cast<float>( GetTickCount64() - pI->m_PlayTimeList[Name] ) ) return;

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
		const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
		const float Length = D3DXVec3Length( &ListenerVec );

		// ����邲�Ƃɉ��ʂ������Ă���.
		const int MaxVolume = pI->m_SEList[Name][No]->GetMaxVolume();
		pI->m_SEList[Name][No]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

		// SE�̍Đ�.
		pI->m_SEList[Name][No]->PlaySE( IsStopPlay, true );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v��3D�ŋt�Đ����܂���", Color4::Cyan);
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		if ( IsStopPlay == false && s->IsStopped() == false ) continue;

		// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
		const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
		const float Length = D3DXVec3Length( &ListenerVec );

		// ����邲�Ƃɉ��ʂ������Ă���.
		const int MaxVolume = s->GetMaxVolume();
		s->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

		// SE�̍Đ�.
		s->PlaySE( IsStopPlay, true );
		pI->m_PlayTimeList[Name] = GetTickCount64();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v��3D�ŋt�Đ����܂���", Color4::Cyan);
		break;
	}

	if ( IsStopPlay ) return;
	const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
	const float Length = D3DXVec3Length( &ListenerVec );

	// ����邲�Ƃɉ��ʂ������Ă���.
	const int MaxVolume = pI->m_SEList[Name][0]->GetMaxVolume();
	pI->m_SEList[Name][0]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

	// SE�̍Đ�.
	pI->m_SEList[Name][0]->PlaySE( IsStopPlay, true );
	pI->m_PlayTimeList[Name] = GetTickCount64();
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v��3D�ŋt�Đ����܂���", Color4::Cyan);
}

//----------------------------.
// BGM���t�Đ�����.
//	���[�v��~���̏ꍇ�����͍s��Ȃ�.
//----------------------------.
void CSoundManager::ReversePlayBGM( std::string Name, bool OldBGMStop, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �����Ă���BGM���~�߂�.
	if ( OldBGMStop && pI->m_NowBGMName != "" && pI->m_NowBGMName != Name ) {
		pI->m_BGMList[pI->m_NowBGMName][0]->Stop();
	}

	// BGM���Đ�����.
	pI->m_BGMList[Name][0]->ResetVolume();
	if ( pI->m_BGMList[Name][0]->PlayLoop( true ) == false ) return;
	if ( OldBGMStop ) pI->m_NowBGMName = Name;
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// BGM���t�F�[�h�Ő؂�ւ��ċt�Đ�����.
//	���[�v��~���̏ꍇ�����͍s��Ȃ�.
//----------------------------.
void CSoundManager::ReversePlayFadeBGM( std::string Name, int FadeSpeed, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �����Ă���BGM�����X�Ɏ~�߂�.
	if ( pI->m_NowBGMName != "" && pI->m_NowBGMName != Name ) {
		pI->m_BGMList[pI->m_NowBGMName][0]->AddVolume( -FadeSpeed );
		pI->m_BGMList[Name][0]->SetVolume( 
			pI->m_BGMList[Name][0]->GetMaxVolume() - pI->m_BGMList[Name][0]->GetMaxVolume() * (
			pI->m_BGMList[pI->m_NowBGMName][0]->GetVolume() / pI->m_BGMList[pI->m_NowBGMName][0]->GetMaxVolume() 
		) );

		// BGM���~�߂�.
		if ( pI->m_BGMList[pI->m_NowBGMName][0]->GetVolume() <= 0 ) {
			pI->m_BGMList[pI->m_NowBGMName][0]->ResetVolume();
			pI->m_BGMList[pI->m_NowBGMName][0]->Stop();
			pI->m_BGMList[Name][0]->ResetVolume();
			pI->m_NowBGMName = Name;
		}
	}

	// BGM���Đ�����.
	if ( pI->m_BGMList[Name][0]->PlayLoop( true ) == false ) return;
	if ( pI->m_NowBGMName	== ""		) pI->m_NowBGMName = Name;
	if ( IsMsgStop			== false	) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// 3D��BGM���t�Đ�����.
//	���[�v��~���̏ꍇ�����͍s��Ȃ�.
//----------------------------.
void CSoundManager::ReversePlayBGM3D( std::string Name, D3DXPOSITION3 PlayPos, bool OldBGMStop, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_LoopStopFlag ) return;

	// �����Ă���BGM���~�߂�.
	if ( OldBGMStop && pI->m_NowBGMName != "" && pI->m_NowBGMName != Name ) pI->m_BGMList[pI->m_NowBGMName][0]->Stop();

	// �Đ��ʒu�ƃ��X�i�[�̋����𒲂ׂ�.
	const D3DXVECTOR3& ListenerVec = PlayPos - pI->m_pListener->GetPosition();
	const float Length = D3DXVec3Length( &ListenerVec );

	// ����邲�Ƃɉ��ʂ������Ă���.
	const int MaxVolume = pI->m_BGMList[Name][0]->GetMaxVolume();
	pI->m_BGMList[Name][0]->SetVolume( MaxVolume - static_cast<int>( Length / VOLUME_DOWN_LENGTH ) );

	// �T�E���h�̍Đ�.
	if ( pI->m_BGMList[Name][0]->PlayLoop( true ) == false ) return;
	if ( OldBGMStop ) pI->m_NowBGMName = Name;
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v�����[�v�����܂���", Color4::Green );
}

//----------------------------.
// SE���~������.
//----------------------------.
void CSoundManager::StopSE( std::string Name, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		if ( pI->m_SEList[Name][No]->Stop() ) {
			if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���~�����܂���", Color4::Green );
		}
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		if ( s->IsStopped() ) continue;

		if ( s->Stop() ) {
			if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���~�����܂���", Color4::Green );
		}
	}
}

//----------------------------.
// SE���ꎞ��~������.
//----------------------------.
void CSoundManager::PauseSE( std::string Name, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No = pI->m_SelectNo;
		pI->m_SelectNo = -1;

		pI->m_SEList[Name][No]->Pause();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���ꎞ��~�����܂���", Color4::Green );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		if ( s->IsStopped() ) continue;

		s->Pause();
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"SE�u" + Name + u8"�v���ꎞ��~�����܂���", Color4::Green );
	}
}

//---------------------------.
// BGM���~������.
//---------------------------.
void CSoundManager::StopBGM( std::string Name, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_BGMList[Name][0]->IsStopped() ) return;
	pI->m_NowBGMName = "";
	if ( pI->m_BGMList[Name][0]->Stop() ) {
		if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v���~�����܂���", Color4::Green );
	}
}

//---------------------------.
// BGM���ꎞ��~������.
//---------------------------.
void CSoundManager::PauseBGM( std::string Name, bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	if ( pI->m_BGMList[Name][0]->IsStopped() ) return;
	pI->m_NowBGMName = "";
	pI->m_BGMList[Name][0]->Pause();
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM�u" + Name + u8"�v���ꎞ��~�����܂���", Color4::Green );
}

//----------------------------.
// BGM��S�Ē�~������.
//----------------------------.
void CSoundManager::BGMAllStop( bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	pI->m_NowBGMName = "";
	for ( auto& vb : pI->m_BGMList ){
		for ( auto& b : vb.second ) b->Stop();
	}
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM��S�Ē�~�����܂���", Color4::Green );
}

//----------------------------.
// BGM��S�Ĉꎞ��~������.
//----------------------------.
void CSoundManager::BGMAllPause( bool IsMsgStop )
{
	CSoundManager* pI = GetInstance();

	pI->m_NowBGMName = "";
	for ( auto& vb : pI->m_BGMList ){
		for ( auto& b : vb.second ) b->Pause();
	}
	if ( IsMsgStop == false ) CMessageWindow::PushMessage( u8"BGM��S�Ĉꎞ��~�����܂���", Color4::Green );
}

//----------------------------.
// ���E�̃I�[�f�B�I��ON/OFF.
//----------------------------.
void CSoundManager::SetIsBGMAllON( std::string Name, bool Flag )
{
	CSoundManager* pI = GetInstance();

	for ( auto& b : pI->m_BGMList[Name] ) {
		b->SetIsAudioAllON( Flag );
	}
}
void CSoundManager::SetIsSEAllON( std::string Name, bool Flag )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->SetIsAudioAllON( Flag );
		return;
	}

	for ( auto& b : pI->m_SEList[Name] ) {
		b->SetIsAudioAllON( Flag );
	}
}

//----------------------------.
// ���I�[�f�B�I��ON/OFF.
//----------------------------.
void CSoundManager::SetIsBGMLeftON( std::string Name, bool Flag )
{
	CSoundManager* pI = GetInstance();

	for ( auto& b : pI->m_BGMList[Name] ) {
		b->SetIsAudioLeftON( Flag );
	}
}
void CSoundManager::SetIsSELeftON( std::string Name, bool Flag )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->SetIsAudioLeftON( Flag );
		return;
	}

	for ( auto& b : pI->m_SEList[Name] ) {
		b->SetIsAudioLeftON( Flag );
	}
}

//----------------------------.
// �E�I�[�f�B�I��ON/OFF.
//----------------------------.
void CSoundManager::SetIsBGMRightON( std::string Name, bool Flag )
{
	CSoundManager* pI = GetInstance();

	for ( auto& b : pI->m_BGMList[Name] ) {
		b->SetIsAudioRightON( Flag );
	}
}
void CSoundManager::SetIsSERightON( std::string Name, bool Flag )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->SetIsAudioRightON( Flag );
		return;
	}

	for ( auto& b : pI->m_SEList[Name] ) {
		b->SetIsAudioRightON( Flag );
	}
}

//----------------------------.
// ���[�v���~�����邩.
//----------------------------.
void CSoundManager::SetIssLoopStop( bool Flag, bool IsMsgStop )
{
	GetInstance()->m_LoopStopFlag = Flag;
	if ( IsMsgStop ) return;
	if( Flag )	CMessageWindow::PushMessage( u8"���[�v�Đ����~�����܂���", Color4::Green );
	else		CMessageWindow::PushMessage( u8"���[�v�Đ������ɖ߂��܂���", Color4::Green );
}

//----------------------------.
// ���ʂ����ɖ߂�.
//----------------------------.
void CSoundManager::ResetSEVolume( std::string Name )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->ResetVolume();
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ) {
		// ���ʂ����ɖ߂�.
		s->ResetVolume();
	}
}

//----------------------------.
// ���ʂ̐ݒ�.
//----------------------------.
void CSoundManager::SetSEVolume( std::string Name, int Volume )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->SetVolume( Volume );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		// ���ʂ̐ݒ�.
		s->SetVolume( Volume );
	}
}
void CSoundManager::SetSEVolume( std::string Name, float Volume )
{
	SetSEVolume( Name, static_cast<int>( NORMAL_VOLUME * Volume ) );
}

//----------------------------.
// ���ʂ̒ǉ�.
//----------------------------.
void CSoundManager::AddSEVolume( std::string Name, int Volume )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->AddVolume( Volume );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ) {
		// ���ʂ̐ݒ�.
		s->AddVolume( Volume );
	}
}
void CSoundManager::AddSEVolume( std::string Name, float Volume )
{
	AddSEVolume( Name, static_cast<int>( NORMAL_VOLUME * Volume ) );
}

//----------------------------.
// ���ʂ����ɖ߂�.
//----------------------------.
void CSoundManager::ResetBGMVolume( std::string Name )
{
	CSoundManager* pI = GetInstance();

	for ( auto& s : pI->m_BGMList[Name] ) {
		// ���ʂ����ɖ߂�.
		s->ResetVolume();
	}
}

//----------------------------.
// ���ʂ̐ݒ�.
//----------------------------.
void CSoundManager::SetBGMVolume( std::string Name, int Volume )
{
	CSoundManager* pI = GetInstance();

	for ( auto& s : pI->m_BGMList[Name] ) {
		// ���ʂ̐ݒ�.
		s->SetVolume( Volume );
	}
}
void CSoundManager::SetBGMVolume( std::string Name, float Volume )
{
	SetBGMVolume( Name, static_cast< int >( NORMAL_VOLUME * Volume ) );
}

//----------------------------.
// ���ʂ̒ǉ�.
//----------------------------.
void CSoundManager::AddBGMVolume( std::string Name, int Volume )
{
	CSoundManager* pI = GetInstance();

	for ( auto& s : pI->m_BGMList[Name] ) {
		// ���ʂ̐ݒ�.
		s->AddVolume( Volume );
	}
}
void CSoundManager::AddBGMVolume( std::string Name, float Volume )
{
	AddBGMVolume( Name, static_cast< int >( NORMAL_VOLUME * Volume ) );
}

//----------------------------.
// �Đ����x�����ɖ߂�.
//----------------------------.
void CSoundManager::ResetSEPlaySpeed( std::string Name )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No	= pI->m_SelectNo;

		pI->m_SEList[Name][No]->ResetPlaySpeed();
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ) {
		// �Đ����x�̐ݒ�.
		s->ResetPlaySpeed();
	}
}

//----------------------------.
// �Đ����x�̐ݒ�.
//----------------------------.
void CSoundManager::SetSEPlaySpeed( std::string Name, int Speed )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No = pI->m_SelectNo;
		pI->m_SelectNo = -1;

		pI->m_SEList[Name][No]->SetPlaySpeed( Speed );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ){
		// �Đ����x�̐ݒ�.
		s->SetPlaySpeed( Speed );
	}
}
void CSoundManager::AddSEPlaySpeed( std::string Name, int Speed )
{
	CSoundManager* pI = GetInstance();

	// ���䂷��ԍ������܂��Ă��邩.
	if ( pI->m_SelectNo >= 0 ) {
		const int No = pI->m_SelectNo;
		pI->m_SelectNo = -1;

		pI->m_SEList[Name][No]->AddPlaySpeed( Speed );
		return;
	}

	for ( auto& s : pI->m_SEList[Name] ) {
		// �Đ����x�̐ݒ�.
		s->AddPlaySpeed( Speed );
	}
}

//----------------------------.
// �Đ����x�����ɖ߂�.
//----------------------------.
void CSoundManager::ResetBGMPlaySpeed( std::string Name )
{
	CSoundManager* pI = GetInstance();

	for ( auto& s : pI->m_BGMList[Name] ) {
		// �Đ����x�̐ݒ�.
		s->ResetPlaySpeed();
	}
}

//----------------------------.
// �Đ����x�̐ݒ�.
//----------------------------.
void CSoundManager::SetBGMPlaySpeed( std::string Name, int Speed )
{
	CSoundManager* pI = GetInstance();

	for ( auto& s : pI->m_BGMList[Name] ){
		// �Đ����x�̐ݒ�.
		s->SetPlaySpeed( Speed );
	}
}
void CSoundManager::AddBGMPlaySpeed( std::string Name, int Speed )
{
	CSoundManager* pI = GetInstance();

	for ( auto& s : pI->m_BGMList[Name] ) {
		// �Đ����x�̐ݒ�.
		s->AddPlaySpeed( Speed );
	}
}

//----------------------------.
// �����̒�~���m�F����.
//----------------------------.
bool CSoundManager::IsStoppedBGM( std::string Name )
{
	return GetInstance()->m_BGMList[Name][0]->IsStopped();
}
bool CSoundManager::IsStoppedSE( std::string Name )
{
	return GetInstance()->m_SEList[Name][0]->IsStopped();
}

//----------------------------.
// �T�E���h�t�@�C�����J��.
//----------------------------.
void CSoundManager::OpenSound(
	std::vector<std::shared_ptr<CSound>>* List,	const HWND&			hWnd,
	const int			PlayMax,				const int			MaxVolume,
	const std::string&	FileName,				const std::wstring& wFilePath )
{
	// ���d�Đ��\�����������쐬����.
	List->resize( PlayMax );
	for ( auto& s : *List ) s = std::make_shared<CSound>();

	// �T�E���h�̐ݒ�.
	for ( int i = 0; i < PlayMax; ++i ) {
		// �G�C���A�X�� w_char �ɕϊ�����.
		const std::string&	Alias	= FileName + "_" + std::to_string( i );
		const std::wstring&	wAlias	= StringConversion::to_wString( Alias );

		// �T�E���h�t�@�C�����J��.
		( *List )[i]->Open( wFilePath.c_str(), wAlias.c_str(), hWnd, MaxVolume );
	}
}