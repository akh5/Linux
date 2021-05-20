#include <iostream>
#include <string>

using namespace std;

class Slution
{
    public:
        string slute(string s)
        {
         dsflkjsdl

        }
};
#ifndef CompileOnline
#include "header.cpp"
#endif
#include <vector>

int main()
{
  Slution s;
  vector<string> test{"We Are Happy","test  one two "," first"};
  vector<string> out{"We%20Are%20Happy","test%20%20one%20two%20","%20first"};
  int i=0;
  for(;i<test.size();i++)
  {
    if(s.slute(test[i])!=out[i])
    {
      cout<<"程序运行失败，通过率为"<<(i/test.size())*100<<"%"<<"你的输出为：\""<<s.slute(test[i])<<"\"，实际输出为：\""<<out[i]<<"\""<<endl;
      return 0;
    }
  }
  cout<<"程序运行成功，通过率100%"<<endl;
  return 0;

}
