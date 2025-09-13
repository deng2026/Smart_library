#include "keyboard.h"

/**
  * @brief  ���Ƽ��̱����ͱ߿�
  * @param  start_x: ������ʼX����
  * @param  start_y: ������ʼY����
  * @param  width: �����ܿ��
  * @param  height: �����ܸ߶�
  * @param  bg_color: ������ɫ
  * @param  border_color: �߿���ɫ
  * @retval ��
  */
void Draw_Keyboard_Bg(u16 start_x, u16 start_y, u16 width, u16 height, u16 bg_color, u16 border_color)
{
    // ���Ƽ��̱���
    LCD_Fill(start_x, start_y, start_x + width, start_y + height, bg_color);
    // ���Ƽ��̱߿�
    LCD_DrawLine_Color(start_x, start_y, start_x + width, start_y, border_color); // �ϱ߿�
    LCD_DrawLine_Color(start_x, start_y, start_x, start_y + height, border_color); // ��߿�
    LCD_DrawLine_Color(start_x + width, start_y, start_x + width, start_y + height, border_color); // �ұ߿�
    LCD_DrawLine_Color(start_x, start_y + height, start_x + width, start_y + height, border_color); // �±߿�
}

/**
  * @brief  ���Ƶ�������
  * @param  key: �����ṹ��ָ��
  * @param  bg_color: ����������ɫ
  * @param  text_color: ����������ɫ
  * @param  border_color: �����߿���ɫ
  * @retval ��
  */
void Draw_Single_Key(const Key_TypeDef *key, u16 bg_color, u16 text_color, u16 border_color) 
{
		u16 text_x =0;
		u16 text_y =0;
		FRONT_COLOR = text_color;BACK_COLOR=bg_color;
    if (key == NULL) {
        return;
    }
    // ���ư�������
    LCD_Fill(key->x, key->y, key->x + key->width, key->y + key->height, bg_color);
    
    // ���ư����߿�
    LCD_DrawLine_Color(key->x, key->y, key->x + key->width, key->y, border_color); // �ϱ߿�
    LCD_DrawLine_Color(key->x, key->y, key->x, key->y + key->height, border_color); // ��߿�
    LCD_DrawLine_Color(key->x + key->width, key->y, key->x + key->width, key->y + key->height, border_color); // �ұ߿�
    LCD_DrawLine_Color(key->x, key->y + key->height, key->x + key->width, key->y + key->height, border_color); // �±߿�
    
    // ����ǹ��ܼ���û���ַ�������ʾ����
    if (key->is_function && key->normal == 0) return;
    
    // ����������ʾλ��
    text_x = key->x + (key->width - 8) / 2; // �����ַ����Ϊ8,����Ӳ����һ��
    text_y = key->y + (key->height - 16) / 2; // �����ַ��߶�Ϊ16
    
    // ��ʾ�����ַ�
    if (key->normal == '\b') {
        LCD_ShowChar(text_x, text_y, '<', 16, 0); // �˸����ʾ <
    } else if (key->normal == ' ') {
        // �ո������ʾ�ַ�
    } else {
        char c = key->normal;
        if (c >= 32 && c <= 126) { // �ɴ�ӡASCII�ַ�
            LCD_ShowChar(text_x, text_y, c, 16, 0);
        }
    }
		FRONT_COLOR = UI_FRONTCOLOR	;BACK_COLOR=UI_BACKCOLOR;
}

//shift����£���������
void Draw_Single_AKey(const Key_TypeDef *key, u16 bg_color, u16 text_color, u16 border_color) 
{
		u16 text_x =0;
		u16 text_y =0;
		FRONT_COLOR = text_color;BACK_COLOR=bg_color;
    if (key == NULL) {
        return;
    }
    // ���ư�������
    LCD_Fill(key->x, key->y, key->x + key->width, key->y + key->height, bg_color);
    
    // ���ư����߿�
    LCD_DrawLine_Color(key->x, key->y, key->x + key->width, key->y, border_color); // �ϱ߿�
    LCD_DrawLine_Color(key->x, key->y, key->x, key->y + key->height, border_color); // ��߿�
    LCD_DrawLine_Color(key->x + key->width, key->y, key->x + key->width, key->y + key->height, border_color); // �ұ߿�
    LCD_DrawLine_Color(key->x, key->y + key->height, key->x + key->width, key->y + key->height, border_color); // �±߿�
    
    // ����ǹ��ܼ���û���ַ�������ʾ����
    if (key->is_function && key->shift == 0) return;
    
    // ����������ʾλ��
    text_x = key->x + (key->width - 8) / 2; // �����ַ����Ϊ8,����Ӳ����һ��
    text_y = key->y + (key->height - 16) / 2; // �����ַ��߶�Ϊ16
    
    // ��ʾ�����ַ�
    if (key->shift == '\b') {
        LCD_ShowChar(text_x, text_y, '<', 16, 0); // �˸����ʾ <
    } else if (key->shift == ' ') {
        // �ո������ʾ�ַ�
    } else {
        char c = key->shift;
        if (c >= 32 && c <= 126) { // �ɴ�ӡASCII�ַ�
            LCD_ShowChar(text_x, text_y, c, 16, 0);
        }
    }
		FRONT_COLOR = UI_FRONTCOLOR	;BACK_COLOR=UI_BACKCOLOR;
}

/**
  * @brief  ������������
  * @param  bg_color: ���̱�����ɫ
  * @param  key_bg_color: ����������ɫ
  * @param  text_color: ����������ɫ
  * @param  border_color: �߿���ɫ
  * @retval ��
  */
void Draw_Keyboard(u16 bg_color, u16 key_bg_color, u16 text_color, u16 border_color,u8 mode) 
{
		u8 row = 0;u8 col = 0;
    // ���Ƽ��̱���
    Draw_Keyboard_Bg(0, START_Y, 480, KEYBOARD_ROWS*(KEY_HEIGHT + KEY_SPACING), bg_color, border_color);
    
    // �������а���
    for (row = 0; row < KEYBOARD_ROWS; row++) {
        for (col = 0; col < keyboard[row].key_count; col++) {
            if(mode==0)Draw_Single_Key(&keyboard[row].keys[col], key_bg_color, text_color, border_color);
						else Draw_Single_AKey(&keyboard[row].keys[col], key_bg_color, text_color, border_color);
        }
    }
    
    // ��ʾ���ܼ���ǩ
		 // Enter����ǩ
    gui_show_strmid(415, START_Y + 2 * (KEY_HEIGHT + KEY_SPACING) , 65, 50, BLACK,16, "Enter");
    // Shift����ǩ
    gui_show_strmid(0, START_Y + 3 * (KEY_HEIGHT + KEY_SPACING) , 60, 50, BLACK, 16, "Shift");
    // Caps����ǩ
    gui_show_strmid(0, START_Y + 4 * (KEY_HEIGHT + KEY_SPACING), 70, 50, BLACK, 16, "Caps");
    // �ո����ǩ
    gui_show_strmid(150, START_Y + 4 * (KEY_HEIGHT + KEY_SPACING),180,50, BLACK, 16, "Space");
}


/**
  * @brief  ��ʾĬ����ɫ�ļ���
  * @retval ��
  */
void Show_Default_Keyboard(u8 mode) 
{
    // ��ɫ���� (RGB565��ʽ)
    #define BG_COLOR     LIGHTBLUE  // ǳ��ɫ����
    #define KEY_COLOR    0xFFFF  // ��ɫ����
    #define TEXT_COLOR   0x0000  // ��ɫ����
    #define BORDER_COLOR 0x7BEF  // ��ɫ�߿�
		
    Draw_Keyboard(BG_COLOR, KEY_COLOR, TEXT_COLOR, BORDER_COLOR,mode);

}



// ���幦�ܼ�����ֵ��������ASCII��ͻ��
#define FUNC_BACKSPACE 0x08    // �˸��(��'\b'��ͬ)
#define FUNC_ENTER     0x0D     // �س���(��'\n'��ͬ)
#define FUNC_SHIFT     0x80     // Shift���ܼ�
#define FUNC_CAPS      0x81     // Caps���ܼ�  
#define FUNC_SPACE     0x20     // �ո��(��' '��ͬ)
#define FUNC_SYMBOL    0x82     // ���ż�

char Detect_Keyboard_Touch(u8 mode) 
{
    static u16 last_x = 0, last_y = 0;
    static u8 touch_state = 0;
    char key_value = 0;
    u8 row = 0,col=0;
    tp_dev.scan(0);
    
    // ��ⴥ������
    if(tp_dev.sta & TP_PRES_DOWN) {
        last_x = tp_dev.x[0];
        last_y = tp_dev.y[0];
        touch_state = 1;
    } 
    // �������ͷ�
    else if(touch_state) {
				touch_state = 0;
        for( row = 0; row < KEYBOARD_ROWS; row++) {
            for( col = 0; col < keyboard[row].key_count; col++) {
                const Key_TypeDef *key = &keyboard[row].keys[col];
                
                // ��鴥�����Ƿ��ڰ���������
                if(last_x > key->x && last_x < key->x + key->width &&
                   last_y > key->y && last_y < key->y + key->height) {
                    //Draw_Single_Key(key,YELLOW, RED, GBLUE);//����������ɫ
                    // �����ܼ�
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
                    // ������ͨ�ַ���
                    else {
                        return mode ? key-> shift : key->normal;
                    }
                }
            }
        }
       
    }
    
    return 0; // ����Ч����
}



//�������������
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
            shift_state ^= 1; // �л�Shift״̬
            Show_Default_Keyboard(shift_state ^ caps_state); // �ػ����
            break;
            
        case FUNC_CAPS:
            caps_state ^= 1; // �л�Caps״̬
            Show_Default_Keyboard(shift_state ^ caps_state); // �ػ����
            break;
                   
        case  FUNC_ENTER: 
						buf[i]='\0';//�ַ����������ˣ����ﲻ��������
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
                // ������ͨ�ַ�����
                buf[i++]=key;
								buf[i]=0;
								return (u8)buf[i-1];
            }
    }
}