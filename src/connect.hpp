#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__



//负责数据的读取和发送
class Connect
{
public:
    Connect(int sock)
        :sock_(sock)
    {}

    //读取请求
    void ReadRequestLine(std::string & line);
    void ReadRequestHead(std::string & head);
    void ReadRequestText(std::string & text);

    //发送
    void SendReplay(const Replay * rep);
    
private:
    //从缓存区读一行，另外将所有的换行转换为\n
    std::string & ReadOneLine();
private:
    int sock_;
};

#endif
