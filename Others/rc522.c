#include "rc522.h"

// M1卡分为16个扇区，每个扇区由四个块（块0、块1、块2、块3）组成
// 将16个扇区的64个块按绝对地址编号为：0~63
// 第0个扇区的块0（即绝对地址0块），用于存放厂商代码，已经固化不可更改 
// 每个扇区的块0、块1、块2为数据块，可用于存放数据
// 每个扇区的块3为控制块（绝对地址为:块3、块7、块11.....）包括密码A，存取控制、密码B等
 
/*******************************
*连线说明：
*1--SDA  <----->PA4
*2--SCK  <----->PA5
*3--MOSI <----->PA7
*4--MISO <----->PA6
*5--悬空
*6--GND <----->GND
*7--RST <----->PA0
*8--VCC <----->VCC
************************************/
 
#define   RC522_DELAY()  delay_us(50)

unsigned char CT[2];//卡类型
unsigned char SN[4]; //卡号

u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
u8 KEY_B[6]= {0xff,0xff,0xff,0xff,0xff,0xff};

//个人信息
unsigned char adr3_0=0x0C;//	第3扇区0区块(第13块)  学号
unsigned char adr3_1=0x0D;//	第3扇区1区块(第14块)   姓名
unsigned char adr3_2=0x0E;//	第3扇区2区块(第15块)   年级
unsigned char adr3_3=0x0F;//	第3扇区2区块(第16块)

unsigned char DATA3_0[16]; //读
unsigned char DATA3_1[16]; //读
unsigned char DATA3_2[16]; //读

//借阅书籍  存入对应书籍的编号
unsigned char adr4_0=0x10;//	第4扇区1区块(第17块)
unsigned char adr4_1=0x11;//	第4扇区2区块(第18块)
unsigned char adr4_3=0x13;//	第4扇区2区块(第20块)

unsigned char DATA4_0[16]; //读
unsigned char DATA4_1[16]; //读

book_item* rc522_borrow_book = NULL;
u16 rc522_book_id=0;

u8 rc522_disp_bit=1;
u8 rc522_bit = 0;
u8 rc522_state_ok; 
char ss[255];
u8 rc522_num=0;
u8 rc522_status=0;
u8 rc522_name[USART1_REC_LEN];
u8 rc522_empty_bit=0;
u8 rc522_clear = 0;
u8 rc522_clear_campare = 0;

  
void Bo_Re(u8 type)
{
	u8 len,t,i,j;  
	if(PcdRequest(PICC_REQALL,CT) != MI_OK)// 第一次寻卡失败
	{
			if(PcdRequest(PICC_REQALL,CT) != MI_OK) 
			{				
				LED1 = LED2 = 1;
				rc522_clear = 1;
				if(rc522_clear != rc522_clear_campare)
				{ 
					rc522_clear_campare = rc522_clear;
					LCD_Clear(WHITE);
					gui_show_strmid(0,300,480,40,GREEN,24,"PLEASE PUT CARD ON THE CONSOLE");      
					gui_show_strmid(0,700,480,100,RED,24,"RETURN TO THE COVER");
				}
			}	
			else		
			{			
				if(PcdAnticoll(SN) == MI_OK);// 防冲撞 获得UID 存入SN
					rc522_status = 1;
				LED1 = 0;
			}
	}
	else
	{
			if(PcdAnticoll(SN) == MI_OK);// 防冲撞 获得UID 存入SN
					rc522_status = 1;
				LED1 = 0;
	}
	
	if(rc522_status)
	{
		rc522_status = 0;
		if(MI_OK == PcdSelect(SN))
		{

			if((PcdAuthState(KEYA, adr3_3, KEY_A, SN) == MI_OK) && (PcdAuthState(KEYB, adr3_3, KEY_B, SN) == MI_OK))
			{
					LED2 = 0;
					if((MI_OK == PcdRead(adr3_0, DATA3_0)) && (MI_OK == PcdRead(adr3_1, DATA3_1)) && (MI_OK == PcdRead(adr3_2, DATA3_2)))
					{
						rc522_clear = 0;
						if(rc522_clear != rc522_clear_campare)
						{
							rc522_clear_campare = rc522_clear;
							LCD_Clear(WHITE);
							LCD_ShowString(10, 30, tftlcd_data.width, tftlcd_data.height, 24, "ID:");
							LCD_ShowString(46, 30, tftlcd_data.width, tftlcd_data.height, 24, (u8*)DATA3_0);
							LCD_ShowString(10, 60, tftlcd_data.width, tftlcd_data.height, 24, "name:");
							LCD_ShowString(70, 60, tftlcd_data.width, tftlcd_data.height, 24, (u8*)DATA3_1);
							LCD_ShowString(10, 90, tftlcd_data.width, tftlcd_data.height, 24, "grade:");
							LCD_ShowString(82, 90, tftlcd_data.width, tftlcd_data.height, 24, (u8*)DATA3_2);
						}
					}
			}
			else 
				LED2 = 1;
			if((PcdAuthState(KEYA, adr4_3, KEY_A, SN) == MI_OK) && (PcdAuthState(KEYB, adr4_3, KEY_B, SN) == MI_OK))
			{
					if((MI_OK == PcdRead(adr4_0, DATA4_0)) && (MI_OK == PcdRead(adr4_1, DATA4_1)))
					{
						rc522_num = DATA4_0[0];
						LCD_ShowString(10, 120, tftlcd_data.width, tftlcd_data.height, 24, "Borrowed Book:");
						if(rc522_num == 0)
						{
							LCD_ShowString(10, 150, tftlcd_data.width, tftlcd_data.height, 24, "No Message");
							rc522_empty_bit = 1;
						}
						else
						{
								if(rc522_empty_bit)
								{
										LCD_Fill(10,150,480,180,WHITE);
										rc522_empty_bit = 0;
								}
								//根据编号寻找书籍
								for(i=0;i<rc522_num;i++)
								{
										LCD_ShowString(10, 150+i*30, tftlcd_data.width, tftlcd_data.height, 24, (u8*)book_id_find(DATA4_1[i])->text);
								}										
						}
					}
			}
			if(USART1_RX_STA & 0x8000) 
			{        
				rc522_book_id = 0;
				len=USART1_RX_STA&0x3fff;						            
				for(t=0;t<len;t++)
				{
					rc522_name[t]=USART1_RX_BUF[t];
					if (rc522_name[t] >= '0' && rc522_name[t] <= '9')
						rc522_book_id = rc522_book_id*10+ (rc522_name[t]-'0');
				}                
				rc522_name[len] = '\0';
				USART1_RX_STA = 0;  // 清除标志
				if(book_id_find(rc522_book_id)->id != 0)//判断书库是否有这本书
				{                                                                  
					rc522_bit = 1;
					LCD_ShowString(50, 750, tftlcd_data.width, tftlcd_data.height, 24, "OK"); //有
				}
				else
					LCD_ShowString(50, 750, tftlcd_data.width, tftlcd_data.height, 24, "ERROE");//没有
			}
			
		}
			if(type)
			{
					if(rc522_bit == 1)//借书
					{
					
						if((PcdAuthState(KEYA, adr4_3, KEY_A, SN) == MI_OK) && (PcdAuthState(KEYB, adr4_3, KEY_B, SN) == MI_OK) && 	(book_id_find(rc522_book_id)->state != 0))
						{
							DATA4_1[rc522_num] = rc522_book_id;
						
							DATA4_0[0]++;//卡中书籍增加
							rc522_bit = 0; //仅进入一次
							
							printf("进入成功\r\n");
							if((PcdWrite(adr4_1, DATA4_1) == MI_OK)&& ((PcdWrite(adr4_0, DATA4_0) == MI_OK)))//写卡成功
							{
								book_id_find(rc522_book_id)->state = 0;//将书借走
								printf("借书成功\r\n");
							}
							else		
								printf("借书失败\r\n");
						}
						else
							printf("借书失败\r\n");
				}
			
			
			
			}
			else
			{
					if(rc522_bit == 1)//还书
					{
						if((PcdAuthState(KEYA, adr4_3, KEY_A, SN) == MI_OK) && (PcdAuthState(KEYB, adr4_3, KEY_B, SN) == MI_OK) &&	(book_id_find(rc522_book_id)->state == 0))
						{
							printf("进入成功\r\n");
							for(j=0;j<rc522_num;j++)  //查找是否有这本书
							{
								if(rc522_book_id == DATA4_1[j])
								{
									rc522_state_ok = 1;
									break;
								}
							}
							if(rc522_state_ok == 1)//找到了
							{
								printf("找到了");
								rc522_state_ok = 0;
								for(;DATA4_1[j+1] != 0;j++)  //删除该书籍并将书籍移位
								{
										DATA4_1[j] = DATA4_1[j+1];
										DATA4_1[j+1] = 0x00;
								}
								DATA4_0[0]--;
								rc522_bit = 0;
							//写入
								if((PcdWrite(adr4_1, DATA4_1) == MI_OK)&& ((PcdWrite(adr4_0, DATA4_0) == MI_OK)))//写卡成功
								{
									book_id_find(rc522_book_id)->state = 1;//变为可借状态
									printf("还书成功\r\n");
									LCD_Fill(10,150,480,700,WHITE);
								}
								else
									printf("还书失败\r\n");
							}
							else
								printf("未找到");
						}
					}
			}
		}
}	
void RC522_Init ( void )
{
	SPI1_Init();//初始化引脚
	RC522_Reset_Disable();//复位 （低电平有效） PA0
	RC522_CS_Disable();// 片选 （低电平有效）PA4
  PcdReset();
    
	M500PcdConfigISOType ( 'A' );//设置工作方式
 
}
 

void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    // 使能SPI1和GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置SPI1引脚
    // SCK   PA5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // MISO  PA6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // MOSI  PA7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // CS    PA4
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 初始化SPI1
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工工模式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // 主机模式，STM32控制时钟线
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8位数据帧
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 空闲时为SCK为低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//第一个时钟上升采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//软件片选
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//分频系数
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位先行
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC校验多项式
    SPI_Init(SPI1, &SPI_InitStructure);

    // 使能SPI1
    SPI_Cmd(SPI1, ENABLE);
    
}
 
 
/*
 * 函数名：SPI_RC522_SendByte
 * 描述  ：向RC522发送1 Byte 数据
 * 输入  ：byte，要发送的数据
 * 返回  : RC522返回的数据
 * 调用  ：内部调用
 */
//发送一个字节
void SPI_RC522_SendByte(u8 byte)
{
    // 等待发送缓冲区为空
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // 发送数据
    SPI_I2S_SendData(SPI1, byte);
    //等待接收缓冲区为空(发送和接受绑定的,发送一个数据同时返回一个数据)
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // 读取接受的数据(丢弃)
    SPI_I2S_ReceiveData(SPI1);
}

//读取一个字节
u8 SPI_RC522_ReadByte(void)
{
    // 发送虚拟字节触发时钟信号
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0xFF);
    //等待接受缓冲区
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    //返回接收数据
    return SPI_I2S_ReceiveData(SPI1);
}
 
 
 
/*
 * 函数名：ReadRawRC
 * 描述  ：读RC522寄存器
 * 输入  ：ucAddress，寄存器地址
 * 返回  : 寄存器的当前值
 * 调用  ：内部调用
 */
u8 ReadRawRC ( u8 ucAddress )
{
	u8 ucAddr, ucReturn;
	
	ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80; //读模式
	
	RC522_CS_Enable();//选中
	SPI_RC522_SendByte(ucAddr);
	ucReturn = SPI_RC522_ReadByte();
	RC522_CS_Disable();//关闭
	return ucReturn;
}
 
 
/*
 * 函数名：WriteRawRC
 * 描述  ：写RC522寄存器
 * 输入  ：ucAddress，寄存器地址
 *         ucValue，写入寄存器的值
 * 返回  : 无
 * 调用  ：内部调用
 */
void WriteRawRC ( u8 ucAddress, u8 ucValue )
{  
	u8 ucAddr;
	ucAddr = ( ucAddress << 1 ) & 0x7E;  //写模式
	RC522_CS_Enable();
	SPI_RC522_SendByte ( ucAddr );
	SPI_RC522_SendByte ( ucValue );
	RC522_CS_Disable();	
}
 
 
/*
 * 函数名：SetBitMask
 * 描述  ：对RC522寄存器置位
 * 输入  ：ucReg，寄存器地址
 *         ucMask，置位值
 * 返回  : 无
 * 调用  ：内部调用
 */
void SetBitMask ( u8 ucReg, u8 ucMask )  
{
    u8 ucTemp;
    ucTemp = ReadRawRC ( ucReg );
    WriteRawRC ( ucReg, ucTemp | ucMask );         // set bit mask
}
/*
 * 函数名：ClearBitMask
 * 描述  ：对RC522寄存器清位
 * 输入  ：ucReg，寄存器地址
 *         ucMask，清位值
 * 返回  : 无
 * 调用  ：内部调用
 */
void ClearBitMask ( u8 ucReg, u8 ucMask )  
{
    u8 ucTemp;
 
    ucTemp = ReadRawRC ( ucReg );
	
    WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) );  // clear bit mask
	
	
}
 
 
/*
 * 函数名：PcdAntennaOn
 * 描述  ：开启天线 
 * 输入  ：无
 * 返回  : 无
 * 调用  ：内部调用
 */
void PcdAntennaOn ( void )
{
    u8 uc;
    uc = ReadRawRC( TxControlReg );
    if ( ! ( uc & 0x03 ) )
			SetBitMask(TxControlReg, 0x03);
}
 
 
/*
 * 函数名：PcdAntennaOff
 * 描述  ：关闭天线 
 * 输入  ：无
 * 返回  : 无
 * 调用  ：内部调用
 */
void PcdAntennaOff ( void )
{
    ClearBitMask ( TxControlReg, 0x03 );
}
 
 
/*
 * 函数名：PcdRese
 * 描述  ：复位RC522 
 * 输入  ：无
 * 返回  : 无
 * 调用  ：外部调用
 */
void PcdReset ( void )
{
    RC522_Reset_Disable();
    delay_us ( 1 );
    RC522_Reset_Enable();
    delay_us ( 1 );
    RC522_Reset_Disable();
    delay_us ( 1 );
    WriteRawRC ( CommandReg, 0x0f );
    while ( ReadRawRC ( CommandReg ) & 0x10 );
 
    delay_us ( 1 );
 
    WriteRawRC ( ModeReg, 0x3D );            //定义发送和接收常用模式 和Mifare卡通讯，CRC初始值0x6363
 
    WriteRawRC ( TReloadRegL, 30 );          //16位定时器低位    
    WriteRawRC ( TReloadRegH, 0 );			     //16位定时器高位
 
    WriteRawRC ( TModeReg, 0x8D );				   //定义内部定时器的设置
 
    WriteRawRC ( TPrescalerReg, 0x3E );			 //设置定时器分频系数
 
    WriteRawRC ( TxAutoReg, 0x40 );				   //调制发送信号为100%ASK	
	
 
}
 
 
/*
 * 函数名：M500PcdConfigISOType
 * 描述  ：设置RC522的工作方式
 * 输入  ：ucType，工作方式
 * 返回  : 无
 * 调用  ：外部调用
 */
void M500PcdConfigISOType ( u8 ucType )
{
	if ( ucType == 'A')                     //ISO14443_A
  {
		ClearBitMask ( Status2Reg, 0x08 );
		
    WriteRawRC ( ModeReg, 0x3D );//3F
		
		WriteRawRC ( RxSelReg, 0x86 );//84
		
		WriteRawRC( RFCfgReg, 0x7F );   //4F
		
		WriteRawRC( TReloadRegL, 30 );//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		
		WriteRawRC ( TReloadRegH, 0 );
		
		WriteRawRC ( TModeReg, 0x8D );
		
		WriteRawRC ( TPrescalerReg, 0x3E );
		
		delay_us ( 2 );
		
		PcdAntennaOn ();//开天线
		
   }
 
}
 
 

/*
 * 函数名：PcdComMF522
 * 描述  ：通过RC522和ISO14443卡通讯
 * 输入  ：ucCommand，RC522命令字
 *         pInData，通过RC522发送到卡片的数据
 *         ucInLenByte，发送数据的字节长度
 *         pOutData，接收到的卡片返回数据
 *         pOutLenBit，返回数据的位长度
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：内部调用
 */
char PcdComMF522 ( u8 ucCommand, u8 * pInData, u8 ucInLenByte, u8 * pOutData, u32 * pOutLenBit )
{
    char cStatus = MI_ERR;
    u8 ucIrqEn   = 0x00;
    u8 ucWaitFor = 0x00;
    u8 ucLastBits;
    u8 ucN;
    u32 ul;
 
    switch ( ucCommand )
    {
    case PCD_AUTHENT:		//Mifare认证
        ucIrqEn   = 0x12;		//允许错误中断请求ErrIEn  允许空闲中断IdleIEn
        ucWaitFor = 0x10;		//认证寻卡等待时候 查询空闲中断标志位
        break;
 
    case PCD_TRANSCEIVE:		//接收发送 发送接收
        ucIrqEn   = 0x77;		//允许TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
        ucWaitFor = 0x30;		//寻卡等待时候 查询接收中断标志位与 空闲中断标志位
        break;
 
    default:
        break;
    }
 
    WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 );		//IRqInv置位管脚IRQ与Status1Reg的IRq位的值相反
    ClearBitMask ( ComIrqReg, 0x80 );			//Set1该位清零时，CommIRqReg的屏蔽位清零
    WriteRawRC ( CommandReg, PCD_IDLE );		//写空闲命令
    SetBitMask ( FIFOLevelReg, 0x80 );			//置位FlushBuffer清除内部FIFO的读和写指针以及ErrReg的BufferOvfl标志位被清除
 
    for ( ul = 0; ul < ucInLenByte; ul ++ )
        WriteRawRC ( FIFODataReg, pInData [ ul ] );    		//写数据进FIFOdata
 
    WriteRawRC ( CommandReg, ucCommand );					//写命令
 
    if ( ucCommand == PCD_TRANSCEIVE )
        SetBitMask(BitFramingReg,0x80);  				//StartSend置位启动数据发送 该位与收发命令使用时才有效
 
    ul = 1000;//根据时钟频率调整，操作M1卡最大等待时间25ms
 
    do 														//认证 与寻卡等待时间
    {
        ucN = ReadRawRC ( ComIrqReg );							//查询事件中断
        ul --;
    } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );		//退出条件i=0,定时器中断，与写空闲命令
 
    ClearBitMask ( BitFramingReg, 0x80 );					//清理允许StartSend位
 
    if ( ul != 0 )
    {
        if ( ! (( ReadRawRC ( ErrorReg ) & 0x1B )) )			//读错误标志寄存器BufferOfI CollErr ParityErr ProtocolErr
        {
            cStatus = MI_OK;
            if ( ucN & ucIrqEn & 0x01 ){					//是否发生定时器中断
                cStatus = MI_NOTAGERR;
						
						}
 
            if ( ucCommand == PCD_TRANSCEIVE )
            {
                ucN = ReadRawRC ( FIFOLevelReg );			//读FIFO中保存的字节数
 
                ucLastBits = ReadRawRC ( ControlReg ) & 0x07;	//最后接收到得字节的有效位数
 
                if ( ucLastBits )
                    * pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	//N个字节数减去1（最后一个字节）+最后一位的位数 读取到的数据总位数
                else
                    * pOutLenBit = ucN * 8;   					//最后接收到的字节整个字节有效
 
                if ( ucN == 0 )
                    ucN = 1;
 
                if ( ucN > MAXRLEN )
                    ucN = MAXRLEN;
 
                for ( ul = 0; ul < ucN; ul ++ )
                    pOutData [ ul ] = ReadRawRC ( FIFODataReg );
            }
        }
        else{
            cStatus = MI_ERR;
				}
    }
    SetBitMask ( ControlReg, 0x80 );           // stop timer now
    WriteRawRC ( CommandReg, PCD_IDLE );
	
    return cStatus;
}


 
/*
 * 函数名：PcdRequest
 * 描述  ：寻卡
 * 输入  ：ucReq_code，寻卡方式
 *                     = 0x52，寻感应区内所有符合14443A标准的卡
 *                     = 0x26，寻未进入休眠状态的卡
 *         pTagType，卡片类型代码
 *                   = 0x4400，Mifare_UltraLight
 *                   = 0x0400，Mifare_One(S50)
 *                   = 0x0200，Mifare_One(S70)
 *                   = 0x0800，Mifare_Pro(X))
 *                   = 0x4403，Mifare_DESFire
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdRequest ( u8 ucReq_code, u8 * pTagType )
{
    char cStatus;  
    u8 ucComMF522Buf [ MAXRLEN ]; 
    u32 ulLen;
 
    ClearBitMask ( Status2Reg, 0x08 );	//清理指示MIFARECyptol单元接通以及所有卡的数据通信被加密的情况
    WriteRawRC ( BitFramingReg, 0x07 );	//	发送的最后一个字节的 七位
    SetBitMask ( TxControlReg, 0x03 );	//TX1,TX2管脚的输出信号传递经发送调制的13.56的能量载波信号
 
    ucComMF522Buf [ 0 ] = ucReq_code;		//存入 卡片命令字
 
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE,	ucComMF522Buf, 1, ucComMF522Buf, & ulLen );	//寻卡  
 
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	//寻卡成功返回卡类型 
    {    
       * pTagType = ucComMF522Buf [ 0 ];
       * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
    }
     
    else
     cStatus = MI_ERR;
 
    return cStatus;
 
}
 
 
/*
 * 函数名：PcdAnticoll
 * 描述  ：防冲撞
 * 输入  ：pSnr，卡片序列号，4字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdAnticoll ( u8 * pSnr )
{
    char cStatus;
    u8 uc, ucSnr_check = 0;
    u8 ucComMF522Buf [ MAXRLEN ]; 
	u32 ulLen;
 
    ClearBitMask ( Status2Reg, 0x08 );		//清MFCryptol On位 只有成功执行MFAuthent命令后，该位才能置位
    WriteRawRC ( BitFramingReg, 0x00);		//清理寄存器 停止收发
    ClearBitMask ( CollReg, 0x80 );			//清ValuesAfterColl所有接收的位在冲突后被清除
   
    ucComMF522Buf [ 0 ] = 0x93;	//卡片防冲突命令
    ucComMF522Buf [ 1 ] = 0x20;
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, & ulLen);//与卡片通信
	
    if ( cStatus == MI_OK)		//通信成功
    {
		for ( uc = 0; uc < 4; uc ++ )
        {
            * ( pSnr + uc )  = ucComMF522Buf [ uc ];			//读出UID
            ucSnr_check ^= ucComMF522Buf [ uc ];
        }
			
        if ( ucSnr_check != ucComMF522Buf [ uc ] )
        		cStatus = MI_ERR;    
				 
    }
    
    SetBitMask ( CollReg, 0x80 );
 
    return cStatus;
	
}
 
 
/*
 * 函数名：CalulateCRC
 * 描述  ：用RC522计算CRC16
 * 输入  ：pIndata，计算CRC16的数组
 *         ucLen，计算CRC16的数组字节长度
 *         pOutData，存放计算结果存放的首地址
 * 返回  : 无
 * 调用  ：内部调用
 */
void CalulateCRC ( u8 * pIndata, u8 ucLen, u8 * pOutData )
{
    u8 uc, ucN;
 
    ClearBitMask(DivIrqReg,0x04);
	
    WriteRawRC(CommandReg,PCD_IDLE);
	
    SetBitMask(FIFOLevelReg,0x80);
	
    for ( uc = 0; uc < ucLen; uc ++)
	    WriteRawRC ( FIFODataReg, * ( pIndata + uc ) );   
 
    WriteRawRC ( CommandReg, PCD_CALCCRC );
	
    uc = 0xFF;
	
    do 
    {
        ucN = ReadRawRC ( DivIrqReg );
        uc --;
    } while ( ( uc != 0 ) && ! ( ucN & 0x04 ) );
		
    pOutData [ 0 ] = ReadRawRC ( CRCResultRegL );
    pOutData [ 1 ] = ReadRawRC ( CRCResultRegM );
	
}
 
 
/*
 * 函数名：PcdSelect
 * 描述  ：选定卡片
 * 输入  ：pSnr，卡片序列号，4字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdSelect ( u8 * pSnr )
{
    char ucN;
    u8 uc;
	  u8 ucComMF522Buf [ MAXRLEN ]; 
    u32  ulLen;
 
    ucComMF522Buf [ 0 ] = PICC_ANTICOLL1;
    ucComMF522Buf [ 1 ] = 0x70;
    ucComMF522Buf [ 6 ] = 0;
	
    for ( uc = 0; uc < 4; uc ++ )
    {
    	ucComMF522Buf [ uc + 2 ] = * ( pSnr + uc );
    	ucComMF522Buf [ 6 ] ^= * ( pSnr + uc );
    }
		
    CalulateCRC ( ucComMF522Buf, 7, & ucComMF522Buf [ 7 ] );
  
    ClearBitMask ( Status2Reg, 0x08 );
 
    ucN = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 9, ucComMF522Buf, & ulLen );
    
// if ( ( ucN == MI_OK ) && ( ulLen == 0x18 ) ){
//      ucN = MI_OK;
//			printf("选卡成功\r\n");}
//    else{
//      ucN = MI_ERR;
//      printf("选卡失败\r\n");    
//		}
    return ucN;
	
}
 
 
/*
 * 函数名：PcdAuthState
 * 描述  ：验证卡片密码
 * 输入  ：ucAuth_mode，密码验证模式
 *                     = 0x60，验证A密钥
 *                     = 0x61，验证B密钥
 *         u8 ucAddr，块地址
 *         pKey，密码
 *         pSnr，卡片序列号，4字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdAuthState ( u8 ucAuth_mode, u8 ucAddr, u8 * pKey, u8 * pSnr )
{
    char cStatus;
	  u8 uc, ucComMF522Buf [ MAXRLEN ];
    u32 ulLen;
 
    ucComMF522Buf [ 0 ] = ucAuth_mode;
    ucComMF522Buf [ 1 ] = ucAddr;
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 2 ] = * ( pKey + uc );   
	
    for ( uc = 0; uc < 6; uc ++ )
	    ucComMF522Buf [ uc + 8 ] = * ( pSnr + uc );   
 
    cStatus = PcdComMF522 ( PCD_AUTHENT, ucComMF522Buf, 12, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus != MI_OK ) || ( ! ( ReadRawRC ( Status2Reg ) & 0x08 ) ) )
			
		{
			cStatus = MI_ERR; 
    }
		
    return cStatus;
		
}
 
 
/*
 * 函数名：PcdWrite
 * 描述  ：写数据到M1卡一块
 * 输入  ：u8 ucAddr，块地址
 *         pData，写入的数据，16字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdWrite ( u8 ucAddr, u8 * pData )
{
    char cStatus;
    u8 uc, ucComMF522Buf [ MAXRLEN ];
    u32 ulLen;
 
    ucComMF522Buf [ 0 ] = PICC_WRITE;
    ucComMF522Buf [ 1 ] = ucAddr;
 
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
 
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
 
    if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
        cStatus = MI_ERR;
 
    if ( cStatus == MI_OK )
    {
        memcpy(ucComMF522Buf, pData, 16);
        for ( uc = 0; uc < 16; uc ++ )
            ucComMF522Buf [ uc ] = * ( pData + uc );
 
        CalulateCRC ( ucComMF522Buf, 16, & ucComMF522Buf [ 16 ] );
 
        cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 18, ucComMF522Buf, & ulLen );
 
        if ( ( cStatus != MI_OK ) || ( ulLen != 4 ) || ( ( ucComMF522Buf [ 0 ] & 0x0F ) != 0x0A ) )
            cStatus = MI_ERR;
 
    }
    return cStatus;
}
 
 
/*
 * 函数名：PcdRead
 * 描述  ：读取M1卡一块数据
 * 输入  ：u8 ucAddr，块地址
 *         pData，读出的数据，16字节
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdRead ( u8 ucAddr, u8 * pData )
{
    char cStatus;
	  u8 uc, ucComMF522Buf [ MAXRLEN ]; 
    u32 ulLen;
 
    ucComMF522Buf [ 0 ] = PICC_READ;
    ucComMF522Buf [ 1 ] = ucAddr;
	  //CRC校验
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
	
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x90 ) )
    {
			for ( uc = 0; uc < 16; uc ++ )
        * ( pData + uc ) = ucComMF522Buf [ uc ];   
    }
		
    else
      cStatus = MI_ERR;   
	
    return cStatus;
 
}
 
 
/*
 * 函数名：PcdHalt
 * 描述  ：命令卡片进入休眠状态
 * 输入  ：无
 * 返回  : 状态值
 *         = MI_OK，成功
 * 调用  ：外部调用
 */
char PcdHalt( void )
{
    u8 ucComMF522Buf [ MAXRLEN ]; 
    u32  ulLen;
 
    ucComMF522Buf [ 0 ] = PICC_HALT;
    ucComMF522Buf [ 1 ] = 0;
 
    CalulateCRC ( ucComMF522Buf, 2, & ucComMF522Buf [ 2 ] );
    	PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 4, ucComMF522Buf, & ulLen );
 
    return MI_OK;
	
}
 // UID为你要修改的卡的UID key_type：0为KEYA，非0为KEYB KEY为密钥 RW:1是读，0是写 data_addr为修改的地址 data为数据内容
void IC_RW ( u8 * UID, u8 key_type, u8 * KEY, u8 RW, u8 data_addr, u8 * data )
{
	char status;
	u8 i = 0;
    u8 ucArray_ID [ 4 ] = { 0 };//先后存放IC卡的类型和UID(IC卡序列号)
 
    status = PcdRequest ( 0x52, ucArray_ID );//寻卡
	if(status == MI_OK)
		ShowID(ucArray_ID);
	else
		return;
 
    status = PcdAnticoll ( ucArray_ID );//防冲撞
	if(status != MI_OK)
		return;
 
    status = PcdSelect ( UID );//选定卡
	if(status != MI_OK)
	{
		printf("UID don't match\r\n");
		return;
	}
		
	if(0 == key_type)
		status = PcdAuthState ( KEYA, data_addr, KEY, UID );//校验
	else
		status = PcdAuthState ( KEYB, data_addr, KEY, UID );//校验
 
	if(status != MI_OK)
	{
		printf("KEY don't match\r\n");
		return;
	}
	
    if ( RW )//读写选择，1是读，0是写
    {
		status = PcdRead ( data_addr, data );
		if(status == MI_OK)
		{
			printf("data:");
			for(i = 0; i < 16; i++)
            {
                printf("%02x", data[i]);
            }
            printf("\r\n");
		}
		else
		{
			printf("PcdRead() failed\r\n");
			return;
		}
	}
    else
		{
        status = PcdWrite ( data_addr, data );
				if(status == MI_OK)
				{
					printf("PcdWrite() finished\r\n");
				}
				else
				{
					printf("PcdWrite() failed\r\n");
					return;
				}
		}
 
    status = PcdHalt(); //休眠状态
		if(status == MI_OK)
		{
			printf("PcdHalt() finished\r\n");
		}
		else
		{
			printf("PcdHalt() failed\r\n");
			return;
		}
}
 
void IC_CMT ( u8 * UID, u8 * KEY, u8 RW, u8 * Dat )
{
    u8 ucArray_ID [ 4 ] = { 0 };//先后存放IC卡的类型和UID(IC卡序列号)
 
    PcdRequest ( 0x52, ucArray_ID );//寻卡
 
    PcdAnticoll ( ucArray_ID );//防冲撞
 
    PcdSelect ( UID );//选定卡
 
    PcdAuthState ( 0x60, 0x10, KEY, UID );//校验
 
    if ( RW )//读写选择，1是读，0是写
        PcdRead ( 0x10, Dat );

    else 
        PcdWrite ( 0x10, Dat );
     
    PcdHalt ();	 
	 
}


void ShowID(u8 *p)	 //显示卡的卡号，以十六进制显示
{
	u8 num[9];
	u8 i;
 
 	for(i=0;i<4;i++)
	{
		num[i*2]=p[i]/16;
		num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
		num[i*2+1]=p[i]%16;
		num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
		//printf("your card id is %s\r\n", num);
	}
	
		sprintf(ss,"%s",num);
	  num[8] = 0;
    printf("your card id is %s\r\n", num);
	
	  printf("ID>>>%s\r\n", num);
 
}
//等待卡离开
void WaitCardOff(void)
{
    char status;
    unsigned char	TagType[2];
 
    while(1)
    {
			
        status = PcdRequest(REQ_ALL, TagType);
        if(status)
        {
            status = PcdRequest(REQ_ALL, TagType);
            if(status)
            {
                status = PcdRequest(REQ_ALL, TagType);
                if(status)
                {
                    return;
                }
            }
        }
        delay_ms(1000);
    }
}
//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//          addr[IN]：钱包地址
//          pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
 
//status = PcdValue(PICC_INCREMENT, adr3_0, Add_Money);

char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4)|| ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
			status = MI_ERR;
		}
        
    if (status == MI_OK)
    {
       // memcpy(ucComMF522Buf, pValue, 4);
        for (i=0; i<16; i++)
        {    
					ucComMF522Buf[i] = *(pValue+i);   
				}
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {  
					status = MI_OK;    
				}
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
				if ((status != MI_OK) || (unLen != 4)|| ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   
		
					status = MI_ERR;   
				}
    }
		
    return status;

}
