

#include "delay.h"
void Delay(unsigned int a)//��ʱ����
{
    while(a)
    {
        a--;
        //asm("NOP");
        __nop();
        //__asm{nop};
    }
}

