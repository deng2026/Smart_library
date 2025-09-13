#include "menu.h"
#include "picture.h"

extern u8 key_value;
static u8 id=99;
/**�˵���㣬��չ�����������Ӷ���**/
menu_item *current_menu = NULL;

menu_item *main_menu = NULL;//0���˵����
menu_item *find_menu = NULL;//1��
menu_item *borrow_menu = NULL;
menu_item *return_menu = NULL;
menu_item *change_menu = NULL;
menu_item *search_name = NULL;//2���˵�
menu_item *anima_science = NULL;
menu_item *electronics_science = NULL;
menu_item *literary= NULL;
menu_item *add_book = NULL;
menu_item *sub_book = NULL;

// �����˵���
menu_item* create_menu_item(char *text, void (*action)(u8 id), menu_item *parent, u8 id) 
{
    menu_item *item = (menu_item*)malloc(sizeof(menu_item));
    item->text = text;
    item->action = action;
    item->parent = parent;
    item->children = NULL;
    item->next = item->prev = item;//�����ʼ��
    item->id = id;
    item->children_count = 0;
    return item;
}

// ����Ӳ˵�
void add_menu_child(menu_item *parent, menu_item *child) 
{
    if(parent->children == NULL) {
        parent->children = child;
    } 
	else {
        menu_item *last = parent->children->prev;
        last->next = child;
        child->prev = last;
        child->next = parent->children;
        parent->children->prev = child;//��������ǰ����,�������
				
    }
    parent->children_count++;
}

// �˵���������action()���ײ�˵��ڵ��ִ�й���
static void show_picture(u8 id) {
	LCD_Fill(0,400,480-1,800-1,WHITE);//����°���Ļ
//	LCD_ShowPicture(10,420,300,167,(u8 *)gImage_picture);
}
static void clock_task(u8 id) {
	//�����߳�
}

// ��ʼ���˵�ϵͳ
void menu_system_init(void) {
    // �������˵�
    main_menu = create_menu_item("Smart Library System", NULL, NULL, 0);
    
    // ����Ӳ˵�
   find_menu = create_menu_item("Find Book", NULL, main_menu, 1);
   borrow_menu = create_menu_item("Borrow Book", NULL, main_menu, 2);
   return_menu = create_menu_item("Return Book", NULL, main_menu, 3);
   change_menu = create_menu_item("Add or Delete Book", NULL, main_menu, 4);
	
    add_menu_child(main_menu, find_menu);
    add_menu_child(main_menu, borrow_menu);
    add_menu_child(main_menu, return_menu);
    add_menu_child(main_menu, change_menu);
    
		search_name = create_menu_item("Search The Name of Book", NULL, find_menu, 1);//2���˵�
		anima_science =create_menu_item("Anima Science", NULL, find_menu, 2);
		electronics_science = create_menu_item("Electronics Science", NULL, find_menu, 3);
		literary= create_menu_item("Literary", NULL, find_menu, 4);
		add_book = create_menu_item("Add Book", NULL, change_menu, 1);
		sub_book = create_menu_item("Delete Book", NULL, change_menu, 2);
	
		add_menu_child(find_menu, search_name);
		add_menu_child(find_menu, anima_science);
		add_menu_child(find_menu, electronics_science);
		add_menu_child(find_menu, literary);
		add_menu_child(change_menu, add_book);
		add_menu_child(change_menu, sub_book);
		
		
    current_menu = main_menu;//ָ�������˵���
}

// ��������˵�����
void handle_menu_navigation(u8 key) {
    menu_item *sibling=NULL;
		u8 i=0;
		if(key == KEY_NONE) return;
    
    switch(key) {
        case KEY_UP: 
            current_menu = current_menu->prev;
            break;
            
        case KEY_DOWN:
            current_menu = current_menu->next;
            break;
            
        case KEY_ENTER:
            if(current_menu->action) {
                current_menu->action(current_menu->id); // ִ�ж���
            }
						if(current_menu->children_count > 0) {
                current_menu = current_menu->children; // �����Ӳ˵�
            } 						
            break;
            
        case KEY_BACK:
            if(current_menu->parent) {
                current_menu = current_menu->parent; // �����ϼ��˵�
            }
            break;
		
				case KEY_TOUCH:	
						if(id){
										if (current_menu->id == id) {
														if (current_menu->action) 
																current_menu->action(current_menu->id);
														if (current_menu->children_count > 0) 
																current_menu = current_menu->children;
													
													} 
										else {
														sibling = current_menu->parent->children;
														for (i = 1; i < id; i++) 
																sibling = sibling->next;													
													
														current_menu = sibling;													
											  }
									}
						else current_menu = current_menu->parent;		
						break;
    }
}

//������������
void handle_menu_touch() {
  
		static u16 x=1000,y=1000;
	
		//if(!TP_Scan(0)) return ;//û�����ͷ���
	
		if(TP_Scan(0)){
		y=tp_dev.y[0];
		return;
		}//�д�����ʱ��ִ��
			
		if(!(TP_Scan(0)==0&&y!=1000))return;//û����������¼������ֵ��Чʱ��Ҳ��ִ��
	
	
		if(current_menu->id==0){//�������		
		current_menu = current_menu->children; // �����Ӳ˵�
		return;
		}
		else if(y<50)  id=0;
		else if(y>=50&&y<90) id=1;
		else if(y>=90&&y<130) id=2;
		else if(y>=130&&y<170) id=3;
		else if(y>=170&&y<210) id=4;
		else id=99;
		y=1000;
		key_value=KEY_TOUCH;//
		rt_kprintf("key_value=%d\n",key_value);//���Ա��

		return;
}

// ��ʾ��ǰ�˵�
void display_current_menu(void) {
	u16 y_pos = 50; 
	static menu_item *item=NULL;
	
	//LCD_Clear(WHITE);  
	LCD_Fill(0,0,480-1,400-1,WHITE);//����ϰ���Ļ��������ʾ�˵�
  
	// ��ʾ��ǰ�˵�����(��һ���˵����)  
	FRONT_COLOR = DARKBLUE;
  if(current_menu->parent)LCD_ShowString(160, 10, tftlcd_data.width, tftlcd_data.height, 24, current_menu->parent->text);    

	// ��ʾ�˵���   
	if(current_menu->parent){
	item = current_menu->parent->children;
	if(item) {
				do {
					// ������ʾ��ǰѡ����
					u16 color = (item == current_menu) ? RED : BLACK;
					FRONT_COLOR = color;
					
					LCD_ShowString(160, y_pos, tftlcd_data.width, tftlcd_data.height, 24, item->text);
					y_pos += 40;
					item = item->next;
				} while(item != current_menu->parent->children);
			}
	}
	
	//�����ǿ��ǵ���0���˵�MAIN��ֻ����һ����㣬��������ʵ��ã�ûʲô����,������Ǻ����ļ�֮࣬��Ҳ����ɾ��
	else{
	item=current_menu;
	FRONT_COLOR = GREEN;
	LCD_ShowString(150, 180, tftlcd_data.width, tftlcd_data.height, 24, item->text);
	LCD_ShowPicture(100,420,256,256,(u8 *)gImage_cover);
	}
   

	//���������Щ������Ҫ������ʾ�����ã�����������������action()��Ȼ����main��ĳɶ�ʱ��ˢ����ʾ������ʱ����ʾʲô��
	//if(!(current_menu->action)) LCD_Fill(0,400,480-1,800-1,WHITE);//����°���Ļ
	//else current_menu->action(current_menu->id);

	// ����������ɫ
    FRONT_COLOR = BLACK;
}