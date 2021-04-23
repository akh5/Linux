#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <jsoncpp/json/json.h>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <fcntl.h>

#include "oj_log.hpp"
#include "tools.hpp"


enum ErrorNo
{
    OK=0,
    COMPILE_ERROR,
    RUN_ERROR,
    PRAM_ERROR,
    INTERNAL_ERROR
};


class Compiler
{
    public:
        static void CompileAndRun(Json::Value Req,Json::Value* Resp)
        {

            if(Req["code"].empty())
            {
                (*Resp)["errorno"] = PRAM_ERROR;
                (*Resp)["reason"] = "Pram error";
                LOG(ERROR,"Request code is Empty")<<std::endl;
                return;
            }
            std::string code = Req["code"].asString();

            std::string tmp_filename = WriteTmFile(code);
            if(tmp_filename == "")
            {
                (*Resp)["errorno"] = INTERNAL_ERROR;
                (*Resp)["reason"] = "Create file failed";
                LOG(ERROR,"writeSource failed");
                return;
            }
            if(!Compile(tmp_filename))
            {
                (*Resp)["errorno"] = COMPILE_ERROR;
                std::string reason;
                FileOpen::ReadDataFromFile(ErrPath(tmp_filename),reason);
                (*Resp)["reason"] = reason; 
                LOG(ERROR,"Compile Error")<<std::endl;
                return;
            }

            int sig = Run(tmp_filename);
            if(sig!=0)
            {
                (*Resp)["errorno"] = RUN_ERROR;
                (*Resp)["reason"] = "Program exit by sig"+std::to_string(sig);
                LOG(ERROR,"run error")<<std::endl;
                return;
            }
            (*Resp)["errorno"] = OK;
            (*Resp)["reason"] = "Compile and run is okey!";

            std::string stdout_reason;
            FileOpen::ReadDataFromFile(StdoutPath(tmp_filename),stdout_reason);
            (*Resp)["stdout"] = stdout_reason;

            //
            std::string stderr_reason;
            FileOpen::ReadDataFromFile(StderrPath(tmp_filename),stderr_reason);
            (*Resp)["stderr"] = stderr_reason;
            return;

        }
        static void WriteResoutToFile(const char* filename,Json::Value Resp)
        {
            std::cout<<filename<<std::endl;
            FILE *fl = fopen(filename,"w");
            std::string err = Resp["errno"].asString();
            err.append("\r\n");
            std::cout<<err<<std::endl;
            std::string reason =Resp["reason"].asString();
            reason.append("\r\n");
            std::cout<<reason<<std::endl;
            std::string sout = Resp["stdout"].asString();
            sout.append("\r\n");
            std::cout<<sout<<std::endl;
            fwrite(err.c_str(),err.size(),1,fl);
            fwrite(reason.c_str(),reason.size(),1,fl);
            fwrite(sout.c_str(),sout.size(),1,fl);
            fclose(fl);
        }
    private:
        static std::string WriteTmFile(const std::string& code)
        {
            std::string tmp_filename = "/tmp_"+std::to_string(LogTime::GetTimeStamp());
            
            int ret = FileOpen::WriteDataToFile(SrcPath(tmp_filename),code);
            if(ret<0)
            {
                LOG(ERROR,"write code to source failed");
                return "";
            }
            return tmp_filename;
        }
        static std::string  SrcPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".cpp";
        }
        static std::string  ExePath(const std::string& filename)
        {
            return "./tmp_files"+filename+".executable";
        }
        static std::string  ErrPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".err";
        }
        static std::string  StdoutPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".stdout";
        }
        static std::string  StderrPath(const std::string& filename)
        {
            return "./tmp_files"+filename+".stderr";
        }



        static bool Compile(const std::string& filename)
        {
            const int commandcount = 20;
            char buf[commandcount][50] = {{0}};
            char* Command[commandcount] = {0};
           for(int i=0;i<commandcount;i++)
           {
               Command[i] = buf[i];
           }
           snprintf(Command[0],49,"%s","g++");
           snprintf(Command[1],49,"%s",SrcPath(filename).c_str());
           snprintf(Command[2],49,"%s","-o");
           snprintf(Command[3],49,"%s",ExePath(filename).c_str());
           snprintf(Command[4],49,"%s","-std=c++11");
           snprintf(Command[5],49,"%s","-D");
           snprintf(Command[6],49,"%s","CompileOnline");
           Command[7] = NULL;
           int pid = fork();
           if(pid<0)
           {
               LOG(ERROR,"Create child process failed");
                return false;
           }
           else if(pid == 0)
           {
                int fd = open(ErrPath(filename).c_str(),O_CREAT|O_RDWR,0664);
                if(fd<0)
                {
                    LOG(ERROR,"open compile errorfile failed")<<ErrPath(filename)<<std::endl;
                    exit(1);
                }
                dup2(fd,2);
                execvp(Command[0],Command);
                exit(0);

           }
           else
           {
                waitpid(pid,NULL,0);
           }
           struct stat st;
           int ret = stat(ExePath(filename).c_str(),&st);
           if(ret<0)
           {
               LOG(ERROR,"Compile ERROR exe filname is ")<<ExePath(filename)<<std::endl;
               return false;
           }
           return true;
        }

        static int Run(std::string &filename)
        {
            int pid = fork();
            if(pid<0)
            {
                LOG(ERROR,"Exec pragma failed! create child process failed")<<std::endl;
                return -1;
            }
            else if(pid == 0)
            {
                alarm(1);
                struct rlimit rl;
                rl.rlim_cur = 1024*30000;
                rl.rlim_max = RLIM_INFINITY;
                setrlimit(RLIMIT_AS,&rl);

                int stdout_fd = open(StdoutPath(filename).c_str(),O_CREAT|O_RDWR,0664);
                if(stdout_fd<0)
                {
                    LOG(ERROR,"Open stdout file failed")<<StdoutPath(filename)<<std::endl;
                    return -1;
                }
                dup2(stdout_fd,1);
                
                int stderr_fd = open(StderrPath(filename).c_str(),O_CREAT|O_RDWR,0664);
                if(stderr_fd<0)
                {
                    LOG(ERROR,"Open stderr file failed")<<StdoutPath(filename)<<std::endl;
                    return -1;
                }
                dup2(stdout_fd,2);

                execl(ExePath(filename).c_str(),ExePath(filename).c_str(),NULL);
                exit(1);
            }
            int Status = -1;
            waitpid(pid,&Status,0);
            return Status & 0x7f;

        }
};
