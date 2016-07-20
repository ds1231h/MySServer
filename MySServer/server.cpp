/*
My socket server 
version 1.1
MuPei
2016.07.19
function: server for only one client
			send file to client
*/

/*
step:
1.initial socket dll
2.creat socket
3.bind socket with address
4.start listen(in this version, only a client)
5.accept client request
6.accept client data
7.send data to client
8.exit
*/

#include <stdafx.h>
#include <WinSock2.h>
#include <iostream>

using namespace std;

const int BUFSIZE = 1019;
char sendBuf[BUFSIZE] = {0};
sockaddr_in clientAddr;
int clientAddrLen = sizeof(clientAddr);
SOCKET sServer;
SOCKET sClient;

void sendData(void* soc)
{
	sClient = *(SOCKET*)soc;
	int retVal = 0;

	while (TRUE)
	{
		// send data to server
		cin >> sendBuf;
		retVal = send(sClient, sendBuf, strlen(sendBuf), 0); // para: in socket, out recvBuf,in len, flags
		if (SOCKET_ERROR == retVal)
		{
			cout << "send data failed!" << endl;
			// 			closesocket(*sServer);
			// 			WSACleanup();
			// 			return ;
			continue;
		}
	}
	// exit
	closesocket(sClient);
	WSACleanup();
	return ;
}

void recvData(void* soc)
{
	sClient = *(SOCKET*)soc;
	int retVal = 0;
	char recvBuf[BUFSIZE] = {0};

	while(TRUE)
	{
		// accept client's data
		retVal = recv(sClient, recvBuf, BUFSIZE, 0); // para: in socket, out recvBuf,in len, flags
		if (SOCKET_ERROR == retVal)
		{
			cout << "accetp data failed!" << endl;
			closesocket(sClient);
			WSACleanup();
			return;
		}
		cout << "data from client:" << recvBuf << endl;
	}
	// exit
	closesocket(sClient);
	WSACleanup();
	return ;
}

SOCKET acceptReqst()
{
	// accetp client's request
	sClient = accept(sServer, (sockaddr*)& clientAddr, &clientAddrLen); // para: in socket, out address, addrlen 
	if (INVALID_SOCKET == sClient)
	{
		cout << "accetp request failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}
	return sClient;
}

void sendFile()
{
	char bufTop[BUFSIZE+5] = {"f1996"};
	int nCount = 0; // for send file

	// send file
	// check whether the file is exist
	char* filename = "D:\\sqlmap.rar";
	// char* filename = "E:\\python workspace\\test.txt";
	FILE* fp = NULL;
	if (fopen_s(&fp, filename, "rb") != 0)
	{
		cout << "open file failed!" << endl;
	}

	// transform the file
	while ((nCount = fread_s(sendBuf, BUFSIZE, 1, BUFSIZE-1, fp)) > 0)
	{
		strcat(bufTop, sendBuf);
		// strcat(bufTop, bufEnd);
		send(sClient, bufTop, sizeof(bufTop), 0);
		memset(bufTop+5, 0, BUFSIZE);
		// time++;
	}
	send(sClient, "1996e", 5, 0);
	fclose(fp); // transform file finish
}

int main()
{
	WSADATA wsaD;
	sockaddr_in servAddr;
	const int portId = 1996;
	int retVal = 0;
	// int time = 0; // load in times.
	bool tFile = FALSE; // whether transform file

	// initial socket dll
	if (WSAStartup(MAKEWORD(2, 2), &wsaD) != 0) // windows sokcet api version 2.2
	{
		cout << "WSA startup failed!" << endl;
		return -1;
	}

	// create socket
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // 3 parameter for socket
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket failed!" << endl;
		WSACleanup();
		return -1;
	}

	// server socket address
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portId);
	servAddr.sin_addr.s_addr = INADDR_ANY;

	// bind socket
	retVal = bind(sServer, (LPSOCKADDR)&servAddr, sizeof(sockaddr_in));
	if (SOCKET_ERROR == retVal)
	{
		cout << "bind failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "startup succeed!" << endl;
	}

	// start listening
	retVal = listen(sServer, 1); // listen one client

	_beginthread(sendData, 0, &sClient);

	while (TRUE)
	{
		sClient = acceptReqst();
		if (!tFile)
		{
			sendFile();
			tFile = TRUE;
		}
		_beginthread(recvData, 0, &sClient);
	}


	// exit
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();

	return 0;
}