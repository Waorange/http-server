#include <iostream>
#include <stdlib.h>
#include <string>

#include "server.hpp"


int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: http_server <port>"<<std::endl;
        exit(1);
    }
    //忽略SIGPIPE信号防止对方提前关闭连接导致服务器被关闭
    signal(SIGPIPE, SIG_IGN);
    //初始化http服务器
    HttpServer * server = new HttpServer(atoi(argv[1]));
    server->InitServer();
    

    //启动http服务器
    server->run();    
    
    return 0;
}
