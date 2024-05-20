#include "Message.h"

CMessage::CMessage()
	: m_MsgList	()
{
}

CMessage::~CMessage()
{
}

//---------------------------.
// インスタンスの取得.
//---------------------------.
CMessage* CMessage::GetInstance()
{
	static std::unique_ptr<CMessage> pInstance = std::make_unique<CMessage>();
	return pInstance.get();
}

//---------------------------.
// 更新.
//---------------------------.
void CMessage::Update( const float& DeltaTime )
{
	CMessage* pI = GetInstance();

	auto Itr = pI->m_MsgList.begin();
	while ( Itr != pI->m_MsgList.end() ) {
		std::string Key	 = Itr->first;
		bool		Flag = Itr->second;

		// メッセージの削除.
		if (		Flag == false								) Itr = pI->m_MsgList.erase( Itr );
		else if (	Key.substr( 0, 3 ) == Msg::ONE_FRAME_MSG	) Itr = pI->m_MsgList.erase( Itr );
		else Itr++;
	}
}

//---------------------------.
// メッセージを送る.
//---------------------------.
void CMessage::Send( const std::string& Message, const Msg::EMsgType Type )
{
	CMessage* pI = GetInstance();

	Msg::EMsgType	NowType = Msg::EMsgType::None;
	std::string		EndMsg	= Message;

	// メッセージの名前に識別文字が含まれるか.
	const std::string MsgType = EndMsg.substr( 0, 3 );
	if (		MsgType == Msg::ONE_FRAME_MSG	) NowType = Msg::EMsgType::OneFrameMsg;
	else if (	MsgType == Msg::CHECK_FRAME_MSG ) NowType = Msg::EMsgType::CheckFrameMsg;
	else if (	MsgType == Msg::CHECK_MSG		) NowType = Msg::EMsgType::CheckMsg;
	else if (	MsgType == Msg::NEW_MSG			) NowType = Msg::EMsgType::NewMsg;
	else if (	MsgType == Msg::DELETE_MSG		) NowType = Msg::EMsgType::DeleteMsg;
	else if (	MsgType == Msg::AUTO_MSG		) {
		NowType = Msg::EMsgType::AutoMsg;
		EndMsg	= Msg::ONE_FRAME_MSG + EndMsg.substr( 3 );
	}

	// 識別文字が含まれていない場合前に付ける.
	if ( NowType == Msg::EMsgType::None ) {
		std::string AddName = "";
		if (		Type == Msg::EMsgType::OneFrameMsg		) AddName = Msg::ONE_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckFrameMsg	) AddName = Msg::CHECK_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckMsg			) AddName = Msg::CHECK_MSG;
		else if (	Type == Msg::EMsgType::NewMsg			) AddName = Msg::NEW_MSG;
		else if (	Type == Msg::EMsgType::DeleteMsg		) AddName = Msg::DELETE_MSG;
		else												  AddName = Msg::ONE_FRAME_MSG;
		EndMsg = AddName + EndMsg;
	}

	// デリートメッセージの場合、ニューメッセージを削除する.
	if ( NowType == Msg::EMsgType::DeleteMsg || Type == Msg::EMsgType::DeleteMsg ) {
		NewMsgDelete( EndMsg );
		return;
	}

	// メッセージの追加.
	pI->m_MsgList[EndMsg] = true;
}

//---------------------------.
// メッセージの確認.
//---------------------------.
bool CMessage::Check( const std::string& Message, const Msg::EMsgType Type )
{
	CMessage* pI = GetInstance();

	Msg::EMsgType	NowType = Msg::EMsgType::None;
	std::string		EndMsg	= Message;

	// メッセージの名前に識別文字が含まれるか.
	const std::string MsgType = EndMsg.substr( 0, 3 );
	if (		MsgType == Msg::ONE_FRAME_MSG	) NowType = Msg::EMsgType::OneFrameMsg;
	else if (	MsgType == Msg::CHECK_FRAME_MSG ) NowType = Msg::EMsgType::CheckFrameMsg;
	else if (	MsgType == Msg::CHECK_MSG		) NowType = Msg::EMsgType::CheckMsg;
	else if (	MsgType == Msg::NEW_MSG			) NowType = Msg::EMsgType::NewMsg;
	else if (	MsgType == Msg::DELETE_MSG		) NowType = Msg::EMsgType::DeleteMsg;
	else if (	MsgType == Msg::AUTO_MSG		) NowType = Msg::EMsgType::AutoMsg;

	// 識別文字が含まれていない場合前に付ける.
	if ( NowType == Msg::EMsgType::None ) {
		std::string AddName = "";
		if (		Type == Msg::EMsgType::OneFrameMsg		) AddName = Msg::ONE_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckFrameMsg	) AddName = Msg::CHECK_FRAME_MSG;
		else if (	Type == Msg::EMsgType::CheckMsg			) AddName = Msg::CHECK_MSG;
		else if (	Type == Msg::EMsgType::NewMsg			) AddName = Msg::NEW_MSG;
		else if (	Type == Msg::EMsgType::DeleteMsg		) AddName = Msg::DELETE_MSG;
		else if (	Type == Msg::EMsgType::AutoMsg			) AddName = Msg::AUTO_MSG;
		else												  AddName = Msg::ONE_FRAME_MSG;
		EndMsg = AddName + EndMsg;
	}

	// チェックフレームメッセージの場合、ワンフレームメッセージに変更する.
	if ( NowType == Msg::EMsgType::CheckFrameMsg || Type == Msg::EMsgType::CheckFrameMsg ) {
		return CheckFrameMsgChange( EndMsg );
	}

	// チェックメッセージの場合、確認後メッセージを削除する.
	if ( NowType == Msg::EMsgType::CheckMsg || Type == Msg::EMsgType::CheckMsg ) {
		return CheckMsgDelete( EndMsg );
	}

	// オートメッセージの場合、全ての識別文字で調べる.
	if ( NowType == Msg::EMsgType::AutoMsg || Type == Msg::EMsgType::AutoMsg ) {
		return AutoMsgCheck( EndMsg );
	}

	// メッセージが届いているか.
	return pI->m_MsgList[EndMsg];
}

//---------------------------.
// ニューメッセージを削除.
//---------------------------.
bool CMessage::NewMsgDelete( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	std::string NewMsg = Msg::NEW_MSG + CheckMsg.substr( 3 );
	pI->m_MsgList.erase( NewMsg );
	return true;
}

//---------------------------.
// チェックフレームメッセージの変更.
//---------------------------.
bool CMessage::CheckFrameMsgChange( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	const bool Flag = pI->m_MsgList[CheckMsg];
	if ( Flag ) {
		const std::string Msg = CheckMsg.substr( 3 );
		pI->m_MsgList.erase( CheckMsg );
		pI->m_MsgList[Msg::ONE_FRAME_MSG + Msg] = true;
	}
	return Flag;
}

//---------------------------.
// チェックメッセージを削除.
//---------------------------.
bool CMessage::CheckMsgDelete( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	const bool Flag = pI->m_MsgList[CheckMsg];
	pI->m_MsgList.erase( CheckMsg );
	return Flag;
}

//---------------------------.
// オートメッセージの確認.
//---------------------------.
bool CMessage::AutoMsgCheck( const std::string& CheckMsg )
{
	CMessage* pI = GetInstance();

	const std::string Msg = CheckMsg.substr( 3 );
	if( pI->m_MsgList[Msg::ONE_FRAME_MSG	+ Msg] ) return true;
	if( pI->m_MsgList[Msg::CHECK_FRAME_MSG	+ Msg] ) return CheckFrameMsgChange( Msg::CHECK_FRAME_MSG + Msg );
	if( pI->m_MsgList[Msg::CHECK_MSG		+ Msg] ) return CheckMsgDelete( Msg::CHECK_MSG + Msg );
	if( pI->m_MsgList[Msg::NEW_MSG			+ Msg] ) return true;
	return false;
}
