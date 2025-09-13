#ifndef _book_H
#define _book_H

#include "board.h"
#include "stdio.h"



typedef struct book_item {
    char *text;                 // �˵���ʾ�ı�
    u8 id;                      // �˵���ID
    u8 state;          //ͼ��״̬	
		struct book_item *next;     // ͬ����һ���˵���
    struct book_item *prev;     // ͬ����һ���˵���
} book_item;

/***********�ⲿ����************/
extern char name[30][20];
extern book_item* book_head;
extern book_item* book_stock[20];
extern volatile u32 book_num;

/*************��������*************/
book_item* create_book_item(char *text, u8 id,u8 state,book_item *head) ;
book_item* book_system_init(void);
book_item* book_find(char* name);
void delete_book(book_item*a);
void show_book(void);
book_item* book_id_find(u8 ID);


#endif