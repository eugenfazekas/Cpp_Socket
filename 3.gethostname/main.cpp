#include <iostream>
#include <winsock2.h>

using namespace std;

struct sockaddr_in srv;

int main() {

    WSADATA ws;
    WSAStartup(MAKEWORD(2,2),& ws);

    char strHostName[32] = {0,};
    int nRet = gethostname(strHostName,32);
    if(nRet < 0) {
        cout<<endl<<"Call failed";
    } else {
        cout<<endl<<"The name of the host:"<<strHostName;
    }
}

// -o main -lws2_32