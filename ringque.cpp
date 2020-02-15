#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
using namespace std;

class Ringqueue
{
    public:
        Ringqueue(int size=4)
            :_vec(size)
        {
            _cap = size;
            _writepos = 0;
            _readpos = 0;

            sem_init(&_produ,0,size);
            sem_init(&_consum,0,0);
            sem_init(&_locksem,0,1);
        }
        ~Ringqueue()
        {
            sem_destroy(&_produ);
            sem_destroy(&_consum);
            sem_destroy(&_locksem);
        }
   
        void push(int data)
        {
            sem_wait(&_produ);
            sem_wait(&_locksem);
            _vec[_writepos] = data;
            _writepos = (_writepos +1)%_cap;
            sem_post(&_locksem);
            sem_post(&_consum);
        }
        int pop()
        {
            sem_wait(&_consum);
            sem_wait(&_locksem);
            int data = _vec[_readpos];
            _readpos = (_readpos+1)%_cap;
            sem_post(&_locksem);
            sem_post(&_produ);
            return data;
        }

    private:
        vector<int> _vec;
        size_t _cap;
        int _writepos;
        int _readpos;

        sem_t _produ;
        sem_t _consum;
        sem_t _locksem;
};

void* Product(void* arg)
{
    Ringqueue* que =(Ringqueue*)arg ;
    int i =0;
    while(1)
    {
        que->push(i);
        i++;
        cout<<"producter product ["<<i<<"]"<<endl;
    }
}
void* Consume(void* arg)
{
    Ringqueue* que =(Ringqueue*)arg;
    while(1)
    {
        int i = que->pop();
        cout<<"consumer consum["<<i<<"]"<<endl;
    }
}


int main()
{
    pthread_t produ[4],consum[4];
    Ringqueue* que = new Ringqueue(5);
    int i =0;
    int ret = -1;
    for(;i<4;i++)
    {
        ret = pthread_create(&produ[i],NULL,Product,(void*)que);
        pthread_create(&consum[i],NULL,Consume,(void*)que);
    }
    for(i=0;i<4;i++)
    {
        pthread_join(produ[i],NULL);
        pthread_join(consum[i],NULL);
    }
    delete que;
    return 0;
}
