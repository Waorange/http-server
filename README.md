# http-server
基于HTTP1.0的自主web服务器

#version1
采用线程池和任务队列的方式处理请求

主线程负责连接，工作线程从任务队列获取任务进行处理，直到处理完成关闭连接重新获取任务


#version2
解决处理cgi时工作线程阻塞等待问题
采用epoll + 线程池 + 事件池 + 就绪队列的方式处理

主线程负责等待epoll事件触发，当收到一个连接在事件池进行注册事件，并将其添加
到epoll中,当该连接可读时添加到就绪队列，线程池中工作线程从就绪队列中获取该事
件在事件池中指针运行注册的事件，当该请求需处理cgi时，则创建子进程并发送参数
， 然后将管道文件描述符加入到epoll并在事件池注册事件，当可读时加入就绪队列由
工作线程进行运行。

