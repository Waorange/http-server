#include "connect.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

bool Connect::ReadOneLine(std::string & str)
{
    char ch_ = 0;
    while(ch_ != '\n')
    {
        ssize_t size_ = recv(sock_, &ch_, 1, 0);
        if(size_ > 0)
        { 
            if(ch_ == '\r')
            {
                recv(sock_, &ch_, 1, MSG_PEEK);
                if(ch_ == '\n')
                {
                    recv(sock_, &ch_, 1, 0);
                }
                ch_ = '\n';
            }
            str += ch_;
        }
        else 
        {
            return false;
        }
    }
    return true;
}
//读取请求
bool Connect::ReadRequestLine(std::string & line)
{
    if(!ReadOneLine(line))
        return false;
    return true;
}
bool Connect::ReadRequestHead(std::string & head)
{
    std::string line_;
    while(line_ != "\n")
    {
        line_.clear();
        if(!ReadOneLine(line_))
        {
            return false;
        }
        if(line_ == "\n")
        {
            break;
        }        
        head += line_;
    }
    return true;
}
bool Connect::ReadRequestText(std::string & param)
{
    int i = 0;
    char ch_;
    param.reserve(param.size() + content_length_);
    while(i < content_length_)
    {
        ssize_t size = recv(sock_, &ch_, 1, 0);
        if(size > 0)
        {
            param.push_back(ch_);
        }
        else
        {
            return false;
        }
        ++i;
    }
}

//发送
void Connect::SendReplay(bool cgi, Replay & rep, Resourse & res)
{
    const std::string & line_ = rep.GetReplayLine();
    const std::string & head_ = rep.GetReplayHead();
    const std::string & blank_ = rep.GetReplayBlank();

    send(sock_, line_.c_str(), line_.size(), 0);
    send(sock_, head_.c_str(), head_.size(), 0);
    send(sock_, blank_.c_str(), blank_.size(), 0);
    if(cgi)
    {
        const std::string & text_ = rep.GetReplayText();
        send(sock_, text_.c_str(), text_.size(), 0);
    }
    else
    {   
        const std::string & path_ = res.GetPath();
        int fd = open(path_.c_str(), O_RDONLY);
        sendfile(sock_, fd, NULL, res.GetResSize());
        close(fd); 
    }
}




