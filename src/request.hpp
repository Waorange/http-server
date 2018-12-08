#ifndef __REQUEST__HPP_
#define __REQUEST__HPP_
#include <string>
#include <unordered_map>

class Request
{
public:
    std::string & SetReqLine()
    {
        return req_line_;
    }
    std::string & SetReqHead()
    {
        return req_head_;
    }
    std::string & SetReqParam()
    {
        return param_;
    }
    void GetResoursePath(std::string & path);
    void GetContentLength(int & size);
    void RequestLineParse();
    void RequestUriParse();
    void RequestHeadParse();
    
    //暂时只接收get和pust方法
    bool IsMathodLegal();

    bool IsHaveText();

private:
    std::string req_line_;
    std::string req_head_;
    //空行直接丢弃
    
    //正文直接放入参数中
    //std::string req_text_;

    std::string mathod_;
    std::string uri_;    
    std::string version_;

    std::unordered_map<std::string, std::string> head_argu;
    std::string param_; //传递给cgi解析
};


#endif

