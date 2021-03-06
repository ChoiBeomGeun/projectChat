#pragma once
#include <string>
//=================================================================================================
// @brief 임시 스트링 테이블
//=================================================================================================
namespace StringTable
{
	  const string InvalidInputDescription = "------------------------입력을 확인하세요!\r\n";
	  const string InvalidLogin = "------------------------입력을 확인하세요! Login, /l {닉네임}\n";
	  const string InvalidCreateRoom = "------------------------입력을 확인하세요! /CreateRoom, /cr {방이름} {최대인원}\r\n";
	  const string InvalidDestroyRoom = "------------------------입력을 확인하세요!\r\n";
	  const string InvalidWhisper = "------------------------입력을 확인하세요! /Whisper, /w {닉네임} {메시지}!\r\n";

	  const string LoginDescription = "------------------------로그인을 하세요!\r\n명령어 /Login,/l {닉네임} \r\n";
	  const string CommandDescription =
		  "------------------------명령어 리스트------------------------\w\r\n\
로그인				/Login, /l 		{닉네임}		    \r\n\
방 생성				/CreateRoom, /cr 	{방이름} {최대인원} \r\n\
방 입장				/EnterRoom, /er		{방이름}			\r\n\
귓속말				/Whisper, /w 		{닉네임} {메시지}   \r\n\
방 목록				/RoomList, /rl 		{None}              	\r\n\
방 참여자 목록			/RoomUserList, /rul	{방번호}        \r\n\
접속자 목록			/UserList, /ul 		{None}                  \r\n\
명령어 목록			/help, /? 		{None}	        \r\n";  \
	const string RoomCommandDescription =
"------------------------명령어 리스트------------------------\r\n\
방 나가기			/exit 			{None}              \r\n\
방 초대			        /invite, /i 	        {닉네임}              \r\n\
귓속말				/Whisper, /w 		{닉네임} {메시지}   \r\n\
방 목록				/RoomList, /rl 		{None}              	\r\n\
방 참여자 목록			/RoomUserList, /rul	{방번호}        \r\n\
접속자 목록			/UserList, /ul 		{None}                  \r\n\
명령어 목록			/help, /? 		{None}	        \r\n";  \
	 const string AlarmLogin = "/------------------------로그인에 성공하셨습니다!\r\n";
	 const string AlarmEnterSelfRoom = "/[{} ({})]에 입장하셨습니다!\r\n";
	 const string AlarmExitSelfRoom = "/-방에서 나갔습니다!\r\n";
	 const string AlarmCreatingRoom = "/-방이 생성되었습니다!\r\n";
	 const string AlarmNoRoom = "/-생성된 방이 없습니다.!\r\n";
	 const string AlarmNotInRoom = "/-방에 접속한 상태가 아닙니다!\r\n";
	 const string AlarmEnterServer = "/-{0} 님이 서버에 들어왔습니다\r\n";
	 const string AlarmEnterRoom = "/-{0} 님이 [{4} ({3})] 방에 들어왔습니다. 현재 인원 ({1}/{2})\r\n";
	 const string AlarmExitRoom = "-{0} 님이 방에서 나갔습니다. 현재 인원 ({1}/{2})\r\n";
	 const string AlarmDestroyRoom = "/-{0} 방이 파괴되었습니다! \r\n";
	 const string AlarmFullRoom = "/-방의 인원수가 가득찼습니다! \r\n";
	 const string AlarmRoomUserList = "/-해당 방의 대한 정보 입니다. \r\n";
	 const string AlarmSendWhispher = "/-귓속말이 전송되었습니다.\r\n";
	 const string AlarmSendInvite = "/-초대장을 보냈습니다.";
	 const string AlarmExitSession =  "/-세션이 종료되었습니다.\r\n";
	 const string AlarmSameSession = "/-동일한 사람에게 보낼수 없습니다.\r\n";
	 const string AlarmNoClient = "/-상대를 찾을 수 없습니다. \r\n";
	 const string AlarmExistSession = "/- 이미 로그인한 세션입니다!\r\n";
	 const string AlarmRoomList = "/- 방 목록입니다\r\n";
	 const string TemplateRoomList = "/-[방 번호 : {} 방 이름 : {}, 방 주인 : {}, 인원 수 : {}]\r\n";
	 const string TemplateClientInfo = "/-[유저 닉네임 : ({}) 유저 주소 : {} ]\r\n";

	 const string TemplateWhispher = "/ [{}]님의 귓속말 : ";
	 const string TemplateInvite = "/# {}님이 [{}]번방에서 초대 요청!\r\n";
}


