#include "keyboard.h"

/**
  * @brief  绘制键盘背景和边框
  * @param  start_x: 背景起始X坐标
  * @param  start_y: 背景起始Y坐标
  * @param  width: 背景总宽度
  * @param  height: 背景总高度
  * @param  bg_color: 背景颜色
  * @param  border_color: 边框颜色
  * @retval 无
  */
void Draw_Keyboard_Bg(u16 start_x, u16 start_y, u16 width, u16 height, u16 bg_color, u16 border_color)
{
    // 绘制键盘背景
    LCD_Fill(start_x, start_y, start_x + width, start_y + height, bg_color);
    // 绘制键盘边框
    LCD_DrawLine_Color(start_x, start_y, start_x + width, start_y, border_color); // 上边框
    LCD_DrawLine_Color(start_x, start_y, start_x, start_y + height, border_color); // 左边框
    LCD_DrawLine_Color(start_x + width, start_y, start_x + width, start_y + height, border_color); // 右边框
    LCD_DrawLine_Color(start_x, start_y + height, start_x + width, start_y + height, border_color); // 下边框
}

/**
  * @brief  绘制单个按键
  * @param  key: 按键结构体指针
  * @param  bg_color: 按键背景颜色
  * @param  text_color: 按键文字颜色
  * @param  border_color: 按键边框颜色
  * @retval 无
  */
void Draw_Single_Key(const Key_TypeDef *key, u16 bg_color, u16 text_color, u16 border_color) 
{
		u16 text_x =0;
		u16 text_y =0;
		FRONT_COLOR = text_color;BACK_COLOR=bg_color;
    if (key == NULL) {
        return;
    }
    // 绘制按键背景
    LCD_Fill(key->x, key->y, key->x + key->width, key->y + key->height, bg_color);
    
    // 绘制按键边框
    LCD_DrawLine_Color(key->x, key->y, key->x + key->width, key->y, border_color); // 上边框
    LCD_DrawLine_Color(key->x, key->y, key->x, key->y + key->height, border_color); // 左边框
    LCD_DrawLine_Color(key->x + key->width, key->y, key->x + key->width, key->y + key->height, border_color); // 右边框
    LCD_DrawLine_Color(key->x, key->y + key->height, key->x + key->width, key->y + key->height, border_color); // 下边框
    
    // 如果是功能键且没有字符，则不显示文字
    if (key->is_function && key->normal == 0) return;
    
    // 计算文字显示位置
    text_x = key->x + (key->width - 8) / 2; // 假设字符宽度为8,这里硬编码一下
    text_y = key->y + (key->height - 16) / 2; // 假设字符高度为16
    
    // 显示按键字符
    if (key->normal == '\b') {
        LCD_ShowChar(text_x, text_y, '<', 16, 0); // 退格键显示 <
    } else if (key->normal == ' ') {
        // 空格键不显示字符
    } else {
        char c = key->normal;
        if (c >= 32 && c <= 126) { // 可打印ASCII字符
            LCD_ShowChar(text_x, text_y, c, 16, 0);
        }
    }
		FRONT_COLOR = UI_FRONTCOLOR	;BACK_COLOR=UI_BACKCOLOR;
}

//shift情况下，单个按键
void Draw_Single_AKey(const Key_TypeDef *key, u16 bg_color, u16 text_color, u16 border_color) 
{
		u16 text_x =0;
		u16 text_y =0;
		FRONT_COLOR = text_color;BACK_COLOR=bg_color;
    if (key == NULL) {
        return;
    }
    // 绘制按键背景
    LCD_Fill(key->x, key->y, key->x + key->width, key->y + key->height, bg_color);
    
    // 绘制按键边框
    LCD_DrawLine_Color(key->x, key->y, key->x + key->width, key->y, border_color); // 上边框
    LCD_DrawLine_Color(key->x, key->y, key->x, key->y + key->height, border_color); // 左边框
    LCD_DrawLine_Color(key->x + key->width, key->y, key->x + key->width, key->y + key->height, border_color); // 右边框
    LCD_DrawLine_Color(key->x, key->y + key->height, key->x + key->width, key->y + key->height, border_color); // 下边框
    
    // 如果是功能键且没有字符，则不显示文字
    if (key->is_function && key->shift == 0) return;
    
    // 计算文字显示位置
    text_x = key->x + (key->width - 8) / 2; // 假设字符宽度为8,这里硬编码一下
    text_y = key->y + (key->height - 16) / 2; // 假设字符高度为16
    
    // 显示按键字符
    if (key->shift == '\b') {
        LCD_ShowChar(text_x, text_y, '<', 16, 0); // 退格键显示 <
    } else if (key->shift == ' ') {
        // 空格键不显示字符
    } else {
        char c = key->shift;
        if (c >= 32 && c <= 126) { // 可打印ASCII字符
            LCD_ShowChar(text_x, text_y, c, 16, 0);
        }
    }
		FRONT_COLOR = UI_FRONTCOLOR	;BACK_COLOR=UI_BACKCOLOR;
}

/**
  * @brief  绘制整个键盘
  * @param  bg_color: 键盘背景颜色
  * @param  key_bg_color: 按键背景颜色
  * @param  text_color: 按键文字颜色
  * @param  border_color: 边框颜色
  * @retval 无
  */
void Draw_Keyboard(u16 bg_color, u16 key_bg_color, u16 text_color, u16 border_color,u8 mode) 
{
		u8 row = 0;u8 col = 0;
    // 绘制键盘背景
    Draw_Keyboard_Bg(0, START_Y, 480, KEYBOARD_ROWS*(KEY_HEIGHT + KEY_SPACING), bg_color, border_color);
    
    // 绘制所有按键
    for (row = 0; row < KEYBOARD_ROWS; row++) {
        for (col = 0; col < keyboard[row].key_count; col++) {
            if(mode==0)Draw_Single_Key(&keyboard[row].keys[col], key_bg_color, text_color, border_color);
						else Draw_Single_AKey(&keyboard[row].keys[col], key_bg_color, text_color, border_color);
        }
    }
    
    // 显示功能键标签
		 // Enter键标签
    gui_show_strmid(415, START_Y + 2 * (KEY_HEIGHT + KEY_SPACING) , 65, 50, BLACK,16, "Enter");
    // Shift键标签
    gui_show_strmid(0, START_Y + 3 * (KEY_HEIGHT + KEY_SPACING) , 60, 50, BLACK, 16, "Shift");
    // Caps键标签
    gui_show_strmid(0, START_Y + 4 * (KEY_HEIGHT + KEY_SPACING), 70, 50, BLACK, 16, "Caps");
    // 空格键标签
    gui_show_strmid(150, START_Y + 4 * (KEY_HEIGHT + KEY_SPACING),180,50, BLACK, 16, "Space");
}


/**
  * @brief  显示默认颜色的键盘
  * @retval 无
  */
void Show_Default_Keyboard(u8 mode) 
{
    // 颜色定义 (RGB565格式)
    #define BG_COLOR     LIGHTBLUE  // 浅蓝色背景
    #define KEY_COLOR    0xFFFF  // 白色按键
    #define TEXT_COLOR   0x0000  // 黑色文字
    #define BORDER_COLOR 0x7BEF  // 灰色边框
		
    Draw_Keyboard(BG_COLOR, KEY_COLOR, TEXT_COLOR, BORDER_COLOR,mode);

}



// 定义功能键返回值（避免与ASCII冲突）
#define FUNC_BACKSPACE 0x08    // 退格键(与'\b'相同)
#define FUNC_ENTER     0x0D     // 回车键(与'\n'相同)
#define FUNC_SHIFT     0x80     // Shift功能键
#define FUNC_CAPS      0x81     // Caps功能键  
#define FUNC_SPACE     0x20     // 空格键(与' '相同)
#define FUNC_SYMBOL    0x82     // 符号键

char Detect_Keyboard_Touch(u8 mode) 
{
    static u16 last_x = 0, last_y = 0;
    static u8 touch_state = 0;
    char key_value = 0;
    u8 row = 0,col=0;
    tp_dev.scan(0);
    
    // 检测触摸按下
    if(tp_dev.sta & TP_PRES_DOWN) {
        last_x = tp_dev.x[0];
        last_y = tp_dev.y[0];
        touch_state = 1;
    } 
    // 处理触摸释放
    else if(touch_state) {
				touch_state = 0;
        for( row = 0; row < KEYBOARD_ROWS; row++) {
            for( col = 0; col < keyboard[row].key_count; col++) {
                const Key_TypeDef *key = &keyboard[row].keys[col];
                
                // 检查触摸点是否在按键区域内
                if(last_x > key->x && last_x < key->x + key->width &&
                   last_y > key->y && last_y < key->y + key->height) {
                    //Draw_Single_Key(key,YELLOW, RED, GBLUE);//换个按键颜色
                    // 处理功能键
                    if(key->is_function) {
                        switch(key->is_function) {
                            case KEY_SHIFT:   return FUNC_SHIFT;
                            case KEY_CAPS:    return FUNC_CAPS;
                            case KEY_BACKSPACE: return FUNC_BACKSPACE;
                            case KEY_ENTER:   return FUNC_ENTER;
                            case KEY_SPACE:   return FUNC_SPACE; 
                            case KEY_SYMBOL:  return FUNC_SYMBOL;
                        }
                    }
                    // 处理普通字符键
                    else {
                        return mode ? key-> shift : key->normal;
                    }
                }
            }
        }
       
    }
    
    return 0; // 无有效按键
}



//按键结果处理函数
u8 Process_Keyboard(char* buf) 
{
    
    static u8 shift_state = 0;
		static u8 caps_state = 0;
		char key=0;
		static char i;
		key	= Detect_Keyboard_Touch(shift_state ^ caps_state);
    switch(key) {
				case 0:return 0;break;
				case FUNC_SHIFT:
            shift_state ^= 1; // 切换Shift状态
            Show_Default_Keyboard(shift_state ^ caps_state); // 重绘键盘
            break;
            
        case FUNC_CAPS:
            caps_state ^= 1; // 切换Caps状态
            Show_Default_Keyboard(shift_state ^ caps_state); // 重绘键盘
            break;
                   
        case  FUNC_ENTER: 
						buf[i]='\0';//字符串输入完了，这里不用作换行
						i=0;
						return 0xff;
						break;
				
				case FUNC_BACKSPACE:
						if(i)buf[--i]='\0';
						break;
				
				case FUNC_SPACE:
						buf[i++]=' ';
						buf[i]=0;
						break;
				
        default:
            if(key > 0) {
                // 处理普通字符输入
                buf[i++]=key;
								buf[i]=0;
								return (u8)buf[i-1];
            }
    }
}