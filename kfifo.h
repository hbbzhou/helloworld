#ifndef _NO_LOCK_QUEUE_H_  
#define _NO_LOCK_QUEUE_H_  
  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <string>  

#include <iostream>  
using namespace  std;  

class Kfifo   
{  
public:  
    Kfifo(unsigned int  isize);  
    ~Kfifo();  
    unsigned int get(unsigned char *buffer, unsigned int len);  
    unsigned int put(const unsigned char *buffer, unsigned int len);  
    static unsigned long roundup_power_of_two(unsigned long val);  
private:  
    inline bool is_power_of_2(unsigned long n)  
    {  
        return (n != 0 && ((n & (n - 1)) == 0));   
    };  
    inline unsigned int unused()  
    {  
        return (mask + 1) - (in - out);  
    }  
      
private:  
    unsigned int size;  
    unsigned int in;   
    unsigned int out;  
    unsigned int    mask;  
    unsigned char* buffer;  
};  
  
struct proto  
{  
    unsigned int msgid;  
    unsigned int cmd;  
    unsigned int info;  
    proto():msgid(0),cmd(0),info(0){}  
};  
  
#endif  