#include <iostream>
#include <string>

using namespace std;

class Slution
{
    public:
        string slute(string s)
        {
              int i=0;
              string out;
              for(;i<s.size();i++)
              {
                    if(s[i]==' ')
                       out.append("%20");
                    else
                       out.push_back(s[i]);
              }
              return out;

        }
};
