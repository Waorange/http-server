#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>

#define THREAD_AMOUNT_MAX 5


class Task
{
public:
    Task(int sock = 0)
        :sock_(sock)
    {}
    void run()
    {
        Handler * handler = new Handler(sock_);
        handler->run();
        delete handler;
    }
private:
    int sock_;
};

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
    void TaskPush(Task & task)
    {
        LockQueue();
        if(hangup_amount_ > 0)
        {
            RouseOneThread();
        }
        task_queue_.push(task);
        UnLockQueue();
    }
    ~ThreadPool()
    {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
    }
private:
    void TaskPop(Task & task)
    {
        task = task_queue_.front();
        task_queue_.pop();
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
    bool TaskIsEmpty()
    {
        return task_queue_.empty();
    }
    static void * start_rounte(void * arg)
    {
        ThreadPool *tp = static_cast<ThreadPool *>(arg);
        pthread_detach(pthread_self());
        for(;;)
        {
            tp->LockQueue();
            while(tp->TaskIsEmpty())
            {
                tp->ThreadHangup();
            }
            Task task_;
            tp->TaskPop(task_);
            tp->UnLockQueue();
            task_.run();    
        }
    }

    void ThreadHangup()
    {
        ++hangup_amount_;
        pthread_cond_wait(&cond_, &mutex_);
        --hangup_amount_;
    }

private:
    std::queue<Task> task_queue_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
    int thread_amount_;
    int hangup_amount_;
};






