#ifndef _KEYBOARD_H
#define _KEYBOARD_H 

#include "board.h"
#include "stdio.h"


// 键盘按键定义
typedef struct {
    uint16_t x;      // 左上角x坐标 (0-480)
    uint16_t y;      // 左上角y坐标 (320-800)
    uint16_t width;  // 按键宽度
    uint16_t height; // 按键高度
    char normal;     // 正常状态显示的字符
    char shift;      // Shift状态显示的字符
    uint8_t is_function; // 是否为功能键(1-功能键，0-字符键)
} Key_TypeDef;

// 定义功能键标识
#define KEY_SHIFT    0x01
#define KEY_BACKSPACE 0x02
#define KEY_ENTER    0x03
#define KEY_SPACE    0x04
#define KEY_CAPS     0x05
#define KEY_SYMBOL   0x06  // 符号/数字切换键

// 键盘布局参数
#define KEY_WIDTH     44   // 普通键宽度(减小宽度以适应间距)
#define KEY_HEIGHT    50   // 键高度
#define KEY_SPACING   1    // 键间距
#define START_Y       450  // 键盘起始Y坐标

// 计算按键X坐标的宏(列索引从0开始)
#define KEY_XPOS(col) (0 + (col)*(KEY_WIDTH+KEY_SPACING))

// 第一行(数字/符号) - 11个键
const Key_TypeDef keyboard_row1[] = {
    {KEY_XPOS(0),  START_Y, KEY_WIDTH, KEY_HEIGHT, '1', '!', 0},
    {KEY_XPOS(1),  START_Y, KEY_WIDTH, KEY_HEIGHT, '2', '@', 0},
    {KEY_XPOS(2),  START_Y, KEY_WIDTH, KEY_HEIGHT, '3', '#', 0},
    {KEY_XPOS(3),  START_Y, KEY_WIDTH, KEY_HEIGHT, '4', '$', 0},
    {KEY_XPOS(4),  START_Y, KEY_WIDTH, KEY_HEIGHT, '5', '%', 0},
    {KEY_XPOS(5),  START_Y, KEY_WIDTH, KEY_HEIGHT, '6', '^', 0},
    {KEY_XPOS(6),  START_Y, KEY_WIDTH, KEY_HEIGHT, '7', '&', 0},
    {KEY_XPOS(7),  START_Y, KEY_WIDTH, KEY_HEIGHT, '8', '*', 0},
    {KEY_XPOS(8),  START_Y, KEY_WIDTH, KEY_HEIGHT, '9', '(', 0},
    {KEY_XPOS(9),  START_Y, KEY_WIDTH, KEY_HEIGHT, '0', ')', 0},
    {KEY_XPOS(10), START_Y, 30, KEY_HEIGHT, '\b', '\b', KEY_BACKSPACE} // Backspace(宽度减小)
};

// 第二行(字母Q-P) - 10个键
const Key_TypeDef keyboard_row2[] = {
    {KEY_XPOS(0)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'q', 'Q', 0}, // 向右偏移5px
    {KEY_XPOS(1)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'w', 'W', 0},
    {KEY_XPOS(2)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'e', 'E', 0},
    {KEY_XPOS(3)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'r', 'R', 0},
    {KEY_XPOS(4)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 't', 'T', 0},
    {KEY_XPOS(5)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'y', 'Y', 0},
    {KEY_XPOS(6)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'u', 'U', 0},
    {KEY_XPOS(7)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'i', 'I', 0},
    {KEY_XPOS(8)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'o', 'O', 0},
    {KEY_XPOS(9)+5, START_Y+KEY_HEIGHT+KEY_SPACING, KEY_WIDTH, KEY_HEIGHT, 'p', 'P', 0}
};

// 第三行(字母A-L) - 10个键
const Key_TypeDef keyboard_row3[] = {
    {KEY_XPOS(0)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'a', 'A', 0}, // 向右偏移10px
    {KEY_XPOS(1)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 's', 'S', 0},
    {KEY_XPOS(2)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'd', 'D', 0},
    {KEY_XPOS(3)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'f', 'F', 0},
    {KEY_XPOS(4)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'g', 'G', 0},
    {KEY_XPOS(5)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'h', 'H', 0},
    {KEY_XPOS(6)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'j', 'J', 0},
    {KEY_XPOS(7)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'k', 'K', 0},
    {KEY_XPOS(8)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'l', 'L', 0},
    {KEY_XPOS(9)+10, START_Y+2*(KEY_HEIGHT+KEY_SPACING), 70, KEY_HEIGHT, 0, 0, KEY_ENTER} // Enter键
};

// 第四行(Shift+Z-M) - 10个键
const Key_TypeDef keyboard_row4[] = {
    {KEY_XPOS(0), START_Y+3*(KEY_HEIGHT+KEY_SPACING), 60, KEY_HEIGHT, 0, 0,KEY_SHIFT}, // Shift键
    {KEY_XPOS(1)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'z', 'Z', 0}, // 向右偏移15px
    {KEY_XPOS(2)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'x', 'X', 0},
    {KEY_XPOS(3)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'c', 'C', 0},
    {KEY_XPOS(4)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'v', 'V', 0},
    {KEY_XPOS(5)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'b', 'B', 0},
    {KEY_XPOS(6)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'n', 'N', 0},
    {KEY_XPOS(7)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, 'm', 'M', 0},
    {KEY_XPOS(8)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, ',', '<', 0},
    {KEY_XPOS(9)+15, START_Y+3*(KEY_HEIGHT+KEY_SPACING), KEY_WIDTH, KEY_HEIGHT, '.', '>', 0}
};

// 第五行(功能键) - 5个键
const Key_TypeDef keyboard_row5[] = {
    {0, START_Y+4*(KEY_HEIGHT+KEY_SPACING), 70, KEY_HEIGHT, 0, 0, KEY_CAPS}, // Caps Lock
    {75, START_Y+4*(KEY_HEIGHT+KEY_SPACING), 70, KEY_HEIGHT, 0, 0,  KEY_SYMBOL}, // 符号键
    {150, START_Y+4*(KEY_HEIGHT+KEY_SPACING), 180, KEY_HEIGHT, ' ', ' ', KEY_SPACE}, // 空格键
    {335, START_Y+4*(KEY_HEIGHT+KEY_SPACING), 70, KEY_HEIGHT, 0, 0,KEY_SYMBOL}, // 符号键
    {410, START_Y+4*(KEY_HEIGHT+KEY_SPACING), 70, KEY_HEIGHT, 0, 0,KEY_SYMBOL} // 自定义功能键
};

// 键盘行信息
typedef struct {
    const Key_TypeDef *keys;
    uint8_t key_count;
} KeyboardRow_TypeDef;

// 完整的键盘布局
const KeyboardRow_TypeDef keyboard[] = {
    {keyboard_row1, sizeof(keyboard_row1)/sizeof(Key_TypeDef)},
    {keyboard_row2, sizeof(keyboard_row2)/sizeof(Key_TypeDef)},
    {keyboard_row3, sizeof(keyboard_row3)/sizeof(Key_TypeDef)},
    {keyboard_row4, sizeof(keyboard_row4)/sizeof(Key_TypeDef)},
    {keyboard_row5, sizeof(keyboard_row5)/sizeof(Key_TypeDef)}
};

#define KEYBOARD_ROWS (sizeof(keyboard)/sizeof(KeyboardRow_TypeDef))

void Draw_Keyboard_Bg(u16 start_x, u16 start_y, u16 width, u16 height, u16 bg_color, u16 border_color);
void Draw_Single_Key(const Key_TypeDef *key, u16 bg_color, u16 text_color, u16 border_color) ;
void Draw_Single_AKey(const Key_TypeDef *key, u16 bg_color, u16 text_color, u16 border_color);
void Draw_Keyboard(u16 bg_color, u16 key_bg_color, u16 text_color, u16 border_color,u8 mode);
void Show_Default_Keyboard(u8 mode);
char Detect_Keyboard_Touch(u8 mode) ;
u8 Process_Keyboard(char* buf);
#endif