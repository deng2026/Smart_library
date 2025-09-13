/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 */
#include <rthw.h>
#include <rtthread.h>
#include "board.h"


#ifdef __CC_ARM
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 4096
//���ڲ�SRAM�������һ���־�̬�ڴ�����Ϊrtt�Ķѿռ䣬��������Ϊ4KB
static uint32_t rt_heap[RT_HEAP_SIZE];
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif
#endif

extern uint8_t OSRunning;



/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{
	SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	USART1_Init(115200);
	
	LED_Init();
	KEY_Init();   
  My_EXTI_Init();
  //BEEP_Init();
  TFTLCD_Init();  
	TP_Init();
	//Lsens_Init();
	AT24CXX_Init();	 
	TIM4_Init(20,36000-1);  //��ʱ10ms
	//TIM3_CH2_PWM_Init(500,72-1); //Ƶ����2Kh
	RC522_Init(); 
	OSRunning=1;
	
    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
    
#if defined(RT_USING_CONSOLE) && defined(RT_USING_DEVICE)
	rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif
    
#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif
}

void SysTick_Handler(void)
{
	/* enter interrupt */
	rt_interrupt_enter();
	
	/* ����ʱ�� */
	rt_tick_increase();

	/* leave interrupt */
	rt_interrupt_leave();
}

//��ӳ�䴮��1��rt_kprintf
void rt_hw_console_output(const char *str)
{
	/* �����ٽ�� */
	rt_enter_critical();
	/* ֱ���ַ������� */
	while(*str!='\0')
	{
		if(*str=='\n')
		{
			USART1->DR = (u8)  '\r'; 
			while((USART1->SR&0X40)==0);
		}
		USART1->DR =*str++;
		while((USART1->SR&0X40)==0);	
	}
	/* �˳��ٽ�� */
	rt_exit_critical();
}
