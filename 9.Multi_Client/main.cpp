#include <stdio.h>
#include <winsock.h>
#define PORT 9991

int main()
{
    //Initiate the Socket environment
    WSADATA w;
    int nRet = 0;

    sockaddr_in srv;

    nRet = WSAStartup(MAKEWORD(2, 2), &w);
    if (nRet < 0)
    {
        printf("\nCannot Initialize socket lib");
        return -1;
    }
    //Open a socket - listener
    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        //errno is a system global variable which gets updated
        //with the last API call return value/result.
        printf("\nCannot Initialize listener socket:%d", errno);;
        return -1;
    }

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr("192.168.1.2");
    srv.sin_port = htons(PORT);
    memset(&(srv.sin_zero), 0, 8);

    nRet = connect(nSocket, (struct sockaddr*)&srv, sizeof(srv));
    if (nRet < 0)
    {
        printf("\nCannot connect to server:%d", errno);;
        return -1;
    }

    //Keep sending the messages 
    char sBuff[1024] = { 0, };
    while (1)
    {
        Sleep(2000);
        printf("\nWhat message you want to send..?\n");
        fgets(sBuff, 1023, stdin);
        send(nSocket, sBuff, strlen(sBuff), 0);
    }
}