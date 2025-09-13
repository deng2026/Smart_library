#ifndef _time_H
#define _time_H

#include "system.h"

extern  u8 key_flag ;
extern u16 ten_ms,one_s,one_min; 
void Clock_clear();
void TIM4_Init(u16 per,u16 psc);
void TIM5_Init(u16 per,u16 psc);

#endif
