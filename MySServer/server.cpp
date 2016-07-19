/*
My socket server 
version 1.0
MuPei
2016.07.19
function: server for only one client
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

int main()
{
	WSADATA wsaD;
	SOCKET sServer;
	SOCKET sClient;
	sockaddr_in servAddr;
	sockaddr_in clientAddr;
	const int BUFSIZE = 1024;
	const int portId = 1996;
	char buf[BUFSIZE];
	char sendBuf[BUFSIZE];
	int retVal;

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

	// accetp client's request
	int clientAddrLen = sizeof(clientAddr);
	sClient = accept(sServer, (sockaddr*)& clientAddr, &clientAddrLen); // para: server in ,address out ,addrlen 
	if (INVALID_SOCKET == sClient)
	{
		cout << "accetp request failed!" << endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}

	// accept client's data
	while(TRUE)
	{
		ZeroMemory(buf, BUFSIZE);
		retVal = recv(sClient, buf, BUFSIZE, 0); // para: server in, buf out, len in, flags
		if (SOCKET_ERROR == retVal)
		{
			cout << "accetp data failed!" << endl;
			closesocket(sServer);
			closesocket(sClient);
			WSACleanup();
			return -1;
		}

		if (buf[0] == '\0')
		{
			break;
		}

		cout << "data from client:" << buf << endl;

		// send data to client
		cout << "send data to client:";
		cin >> sendBuf;
		send(sClient, sendBuf, strlen(sendBuf)+1, 0);
	}

	// exit
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();

	return 0;
}