#ifndef _book_H
#define _book_H

#include "board.h"
#include "stdio.h"



typedef struct book_item {
    char *text;                 // 菜单显示文本
    u8 id;                      // 菜单项ID
    u8 state;          //图书状态	
		struct book_item *next;     // 同级下一个菜单项
    struct book_item *prev;     // 同级上一个菜单项
} book_item;

/***********外部变量************/
extern char name[30][20];
extern book_item* book_head;
extern book_item* book_stock[20];
extern volatile u32 book_num;

/*************函数声明*************/
book_item* create_book_item(char *text, u8 id,u8 state,book_item *head) ;
book_item* book_system_init(void);
book_item* book_find(char* name);
void delete_book(book_item*a);
void show_book(void);
book_item* book_id_find(u8 ID);


#endif