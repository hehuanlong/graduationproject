#ifndef _BYJ_48_H
#define _BYJ_48_H
#include "sys.h"
#define OUT(a,b,c,d) PAout(8)=a,PBout(6)=b,PDout(6)=c,PDout(7)=d
#define A OUT(1,0,0,0)
#define B OUT(0,1,0,0)
#define C OUT(0,0,1,0)
#define D OUT(0,0,0,1)
#define DOUBLEA OUT(1,1,0,0)
#define DOUBLEB OUT(0,1,1,0)
#define DOUBLEC OUT(0,0,1,1)
#define DOUBLED OUT(1,0,0,1)
void byj_48_init(void);
void  Forward_byj_48(void );
#endif 
