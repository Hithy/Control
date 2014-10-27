#include <stdio.h>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib,"ws2_32.lib")

#define CONNECT 1
#define DISCONNECT 2
//#define ERROR 3
#define QUIT 4
#define ERROREX 5
#define SUCCESSEX 6
#define ENOK 7

#define POWEROFF 100
#define BEEPTEST 101
#define MYCMD 102

BOOL CmdCtrl(SOCKET*);
int main()
{
	DWORD flag;
	SOCKET Listen,Client;
	SOCKADDR_IN hinst;
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);
	hinst.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	hinst.sin_family = AF_INET;
	hinst.sin_port = htons(10010);
	Listen = socket(AF_INET, SOCK_STREAM, 0);
	bind(Listen, (SOCKADDR*)&hinst, sizeof(hinst));
	listen(Listen, SOMAXCONN);
	Client = accept(Listen, NULL, NULL);
	recv(Client, (char *)&flag, sizeof(DWORD), 0);
	
	if (flag == CONNECT)
		printf("Welcome!\n");
	else
	{
		printf("Wrong\n");
	}
	for (;;)
	{
		scanf_s("%d", &flag);
		send(Client, (char *)&flag, sizeof(DWORD), 0);
		if (flag == MYCMD)
			CmdCtrl(&Client);
		if (flag == QUIT)
			return 0;
	}
	/*for (;;)
	{
		recv(Client, &flag, sizeof(flag), 0);
		printf("%d\n", flag);
	}*/
	return 0;
}

BOOL CmdCtrl(SOCKET* Client)
{ 
	DWORD flag;
	CHAR CmdLine[2048];
	CHAR buffer[2048];
	recv(*Client, (char*)&flag, sizeof(DWORD), 0);
	if (ENOK != flag)
		return 0;
	while (getchar() != '\n');
	for (;;)
	{
		gets_s(CmdLine, 2048);
		send(*Client, CmdLine, 2048, 0);
		if (lstrcmp(CmdLine, "quit") == 0)
			break;
		for (;;)
		{
			memset(buffer, 0, 2048);
			recv(*Client, buffer, 2048, 0);
			if (buffer[0] == 'm' && buffer[1] == 'y' && buffer[2] == 'e' && buffer[3] == 'n' && buffer[4] == 'd')
				break;
			buffer[2017] = '\0';
			printf("%s", buffer);
		}
		memset(CmdLine, 0, 2048);
	}
	return 0;
}