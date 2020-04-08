#include <stdio.h>

int main()
{
  while(printf("输入一个数:"))
  {
    int states=0,error=0;
    char c;
    while( (c=getchar())!='\n') 
    {
      switch(states)
      {
        case 0:
          if(c>='0' && c<='9')
            states=1;
          else
            error=1;
          break;

        case 1:
          if(c>='0' && c<='9')
            states=1; 
          else
            if(c=='.') states=2;
            else
              if(c=='E' || c=='e') states=4;
              else
                error=1;
          break;

        case 2:
          if(c>='0' && c<='9')
            states=3;
          else
            error=1;
          break;

        case 3:
          if(c>='0' && c<='9')
            states=3; 
          if(c=='E' || c=='e') states=4;
          else
            error=1;
          break;

        case 4:
          if(c>='0' && c<='9')
            states=6;
          else
            if(c=='+' || c=='-') states=5;
            else
              error=1;
          break;

        case 5:
          if(c>='0' && c<='9')
            states=6;
          else
            error=1;
          break;

        case 6:
          if(c>='0' && c<='9')
            states=6; 
          else
            error=1;
      }
    }

    if(error==0)
      printf("是无符号实数\n");
    else
      printf("不是无符号实数\n");
  }
}
