#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__
#include <string>

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
    void ReadRequestLine(std::string & line);
    void ReadRequestHead(std::string & head);
    void ReadRequestText(std::string & text);

    //发送
    void SendReplay(const Replay & rep);
    
private:
    //从缓存区读一行，另外将所有的换行转换为\n
    void ReadOneLine(std::string & str);
private:
    int sock_;
    int content_length_;
};

#endif
