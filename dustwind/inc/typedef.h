
#ifndef _DW_TYPEDEF_H_
#define _DW_TYPEDEF_H_

#include "dw_define.h"

#define unchar unsigned char
#define unint unsigned int
typedef struct
{
    unchar ID;
    unchar x;
    unchar parentID;
    unchar childnum;
    unchar childstart;
    unchar cur;
    unchar mdatax, mdatay;
    const unchar  *mdata;
} structmenu;

#endif
