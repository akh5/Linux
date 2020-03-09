#include "udpsvr.hpp"

int main(int argc,char* argv[])
{
  if(argc!=3)
  {
    printf("./svr [ip] [port]");
    return 0;
  }
  string ip = argv[1];
  uint16_t port = atoi(argv[2]);
  UdpSvr us;
  if(us.CreateSocket()==false)
  {
    return 0;
  }
  if(!us.Bind(ip,port))
  {
    return 0;
  }
  string buff;
  struct sockaddr_in addr;
  us.Recv(buff,&addr);
  printf("client:[%s]\n",buff.c_str());
  printf("svr:");
  fflush(stdout);
  cin>>buff;
  us.Send(buff,&addr);
  return 0;
  
  return 0;
}
