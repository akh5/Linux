#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
using namespace std;

class Udpsvr
{
    public:
        Udpsvr()
        {
            _sock = -1;
        }
        ~Udpsvr()
        {
            close(_sock);
        }


        bool Create()
        {
            _sock = socket(AF_INET,SOCK_DGRAM,0);
            if(_sock<0)
            {
                perror("socket");
                return false;
            }
            return true;
        }
        bool Bind(string &ip,uint16_t port)
        {
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr(ip.c_str());
            addr.sin_port = htons(port);
            int ret = bind(_sock,(struct sockaddr*)&addr,sizeof(addr));
            if(ret<0)
            {
                perror("bind");
                return false;
            }
            return true;
        }
        bool send(string &buf,struct sockaddr_in& dest_addr)
        {
            int swight = sendto(_sock,buf.c_str(),buf.size(),0,(sockaddr*)&dest_addr,sizeof(struct sockaddr_in));
            if(swight<0)
            {
                perror("sendto");
                return false;
            }
            return true;
        }
        bool recv (string& buf,struct sockaddr_in& src_addr)
        {
            char tem[1024] = {0};
            socklen_t addrlen = sizeof(struct sockaddr_in);
            int rwight = recvfrom(_sock,tem,sizeof(tem)-1,0,(struct sockaddr*)&src_addr,&addrlen);
            if(rwight<0)
            {
                perror("recvfrom");
                return false;
            }
            buf.assign(tem,rwight);
            return true;
        }



    private:
        uint32_t _sock;
        sockaddr_in addr;
};
