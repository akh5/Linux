class Slution
{
    public:
        bool slute(int a)
        {

        }
};
#include <iostream>
#include "header.cpp"
using namespace std;

int main()
{
  Slution s;
  if(s.slute(1))
    cout<<"test1 ok"<<endl;
  else
    cout<<"test1 failed"<<endl;
  if(s.slute(2))
      cout<<"test2 ok"<<endl;
  else
      cout<<"test2 failed"<<endl;
}
