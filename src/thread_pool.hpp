#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include "event_pool.hpp"
#define THREAD_AMOUNT_MAX 5


//包含就绪队列
class ThreadPool
{
public:
    ThreadPool(int num = THREAD_AMOUNT_MAX)
        :thread_amount_(num)
        ,hangup_amount_(0)
    {
        pthread_mutex_init(&mutex_, NULL);
        pthread_cond_init(&cond_, NULL);
    }
    void InitThreadPool()
    {
        for(int i = 0; i < thread_amount_; ++i)
        {
            pthread_t tid_;
            pthread_create(&tid_, NULL, start_rounte, this);
        }
    }
    void EventPush(Event * event)
    {
        LockQueue();
        if(hangup_amount_ > 0)
        {
            RouseOneThread();
        }
        ready_queue_.push(event);
        UnLockQueue();
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
    static void RemoveEventPool(EventPool * peventpool)
    {
        s_peventpool = peventpool;
    }
private:
    void EventPop(Event & event)
    {
        event = *(ready_queue_.front());
        ready_queue_.pop();
    }
    void RouseOneThread()
    {
        pthread_cond_signal(&cond_);
    }
    void LockQueue()
    {
        pthread_mutex_lock(&mutex_);
    }
    void UnLockQueue()
    {
        pthread_mutex_unlock(&mutex_);
    }
    bool EventIsEmpty()
    {
        return ready_queue_.empty();
    }
    static void * start_rounte(void * arg)
    {
        ThreadPool *tp = static_cast<ThreadPool *>(arg);
        pthread_detach(pthread_self());
        for(;;)
        {
            tp->LockQueue();
            while(tp->EventIsEmpty())
            {
                tp->ThreadHangup();
            }
            Event event_;
            tp->EventPop(event_);
            tp->UnLockQueue();
            s_peventpool->EventPop(event_.GetFd());
            event_.run();    
        }
    }

    void ThreadHangup()
    {
        ++hangup_amount_;
        pthread_cond_wait(&cond_, &mutex_);
        --hangup_amount_;
    }

private:
    std::queue<Event *> ready_queue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    int thread_amount_;
    int hangup_amount_;
    static EventPool * s_peventpool;
};

EventPool* ThreadPool::s_peventpool = NULL;



