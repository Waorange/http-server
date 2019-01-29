#ifndef __EVENT_POOL_HPP__
#define __EVENT_POOL_HPP__
#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/epoll.h>
#include <unordered_map>

class Handler;

//事件
class Event
{
typedef void (*EventHandler)(Handler * handler, int fd);
public:
    Event()
        :fd_(0)
        ,handler_(NULL)
        ,evhandler_(NULL)
    {
    }
    Event(int fd, EventHandler evhandler, Handler * handler = NULL)
        :fd_(fd)
        ,handler_(handler)
        ,evhandler_(evhandler)
    {}
    void run()
    {
        evhandler_(handler_, fd_);
    }
    int GetFd()
    {
        return fd_;
    }
private:
    int fd_;
    Handler * handler_;
    EventHandler evhandler_;
};

//事件池
class EventPool
{
public:
    EventPool()
    {
        pthread_mutex_init(&mutex_, NULL);
    }
    void EventPush(int fd, Event & event)
    {
        LockMap();
        events_.insert(std::make_pair(fd, event));
        UnLockMap();
    }
    //返回Event指针存储到就绪队列中
    Event* GetPEvent(int fd)
    {
        //随后改读写锁
        LockMap();
        Event * ret = &events_[fd];
        UnLockMap();
        return ret;
    }

    void EventPop(int fd)
    {
        LockMap();
        events_.erase(fd);
        UnLockMap();
    }
private:
    void LockMap()
    {
        pthread_mutex_lock(&mutex_);
    } 
    void UnLockMap()
    {
        pthread_mutex_unlock(&mutex_);
    } 
private:
    std::unordered_map<int, Event> events_;
    pthread_mutex_t mutex_;
};


class Epoll
{
public:

    Epoll()
    {
        epoll_fd = epoll_create(10);
        revents = new struct epoll_event[100];
    }
    ~Epoll()
    {
        close(epoll_fd);
        delete [] revents;
    }
    bool EventAdd(int fd, bool oneshot = true) 
    {
        struct epoll_event event;
        if(oneshot){
            event.events = EPOLLIN | EPOLLONESHOT;
        }
        else{
            event.events = EPOLLIN;
        }
        event.data.fd = fd;
        
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) < 0){
            return false;
        }
        return true;
    }

    bool EventDel(int fd)
    {
        if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL) < 0){
            return false;
        }
        return true;
    }

    bool EpollWait(std::vector<int> & fdvector)
    { 
        
        int fd_num = epoll_wait(epoll_fd, revents, 100, -1);
        if(fd_num < 0){
            return false;
        }

        for(int i = 0; i < fd_num; ++i)
        {
            int event_fd;
            event_fd = revents[i].data.fd;
            fdvector.push_back(event_fd);
        }
        return true;
    }

    
private:
    int epoll_fd;
    struct epoll_event * revents;
};

//使用饿汉单例模式启动epoll和eventpool
class Singleton
{
public:
    static Epoll * GetEpoll()
    {
        return epoll;
    }
    static EventPool * GetEventPool()
    {
        return eventpool;
    }
private:
    Singleton() = delete;
    Singleton(Singleton const &) = delete;
    Singleton & operator=(Singleton const&) = delete;
private:
    static EventPool * eventpool;
    static Epoll * epoll;
};

EventPool * Singleton::eventpool = new EventPool;
Epoll * Singleton::epoll = new Epoll;

#endif
