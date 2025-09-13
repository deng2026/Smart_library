#ifndef _board_H
#define _board_H

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "key.h"
#include "exti.h"
#include "beep.h"
#include "tftlcd.h"
#include "menu.h"
#include "touch.h"
#include <string.h>
#include "adc_temp.h"
#include "lsens.h"
#include "ds18b20.h"
#include "dma.h"
#include "iic.h"
#include "24cxx.h"
#include "time.h"
#include "pwm.h"
#include  "diy_gui.h"
#include "book.h"
#include "rc522.h"

void rt_hw_board_init(void);
void SysTick_Handler(void);

#endif
