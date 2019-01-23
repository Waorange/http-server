#ifndef __CODE_H__
#define __CODE_H__
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include "resourse.hpp"

class Replay
{
public:
    int & SetCode()
    {
        return code_;
    }
    void CgiReplay();
    void NoCgiReplay();

    void MakeStatusLine();
    void MakeReplayHand(bool cgi, Resourse & res);
    void MakeReplayBlank();
    std::string & MakeReplayText();
    static std::string IntToString(int n)
    {
        std::stringstream ss; 
        ss << n;
        return ss.str();
    }
    
    const std::string & GetReplayLine()
    {
        return rep_line_;
    }
    const std::string & GetReplayHead()
    {
        return rep_head_;
    }
    const std::string & GetReplayBlank()
    {
        return rep_blank_;
    }
    const std::string & GetReplayText()
    {
        return rep_text_;
    }
    const int GetCode()
    {
        return code_;
    }
    
private:
    std::string rep_line_;
    std::string rep_head_;
    std::string rep_blank_;
    std::string rep_text_;

    int code_;
};


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
    const std::string & GetReqParam()
    {
        return param_;
    }
    const std::string & GetReqHead()
    {
        return req_head_;
    }

    void GetContentLength(int & size);
    void RequestLineParse();
    void RequestUriParse(std::string & path);
    void RequestHeadParse();
    
    //暂时只接收get和pust方法
    bool IsMathodLegal(bool & cgi);

    bool IsHaveText();
    
    //只处理200
    void JudgeCode(int & code);
private:
    std::string req_line_;
    std::string req_head_;
    //空行直接丢弃
    
    //正文直接放入参数中
    //std::string req_text_;

    std::string mathod_;
    std::string uri_;    
    std::string version_;

    std::unordered_map<std::string, std::string> head_argu_;
    std::string param_; //传递给cgi解析
};


#endif

