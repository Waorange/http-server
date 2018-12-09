#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#include "log.hpp"
#include "handler.hpp"

class HttpServer
{
public:
    HttpServer(int port)
        : port_(port)
        , listen_sock_(0)
    {}
    void InitServer()
    {
        //创建监听套接字
        listen_sock_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(listen_sock_ == -1)
        {
            LOG(FATAL, "listen_sock_ create error");
            exit(1);
        }
        int opt_ = 1;
        setsockopt(listen_sock_, SOL_SOCKET, SO_REUSEADDR, &opt_, sizeof(opt_));
        
        //服务器地址信息
        struct sockaddr_in server_addr_;
        server_addr_.sin_family = AF_INET;
        server_addr_.sin_port = htons(port_);
        server_addr_.sin_addr.s_addr = INADDR_ANY;

        if(bind(listen_sock_, (struct sockaddr *)&server_addr_, sizeof(server_addr_)) < 0)
        {
            LOG(FATAL, "bind socket error");
            exit(1);
        }
        
        if(listen(listen_sock_, 5) < 0)
        {
            LOG(FATAL, "listen socket error");
            exit(1);
        }
        LOG(INFO, "InitServer success");
    }
    
    void run()
    {
        LOG(INFO, "start server");
        // MODIFY
        // 后面添加线程池和epoll
        for(;;)
        {   
            struct sockaddr_in client_;
            socklen_t len_ = sizeof(client_);    
            int sock_ = accept(listen_sock_, (struct sockaddr *)&client_, &len_);

            long sock_long = static_cast<long>(sock_);
            void * sock_voidp = reinterpret_cast<void *>(sock_long);

            if(sock_ < 0)
            {
                LOG(WARNING, "accept error");
                continue;
            }

            //LOG(INFO, "accept success");
            pthread_t pid = 0;
            LOG(INFO, "start handler ...");   
            pthread_create(&pid, NULL, StartRoutine, sock_voidp);
        }
    }
    //MODIFY
    static void * StartRoutine(void * arg)
    {
        pthread_detach(pthread_self());
        long sock_long_ = reinterpret_cast<long>(arg);
        int sock_ = static_cast<int>(sock_long_);

        Handler * handler = new Handler(sock_);
        handler->run();

        delete handler;
        pthread_exit(0);
    }

    ~HttpServer()
    {
        if(listen_sock_ > 0)
            close(listen_sock_);
    }
private:
    int port_;
    int listen_sock_;
};

#endif
