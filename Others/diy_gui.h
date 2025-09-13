#ifndef __COMMON_H
#define __COMMON_H 

#include "system.h"
#include "tftlcd.h"
#include "touch.h"
#include "stdio.h"


//����TFTLCD�������ͺ�ѡ�����ش�С��ѡ���Ӧ�ĺ궨��ֵ
#if	defined(TFTLCD_HX8357D)||defined(TFTLCD_HX8352C)||defined(TFTLCD_ILI9341)|| \
	defined(TFTLCD_R61509V)||defined(TFTLCD_R61509VN)||defined(TFTLCD_R61509V3)|| \
	defined(TFTLCD_ST7793)||defined(TFTLCD_ILI9325)||defined(TFTLCD_R61509VE)|| \
	defined(TFTLCD_SSD1963N)
#define TFTLCD_PIXEL_240X320	//TFTLCD���������أ�240*320��240*400
#endif

#if	defined(TFTLCD_ILI9486)||defined(TFTLCD_ILI9327)|| \
	defined(TFTLCD_HX8357DN)||defined(TFTLCD_ILI9481)|| \
	defined(TFTLCD_ILI9488)
#define TFTLCD_PIXEL_320X480	//TFTLCD���������أ�320*480
#endif

#if	defined(TFTLCD_NT35510)||defined(TFTLCD_SSD1963)|| \
	defined(TFTLCD_ILI9806) 
#define TFTLCD_PIXEL_480X800	//TFTLCD���������أ�480*800
#endif


//����LOGOͼ��ߴ�
#define PIC_KJ_LOGO_WIDTH		175
#define PIC_KJ_LOGO_HEIGHT		83

//UI���汳����ǰ��ɫ
#define UI_BACKCOLOR			BLUE	//����
#define UI_FRONTCOLOR			0xFFFF	//��ɫ



//#ifdef TFTLCD_PIXEL_240X320		//TFTLCD���������أ�240*320��240*400
////ICONӦ��ͼ��ߴ�
//#define PIC_ICON_APP_WIDTH		46
//#define PIC_ICON_APP_HEIGHT		46

//#define PIC_ICON_APP_XSTART		10
//#define PIC_ICON_APP_YSTART		20

//#define PIC_ICON_APP_XSPACE		10
//#define PIC_ICON_APP_YSPACE		20

////APPӦ��ͼ��������ʾ�ߴ�
//#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
//#define PIC_ICON_APP_NAME_HEIGHT	16
//#define PIC_ICON_APP_NAME_FONT_SIZE	12

////����״̬��ʾ����Ʒ�ͺš�ʱ�䣩
//#define TOP_STATUS_NAME_HEIGHT		16
//#define TOP_STATUS_NAME_FONT_SIZE	12
//#endif

#ifdef TFTLCD_PIXEL_240X320		//TFTLCD���������أ�240*320��240*400
//ICONӦ��ͼ��ߴ�
#define PIC_ICON_APP_WIDTH		46
#define PIC_ICON_APP_HEIGHT		46

#define PIC_ICON_APP_XSTART		15
#define PIC_ICON_APP_YSTART		20

#define PIC_ICON_APP_XSPACE		20
#define PIC_ICON_APP_YSPACE		20

//APPӦ��ͼ��������ʾ�ߴ�
#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
#define PIC_ICON_APP_NAME_HEIGHT	16
#define PIC_ICON_APP_NAME_FONT_SIZE	12

//����״̬��ʾ����Ʒ�ͺš�ʱ�䣩
#define TOP_STATUS_NAME_HEIGHT		16
#define TOP_STATUS_NAME_FONT_SIZE	12
#endif

#ifdef TFTLCD_PIXEL_320X480		//TFTLCD���������أ�320*480
#define PIC_ICON_APP_WIDTH		66
#define PIC_ICON_APP_HEIGHT		66

#define PIC_ICON_APP_XSTART		13
#define PIC_ICON_APP_YSTART		25

#define PIC_ICON_APP_XSPACE		10
#define PIC_ICON_APP_YSPACE		30

//APPӦ��ͼ��������ʾ�ߴ�
#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
#define PIC_ICON_APP_NAME_HEIGHT	20
#define PIC_ICON_APP_NAME_FONT_SIZE	12

//����״̬��ʾ����Ʒ�ͺš�ʱ�䣩
#define TOP_STATUS_NAME_HEIGHT		20
#define TOP_STATUS_NAME_FONT_SIZE	16
#endif

#ifdef TFTLCD_PIXEL_480X800		//TFTLCD���������أ�480*800
#define PIC_ICON_APP_WIDTH		200
#define PIC_ICON_APP_HEIGHT		200

#define PIC_ICON_APP_XSTART		25
#define PIC_ICON_APP_YSTART		25

#define PIC_ICON_APP_XSPACE		40
#define PIC_ICON_APP_YSPACE		40

//APPӦ��ͼ��������ʾ�ߴ�
#define PIC_ICON_APP_NAME_WIDTH		PIC_ICON_APP_WIDTH
#define PIC_ICON_APP_NAME_HEIGHT	30
#define PIC_ICON_APP_NAME_FONT_SIZE	24

//����״̬��ʾ����Ʒ�ͺš�ʱ�䣩
#define TOP_STATUS_NAME_HEIGHT		20
#define TOP_STATUS_NAME_FONT_SIZE	16
#endif



//ƽ���ߵ���ֹ��ɫ����
#define WIN_SMOOTH_LINE_SEC	0XB1FFC4	//��ֹ��ɫ
#define WIN_SMOOTH_LINE_MC	0X1600B1	//�м���ɫ

//��������ѡ����Ŀ��������Ϣ
#define APP_ITEM_BTN1_WIDTH		60	  		//��2������ʱ�Ŀ��
#define APP_ITEM_BTN2_WIDTH		100			//ֻ��1������ʱ�Ŀ��
#define APP_ITEM_BTN_HEIGHT		30			//�����߶�
#define APP_ITEM_ICO_SIZE		32			//ICOͼ��ĳߴ�

#define APP_ITEM_SEL_BKCOLOR	0X0EC3		//ѡ��ʱ�ı���ɫ
#define APP_WIN_BACK_COLOR	 	0XC618		//���屳��ɫ


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
