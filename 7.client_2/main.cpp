#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#define PORT 19999
using namespace std;


int main() {

    WSADATA ws;
    int nRet = 0;
    sockaddr_in srv;
    nRet = WSAStartup(MAKEWORD(2,2), & ws);
    if(nRet < 0) {
        printf("\n Cannot Initialize socket lib");
        return -1;
    }

    //Open socket - listener
    int nSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(nSocket < 0) {
        //errno is a system global variable gets updated
        // with the last API cal return value/result. 
        printf("\n Cannot Initialize listener socket: %d", errno);
        return -1;
    }

    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = inet_addr("192.168.1.2");
    srv.sin_port = htons(PORT);
    memset(&(srv.sin_zero),0,8);

    nRet = connect(nSocket,(struct sockaddr*)&srv ,sizeof(srv));
    if(nRet < 0) {
        printf("\n Cannot connect to server");
        return -1;
    }

    // Keep sending the messages
    char sBuffer[1024] = {0,};
    while(1) {
        Sleep(2000);
        printf("\n What message you want to send?!\n");
        fgets(sBuffer, 1023, stdin);
        send(nSocket,sBuffer, strlen(sBuffer),0);
    }
}