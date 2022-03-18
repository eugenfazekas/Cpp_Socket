#include <iostream>
#include <winsock.h>
using namespace std;

#pragma pack(1)
struct TestClass
{
	char sName[32];
	char sAddress[250];
	char sBOD[12];
	int nRegistrationNo;
};

#define PORT 9991

int nSocket;
int nClientSocket[5] = { 0, };
struct sockaddr_in srv;
fd_set fr;
fd_set fw;
fd_set fe;
int nMaxFd = 0;

void HandleNewConnection() 
{
	//nNewClient will be a new file descriptor
	//and now the client communication will take place 
	//using this file descriptor/socket only
	int nNewClient = accept(nSocket, NULL, NULL );
	//If you accept the value in second parameter, then it
	//will be 
	if (nNewClient < 0)
	{
		cout << endl << "Not able to get a new client socket";
	}
	else
	{
		int nIndex;
		for (nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nClientSocket[nIndex] == 0)
			{
				nClientSocket[nIndex] = nNewClient;
				if (nNewClient > nMaxFd)
				{
					nMaxFd = nNewClient + 1;
				}
				break;
			}
		}

		if (nIndex == 5)
		{
			cout << endl << "Server busy. Cannot accept anymore connections";
		}
	}
}

void HandleDataFromClient()
{
	for (int nIndex = 0; nIndex < 5; nIndex++)
	{
		if (nClientSocket[nIndex] > 0)
		{
			if (FD_ISSET(nClientSocket[nIndex], &fr))
			{
				//Read the data from client
				char sBuff[1048] = { 0, };
				int nRet = recv(nClientSocket[nIndex], sBuff, sizeof(TestClass), 0);
				//FD_CLR(nClientSocket[nIndex], &fr);
				if (nRet < 0)
				{
					//This happens when client closes connection abruptly
					cout << endl << "Error at client socket";
					closesocket(nClientSocket[nIndex]);
					nClientSocket[nIndex] = 0;
				}
				else
				{
					//CODE: Here
					TestClass* tc = (TestClass*)sBuff;
					cout << endl << "Name:" << tc->sName;
					cout << endl << "Reg:" << tc->nRegistrationNo;
					cout << endl << "DOB:" << tc->sBOD;
					cout << endl << "Address:" << tc->sAddress;
					break;
				}
			}
		}
	}
}

//Multiclient [5]: A server which can handle more than one client
//connection at a time simultaneously
int main()
{
	int nRet = 0;
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) < 0)
	{
		cout << endl << "Not able to start the socket environment";
		return (EXIT_FAILURE);
	}
	
	//Listener socket
	nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (nSocket < 0)
	{
		cout << endl << "The socket cannot be initialized";
		exit(EXIT_FAILURE);
	}

	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);//MSB first
	srv.sin_addr.s_addr = INADDR_ANY;//Only for server
	memset(&srv.sin_zero, 0, 8);

	nRet = bind(nSocket, (struct sockaddr*)&srv, sizeof(struct sockaddr));
	if (nRet < 0)
	{
		cout << endl << "The socket cannot be bind";
		exit(EXIT_FAILURE);
	}

	nRet = listen(nSocket, 5);
	if (nRet < 0)
	{
		cout << endl << "The listen failed";
		exit(EXIT_FAILURE);
	}

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;


	//One is listener socket
	//There could be new clients which could send you
	//message after connection
	//fd_set fr;
	nMaxFd = nSocket + 1;

	while (1)
	{
		//Set the FD_SET.
		//This need to be done every time
		FD_ZERO(&fr);
		FD_SET(nSocket, &fr);
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nClientSocket[nIndex] > 0)
			{
				FD_SET(nClientSocket[nIndex], &fr);
			}
		}

		nRet = select(nMaxFd, &fr, NULL, NULL, &tv);
		//After above call, every bit is reset by select call
		//in fr
		if (nRet < 0)
		{
			cout << endl << "select api call failed. Will exit";
			return (EXIT_FAILURE);
		}
		else if (nRet == 0)
		{
			//cout << endl << "No client at port waiting for an active connection/new message";
		}
		else
		{
			//There is some client waiting either to connect
			//or some new data came from existing client.
			if (FD_ISSET(nSocket, &fr))
			{
				//Handle New connection
				HandleNewConnection();
			}
			else
			{
				//Check what existing client got the new data
				HandleDataFromClient();
			}
		}
	}
}