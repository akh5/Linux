#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>
using namespace std;

#define THREADCOUNT 4

typedef void (*Handler)(int);
class ThreadTask
{
  public:
    ThreadTask()
    {
      _data = -1;
      _handler = NULL;
    }
    ThreadTask(int Data,Handler h)
    {
      _data = Data;
      _handler = h;
    }

    void run()
    {
      _handler(_data);
    }
  private:
    int _data;
    Handler _handler;
};

class thread_pool
{
  public:
    thread_pool()
    {
      _threadCap = THREADCOUNT; 
      _threadCurNum = THREADCOUNT;
      pthread_mutex_init(&_Mutex,NULL);
      pthread_cond_init(&_Cond,NULL);
      _isQuit = false;
      bool isCreate = ThreadCreat();
      if(!isCreate)
      {
        printf("ThreadPool Create failed\n");
        exit(1);
      }
    }

    bool Push(ThreadTask* Tt)
    {
      pthread_mutex_lock(&_Mutex);
      //如果设置线程池中的线程进行退出，不要进行PUSH将互斥锁释放，让线程退出
      if(_isQuit)
      {
        pthread_mutex_unlock(&_Mutex);
        return false;
      }
      _Que.push(Tt);
      pthread_mutex_unlock(&_Mutex);
      pthread_cond_signal(&_Cond);
      return true;
    }
    bool Pop(ThreadTask** Tt)
    {

      *Tt = _Que.front();
      _Que.pop();
      return true;
    }

    bool PoolClear()
    {
      pthread_mutex_lock(&_Mutex);
      _isQuit = true;
      pthread_mutex_unlock(&_Mutex);
      if(_threadCurNum>0)
      {
        pthread_cond_broadcast(&_Cond);

      }
      return true;
    }
  private:
    static void* ThreadStart(void* arg)
    {
      thread_pool* tp = (thread_pool*)arg;
      while(1)
      {
        pthread_mutex_lock(&tp->_Mutex);
        while(tp->_Que.empty())
        {
          if(tp->_isQuit)
          {
            tp->ThreadQuit();
          }

          pthread_cond_wait(&tp->_Cond,&tp->_Mutex);
        }
        ThreadTask* tt;
        tp->Pop(&tt);
        pthread_mutex_unlock(&tp->_Mutex);
        
        tt->run();
      }
      return NULL;
    }


    bool ThreadCreat()
    {
      int ret = -1;
      pthread_t tid;
      for(int i=0;i<THREADCOUNT;i++)
      {
        ret = pthread_create(&tid,NULL,ThreadStart,(void*)this);
        if(ret!=0)
        {
          printf("create thread fail");
          return false;
        }
        
        //_threadCurNum++;
      }
      return true;
    }    
      void ThreadQuit()
      {
        _threadCurNum--;
        pthread_mutex_unlock(&_Mutex);
        pthread_exit(NULL);

      }

  private:
    size_t _threadCap;
    size_t _threadCurNum;
    queue<ThreadTask*> _Que;
    pthread_mutex_t _Mutex;
    pthread_cond_t _Cond;
    bool _isQuit;
};

void print(int data)
{
  printf("handler function print[%d]\n",data);
}

int main()
{
  thread_pool* tp = new thread_pool();
  for(int i=0;i<10;i++)
  {
    ThreadTask* tt = new ThreadTask(i,print);
    tp->Push(tt); 
  }
  sleep(2);
  tp->PoolClear();
  delete tp;
  return 0;
}
