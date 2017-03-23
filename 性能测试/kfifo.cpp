#include "kfifo.h"


Kfifo::~Kfifo()  
{  
    if (buffer) free(buffer);     
    size = in = out=0;  
}  
unsigned long Kfifo::roundup_power_of_two(unsigned long val)  
{  
    if (val & (val-1) == 0)  
    {  
        return val;  
    }  
    unsigned long maxulong = (unsigned long )((unsigned long ) ~0);  
    unsigned long andv = ~(maxulong&(maxulong>>1));  
    while((andv & val) == 0)  
    andv = andv>>1;  
    return andv<<1;  
}  
Kfifo::Kfifo(unsigned int isize):size(isize),in(0),out(0),mask(size - 1)  
{  
    if (!is_power_of_2(isize))  
    {  
        size = roundup_power_of_two(isize);  
    }  
    buffer =(unsigned char*) malloc(isize);  
}  
  
unsigned int Kfifo::get(unsigned char *_buffer, unsigned int len)  
{  
    unsigned int l;  
    len = min(len, in - out);  
    __sync_synchronize();  
  
    l = min(len,size -(out&(size-1)));  
    memcpy(_buffer,buffer + (out& (size-1)),l);  
    memcpy(_buffer + l,buffer,len - l);  
  
     __sync_synchronize();  
  
     out +=len;  
     return len;  
  
}  
unsigned int Kfifo::put(const unsigned char *_buffer, unsigned int len)  
{  
    unsigned int l;  
    len = min(len, size - in + out);  
     __sync_synchronize();  
     l = min(len, size - (in & (size - 1)));  
     memcpy(buffer + (in & (size - 1)), _buffer, l);  
     memcpy(buffer, _buffer + l, len - l);  
      __sync_synchronize();  
       in += len;  
       return len;  
  
}  
  
void * consumer(void * arg)  
{  
    printf("consumer\n");  
    Kfifo* fifo = (Kfifo*) arg;  
    if (!fifo)  
    {  
        return NULL;  
    }  
  
    for (;;)  
    {  
        proto p;  
        unsigned int len = fifo->get((unsigned char*)&p,sizeof(p));  
        if (len>0)  
        {  
            cout << "~~~~~~~~~~~~~~~~~~~~"<<endl;  
            cout << "consumer proto msg id :"<<p.msgid<<endl;  
            cout << "consumer proto msg cmd :"<<p.cmd<<endl;  
            cout << "consumer proto msg info :"<<p.info<<endl;  
            cout << "~~~~~~~~~~~~~~~~~~~~"<<endl;  
        }  
    }  
  
      
    return (void *)fifo;  
}  
void* producer(void* args)  
{  
    Kfifo* fifo = (Kfifo*) args;  
      
    if (!fifo)  
    {  
        return NULL;  
    }  
    unsigned int i=0;  
    for (;;)  
    {  
        proto p;  
        p.msgid = i++;  
        p.cmd =  333;  
        p.info = 44444;  
        fifo->put((const unsigned char*)&p,sizeof(p));  
        cout<<"producer put msgid :"<<p.msgid<<endl;  
    }  
    return (void*)fifo;  
}  
int main()  
{  
    Kfifo *fifo = new Kfifo(1024);  
    pthread_t consumeid,producerid;  
    pthread_create(&producerid,NULL,producer,(void*)fifo);  
    pthread_create(&consumeid,NULL,consumer,(void*)fifo);  
  
    printf("info!!\n");  
    pthread_join(consumeid,NULL);  
    pthread_join(producerid,NULL);  
    return 0;  
}  