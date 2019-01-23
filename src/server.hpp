#ifndef __SERVER_HPP__
#define __SERVER_HPP__
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>


#include "log.hpp"

#include "event_pool.hpp"

#include "handler.hpp"
#include "thread_pool.hpp"



class HttpServer
{
public:
    HttpServer(int port)
        : port_(port)
        , listen_sock_(0)
        , tp(NULL)
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

        if(!epoll_.EventAdd(listen_sock_, false))
        {
            LOG(FATAL, "listen socket error");
            exit(1);
        }

        //初始化线程池
        tp = new ThreadPool();
        tp->InitThreadPool();

        Handler::RemoveEpoll(&epoll_);
        Handler::RemoveEventPool(&eventpool_);

        ThreadPool::RemoveEventPool(&eventpool_);

        LOG(INFO, "InitServer success");
    }
    void run()
    {
        LOG(INFO, "start server");

        for(;;)
        {
            std::vector<int> revents;
            if(!epoll_.EpollWait(revents))
            {
                LOG(WARNING, "accept error");
                continue;
            }

            struct sockaddr_in client_;
            socklen_t len_ = sizeof(client_); 
            for(auto & fd : revents)
            {
                if(fd == listen_sock_)
                {
                    int sock_ = accept(listen_sock_, (struct sockaddr *)&client_, &len_);

                    if(sock_ < 0){
                        LOG(WARNING, "accept error");
                        continue;
                    }
                    //添加到epoll_监听
                    epoll_.EventAdd(sock_);

                    //添加到事件池
                    Event event(sock_, &Handler::ReadAndDecode);
                    eventpool_.EventPush(sock_, event);
                }
                else
                {   
                    //表示客户端可以读或者cgi程序返回结果, 添加到线程池就绪队列中
                    tp->EventPush(eventpool_.GetPEvent(fd));
                }
            }   
        }
    }
    
//  void run()
//  {
//      LOG(INFO, "start server");
//      for(;;)
//      {   
//          struct sockaddr_in client_;
//          socklen_t len_ = sizeof(client_);    
//          int sock_ = accept(listen_sock_, (struct sockaddr *)&client_, &len_);

//          if(sock_ < 0)
//          {
//              LOG(WARNING, "accept error");
//              continue;
//          }
//          
//          LOG(INFO, "start handler ...");  
//          Task task_(sock_);
//          tp->TaskPush(task_);
//      }
//  }
//  //MODIFY
//  static void * StartRoutine(void * arg)
//  {
//      pthread_detach(pthread_self());
//      long sock_long_ = reinterpret_cast<long>(arg);
//      int sock_ = static_cast<int>(sock_long_);

//      Handler * handler = new Handler(sock_);
//      handler->run();

//      delete handler;
//      pthread_exit(0);
//  }

    ~HttpServer()
    {
        if(listen_sock_ > 0)
            close(listen_sock_);
    }
public:
    Epoll epoll_;
    EventPool eventpool_;
private:
    int port_;
    int listen_sock_;
    ThreadPool * tp;
};



#endif
