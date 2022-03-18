#include <iostream>
#include <winsock2.h>
#define PORT 9090
using namespace std;

int main() {

    WSADATA ws;
    int iResult;

    struct hostent * remoteHost;
    struct in_addr addr;
    struct sockaddr_in srv;

    char** pALias;

    iResult = WSAStartup(MAKEWORD(2,2), &ws);
    if(iResult != 0)
        return 1;

    int nSocket = socket(AF_INET,SOCK_STREAM,0);    

    char szHostName[32] = {0,};
    iResult = gethostname(szHostName,32);    
        if(iResult != 0) {
            cout<<endl<<"Not able to find the hostname";
            return 1;
        }

    remoteHost = gethostbyname(szHostName);

    if(szHostName[0] != '\0') {
        for (pALias = remoteHost->h_aliases; *pALias != 0; pALias++)
        cout<<endl<<*pALias;
        printf("\tAddress type: ");
        switch (remoteHost->h_addrtype) {
            case AF_INET:
                cout<<endl<<"AF_INET";
                srv.sin_family = AF_INET;
                break;
            case AF_NETBIOS:
                cout<<endl<<"AF_NETBIOS";
                break;
            default:
                break;    
        }  
        cout<<endl<<"Addr len:"<<remoteHost->h_length; 
        int i = 0;  
        if(remoteHost->h_addrtype == AF_INET) {
            while(remoteHost->h_addr_list[i] != 0){
                addr.s_addr = *(u_long*)remoteHost->h_addr_list[i++];
                cout<<endl<<inet_ntoa(addr);
            }
        } else if (remoteHost->h_addrtype == AF_NETBIOS)  
            printf("NETBIOS address was returned\n");
        
        srv.sin_port = htons(9999);
        srv.sin_addr.s_addr = inet_addr(inet_ntoa(addr));
        memset(&(srv.sin_zero),0,8);

        iResult = bind(nSocket,(struct sockaddr*)& srv, sizeof(srv));
        iResult = listen(nSocket,10);
    }   
}
