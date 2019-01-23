#ifndef __CONNECTION_H__
#define __CONNECTION_H__
#include <string>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/types.h>
#include "code.h"

//负责数据的读取和发送
class Connect
{
public:
    Connect(int sock)
        :sock_(sock)
    {}

    //设置正文大小
    int & SetContentLength()
    {
        return content_length_;
    }
    //读取请求
    bool ReadRequestLine(std::string & line);
    bool ReadRequestHead(std::string & head);
    bool ReadRequestText(std::string & param);

    //发送
    void SendReplay(bool cgi, Replay & rep, Resourse & res);
    ~Connect()
    {
        if(sock_ > 0)
            close(sock_);
    }
private:
    //从缓存区读一行，另外将所有的换行转换为\n
    bool ReadOneLine(std::string & str);
private:
    int sock_;
    int content_length_;
};

#endif
