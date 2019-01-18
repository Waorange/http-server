#include "request.hpp"
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <strings.h>

//暂时这样处理随后，需要添加配置文件
#define HOME_PAGE "index.html"

void Request::GetContentLength(int & size)
{
   size = atoi(head_argu_["Content-Length"].c_str());
}

void Request::RequestLineParse()
{
    std::stringstream str(req_line_);
    //请求行中用空格进行分隔
    str >> mathod_ >> uri_ >> version_;
}
void Request::RequestUriParse(std::string & path)
{
    if(mathod_ == "POST")
    {
        path += uri_; 
    }
    else if(mathod_ == "GET")
    {
        size_t pos_ = uri_.find('?');
        if(pos_ != std::string::npos)
        {
            path += uri_.substr(0, pos_);
            param_ += uri_.substr(pos_+1);
        }
        else
        {
            path += uri_;
        }
    }
    
    if(path[path.size() - 1] == '/')
    {
        path += HOME_PAGE;
    }
} 
void Request::RequestHeadParse()
{
    size_t pos_ = 0;
    size_t start_ = 0;
    std::string oneline_;
    while(start_ < req_head_.size())
    {
        pos_ = req_head_.find('\n', start_);
        if(pos_ != std::string::npos)
        {
            oneline_ = req_head_.substr(start_, pos_ - start_);
            start_ = pos_ + 1;
            size_t index_ = oneline_.find(": ");
            head_argu_[oneline_.substr(0, index_)] = oneline_.substr(index_ + 2); 
        }
        else
        {
            break;
        }
    }
}

//暂时只接收get和post方法
bool Request::IsMathodLegal(bool & cgi)
{
    if(strcasecmp(mathod_.c_str(), "GET") == 0)
    {
        mathod_ = "GET";
        return true;
    }
    if(strcasecmp(mathod_.c_str(), "POST") == 0)
    {
        mathod_ = "POST";
        cgi = true;
        return true;
    }
    //添加其他方法
}

bool Request::IsHaveText()
{
    if(mathod_ == "POST")
    {
        return true; 
    }

    if(mathod_ == "GET")
    {   
        return false;
    }
    //添加其他方法
    return false;
}

void Request::JudgeCode(int & code) 
{
    //根据报头添加其他
    code = 200; 
}


