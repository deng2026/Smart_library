#ifndef _MENU_H
#define _MENU_H

#include "system.h"
#include "stdio.h"
#include "stdlib.h"
#include "board.h"

// ��������
#define KEY_UP     1
#define KEY_DOWN   3
#define KEY_ENTER  2
#define KEY_BACK   4
#define KEY_NONE   0
#define KEY_TOUCH	 6
typedef struct menu_item {
    char *text;                 // �˵���ʾ�ı�
    void (*action)(u8 id);      // �˵��������
    struct menu_item *parent;   // ���˵�
    struct menu_item *children; // �Ӳ˵�����
    struct menu_item *next;     // ͬ����һ���˵���
    struct menu_item *prev;     // ͬ����һ���˵���
    u8 id;                      // �˵���ID
    u8 children_count;          // �Ӳ˵�����
} menu_item;

// ��������
menu_item* create_menu_item(char *text, void (*action)(u8 id), menu_item *parent, u8 id);
void add_menu_child(menu_item *parent, menu_item *child);
void menu_system_init(void);
void handle_menu_navigation(u8 key);
void display_current_menu(void);
void handle_menu_touch(void);
// ȫ�ֵ�ǰ�˵�ָ��
extern menu_item *current_menu;

#endif

