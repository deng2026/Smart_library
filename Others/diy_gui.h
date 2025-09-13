#ifndef __COMMON_H
#define __COMMON_H 

#include "system.h"
#include "tftlcd.h"
#include "touch.h"
#include "stdio.h"


//根据TFTLCD触摸屏型号选择像素大小，选择对应的宏定义值
#if	defined(TFTLCD_HX8357D)||defined(TFTLCD_HX8352C)||defined(TFTLCD_ILI9341)|| \
	defined(TFTLCD_R61509V)||defined(TFTLCD_R61509VN)||defined(TFTLCD_R61509V3)|| \
	defined(TFTLCD_ST7793)||defined(TFTLCD_ILI9325)||defined(TFTLCD_R61509VE)|| \
	defined(TFTLCD_SSD1963N)
#define TFTLCD_PIXEL_240X320	//TFTLCD触摸屏像素：240*320、240*400
#endif

#if	defined(TFTLCD_ILI9486)||defined(TFTLCD_ILI9327)|| \
	defined(TFTLCD_HX8357DN)||defined(TFTLCD_ILI9481)|| \
	defined(TFTLCD_ILI9488)
#define TFTLCD_PIXEL_320X480	//TFTLCD触摸屏像素：320*480
#endif

#if	defined(TFTLCD_NT35510)||defined(TFTLCD_SSD1963)|| \
	defined(TFTLCD_ILI9806) 
#define TFTLCD_PIXEL_480X800	//TFTLCD触摸屏像素：480*800
#endif


//开机LOGO图标尺寸
#define PIC_KJ_LOGO_WIDTH		175
#define PIC_KJ_LOGO_HEIGHT		83

//UI界面背景、前景色
#define UI_BACKCOLOR			BLUE	//深蓝
#define UI_FRONTCOLOR			0xFFFF	//白色



//#ifdef TFTLCD_PIXEL_240X320		//TFTLCD触摸屏像素：240*320、240*400
////ICON应用图标尺寸
//#define PIC_ICON_APP_WIDTH		46
//#define PIC_ICON_APP_HEIGHT		46

//#define PIC_ICON_APP_XSTART		10
//#define PIC_ICON_APP_YSTART		20

//#define PIC_ICON_APP_XSPACE		10
//#define PIC_ICON_APP_YSPACE		20

////APP应用图标名称显示尺寸
//#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
//#define PIC_ICON_APP_NAME_HEIGHT	16
//#define PIC_ICON_APP_NAME_FONT_SIZE	12

////顶部状态显示（产品型号、时间）
//#define TOP_STATUS_NAME_HEIGHT		16
//#define TOP_STATUS_NAME_FONT_SIZE	12
//#endif

#ifdef TFTLCD_PIXEL_240X320		//TFTLCD触摸屏像素：240*320、240*400
//ICON应用图标尺寸
#define PIC_ICON_APP_WIDTH		46
#define PIC_ICON_APP_HEIGHT		46

#define PIC_ICON_APP_XSTART		15
#define PIC_ICON_APP_YSTART		20

#define PIC_ICON_APP_XSPACE		20
#define PIC_ICON_APP_YSPACE		20

//APP应用图标名称显示尺寸
#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
#define PIC_ICON_APP_NAME_HEIGHT	16
#define PIC_ICON_APP_NAME_FONT_SIZE	12

//顶部状态显示（产品型号、时间）
#define TOP_STATUS_NAME_HEIGHT		16
#define TOP_STATUS_NAME_FONT_SIZE	12
#endif

#ifdef TFTLCD_PIXEL_320X480		//TFTLCD触摸屏像素：320*480
#define PIC_ICON_APP_WIDTH		66
#define PIC_ICON_APP_HEIGHT		66

#define PIC_ICON_APP_XSTART		13
#define PIC_ICON_APP_YSTART		25

#define PIC_ICON_APP_XSPACE		10
#define PIC_ICON_APP_YSPACE		30

//APP应用图标名称显示尺寸
#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
#define PIC_ICON_APP_NAME_HEIGHT	20
#define PIC_ICON_APP_NAME_FONT_SIZE	12

//顶部状态显示（产品型号、时间）
#define TOP_STATUS_NAME_HEIGHT		20
#define TOP_STATUS_NAME_FONT_SIZE	16
#endif

#ifdef TFTLCD_PIXEL_480X800		//TFTLCD触摸屏像素：480*800
#define PIC_ICON_APP_WIDTH		200
#define PIC_ICON_APP_HEIGHT		200

#define PIC_ICON_APP_XSTART		25
#define PIC_ICON_APP_YSTART		25

#define PIC_ICON_APP_XSPACE		40
#define PIC_ICON_APP_YSPACE		40

//APP应用图标名称显示尺寸
#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
#define PIC_ICON_APP_NAME_HEIGHT	30
#define PIC_ICON_APP_NAME_FONT_SIZE	24

//顶部状态显示（产品型号、时间）
#define TOP_STATUS_NAME_HEIGHT		20
#define TOP_STATUS_NAME_FONT_SIZE	16
#endif



//平滑线的起止颜色定义
#define WIN_SMOOTH_LINE_SEC	0XB1FFC4	//起止颜色
#define WIN_SMOOTH_LINE_MC	0X1600B1	//中间颜色

//弹出窗口选择条目的设置信息
#define APP_ITEM_BTN1_WIDTH		60	  		//有2个按键时的宽度
#define APP_ITEM_BTN2_WIDTH		100			//只有1个按键时的宽度
#define APP_ITEM_BTN_HEIGHT		30			//按键高度
#define APP_ITEM_ICO_SIZE		32			//ICO图标的尺寸

#define APP_ITEM_SEL_BKCOLOR	0X0EC3		//选择时的背景色
#define APP_WIN_BACK_COLOR	 	0XC618		//窗体背景色


#define change_xstart 0
#define change_xwidth 480
#define change_ystart 300
#define change_yheight 60

void gui_show_strmid(u16 x,u16 y,u16 width,u16 height,u16 color,u8 size,u8 *str);
u8 get_icon_app_table(void);
void ICON_UI_Init(void);
u8 get_change_table(void);
u8 get_return_table(void);
void Change_ICON_Init(void);
#endif
