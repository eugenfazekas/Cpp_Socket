#include <iostream>
#include <winsock2.h>
#define PORT 9909
using namespace std;

// -o main -lws2_32 

struct sockaddr_in srv;
fd_set fr, fw, fe;
int nMaxFd;
int nSocket;
int nArrClient[5];

void processNewMessage(int nClientSocket) {
    cout<<endl<<"Processing the new message for client socket:"<<nClientSocket;
    char buff[256+1] = {0,};
    int nRet = recv(nClientSocket,buff,256,0);
    if(nRet < 0) {
            cout<<endl<<"Something wrong happend..closing connection for client";
            closesocket(nClientSocket);
            for(int nIndex = 0; nIndex < 5; nIndex++){
                if(nArrClient[nIndex] == nClientSocket) {
                    nArrClient[nIndex] = 0;
                        break;
                }
            }

    } else {
        cout<<endl<<"The message received from client is:"<<buff;
        // send response to client
        send(nClientSocket, "Proccessesed youre request",23,0);
        cout<<endl<<"********************************";
    }
}
void processTheNewReuqest() {
    // New connection request
    if(FD_ISSET(nSocket,&fr)) {
        int nLen = sizeof(struct sockaddr);
        int nClientSocket = accept(nSocket,NULL,&nLen);
        if(nClientSocket > 0 ) {
            int nIndex;
            // Put it into the client fd_set.
            for(nIndex = 0; nIndex < 5; nIndex++){
                if(nArrClient[nIndex] == 0) {
                    nArrClient[nIndex] = nClientSocket;
                    send(nClientSocket, "Got the connection successfully",255,0);
                    break;
                }
            }
            if(nIndex == 5) {
                cout<<endl<<"No space for new connection"; 
            }
        }
    } else {
        for( int nIndex = 0; nIndex < 5; nIndex++){
            if(FD_ISSET(nArrClient[nIndex], &fr)){
                processNewMessage(nArrClient[nIndex]);
            }
        }
    }
}

int main() {

    int nRet = 0; 


    // Step -1 Itialize WSA variables
    WSADATA ws;
    int WSA_StarUp = WSAStartup(MAKEWORD(2,2),&ws);

    WSA_StarUp < 0 ? (
        cout<<"WSA failed to initialize"<<endl
    ) : (
        cout<<"WSA initialized successfully!"<<endl
    );

    // Step - 2 Initialize the socket
    
        nSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

        if (nSocket < 0 ) {
            WSACleanup();
            cout<<"The socket not opened";
            exit(EXIT_FAILURE);
         } else
            cout<<"The socket opened successfully! Socket Id: "<<nSocket;
        
        srv.sin_family = AF_INET;
        srv.sin_port = htons(PORT);
        //srv.sin_addr.s_addr = inet_addr("127.0.0.1");
        srv.sin_addr.s_addr = INADDR_ANY;
        memset(&(srv.sin_zero),0,8);

    // socket options
        int nOptVal = 0;
        int nOptLen = sizeof(nOptVal);
        nRet = setsockopt(nSocket,SOL_SOCKET,SO_REUSEADDR, (const char *)&nOptVal, nOptLen);

    if(!nRet) {
        cout<<endl<<"The setsockopt call succesfull!";
    } else {
        cout<<endl<<"The setsockopt call failed!";
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    // Step - 3 Bind socket to local port

        nRet = bind(nSocket, (sockaddr*)&srv, sizeof(sockaddr));

        if (nRet < 0 ) {
            WSACleanup();
            cout<<endl<<"Failed to bind to local port"<<endl   ;
            exit(EXIT_FAILURE);              
        } else
            cout<<endl<<"Bind to local port successfully!"<<endl;
        
    // Step - 4 Listen the rquest from the client

        nRet = listen(nSocket,5);

        if (nRet < 0 ) {
            WSACleanup();
            cout<<"Failed to start listen to local port"<<endl   ;
            exit(EXIT_FAILURE);
         } else
            cout<<"Started listen on local port successfully!";

    // Step - 5 Keep waiting for new requests and procees as per the request

        nMaxFd = nSocket;
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0; 

    while(1) {
        FD_ZERO(&fr);
        FD_ZERO(&fw);
        FD_ZERO(&fe);

        FD_SET(nSocket,&fr);
        FD_SET(nSocket,&fe);

        //cout<<endl<<"Before select call: "<<fr.fd_count;

        for (int nIndex = 0 ; nIndex < 5; nIndex++) {
            if(nArrClient [nIndex] != 0) { 
                FD_SET(nArrClient[nIndex], &fr);
                FD_SET(nArrClient[nIndex], &fe);
            }
        }

        nRet = select(nMaxFd+1, &fr, &fw, &fe, &tv);
        if (nRet > 0) { 
            // When someone connects or communicates with a message
            // over a dedicated connection
            cout<<endl<<"Data on port...Processing now...";
            processTheNewReuqest();
        }
        else if (nRet == 0) {
            // No connection or nay communication request made or you can say
            // that none of the socket descriptors are ready
            // cout<<endl<<"Nothing on port: "<<PORT;
        } else {
            // It failed and youre appliction should show some usefull message
            WSACleanup();
            cout<<endl<<"I failed";
            exit(EXIT_FAILURE);
        }
    //cout<<endl<<"After the select call: "<<fr.fd_count;
    //Sleep(2000);
    }
}

