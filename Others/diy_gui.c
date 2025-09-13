#include "diy_gui.h"
#include "logo.h"
//ICON应用APP图标列表索引
const unsigned char* icon_ui_app_tbl[][2]=
{
	{gImage_one,"Find Book"},
	{gImage_two,"Borrow Book"},
	{gImage_three,"Return Book"},
	{gImage_four,"Change Book"},
};
	



//在中间位置显示一个字符串
//x,y,width,height:显示区域坐标.
//color:文字颜色.
//size:文字大小
//str:字符串
void gui_show_strmid(u16 x,u16 y,u16 width,u16 height,u16 color,u8 size,u8 *str)
{
	u16 xoff=0,yoff=0;
	u16 strlenth;
	u16 strwidth;
	if(str==NULL)return;
   	strlenth=strlen((const char*)str);	//得到字符串长度
	strwidth=strlenth*size/2;		//字符串显示占用宽度
	if(height>size)yoff=(height-size)/2;
	if(strwidth<=width)//字符串没超过宽度
 	{
		xoff=(width-strwidth)/2;	  
	}
	FRONT_COLOR = color;BACK_COLOR=WHITE;
	LCD_ShowString(x+xoff,y+yoff,x+width-1,y+height-1,size,str);
	FRONT_COLOR = WHITE;BACK_COLOR=BLUE;
}


//人机界面初始化
void ICON_UI_Init(void)
{
	char i=0,j=0;
	
	LCD_Clear(UI_BACKCOLOR);//LIGHTGREEN	
	FRONT_COLOR=UI_FRONTCOLOR;
	BACK_COLOR=UI_BACKCOLOR;
	
	//APP应用图标、名称显示
	for(i=0;i<2;i++)
	{
		for(j=0;j<2;j++)
		{
			LCD_ShowPicture(PIC_ICON_APP_XSTART+(PIC_ICON_APP_XSPACE+PIC_ICON_APP_WIDTH)*j,
							PIC_ICON_APP_YSTART+(PIC_ICON_APP_YSPACE+PIC_ICON_APP_HEIGHT)*i,
							PIC_ICON_APP_WIDTH,PIC_ICON_APP_HEIGHT,
							(u8 *)icon_ui_app_tbl[j+2*i][0]);
			gui_show_strmid(PIC_ICON_APP_XSTART+(PIC_ICON_APP_XSPACE+PIC_ICON_APP_WIDTH)*j,
							PIC_ICON_APP_YSTART+PIC_ICON_APP_HEIGHT+(PIC_ICON_APP_YSPACE+PIC_ICON_APP_HEIGHT)*i,
							PIC_ICON_APP_NAME_WIDTH,PIC_ICON_APP_NAME_HEIGHT,
							BLACK,PIC_ICON_APP_NAME_FONT_SIZE,
							(u8 *)icon_ui_app_tbl[j+2*i][1]);
		}
	}
}

//判断哪个应用APP图标被按下，并返回对应索引
//返回值:0~15,被双击的图标编号
//		0xff,没有任何图标被双击或者按下
u8 get_icon_app_table(void)
{
	u8 index=0xff;
	static u16 curxpos=0;//当前tp按下的x坐标
	static u16 curypos=0;//当前tp按下的y坐标
	static u8 curtpsta=0;//触摸按下标志
	u8 i=0,j=0;
	
	tp_dev.scan(0);	//扫描
	if(tp_dev.sta&TP_PRES_DOWN)//有按键被按下
	{
		curxpos=tp_dev.x[0];	//记录当前坐标
		curypos=tp_dev.y[0];	//记录当前坐标
		curtpsta=1;
	}
	else	//按键松开了
	{
		if(curtpsta)//之前有按下
		{
			for(i=0;i<2;i++)
			{
				for(j=0;j<2;j++)
				{
					if(curxpos>(PIC_ICON_APP_XSTART+(PIC_ICON_APP_XSPACE+PIC_ICON_APP_WIDTH)*j)
						&& curxpos<(PIC_ICON_APP_XSTART+(PIC_ICON_APP_XSPACE+PIC_ICON_APP_WIDTH)*j+PIC_ICON_APP_WIDTH)
						&& curypos>(PIC_ICON_APP_YSTART+(PIC_ICON_APP_YSPACE+PIC_ICON_APP_HEIGHT)*i)
						&& curypos<(PIC_ICON_APP_YSTART+(PIC_ICON_APP_YSPACE+PIC_ICON_APP_HEIGHT)*i+PIC_ICON_APP_HEIGHT))
					{
						index=j+2*i;//得到选中的编号
						break;
					}
				}
			}
//			printf("index=%d\r\n",index);
//			printf("curxpos=%d  curypos=%d\r\n",curxpos,curypos);
		}
		curtpsta=0;//清空标志
	}
	return index;
}

//
void Change_ICON_Init(void)
{
		LCD_Clear(WHITE);
		gui_show_strmid(change_xstart,change_ystart,change_xwidth,change_yheight,GREEN,24,"ADD SOME BOOK TO THE LIBRARY");
		gui_show_strmid(change_xstart,change_ystart+change_yheight,change_xwidth,change_yheight,GREEN,24,"DELETE SOME BOOK FROM LIBARY");
		gui_show_strmid(0,700,480,100,RED,24,"RETURN TO THE COVER");
}

u8 get_change_table(void)
{
	u8 index=0xff;
	static u16 curxpos=0;//当前tp按下的x坐标
	static u16 curypos=0;//当前tp按下的y坐标
	static u8 curtpsta=0;//触摸按下标志
	u8 i=0,j=0;
	
	tp_dev.scan(0);	//扫描
	if(tp_dev.sta&TP_PRES_DOWN)//有按键被按下
	{
		curxpos=tp_dev.x[0];	//记录当前坐标
		curypos=tp_dev.y[0];	//记录当前坐标
		curtpsta=1;
	}
	else	//按键松开了
	{
		if(curtpsta)//之前有按下
		{
//			if(curypos>change_ystart&&curypos<change_ystart+change_yheight)index=0;
//			else if(curypos>change_ystart+change_yheight&&curypos<change_ystart+2*change_yheight)index=1;
			for(i=0;i<2;i++){
				if(curypos>change_ystart+i*change_yheight&&curypos<change_ystart+(i+1)*change_yheight)index=i;
			}
		}
//			printf("index=%d\r\n",index);
//			printf("curxpos=%d  curypos=%d\r\n",curxpos,curypos);
		curtpsta=0;//清空标志
	}
	return index;
}


u8 get_return_table(void)
{
	u8 index=0;
	static u16 curxpos=0;//当前tp按下的x坐标
	static u16 curypos=0;//当前tp按下的y坐标
	static u8 curtpsta=0;//触摸按下标志
	u8 i=0,j=0;
	
	tp_dev.scan(0);	//扫描
	if(tp_dev.sta&TP_PRES_DOWN)//有按键被按下
	{
		curxpos=tp_dev.x[0];	//记录当前坐标
		curypos=tp_dev.y[0];	//记录当前坐标
		curtpsta=1;
	}
	else	//按键松开了
	{
		if(curtpsta)//之前有按下
		{
			if(curypos>700)index=1;
		}
		curtpsta=0;//清空标志
	}
	return index;
}

