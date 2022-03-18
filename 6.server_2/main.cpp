#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#define PORT 19999
using namespace std;


int main() {
    // Initiate socket environment
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
    srv.sin_addr.s_addr = INADDR_ANY;
    srv.sin_port = htons(PORT);
    memset(&(srv.sin_zero),0,8);

    nRet = bind(nSocket, (struct sockaddr*)&srv,sizeof(srv));
    if(nRet < 0) {
        printf("\n Cannot bind port %d",errno);
        return -1;
    }

    nRet = listen(nSocket,5);
        if(nRet < 0) {
        printf("\n Cannot listen at port %d",errno);
        return -1;
    }

    int nClient = 0; 
    int addrlen = sizeof(srv);
    nClient = accept(nSocket,(struct sockaddr*) &srv, &addrlen );
    if(nRet < 0) {
        printf("\n Cannot accept client at port %d",errno);
        return -1;
    }

    char sBuff[1024] = {0,};

    while(1) {
        memset(sBuff,0,1024); // clean income message buffer
        nRet = recv(nClient, sBuff, 1024, 0);
        if(nRet < 0) {
            printf("\n Cannot recv message:%d",errno);
            return -1;
        }
        printf("\nReceived message from client:\n%s",sBuff);
    }

}