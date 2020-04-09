#include "httplib.h"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include "oj_log.hpp"
#include "compile.hpp"
int main()
{
    using namespace httplib;
    Server svr;
    OjModel ojmodel;
    svr.set_base_dir("./");
    svr.Get("/all_questions",[&ojmodel](const Request& req,Response& resp){
        vector<TestQues> ques;
        ojmodel.GetAllQuestions(&ques);  
        string html;
        cout<<html<<endl;
        OjView::ExpandAllQueshtml(&html,ques);
        resp.set_content(html,"text/html;charset=UTF-8"); 
            });


    svr.Get(R"(/question/(\d+))",[&ojmodel](const Request& req,Response& resp){
        std::string desc;
        std::string inp;
        std::string outp;
        std::string header;
        std::string comp;
        LOG(INFO,"req.matches")<<req.matches[0]<<":"<<req.matches[1]<<std::endl;
        TestQues ques;
        ojmodel.GetOneQuestion(req.matches[1].str(),desc,inp,outp,header,comp,ques); 
        std::cout<<req.matches[1].str()<<std::endl;
        std::string html;
        OjView::ExpandOneQuestion(ques,desc,inp,outp,header,comp,&html);
        //cout<<html<<endl;
        resp.set_content(html,"text/html;charset=UTF-8");
        });
    
     svr.Post(R"(/question/(\d+))",[&ojmodel](const Request& req,Response& resp){
            std::string desc;
            std::string inp;
            std::string outp;
            std::string header;
            std::string comp;
            TestQues ques;
            std::unordered_map<std::string,std::string> pram;
            UrlUtil::PraseBody(req.body,&pram);
            for(const auto& pr:pram)
            {
                LOG(INFO,"code ")<<pr.second<<std::endl;
            }
            std::string code;
            std::string path1="."+req.matches[0].str()+"/save.cpp";
            std::string path2="."+req.matches[0].str()+"/result.txt";
            
            FileOpen::WriteDataToFile(path1,pram["code"]); 

            ojmodel.GetOneQuestion(req.matches[1].str(),desc,inp,outp,header,comp,ques); 

            ojmodel.SplicingCode(pram["code"],req.matches[1].str(),&code);
            Json::Value req_json;
            req_json["code"] = code;
            Json::Value Resp_json;
            Compiler::CompileAndRun(req_json,&Resp_json);
            Compiler::WriteResoutToFile(path2.c_str(),Resp_json);
            LOG(INFO,"code ")<<code<<std::endl;

            std::string save;
            std::string result;
            FileOpen::ReadDataFromFile(path1.c_str(),save);
            FileOpen::ReadDataFromFile(path2.c_str(),result);
            std::cout<<result<<endl;

            std::string html ;
            
            OjView::ExpandOneQuestion(ques,desc,inp,outp,save,result,&html);

            resp.set_content(html,"text/html;charset=UTF-8");
            });

    LOG(INFO,"listent 0.0.0.0:19999")<<std::endl;
    LOG(INFO,"Server ready")<<std::endl;
    svr.listen("0.0.0.0",19999);
    return 0;
}
