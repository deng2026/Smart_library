#include "book.h"

book_item* book_head=NULL;
volatile u32 book_num=7;

char name[30][20]={
	"0",
	"abc",
	"West Travel",
	"sheep and wolf",
	"SMALL WOMEN",
	"English book",
	"SUPER MAN",
	"The old and the sea",
	
};

//book_item *one_book = NULL;
//book_item *two_book = NULL;
//book_item *three_book = NULL;
//book_item *four_book = NULL;
//book_item *five_book = NULL;
book_item* book_stock[20]={NULL};
//加书
book_item* create_book_item(char *text, u8 id,u8 state,book_item *head) 
{
		book_item *last = head->prev;  
	
		book_item *item = (book_item*)malloc(sizeof(book_item));
		if(item==NULL)return item;
	
    item->text = text;
    item->id = id;
    item->state=state;
		item->next = item->prev = item;//链表初始化
	
		
		last->next = item;
		item->prev = last;
		item->next =head;
		head->prev = item;//这里是向前插入,方便遍历
		
    return item;
}

//减书
void delete_book(book_item*a)
{
	if(a->id==0)return;
	a->prev->next=a->next;
	a->next->prev=a->prev;
	a->next=a;
	a->prev=a;
	free(a);
}
//初始化
book_item* book_system_init(void)
{
	u8 i=0;
	//建个头节点
	book_head = (book_item*)malloc(sizeof(book_item));
	book_head->text = "BOOK HEAD";
	book_head->id = 0;
	book_head->state=0;
	book_head->next = book_head->prev = book_head;//链表初始化
	
	for(i=1;i<=book_num;i++){
		book_stock[i-1]=create_book_item(name[i],i,1,book_head) ;
	}
	
	return book_head;
}

//找书（书名）
book_item* book_find(char* name)
{
	book_item* a=book_head;
	while(1)
	{
		a=a->next;
		if((!strcmp(a->text,name))||a->id==0)
		break;
	}
	return a;
}

//找书（书号）
book_item* book_id_find(u8 ID)
{
	book_item* a=book_head;
	while(1)
	{
		a=a->next;
		if(ID==a->id||a->id==0)
		break;
	}
	return a;
}

//展示
void show_book(void)
{
	u32 i;
	book_item* a=book_head;
	
	for(i=0;;i++){
	gui_show_strmid(0,50+i*30,240,30,BLACK,24,a->text);
	LCD_ShowNum(300,50+i*30,a->id,3,24);
	if(a->state)gui_show_strmid(350,50+i*30,100,30,BLACK,24,"Available");
	else gui_show_strmid(350,50+i*30,100,30,BLACK,24,"Borrowed");
	a=a->next;
	if(a->id==0)break;
	}
}