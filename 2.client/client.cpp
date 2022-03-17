#include <iostream>
#include <winsock2.h>
#define PORT 9909
using namespace std;

// -o main -lws2_32 

int nClientSocket;
struct sockaddr_in srv;

int main() {

    WSADATA ws;
    int nRet = 0;

        if(WSAStartup(MAKEWORD(2,2), &ws)) {
            cout<<"WSA failed to initialize"<<endl;
            WSACleanup();
                return (EXIT_FAILURE);
        }

    nClientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    if (nClientSocket < 0 ) {
            WSACleanup();
            cout<<"The socket not opened";
            exit(EXIT_FAILURE);
         } else
            cout<<"The socket opened successfully! Socket Id: "<<nClientSocket;

        srv.sin_family = AF_INET;
        srv.sin_port = htons(PORT);
        srv.sin_addr.s_addr = inet_addr("127.0.0.1");
        memset(&(srv.sin_zero),0,8);   

    nRet = connect(nClientSocket, (struct sockaddr*) & srv, sizeof(srv));     
    if(nRet < 0) {
        cout<<endl<<"connection failed";
        WSACleanup();
        return (EXIT_FAILURE);
    } else {
        cout<<endl<<"Connected to the Server  ";
        char buff[255] = {0,};
        recv(nClientSocket,buff,255,0);
        cout<<endl<<"Just press any key on keyboard to see the message received from the server";
        getchar();
        cout<<endl<<buff;
        cout<<endl<<"Now send youre message to the server:";
        while(1){
            fgets(buff,256,stdin);
            send(nClientSocket,buff,256,0);
            cout<<endl<<"Press any key to get the response from server...";
            getchar();
            recv(nClientSocket,buff,256,0);
            cout<<endl<<buff<<endl<<"Now send next message";
        }
    }
    system("pause");
}