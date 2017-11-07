

#include "delay.h"
void Delay(unsigned int a)//ÑÓÊ±º¯Êý
{
    while(a)
    {
        a--;
        //asm("NOP");
        __nop();
        //__asm{nop};
    }
}

