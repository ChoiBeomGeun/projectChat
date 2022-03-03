#pragma once
#include <string>
//=================================================================================================
// @brief �ӽ� ��Ʈ�� ���̺�
//=================================================================================================
namespace StringTable
{
	  const string InvalidInputDescription = "------------------------�Է��� Ȯ���ϼ���!\r\n";
	  const string InvalidLogin = "------------------------�Է��� Ȯ���ϼ���! Login, /l {�г���}\r\n";
	  const string InvalidCreateRoom = "------------------------�Է��� Ȯ���ϼ���! /CreateRoom, /cr {���̸�} {�ִ��ο�}\r\n";
	  const string InvalidDestroyRoom = "------------------------�Է��� Ȯ���ϼ���!\r\n";
	  const string InvalidWhisper = "------------------------�Է��� Ȯ���ϼ���! /Whisper, /w {�г���} {�޽���}!\r\n";

	  const string LoginDescription = "------------------------�α����� �ϼ���!\r\n��ɾ� /Login,/l {�г���} \r\n";
	  const string CommandDescription =
		  "------------------------��ɾ� ����Ʈ------------------------\r\n\
�α���				/Login, /l 		{�г���}		    \r\n\
�� ����				/CreateRoom, /cr 	{���̸�} {�ִ��ο�} \r\n\
�� ����				/EnterRoom, /er		{���̸�}			\r\n\
�ӼӸ�				/Whisper, /w 		{�г���} {�޽���}   \r\n\
�� ���				/RoomList, /rl 		{None}              	\r\n\
�� ������ ���			/RoomUserList, /rul	{���ȣ}        \r\n\
������ ���			/UserList, /ul 		{None}                  \r\n\
��ɾ� ���			/help, /? 		{None}	        \r\n";  \
	const string RoomCommandDescription =
"------------------------��ɾ� ����Ʈ------------------------\r\n\
�� ������			/exit 			{None}              \r\n\
�� �ʴ�			        /invite, /i 	        {�г���}              \r\n\
�ӼӸ�				/Whisper, /w 		{�г���} {�޽���}   \r\n\
�� ���				/RoomList, /rl 		{None}              	\r\n\
�� ������ ���			/RoomUserList, /rul	{���ȣ}        \r\n\
������ ���			/UserList, /ul 		{None}                  \r\n\
��ɾ� ���			/help, /? 		{None}	        \r\n";  \
	 const string AlarmCreatingRoom = "------------------------���� �����Ǿ����ϴ�!\r\n";
	 const string AlarmNoRoom = "------------------------������ ���� �����ϴ�.!\r\n";
	 const string AlarmNotInRoom = "------------------------�濡 ������ ���°� �ƴմϴ�!\r\n";
	 const string AlarmEnterServer = "------------------------{0} ���� ������ ���Խ��ϴ�\r\n";
	 const string AlarmEnterRoom = "------------------------{0} ���� �濡 ���Խ��ϴ�. ���� �ο� ({1}/{2})\r\n";
	 const string AlarmExitRoom = "------------------------{0} ���� �濡�� �������ϴ�. ���� �ο� ({1}/{2})\r\n";
	 const string AlarmDestroyRoom = "------------------------{0} ���� �ı��Ǿ����ϴ�! \r\n";
	 const string AlarmFullRoom = "------------------------���� �ο����� ����á���ϴ�! \r\n";
	 const string AlarmRoomUserList = "------------------------�ش� ���� ���� ���� �Դϴ�. \r\n";
	 const string AlarmSendWhispher = "------------------------�ӼӸ��� ���۵Ǿ����ϴ�.\r\n";
	 const string AlarmSendInvite = "------------------------�ʴ� ��û�� �߽��ϴ�.";
	 const string AlarmExitSession =  "------------------------������ ����Ǿ����ϴ�.\r\n";
	 const string AlarmSameSession = "------------------------������ ������� ������ �����ϴ�.\r\n";
	 const string AlarmNoClient = "------------------------��븦 ã�� �� �����ϴ�. \r\n";
	 const string AlarmExistSession = "------------------------ �̹� �α����� �����Դϴ�!\r\n";
	 const string TemplateRoomList = "------------------------�� ��ȣ : {} �� �̸� : {}, �� ���� : {}, �ο� �� : {}\r\n";
	 const string TemplateClientInfo = "------------------------���� �г��� : {} ���� �ּ� : {} \r\n";

	 const string TemplateWhispher = "{}���� �ӼӸ� : ";
	 const string TemplateInvite = "# {}���� {}���濡�� �ʴ� ��û�� �߽��ϴ�.\r\n";
}


