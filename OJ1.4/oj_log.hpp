#pragma once
#include <iostream>
#include <cstdio>
#include <sys/time.h>
#include <string>
#include <string.h>
class LogTime
{
  public:
    static int64_t GetTimeStamp()
    {
      struct timeval tv;
      gettimeofday(&tv,NULL);
      return tv.tv_sec;
    }
    static void GetTimeStamp(std::string* TimeStamp)
    {
      time_t Systime;
      time(&Systime);
      struct tm* st = localtime(&Systime);
      char buf[30] = {'\0'};
      snprintf(buf,sizeof(buf)-1,"%04d-%02d-%02d %02d:%02d:%02d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
      TimeStamp->assign(buf,strlen(buf));
    }
};

const char* Level[] = 
{
  "INFO",
  "WARNING",
  "ERROR",
  "FATAL",
  "DEBUG",
  "CONNECTFAIL"
};

enum LogLevel
{
  INFO=0,
  WARNING,
  ERROR,
  FATAL,
  DEBUG,
  CONNECTFAIL
};

inline std::ostream& Log(LogLevel lev,const char* file,int line,const std::string& logmsg)
{
  std::string level_info = Level[lev];
  std::string TimeStamp;
  LogTime::GetTimeStamp(&TimeStamp);
  std::cout<<"["<<TimeStamp<<" "<<level_info<<" "<<file<<":"<<line<<"]"<<logmsg<<std::endl;
  return std::cout;
}

#define LOG(lev,msg) Log(lev,__FILE__,__LINE__,msg)
