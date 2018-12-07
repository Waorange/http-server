#ifndef __REQUEST__HPP_
#define __REQUEST__HPP_
#include <string>
#include <unordered_map>

class Request
{
public:
    void RequsetLineParse();
    void RequsetUri();
    void RequsetHand();

    bool LegalJudgeMathod();

private:
    std::string req_line_;
    std::string req_head_;
    //空行直接丢弃
    std::string req_text_;

    std::string mathod_;
    std::string uri_;    
    std::string version_;

    std::unordered_map<std::string, std::string> head_argu;
    std::string parameter_; //传递给cgi解析
};


#endif

