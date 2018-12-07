#ifndef __HANDLER__HPP__
#define __HANDLER__HPP__
#include "request.hpp"
#include "replay.hpp"
#include "connect.hpp"

class Resourse
{
public:
    Resourse()
        :path_("../httproot")
        ,size_(0)
    {
    }
private:
    std::string path_;
    int size_;
    std::string type_;
};

class Handler
{
public:
    Handler(int sock)
        :sock_(sock)
        ,cont_(sock)
    {}
    int & SetCgi()
    {
        return cgi_;
    }

    //待修改
    static void* run(void *arg)
    {}
private:
    int sock_;
    int cgi_; 
    Resourse res_;
    Request req_;
    Replay rep_;
    Connect cont_;
};


#endif
