#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
using namespace std;
class UdpSvr
{
  public:
    UdpSvr()
    {
      _Socket = -1;

    }
    ~UdpSvr()
    {}
    bool CreateSocket()
    {
      _Socket = socket(AF_INET,SOCK_DGRAM,17);
      if(_Socket < 0)
      {
        perror("socket");
        return false;
      }
      return true;
    }
    bool Bind(string& ip,uint16_t port)
    {
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);//2字节
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      int ret = bind(_Socket,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
      if(ret<0)
      {
        perror("bind");
        return false;
      }
      return true;
    }
    bool Send(string &buff,struct sockaddr_in* destaddr)
    {
      int sendsize = sendto(_Socket,buff.c_str(),buff.size(),0,(struct sockaddr*)destaddr,sizeof(struct sockaddr_in));
      if(sendsize<0)
      {
        perror("sendto");
        return false;
      }
      return true;
    }
    bool Recv(string &buff ,struct sockaddr_in* srcaddr)
    {
      char temp[1024];
      socklen_t socklen = sizeof(struct sockaddr_in);
      int recvsize = recvfrom(_Socket,temp,sizeof(temp)-1,0,(struct sockaddr*)srcaddr,&socklen);
      if(recvsize<0)
      {
        perror("recvfrom");
        return false;
      }
      buff.assign(temp,recvsize);

      return true;
    }

    void Close()
    {
      close(_Socket);
      _Socket = -1;
    } 
  private:
    int _Socket;

};
