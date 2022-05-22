#include <iostream>
#include <WS2tcpip.h> //framework that contain winsock, used to access the network socket
#include<string>
#include<string.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	//initialize winsock
	WSADATA winsockData;
	WORD ver = MAKEWORD(2, 2); //specific versio

	int wsock = WSAStartup(ver, &winsockData); //start winsock with specific version that written above
	if (wsock != 0)
	{
		cerr << "initialize error!!! " << endl;
		return;
	}

	//create socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0); //used as address for socket
	if (listening == INVALID_SOCKET)
	{
		cerr << "socket error!!! " << endl;
		return;
	}

	//Bind ip adress and port to specific socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(5400); //host to network short
	hint.sin_addr.S_un.S_addr = INADDR_ANY; 

	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) != 0)
	{
		cout << "Binding Failed" << endl;
	}

	//tell winsock the socket is for listening
	if (listen(listening, SOMAXCONN) != 0)
	{
		cout << "Listening Error" << endl;
	}
	else
	{
		cout << "Server is Listening" << endl;
	}

	//wait for a connection
	sockaddr_in client;
	int clientsize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientsize);

	char host[NI_MAXHOST]; // remote name client
	char service[NI_MAXHOST]; // port that connected the client

	ZeroMemory(host, NI_MAXHOST); 
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected to port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	//used for close listening socket
	closesocket(listening);

	//function to accept and response message back to client using while loop
	char buf[4096];
	string userInput;
	while (true)
	{
		ZeroMemory(buf, 4096);

		// wait for client
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		cout << "Client> ";
		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "Error in recv(). Quitting " << endl;
			break;
		}
		if (bytesReceived == 0)
		{
			cout << "Client disconnected" << endl;
			break;
		}
		cout << string(buf, 0, bytesReceived) << endl;

		//response message back to client
		cout << "> ";
		getline(cin, userInput);
		send(clientSocket, userInput.c_str(), bytesReceived + 1, 0);

	}

	//close the sock
	closesocket(clientSocket);

	//cleanup winsock
	WSACleanup;
}