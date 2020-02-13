#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>                                                                                                                                                                  
using namespace std;
 
class lockqueue
 {
     public:
         lockqueue(int size)
             :_size(size)
         {
             pthread_cond_init(&producter,NULL);
             pthread_cond_init(&consumer,NULL);
             pthread_mutex_init(&mutex,NULL);
         }
         ~lockqueue()
         {
             pthread_cond_destroy(&producter);
             pthread_cond_destroy(&consumer);
             pthread_mutex_destroy(&mutex);
         }
 
         void push(int i)
         {
             pthread_mutex_lock(&mutex);
             while(que.size()==_size)
             {
                 pthread_cond_wait(&producter,&mutex);
             }
             que.push(i);
             pthread_mutex_unlock(&mutex);
             pthread_cond_signal(&consumer);
         }
         int pop()
         {
             pthread_mutex_lock(&mutex);
             while(que.empty())
          {
                 pthread_cond_wait(&consumer,&mutex);
             }
             int out = que.front();
             que.pop();
             pthread_mutex_unlock(&mutex);
             pthread_cond_signal(&producter);
             return out;
         }
 
     private:
         queue<int> que;
         int _size;
         pthread_mutex_t mutex;
         pthread_cond_t producter;
         pthread_cond_t consumer;
 
 };
 
 void* ConsumStart(void* arg)
 {
     lockqueue* que = (lockqueue*)arg;
     while(1)
     {
         int data;
         data = que->pop();
         cout<<"consumer consum ["<<data<<"]"<<endl;
     }
     return NULL;
 }
 void* ProductStart(void* arg)
 {
     lockqueue* que = (lockqueue*)arg;
     int i=0;                                                                                                                                                                         
     while(1)
     {   
         que->push(i);
         cout<<"producter product["<<i<<"]"<<endl;
         i++;
    }
     return NULL;
 }

 int main()
 {
 
     pthread_t consum[4],product[4];
     lockqueue* que = new lockqueue(10);
     int i =0;
     int ret;
     for(;i<4;i++)
     {
         ret = pthread_create(&consum[i],NULL,ConsumStart,(void*)que);
         if(ret!=0)
         {
             cout<<"failed";
             return 0;
         }
         ret = pthread_create(&product[i],NULL,ProductStart,(void*)que);
         if(ret!=0)
         {
               cout<<"failed";
               return 0;
           }
       }
       for(i=0;i<4;i++)
       {
           pthread_join(consum[i],NULL);
           pthread_join(product[i],NULL);
       }
       delete que;
       return 0;
   }                                          