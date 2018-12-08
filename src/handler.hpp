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
    {}
    std::string & SetPath();
    bool IsPathLegal(bool & cgi);

private:
    std::string path_;
    int size_;
    std::string type_;
};

class Handler
{
public:
    Handler(int sock)
        :cont_(sock)
    {}
    
    void ProcessReplay()
    {
        if(cgi_)
        {
            rep_.CgiReplay();
        }
        else
        {
            rep_.NoCgiReplay();
        }
    }
    void run()
    {
        cont_.ReadRequestLine(req_.SetReqLine());
        req_.RequestLineParse();
        if(!req_.IsMathodLegal())
        {
            //TODO
            //ProcessError();
        }
        req_.RequestUriParse();
        req_.GetResoursePath(res_.SetPath());
        if(!res_.IsPathLegal(cgi_))
        {
            //TODO
            //ProcessError();
        }

        cont_.ReadRequestHead(req_.SetReqHead());


        req_.RequestHeadParse();

        if(req_.IsHaveText())
        {
            req_.GetContentLength(cont_.SetContentLength());
            cont_.ReadRequestText(req_.SetReqParam());
        }
        ProcessReplay();
        cont_.SendReplay(rep_);
    }
private: 
    bool cgi_;
    Resourse res_;
    Request req_;
    Replay rep_;
    Connect cont_;
};





#endif
