#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

enum hstate {EMPTY,FULL,DEL};
struct hashnode
{
    int data;
    enum hstate point;
};


class hashlist
{
    public:
        hashlist(int s)
          :_cap(s)
        {
          _data.resize(s);
        }

        ~hashlist()
        {
          
        }

        void insert(int d)
        {
          int pos = hashfunc(d);  
          while( _data[pos].point!=EMPTY)
          {
            pos++;
          }
          _data[pos].data = d;
        }
        void hashprint()
        {
          for(auto e:_data)
          {
            if(e.point==FULL)
            {
              cout<<e.data<<"-->";
            }
          }
          cout<<endl;
        }

    private:
        int hashfunc(int d)
        {
          return d%10;
        }
    private:
        vector<struct hashnode> _data;
        size_t _cap;
};

int main()
{
  int arry[8] = {3,2,5,1,7,4,44,8};
  hashlist hl(10);
  for(int e:arry)
  {
    hl.insert(e);
  }
  hl.hashprint();
  return 0;

}
