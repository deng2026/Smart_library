 #include "board.h"
#include "rtthread.h"

/* �����߳̿��ƿ� */
static rt_thread_t lcd_thread = RT_NULL;
static rt_thread_t find_thread = RT_NULL;
static rt_thread_t borrow_thread = RT_NULL;
static rt_thread_t giveback_thread = RT_NULL;
static rt_thread_t change_thread = RT_NULL;

/* �ź������ƿ� */
static rt_sem_t find_sem = RT_NULL;
static rt_sem_t borrow_sem = RT_NULL;
static rt_sem_t giveback_sem = RT_NULL;
static rt_sem_t change_sem = RT_NULL;
static rt_sem_t lcd_sem = RT_NULL;

/* ���������ƿ� */
static rt_mutex_t lcd_mutex = RT_NULL;
  
/* ȫ�ֱ��� */
extern menu_item *current_menu;
volatile u8 key_value = KEY_NONE;
volatile u8 adc_output;

/******************** �߳����庯�� ********************/
static void lcd_thread_entry(void* parameter);
static void find_thread_entry(void* parameter);
static void borrow_thread_entry(void* parameter);
static void giveback_thread_entry(void* parameter);
static void change_thread_entry(void* parameter);

/*******************************************************************************/
int main(void)
{   
    /* ��ʼ�������� */
    lcd_mutex = rt_mutex_create("lcd_mutex", RT_IPC_FLAG_PRIO);
    
    /* �����ź��� */
    find_sem = rt_sem_create("find_sem", 0, RT_IPC_FLAG_PRIO);
    borrow_sem = rt_sem_create("borrow_sem", 0, RT_IPC_FLAG_PRIO);
    giveback_sem = rt_sem_create("giveback_sem", 0, RT_IPC_FLAG_PRIO);
    change_sem = rt_sem_create("change_sem", 0, RT_IPC_FLAG_PRIO);
    lcd_sem = rt_sem_create("lcd_sem", 0, RT_IPC_FLAG_PRIO);

    /* ����LCD�߳� */
    lcd_thread = rt_thread_create("lcd", lcd_thread_entry, RT_NULL, 
                                512, 1, 20);
    rt_thread_startup(lcd_thread);

    /* ���������߳� */
    find_thread = rt_thread_create("find", find_thread_entry, RT_NULL,
                                512, 3, 20);
    rt_thread_startup(find_thread);

    borrow_thread = rt_thread_create("borrow", borrow_thread_entry, RT_NULL,
                                512, 3, 20);
    rt_thread_startup(borrow_thread);

    giveback_thread = rt_thread_create("giveback", giveback_thread_entry, RT_NULL,
                                512, 3, 20);
    rt_thread_startup(giveback_thread);

    change_thread = rt_thread_create("change", change_thread_entry, RT_NULL,
                                512, 3, 20);
    rt_thread_startup(change_thread);

    return 0;
}

/******************** LCD�������߳� ********************/
static void lcd_thread_entry(void* parameter)
{
    u8 lcd_index = 0xff;
		book_system_init();
    while(1) {
        rt_mutex_take(lcd_mutex, RT_WAITING_FOREVER);
        ICON_UI_Init();  // ��ʼ��������
        rt_mutex_release(lcd_mutex);

        while(1) {
            lcd_index = get_icon_app_table();
            if(lcd_index != 0xff) break;
            rt_thread_delay(20);
        }

        /* ������Ӧ�����߳� */
        switch(lcd_index) {
            case 0: rt_sem_release(find_sem); break;
            case 1: rt_sem_release(borrow_sem); break;
            case 2: rt_sem_release(giveback_sem); break;
            case 3: rt_sem_release(change_sem); break;
        }

        /* �ȴ�������� */
        rt_sem_take(lcd_sem, RT_WAITING_FOREVER);
    }
}

/******************** ��ѯ�����߳� ********************/
static void find_thread_entry(void* parameter)
{
    static char name[30];
		static book_item* find_result=NULL;
		static u8 len=0,t;
		static char string_buf[30];
		static char rec_buf[30];
		while(1) {
        rt_sem_take(find_sem, RT_WAITING_FOREVER);       
        rt_mutex_take(lcd_mutex, RT_WAITING_FOREVER);
        LCD_Clear(WHITE);
        gui_show_strmid(0,30,480,40,BLUE,24,"PLEASE INPUT THE BOOK NAME");       
        gui_show_strmid(0,700,480,100,RED,24,"RETURN TO THE COVER");
				Show_Default_Keyboard(0);
        rt_mutex_release(lcd_mutex);

        while(1) {
     /**********************************/       
			if(Process_Keyboard( rec_buf)==0xff){
				find_result=book_find(rec_buf);	
				LCD_Fill(0,100,480,400,WHITE);
						if(find_result->id)
						{
							gui_show_strmid(0,200,480,30,BLACK,24,find_result->text);
							sprintf(string_buf,"THE ID of BOOK is %d",find_result->id);
							gui_show_strmid(0,230,480,30,BLACK,24,string_buf);
							if(find_result->state)
							gui_show_strmid(0,260,480,30,BLACK,24,"YOU CAN BORROW IT");
							else gui_show_strmid(0,260,480,30,BLACK,24,"IT HAVE BEEN BORROWED");
						}
						else gui_show_strmid(0,260,480,30,BLACK,24,"DO NOT HAVE FIND IT");
				rec_buf[0]=0;
				}
				LCD_Fill(0,400,480,449,WHITE);
				gui_show_strmid(0,400,480,50,RED,24,rec_buf);
		/***************************************/			
            if(get_return_table()) {
                rt_sem_release(lcd_sem);
                break;
            }
            rt_thread_delay(20);
        }
    }
}

/******************** ���Ĺ����߳� ********************/
static void borrow_thread_entry(void* parameter)
{
    while(1) {
        rt_sem_take(borrow_sem, RT_WAITING_FOREVER);
        
        rt_mutex_take(lcd_mutex, RT_WAITING_FOREVER);
        LCD_Clear(WHITE);
        gui_show_strmid(0,300,480,40,GREEN,24,"PLEASE PUT CARD ON THE CONSOLE");      
        gui_show_strmid(0,700,480,100,RED,24,"RETURN TO THE COVER");
        rt_mutex_release(lcd_mutex);

        while(1) {
          /**************�����߼�****************/  
						Bo_Re(1);
 					
				/*************************************/	
						if(get_return_table()) {
                rt_sem_release(lcd_sem);
                break;
            }
            rt_thread_delay(20);
        }
    }
}

/******************** �黹�����߳� ********************/
static void giveback_thread_entry(void* parameter)
{
    while(1) {
        rt_sem_take(giveback_sem, RT_WAITING_FOREVER);
        
        rt_mutex_take(lcd_mutex, RT_WAITING_FOREVER);
        LCD_Clear(WHITE);
        gui_show_strmid(0,300,480,40,GREEN,24,"PLEASE PUT CARD ON THE CONSOLE");       
        gui_show_strmid(0,700,480,100,RED,24,"RETURN TO THE COVER");
        rt_mutex_release(lcd_mutex);

        while(1) {
			/******************** �黹�߼�*************************/
            
					Bo_Re(0);
					
					
					
			/******************************************************/
						if(get_return_table()) {
                rt_sem_release(lcd_sem);
                break;
            }
            rt_thread_delay(20);
        }
    }
}

/******************** �������߳� ********************/
static void change_thread_entry(void* parameter)
{
    u8 change_index = 0xff;
		static u8 change_id,len,t,i,flag_c;
		static char rec_name[30];
		book_item* change_book=	book_head;
		static char string_buf[30];
		change_id=(u8)book_num;
    while(1) {
        rt_sem_take(change_sem, RT_WAITING_FOREVER);        
        rt_mutex_take(lcd_mutex, RT_WAITING_FOREVER);
        Change_ICON_Init();
        rt_mutex_release(lcd_mutex);
				change_index=0xff;flag_c=1;
        while(1) {
			/****************�������߼�***************************/
            if(change_index==0xff)change_index = get_change_table();
            if(change_index == 0) {
								//���ӿ���߼�//
								if(flag_c){
									LCD_Fill(0,50,480,700,WHITE);
									gui_show_strmid(0,10,480,40,BLUE,24,"PLEASE INPUT THE NAME of NEW BOOK");
									show_book();
									flag_c=0;
								}
								if(USART1_RX_STA & 0x8000) {            
									len=USART1_RX_STA&0x3fff;						            
									for(t=0;t<len;t++)
									rec_name[t]=USART1_RX_BUF[t];
									USART1_RX_STA = 0;  // �����־
									rec_name[len]='\0';
									change_id++;									
									strcpy(name[change_id],rec_name);
									change_book=create_book_item(name[change_id],change_id,1,book_head) ;
									if (change_book != NULL) {
                      gui_show_strmid(0, 50, 480, 40, GREEN, 24, "ADD BOOK SUCCESS!");
											book_num++;
											LCD_Fill(0,50,480,700,WHITE);
											show_book();
                    }
									else  gui_show_strmid(0, 50, 480, 40, RED, 24, "ADD BOOK FAILED!");
                    								
									
								}
            } 
            else if(change_index == 1) {               
								// ���ٿ���߼�//		
								if(flag_c){
									LCD_Fill(0,50,480,700,WHITE);
									gui_show_strmid(0,10,480,40,BLUE,24,"PLEASE INPUT THE NAME of Old BOOK");
									show_book();
									flag_c=0;
								}
													
								if(USART1_RX_STA & 0x8000) {            
									len=USART1_RX_STA&0x3fff;						            
									for(t=0;t<len;t++)
									rec_name[t]=USART1_RX_BUF[t];
									USART1_RX_STA = 0;  // �����־
									rec_name[len]='\0';
									
									change_book=book_find(rec_name);
									if(change_book->id!=0){
									delete_book(change_book) ;
									gui_show_strmid(0,50,480,40,GREEN,24,"DELETE BOOK SUCCEE!");
									book_num--;
									LCD_Fill(0,50,480,700,WHITE);
									show_book();
									}
									else gui_show_strmid(0,50,480,40,RED,24,"DELETE BOOK FAIL");
									
								}
							
            }
			/*********************************************************/	
            if(get_return_table()) {
                rt_sem_release(lcd_sem);
                break;
            }
            rt_thread_delay(20);
        }
    }
}

