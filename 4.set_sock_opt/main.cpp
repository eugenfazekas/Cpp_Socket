#include <iostream>
#include <winsock2.h>
#define PORT 9090
using namespace std;

struct sockaddr_in srv;

int main() {

    WSADATA ws;
    WSAStartup(MAKEWORD(2,2),& ws);

    int nSocket = socket(AF_INET, SOCK_STREAM,0);
    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = INADDR_ANY;
    memset(&(srv.sin_zero),0,8);

    int optVal = 1; 
    // optVal = 0 (servers only listen on same port );
    // optVal = 1 (servers can make new connections on same port );
    int nLen = sizeof(optVal);
    int nRet;

    nRet = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR,(const char*)& nLen, nLen);

    if(nRet < 0) {
        cout<<endl<<"The setsockopt failed";
    }

    nRet = bind(nSocket, (struct sockaddr*)& srv, sizeof(srv));

    if(nRet < 0) {
        cout<<endl<<"The bind failed";
    } else {
        cout<<endl<<"The bind successfull";
    }

    nRet = listen(nSocket,10);

    if(nRet < 0) {
        cout<<endl<<"The listen failed";
    }  else {
        cout<<endl<<"The listen successfull";
    }  
    getchar();
}