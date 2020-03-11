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
    if(!us.Bind(ip,port))
    {
        return 0;
    }
    while(1)
    {
        string buf;
        struct sockaddr_in dest_addr;
        us.recv(buf,dest_addr);
        if(buf=="exit")
        {
            printf("client has exited\n");
            break;
        }
        printf("\nclient say:[%s]",buf.c_str());
        printf("\nserver say:");
        getline(cin,buf);
        us.send(buf,dest_addr);
        if(buf=="exit")
        {
            break;
        }
    }
    

    return 0;
}
