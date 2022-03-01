#pragma once
#include <string>
//=================================================================================================
// @brief �ӽ� ��Ʈ�� ���̺�
//=================================================================================================
namespace StringTable
{
	  const string InvalidInputDescription = "��ǲ���� Ȯ���ּ���!\r\n";
	  const string InvalidLogin = "��ǲ���� Ȯ���ּ���!\r\n";
	  const string InvalidCreateRoom = "��ǲ���� Ȯ���ּ���!\r\n";
	  const string InvalidDestroyRoom = "��ǲ���� Ȯ���ּ���!\r\n";
	  const string InvalidWhisper = "��ǲ���� Ȯ���ּ���!\r\n";

	  const string LoginDescription = "�α����� �ϼ���! \r\n ���ɾ� LOGIN {�г���} \r\n";
	  const string CommandDescription =
"------------------------���ɾ� ����Ʈ------------------------\r\n  \
�α���				/Login, /l				{�г���}		    \r\n	\
�� ����				/CreateRoom, /cr		{���̸�} {�ִ��ο�} \r\n	\
�� ����				/EnterRoom, /er			{���̸�}			\r\n	\
�� �ı�				/DestroyRoom, /dr		{None}              \r\n	\
�ӼӸ�				/Whisper, /w 			{�г���} {�޽���}   \r\n	\
�� ���				/RoomList, /rl 		{None}              \r\n	\
�� ������ ���			/RoomUserList, /rul		{None}              \r\n	\
������ ���			/UserList, /ul 		{None}              \r\n    \
���ɾ� ���				/help, /?				{None}	            \r\n";  \
	 const string AlarmCreatingRoom = "���� �����Ǿ����ϴ�!\r\n";
	 const string AlarmNoRoom = "������ ���� �����ϴ�.!\r\n";
	 const string AlarmNotInRoom = "�濡 ������ ���°� �ƴմϴ�!\r\n";
	 const string AlarmEnterServer = "{0} ���� ������ ���Խ��ϴ�\r\n";
	 const string AlarmEnterRoom = "{0} ���� �濡 ���Խ��ϴ�. ���� �ο� ({1}/{2})\r\n";
	 const string AlarmExitRoom = "{0} ���� �濡�� �������ϴ�. ���� �ο� ({1}/{2})\r\n";
	 const string AlarmDestroyRoom = "{0} ���� �ı��Ǿ����ϴ�! \r\n";
	 const string TemplateRoomList = "�� ��ȣ : {} �� �̸� : {}, �� ���� : {}, �ο� �� : {}\r\n";
}

