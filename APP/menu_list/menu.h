#ifndef _MENU_H
#define _MENU_H

#include "system.h"
#include "stdio.h"
#include "stdlib.h"
#include "board.h"

// 按键定义
#define KEY_UP     1
#define KEY_DOWN   3
#define KEY_ENTER  2
#define KEY_BACK   4
#define KEY_NONE   0
#define KEY_TOUCH	 6
typedef struct menu_item {
    char *text;                 // 菜单显示文本
    void (*action)(u8 id);      // 菜单项动作函数
    struct menu_item *parent;   // 父菜单
    struct menu_item *children; // 子菜单链表
    struct menu_item *next;     // 同级下一个菜单项
    struct menu_item *prev;     // 同级上一个菜单项
    u8 id;                      // 菜单项ID
    u8 children_count;          // 子菜单数量
} menu_item;

// 函数声明
menu_item* create_menu_item(char *text, void (*action)(u8 id), menu_item *parent, u8 id);
void add_menu_child(menu_item *parent, menu_item *child);
void menu_system_init(void);
void handle_menu_navigation(u8 key);
void display_current_menu(void);
void handle_menu_touch(void);
// 全局当前菜单指针
extern menu_item *current_menu;

#endif

