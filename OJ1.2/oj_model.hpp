#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include "tools.hpp"
#include "oj_log.hpp"
#include <string>
#include <unordered_map>
using namespace std;

typedef struct TestQues
{
    string _id;
    string _family;
    string _name;
    string _path;
    string _difficulty;
    string _percent;
    
}TestQues;


class OjModel
{
    public:
        OjModel()
        {
            LoadQues("./config_oj.conf");
        }
        bool GetAllQuestions(std::vector<TestQues>* ques)
        {
          for(const auto& kv:_model_map)
          {
            ques->push_back(kv.second);
          }
          std::sort(ques->begin(),ques->end(),[](const TestQues& l,const TestQues& r){
              return std::atoi(l._id.c_str())<std::atoi(r._id.c_str());
              });
          
          return true;
        }
        bool GetOneQuestion(const string& id,string& desc,string& inp,string& outp,string& header,string& comp,TestQues& ques)
        {
          auto iter = _model_map.find(id);
          if(iter == _model_map.end())
          {
            LOG(ERROR,"Not Found Question ")<<id<<endl;
            return false;
          }

          ques=iter->second;


          int ret = FileOpen::ReadDataFromFile(DescPath(iter->second._path),desc);
          if(ret==-1)
          {
            LOG(ERROR,"Read desc failed")<<endl;
            return false;
          }
          ret = FileOpen::ReadDataFromFile(Inp(iter->second._path),inp);
          if(ret==-1)
          {
            LOG(ERROR,"Read inp failed")<<endl;
            return false;
          }
          ret = FileOpen::ReadDataFromFile(Outp(iter->second._path),outp);
          if(ret==-1)
          {
            LOG(ERROR,"Read outp failed")<<endl;
            return false;
          }
          ret = FileOpen::ReadDataFromFile(HeaderPath(iter->second._path),header);
          if(ret==-1)
          {
            LOG(ERROR,"Read header failed")<<endl;
            return false;
          }
          ret = FileOpen::ReadDataFromFile(Comp(iter->second._path),comp);
          if(ret==-1)
          {
            LOG(ERROR,"Read comp failed")<<endl;
            return false;
          }

          return true;
        }
        bool SplicingCode(string user_code,const string& ques_id,string* code)
        {
            auto iter = _model_map.find(ques_id);
            if(iter==_model_map.end())
            {
                LOG(ERROR,"can not find question id is ")<<ques_id<<endl;
                return false;
            }
            string tail_code;
            int ret = FileOpen::ReadDataFromFile(TailPath(iter->second._path),tail_code);
            if(ret<0)
            {
                LOG(ERROR,"Open tail.cpp failed");
                return false;
            }
            *code = user_code + tail_code;
            return true;
        }

    private:
        bool LoadQues(const string& config_path)
        {
           ifstream file(config_path.c_str());
           if(!file.is_open())
               return false; 
           string line;
           while(getline(file,line))
           {
              vector<string> vec;
              StringTools::Split(line," ",vec);
              cout<<endl<<vec.size()<<endl;
              if(vec.size()!=6)
              {
                continue;
              }
              TestQues ques;
              ques._id = vec[0];
              ques._family = vec[1];
              ques._name = vec[2];
              ques._path = vec[3];
              ques._difficulty = vec[4];
              ques._percent = vec[5];
              _model_map[ques._id] = ques;
           }
           file.close();
           return true;
        }
        string DescPath(const string ques_path)
        {
          return ques_path+"desc.txt";
        }
        string Inp(const string ques_path)
        {
          return ques_path+"inp.txt";
        }
        string Outp(const string ques_path)
        {
          return ques_path+"outp.txt";
        }
        string HeaderPath(const string ques_path)
        {
          return ques_path+"header.cpp";
        }
        string Comp(const string ques_path)
        {
          return ques_path+"comp.txt";
        }
        string TailPath(const string ques_path)
        {
          return ques_path+"tail.cpp";
        }

    private:
        unordered_map<string,TestQues > _model_map;

};
