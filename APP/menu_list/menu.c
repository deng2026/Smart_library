#include "menu.h"
#include "picture.h"

extern u8 key_value;
static u8 id=99;
/**菜单结点，拓展结点在这里添加定义**/
menu_item *current_menu = NULL;

menu_item *main_menu = NULL;//0级菜单句柄
menu_item *find_menu = NULL;//1级
menu_item *borrow_menu = NULL;
menu_item *return_menu = NULL;
menu_item *change_menu = NULL;
menu_item *search_name = NULL;//2级菜单
menu_item *anima_science = NULL;
menu_item *electronics_science = NULL;
menu_item *literary= NULL;
menu_item *add_book = NULL;
menu_item *sub_book = NULL;

// 创建菜单项
menu_item* create_menu_item(char *text, void (*action)(u8 id), menu_item *parent, u8 id) 
{
    menu_item *item = (menu_item*)malloc(sizeof(menu_item));
    item->text = text;
    item->action = action;
    item->parent = parent;
    item->children = NULL;
    item->next = item->prev = item;//链表初始化
    item->id = id;
    item->children_count = 0;
    return item;
}

// 添加子菜单
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
        parent->children->prev = child;//这里是向前插入,方便遍历
				
    }
    parent->children_count++;
}

// 菜单动作函数action()，底层菜单节点的执行功能
static void show_picture(u8 id) {
	LCD_Fill(0,400,480-1,800-1,WHITE);//清空下半屏幕
//	LCD_ShowPicture(10,420,300,167,(u8 *)gImage_picture);
}
static void clock_task(u8 id) {
	//启动线程
}

// 初始化菜单系统
void menu_system_init(void) {
    // 创建主菜单
    main_menu = create_menu_item("Smart Library System", NULL, NULL, 0);
    
    // 添加子菜单
   find_menu = create_menu_item("Find Book", NULL, main_menu, 1);
   borrow_menu = create_menu_item("Borrow Book", NULL, main_menu, 2);
   return_menu = create_menu_item("Return Book", NULL, main_menu, 3);
   change_menu = create_menu_item("Add or Delete Book", NULL, main_menu, 4);
	
    add_menu_child(main_menu, find_menu);
    add_menu_child(main_menu, borrow_menu);
    add_menu_child(main_menu, return_menu);
    add_menu_child(main_menu, change_menu);
    
		search_name = create_menu_item("Search The Name of Book", NULL, find_menu, 1);//2级菜单
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
		
		
    current_menu = main_menu;//指定开机菜单项
}

// 按键处理菜单导航
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
                current_menu->action(current_menu->id); // 执行动作
            }
						if(current_menu->children_count > 0) {
                current_menu = current_menu->children; // 进入子菜单
            } 						
            break;
            
        case KEY_BACK:
            if(current_menu->parent) {
                current_menu = current_menu->parent; // 返回上级菜单
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

//触摸屏处理函数
void handle_menu_touch() {
  
		static u16 x=1000,y=1000;
	
		//if(!TP_Scan(0)) return ;//没触屏就返回
	
		if(TP_Scan(0)){
		y=tp_dev.y[0];
		return;
		}//有触摸的时候不执行
			
		if(!(TP_Scan(0)==0&&y!=1000))return;//没触摸，但记录的坐标值无效时，也不执行
	
	
		if(current_menu->id==0){//封面标题		
		current_menu = current_menu->children; // 进入子菜单
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
		rt_kprintf("key_value=%d\n",key_value);//调试标记

		return;
}

// 显示当前菜单
void display_current_menu(void) {
	u16 y_pos = 50; 
	static menu_item *item=NULL;
	
	//LCD_Clear(WHITE);  
	LCD_Fill(0,0,480-1,400-1,WHITE);//清空上半屏幕，用来显示菜单
  
	// 显示当前菜单标题(上一级菜单结点)  
	FRONT_COLOR = DARKBLUE;
  if(current_menu->parent)LCD_ShowString(160, 10, tftlcd_data.width, tftlcd_data.height, 24, current_menu->parent->text);    

	// 显示菜单项   
	if(current_menu->parent){
	item = current_menu->parent->children;
	if(item) {
				do {
					// 高亮显示当前选中项
					u16 color = (item == current_menu) ? RED : BLACK;
					FRONT_COLOR = color;
					
					LCD_ShowString(160, y_pos, tftlcd_data.width, tftlcd_data.height, 24, item->text);
					y_pos += 40;
					item = item->next;
				} while(item != current_menu->parent->children);
			}
	}
	
	//这里是考虑到第0级菜单MAIN我只做了一个结点，当个标题彩蛋用，没什么功能,如果考虑函数的简洁，之后也可以删掉
	else{
	item=current_menu;
	FRONT_COLOR = GREEN;
	LCD_ShowString(150, 180, tftlcd_data.width, tftlcd_data.height, 24, item->text);
	LCD_ShowPicture(100,420,256,256,(u8 *)gImage_cover);
	}
   

	//如果考虑有些功能需要持续显示或作用，可以在这个可以添加action()，然后在main里改成定时器刷新显示，比如时间显示什么的
	//if(!(current_menu->action)) LCD_Fill(0,400,480-1,800-1,WHITE);//清空下半屏幕
	//else current_menu->action(current_menu->id);

	// 重置字体颜色
    FRONT_COLOR = BLACK;
}