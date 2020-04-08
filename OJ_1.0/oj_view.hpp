#pragma once
#include <ctemplate/template.h>
#include <string>
#include <vector>
#include "oj_model.hpp"

class OjView
{
    public:
        static void ExpandAllQueshtml(std::string* html,std::vector<TestQues>& ques)
        {
            ctemplate::TemplateDictionary dict("all_questions");
            for(const auto& que:ques)
            {
                ctemplate::TemplateDictionary* section_dict=dict.AddSectionDictionary("question");
                section_dict->SetValue("id",que._id);
                section_dict->SetValue("name",que._name);
                section_dict->SetValue("difficulty",que._difficulty);
            }
            ctemplate::Template* t1 = ctemplate::Template::GetTemplate("./view/all_questions.html",ctemplate::DO_NOT_STRIP);
            t1->Expand(html,&dict);
        }
        static void ExpandOneQuestion(const TestQues& ques,std::string& desc,std::string& header,std::string* html)
        {
          ctemplate::TemplateDictionary dict("question");
          dict.SetValue("id",ques._id);
          dict.SetValue("name",ques._name);
          dict.SetValue("difficulty",ques._difficulty);
          dict.SetValue("desc",desc);
          dict.SetValue("header",header);

          ctemplate::Template* tpl=ctemplate::Template::GetTemplate("./view/question.html",ctemplate::DO_NOT_STRIP);
          tpl->Expand(html,&dict);
        }
};
