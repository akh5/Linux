#include "udpsvr.hpp"

int main(int arg,char* argv[])
{
    string ip = argv[1];
    uint16_t port = atoi(argv[2]);
    
    Udpsvr us;
    if(!us.Create())
    {
        return 0;
    }
    struct sockaddr_in peer_addr;
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    peer_addr.sin_port = htons(port);
    while(1)
    {

        string buf;
        printf("\nclient say:");
        getline(cin,buf);
        us.send(buf,peer_addr);
        if(buf=="exit")
        {
            break;
        }
        us.recv(buf,peer_addr);
        if(buf=="exit")
        {
            printf("server has shut down\n");
            break;
        }
        printf("\nserver says[%s]",buf.c_str());
    }
    

    return 0;
}
