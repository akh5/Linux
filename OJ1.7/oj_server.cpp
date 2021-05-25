#include "httplib.h"
#include "oj_model.hpp"
#include "oj_view.hpp"
#include "oj_log.hpp"
#include "compile.hpp"
#include "tools.hpp"
#include "MyDB.h"
int main()
{
    using namespace httplib;
    Server svr;
    OjModel ojmodel;
    string usr="";
    string chec;
    svr.set_base_dir("./");
    svr.Get("/all_questions",[&ojmodel,&usr](const Request& req,Response& resp){
        vector<TestQues> ques;
        ojmodel.GetAllQuestions(&ques);  
        string html;
        cout<<html<<endl;
        OjView::ExpandAllQueshtml(&html,ques,usr);
        resp.set_content(html,"text/html;charset=UTF-8"); 
            });
    svr.Get("/all_question",[&ojmodel,&usr](const Request& req,Response& resp){
        vector<TestQues> ques;
        ojmodel.GetAllQuestions(&ques);  
        string html;
        cout<<html<<endl;
        OjView::ExpandAllqueshtml(&html,ques,usr);
        resp.set_content(html,"text/html;charset=UTF-8"); 
            });
    svr.Post("/all_questions",[&ojmodel,&usr](const Request& req,Response& resp){
        vector<TestQues> ques;
        ojmodel.GetAllQuestions(&ques);  
        string html;
        cout<<html<<endl;
				std::unordered_map<std::string,std::string> pram;
        UrlUtil::PraseBody(req.body,&pram);
        const std::string& user_name = pram["username"];
        const std::string& passwrd = pram["password"];
        cout<<"test  "<<endl;
        cout<<"username"<<endl;
        cout<<user_name<<endl;
        cout<<pram["username"]<<endl;
        cout<<"password"<<endl;
        cout<<passwrd<<endl;
        cout<<pram["password"]<<endl;
        cout<<"usr: "<<usr<<endl;
        usr = user_name;

        OjView::ExpandAllQueshtml(&html,ques,usr);
        resp.set_content(html,"text/html;charset=UTF-8"); 
            });

    svr.Get(R"(/question/(\d+))",[&ojmodel,&usr](const Request& req,Response& resp){
        std::string desc;
        std::string inp;
        std::string outp;
        std::string header;
        std::string comp;
        std::string title;
        LOG(INFO,"req.matches")<<req.matches[0]<<":"<<req.matches[1]<<std::endl;
        TestQues ques;
        ojmodel.GetOneQuestion(req.matches[1].str(),desc,inp,outp,header,comp,ques); 
        ojmodel.GetTitle(req.matches[1].str(),title,ques);
        cout<<title<<endl;
        cout<<"------------"<<endl;
        cout<<req.matches[0].str()<<endl;
        cout<<"------------"<<endl;
        std::cout<<req.matches[1].str()<<std::endl;
        cout<<"------------"<<endl;
        string tag=usr+req.matches[0].str();
        MyDB db;
        db.initDB("localhost","root","1231230","oj2");
        string sql1 ="INSERT user_question values('"+usr+"','"+title+"','"+req.matches[0].str()+"','"+tag+"');";
        cout<<"sql:  "<<sql1<<endl;
        db.exeSQL(sql1.c_str());
        std::string html;
        OjView::ExpandOneQuestion(ques,desc,inp,outp,header,comp,&html);
        //cout<<html<<endl;
        resp.set_content(html,"text/html;charset=UTF-8");
        });


    svr.Get("/login",[&ojmodel](const Request& req,Response& resp){
          string html;
          std::unordered_map<std::string,std::string> pram;
          UrlUtil::PraseBody(req.body,&pram);
          const std::string& user_name = pram["usr"];
          const std::string& passwrd = pram["passwrd"];
          LOG(INFO,"code ")<<user_name<<std::endl;
          cout<<"username"<<endl;
          cout<<user_name<<endl; 
          OjView::GetLogin(&html);
          resp.set_content(html,"text/html;charset=UTF-8");
        });
    svr.Post("/jump2",[&ojmodel,&chec](const Request& req,Response& resp){
          string html;
          string flag;
          std::unordered_map<std::string,std::string> pram;
          UrlUtil::PraseBody(req.body,&pram);
					const std::string& user_name = pram["username"];
          const std::string& passwrd = pram["password"];
          const std::string& repasswrd = pram["repassword"];
          const std::string& che = pram["code"];
          string content;
          MyDB db;
          db.initDB("localhost","root","1231230","oj2");
          string sql6 = "SELECT password FROM users WHERE name='"+user_name+"';";
          cout<<"sql  :"<<sql6<<endl;
          string outname = db.selectSQL(sql6.c_str());
          if(passwrd!=repasswrd)
          {
            content = "两次输入密码不一致，请重新注册...";
            cout<<"两次输入密码不一致，请重新注册..."<<endl;
            flag="false";
          }
          else if(che!=chec)
          {
            content = "验证码不匹配，请重新输入...";
            cout<<"验证码不匹配，请重新输入..."<<endl;
            flag = "false";
          }
          else if(outname!="")
          {
            content = "已存在该用户名，请重新输入...";
            flag="false";
          }
          else{
            flag = "true";
            string sql1 ="INSERT users values('"+user_name+"','"+passwrd+"');";
            cout<<"sql:  "<<sql1<<endl;
            db.exeSQL(sql1.c_str());
          }
          LOG(INFO,"code ")<<user_name<<std::endl;
          cout<<"username"<<endl;
          cout<<user_name<<endl; 
          OjView::GetJump2(&html,flag,content);
          resp.set_content(html,"text/html;charset=UTF-8");
        });
    svr.Get("/user",[&ojmodel,&usr](const Request& req,Response& resp){
          string html;
          std::unordered_map<std::string,std::string> pram;
          UrlUtil::PraseBody(req.body,&pram);
          cout<<"user:"<<usr<<endl;
          MyDB db;
          db.initDB("localhost","root","1231230","oj2");
          string sql3 = "SELECT question FROM user_question WHERE user='"+usr+"';";
          cout<<"sql3 :"<<sql3<<endl;
          vector<string> ques;
          db.execSelect(sql3.c_str(),ques);

          string sql4 = "SELECT link FROM user_question WHERE user='"+usr+"';";
          cout<<"sql4 :  "<<sql4<<endl;
          vector<string> links;

          db.execSelect(sql4.c_str(),links);
          cout<<"---------"<<endl;
          for(auto e:ques)
          {
            cout<<e<<endl;
          }
          OjView::ExpandUserQueshtml(&html,ques,links,usr);
          resp.set_content(html,"text/html;charset=UTF-8"); 
        });

    svr.Post("/jump",[&ojmodel,&usr](const Request& req,Response& resp){
          string html;
          string flag;
					std::unordered_map<std::string,std::string> pram;
          UrlUtil::PraseBody(req.body,&pram);        
          const std::string& user_name = pram["username"];
          const std::string& passwrd = pram["password"];
				  MyDB db;                                                                                                                                                                   
          db.initDB("localhost","root","1231230","oj2");
          
          string sql2 = "SELECT password FROM users WHERE name='"+user_name+"';";
          cout<<"sql  :"<<sql2<<endl;
          string outwrd = db.selectSQL(sql2.c_str());
          cout<<"outwrd  :"<<outwrd<<endl;
          if(outwrd==passwrd)
          {
            flag = "true";
            usr = user_name;
            if(user_name=="")
              flag="flase";
          }
          else
          {
            flag = "flase";
          }
          OjView::GetJump(&html,flag);
          resp.set_content(html,"text/html;charset=UTF-8");

        });
    
    svr.Get("/register",[&ojmodel,&chec](const Request& req,Response& resp){
          string html;
          chec.clear();
          Randchec::randstr(chec,8);
          cout<<"生成验证码"<<chec<<endl;
          OjView::GetRegister(&html,chec);
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
            cout<<"test code"<<endl;
            cout<<pram["code"]<<endl;
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
