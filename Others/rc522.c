#include "rc522.h"

// M1����Ϊ16��������ÿ���������ĸ��飨��0����1����2����3�����
// ��16��������64���鰴���Ե�ַ���Ϊ��0~63
// ��0�������Ŀ�0�������Ե�ַ0�飩�����ڴ�ų��̴��룬�Ѿ��̻����ɸ��� 
// ÿ�������Ŀ�0����1����2Ϊ���ݿ飬�����ڴ������
// ÿ�������Ŀ�3Ϊ���ƿ飨���Ե�ַΪ:��3����7����11.....����������A����ȡ���ơ�����B��
 
/*******************************
*����˵����
*1--SDA  <----->PA4
*2--SCK  <----->PA5
*3--MOSI <----->PA7
*4--MISO <----->PA6
*5--����
*6--GND <----->GND
*7--RST <----->PA0
*8--VCC <----->VCC
************************************/
 
#define   RC522_DELAY()  delay_us(50)

unsigned char CT[2];//������
unsigned char SN[4]; //����

u8 KEY_A[6]= {0xff,0xff,0xff,0xff,0xff,0xff};
u8 KEY_B[6]= {0xff,0xff,0xff,0xff,0xff,0xff};

//������Ϣ
unsigned char adr3_0=0x0C;//	��3����0����(��13��)  ѧ��
unsigned char adr3_1=0x0D;//	��3����1����(��14��)   ����
unsigned char adr3_2=0x0E;//	��3����2����(��15��)   �꼶
unsigned char adr3_3=0x0F;//	��3����2����(��16��)

unsigned char DATA3_0[16]; //��
unsigned char DATA3_1[16]; //��
unsigned char DATA3_2[16]; //��

//�����鼮  �����Ӧ�鼮�ı��
unsigned char adr4_0=0x10;//	��4����1����(��17��)
unsigned char adr4_1=0x11;//	��4����2����(��18��)
unsigned char adr4_3=0x13;//	��4����2����(��20��)

unsigned char DATA4_0[16]; //��
unsigned char DATA4_1[16]; //��

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
	if(PcdRequest(PICC_REQALL,CT) != MI_OK)// ��һ��Ѱ��ʧ��
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
				if(PcdAnticoll(SN) == MI_OK);// ����ײ ���UID ����SN
					rc522_status = 1;
				LED1 = 0;
			}
	}
	else
	{
			if(PcdAnticoll(SN) == MI_OK);// ����ײ ���UID ����SN
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
								//���ݱ��Ѱ���鼮
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
				USART1_RX_STA = 0;  // �����־
				if(book_id_find(rc522_book_id)->id != 0)//�ж�����Ƿ����Ȿ��
				{                                                                  
					rc522_bit = 1;
					LCD_ShowString(50, 750, tftlcd_data.width, tftlcd_data.height, 24, "OK"); //��
				}
				else
					LCD_ShowString(50, 750, tftlcd_data.width, tftlcd_data.height, 24, "ERROE");//û��
			}
			
		}
			if(type)
			{
					if(rc522_bit == 1)//����
					{
					
						if((PcdAuthState(KEYA, adr4_3, KEY_A, SN) == MI_OK) && (PcdAuthState(KEYB, adr4_3, KEY_B, SN) == MI_OK) && 	(book_id_find(rc522_book_id)->state != 0))
						{
							DATA4_1[rc522_num] = rc522_book_id;
						
							DATA4_0[0]++;//�����鼮����
							rc522_bit = 0; //������һ��
							
							printf("����ɹ�\r\n");
							if((PcdWrite(adr4_1, DATA4_1) == MI_OK)&& ((PcdWrite(adr4_0, DATA4_0) == MI_OK)))//д���ɹ�
							{
								book_id_find(rc522_book_id)->state = 0;//�������
								printf("����ɹ�\r\n");
							}
							else		
								printf("����ʧ��\r\n");
						}
						else
							printf("����ʧ��\r\n");
				}
			
			
			
			}
			else
			{
					if(rc522_bit == 1)//����
					{
						if((PcdAuthState(KEYA, adr4_3, KEY_A, SN) == MI_OK) && (PcdAuthState(KEYB, adr4_3, KEY_B, SN) == MI_OK) &&	(book_id_find(rc522_book_id)->state == 0))
						{
							printf("����ɹ�\r\n");
							for(j=0;j<rc522_num;j++)  //�����Ƿ����Ȿ��
							{
								if(rc522_book_id == DATA4_1[j])
								{
									rc522_state_ok = 1;
									break;
								}
							}
							if(rc522_state_ok == 1)//�ҵ���
							{
								printf("�ҵ���");
								rc522_state_ok = 0;
								for(;DATA4_1[j+1] != 0;j++)  //ɾ�����鼮�����鼮��λ
								{
										DATA4_1[j] = DATA4_1[j+1];
										DATA4_1[j+1] = 0x00;
								}
								DATA4_0[0]--;
								rc522_bit = 0;
							//д��
								if((PcdWrite(adr4_1, DATA4_1) == MI_OK)&& ((PcdWrite(adr4_0, DATA4_0) == MI_OK)))//д���ɹ�
								{
									book_id_find(rc522_book_id)->state = 1;//��Ϊ�ɽ�״̬
									printf("����ɹ�\r\n");
									LCD_Fill(10,150,480,700,WHITE);
								}
								else
									printf("����ʧ��\r\n");
							}
							else
								printf("δ�ҵ�");
						}
					}
			}
		}
}	
void RC522_Init ( void )
{
	SPI1_Init();//��ʼ������
	RC522_Reset_Disable();//��λ ���͵�ƽ��Ч�� PA0
	RC522_CS_Disable();// Ƭѡ ���͵�ƽ��Ч��PA4
  PcdReset();
    
	M500PcdConfigISOType ( 'A' );//���ù�����ʽ
 
}
 

void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    // ʹ��SPI1��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_GPIOA, ENABLE);

    // ����SPI1����
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

    // ��ʼ��SPI1
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫����ģʽ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; // ����ģʽ��STM32����ʱ����
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8λ����֡
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ����ʱΪSCKΪ�͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//��һ��ʱ����������
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//���Ƭѡ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//��Ƶϵ��
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ����
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRCУ�����ʽ
    SPI_Init(SPI1, &SPI_InitStructure);

    // ʹ��SPI1
    SPI_Cmd(SPI1, ENABLE);
    
}
 
 
/*
 * ��������SPI_RC522_SendByte
 * ����  ����RC522����1 Byte ����
 * ����  ��byte��Ҫ���͵�����
 * ����  : RC522���ص�����
 * ����  ���ڲ�����
 */
//����һ���ֽ�
void SPI_RC522_SendByte(u8 byte)
{
    // �ȴ����ͻ�����Ϊ��
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // ��������
    SPI_I2S_SendData(SPI1, byte);
    //�ȴ����ջ�����Ϊ��(���ͺͽ��ܰ󶨵�,����һ������ͬʱ����һ������)
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // ��ȡ���ܵ�����(����)
    SPI_I2S_ReceiveData(SPI1);
}

//��ȡһ���ֽ�
u8 SPI_RC522_ReadByte(void)
{
    // ���������ֽڴ���ʱ���ź�
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, 0xFF);
    //�ȴ����ܻ�����
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    //���ؽ�������
    return SPI_I2S_ReceiveData(SPI1);
}
 
 
 
/*
 * ��������ReadRawRC
 * ����  ����RC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 * ����  : �Ĵ����ĵ�ǰֵ
 * ����  ���ڲ�����
 */
u8 ReadRawRC ( u8 ucAddress )
{
	u8 ucAddr, ucReturn;
	
	ucAddr = ( ( ucAddress << 1 ) & 0x7E ) | 0x80; //��ģʽ
	
	RC522_CS_Enable();//ѡ��
	SPI_RC522_SendByte(ucAddr);
	ucReturn = SPI_RC522_ReadByte();
	RC522_CS_Disable();//�ر�
	return ucReturn;
}
 
 
/*
 * ��������WriteRawRC
 * ����  ��дRC522�Ĵ���
 * ����  ��ucAddress���Ĵ�����ַ
 *         ucValue��д��Ĵ�����ֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void WriteRawRC ( u8 ucAddress, u8 ucValue )
{  
	u8 ucAddr;
	ucAddr = ( ucAddress << 1 ) & 0x7E;  //дģʽ
	RC522_CS_Enable();
	SPI_RC522_SendByte ( ucAddr );
	SPI_RC522_SendByte ( ucValue );
	RC522_CS_Disable();	
}
 
 
/*
 * ��������SetBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void SetBitMask ( u8 ucReg, u8 ucMask )  
{
    u8 ucTemp;
    ucTemp = ReadRawRC ( ucReg );
    WriteRawRC ( ucReg, ucTemp | ucMask );         // set bit mask
}
/*
 * ��������ClearBitMask
 * ����  ����RC522�Ĵ�����λ
 * ����  ��ucReg���Ĵ�����ַ
 *         ucMask����λֵ
 * ����  : ��
 * ����  ���ڲ�����
 */
void ClearBitMask ( u8 ucReg, u8 ucMask )  
{
    u8 ucTemp;
 
    ucTemp = ReadRawRC ( ucReg );
	
    WriteRawRC ( ucReg, ucTemp & ( ~ ucMask) );  // clear bit mask
	
	
}
 
 
/*
 * ��������PcdAntennaOn
 * ����  ���������� 
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����
 */
void PcdAntennaOn ( void )
{
    u8 uc;
    uc = ReadRawRC( TxControlReg );
    if ( ! ( uc & 0x03 ) )
			SetBitMask(TxControlReg, 0x03);
}
 
 
/*
 * ��������PcdAntennaOff
 * ����  ���ر����� 
 * ����  ����
 * ����  : ��
 * ����  ���ڲ�����
 */
void PcdAntennaOff ( void )
{
    ClearBitMask ( TxControlReg, 0x03 );
}
 
 
/*
 * ��������PcdRese
 * ����  ����λRC522 
 * ����  ����
 * ����  : ��
 * ����  ���ⲿ����
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
 
    WriteRawRC ( ModeReg, 0x3D );            //���巢�ͺͽ��ճ���ģʽ ��Mifare��ͨѶ��CRC��ʼֵ0x6363
 
    WriteRawRC ( TReloadRegL, 30 );          //16λ��ʱ����λ    
    WriteRawRC ( TReloadRegH, 0 );			     //16λ��ʱ����λ
 
    WriteRawRC ( TModeReg, 0x8D );				   //�����ڲ���ʱ��������
 
    WriteRawRC ( TPrescalerReg, 0x3E );			 //���ö�ʱ����Ƶϵ��
 
    WriteRawRC ( TxAutoReg, 0x40 );				   //���Ʒ����ź�Ϊ100%ASK	
	
 
}
 
 
/*
 * ��������M500PcdConfigISOType
 * ����  ������RC522�Ĺ�����ʽ
 * ����  ��ucType��������ʽ
 * ����  : ��
 * ����  ���ⲿ����
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
		
		PcdAntennaOn ();//������
		
   }
 
}
 
 

/*
 * ��������PcdComMF522
 * ����  ��ͨ��RC522��ISO14443��ͨѶ
 * ����  ��ucCommand��RC522������
 *         pInData��ͨ��RC522���͵���Ƭ������
 *         ucInLenByte���������ݵ��ֽڳ���
 *         pOutData�����յ��Ŀ�Ƭ��������
 *         pOutLenBit���������ݵ�λ����
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ڲ�����
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
    case PCD_AUTHENT:		//Mifare��֤
        ucIrqEn   = 0x12;		//��������ж�����ErrIEn  ��������ж�IdleIEn
        ucWaitFor = 0x10;		//��֤Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ
        break;
 
    case PCD_TRANSCEIVE:		//���շ��� ���ͽ���
        ucIrqEn   = 0x77;		//����TxIEn RxIEn IdleIEn LoAlertIEn ErrIEn TimerIEn
        ucWaitFor = 0x30;		//Ѱ���ȴ�ʱ�� ��ѯ�����жϱ�־λ�� �����жϱ�־λ
        break;
 
    default:
        break;
    }
 
    WriteRawRC ( ComIEnReg, ucIrqEn | 0x80 );		//IRqInv��λ�ܽ�IRQ��Status1Reg��IRqλ��ֵ�෴
    ClearBitMask ( ComIrqReg, 0x80 );			//Set1��λ����ʱ��CommIRqReg������λ����
    WriteRawRC ( CommandReg, PCD_IDLE );		//д��������
    SetBitMask ( FIFOLevelReg, 0x80 );			//��λFlushBuffer����ڲ�FIFO�Ķ���дָ���Լ�ErrReg��BufferOvfl��־λ�����
 
    for ( ul = 0; ul < ucInLenByte; ul ++ )
        WriteRawRC ( FIFODataReg, pInData [ ul ] );    		//д���ݽ�FIFOdata
 
    WriteRawRC ( CommandReg, ucCommand );					//д����
 
    if ( ucCommand == PCD_TRANSCEIVE )
        SetBitMask(BitFramingReg,0x80);  				//StartSend��λ�������ݷ��� ��λ���շ�����ʹ��ʱ����Ч
 
    ul = 1000;//����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms
 
    do 														//��֤ ��Ѱ���ȴ�ʱ��
    {
        ucN = ReadRawRC ( ComIrqReg );							//��ѯ�¼��ж�
        ul --;
    } while ( ( ul != 0 ) && ( ! ( ucN & 0x01 ) ) && ( ! ( ucN & ucWaitFor ) ) );		//�˳�����i=0,��ʱ���жϣ���д��������
 
    ClearBitMask ( BitFramingReg, 0x80 );					//��������StartSendλ
 
    if ( ul != 0 )
    {
        if ( ! (( ReadRawRC ( ErrorReg ) & 0x1B )) )			//�������־�Ĵ���BufferOfI CollErr ParityErr ProtocolErr
        {
            cStatus = MI_OK;
            if ( ucN & ucIrqEn & 0x01 ){					//�Ƿ�����ʱ���ж�
                cStatus = MI_NOTAGERR;
						
						}
 
            if ( ucCommand == PCD_TRANSCEIVE )
            {
                ucN = ReadRawRC ( FIFOLevelReg );			//��FIFO�б�����ֽ���
 
                ucLastBits = ReadRawRC ( ControlReg ) & 0x07;	//�����յ����ֽڵ���Чλ��
 
                if ( ucLastBits )
                    * pOutLenBit = ( ucN - 1 ) * 8 + ucLastBits;   	//N���ֽ�����ȥ1�����һ���ֽڣ�+���һλ��λ�� ��ȡ����������λ��
                else
                    * pOutLenBit = ucN * 8;   					//�����յ����ֽ������ֽ���Ч
 
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
 * ��������PcdRequest
 * ����  ��Ѱ��
 * ����  ��ucReq_code��Ѱ����ʽ
 *                     = 0x52��Ѱ��Ӧ�������з���14443A��׼�Ŀ�
 *                     = 0x26��Ѱδ��������״̬�Ŀ�
 *         pTagType����Ƭ���ʹ���
 *                   = 0x4400��Mifare_UltraLight
 *                   = 0x0400��Mifare_One(S50)
 *                   = 0x0200��Mifare_One(S70)
 *                   = 0x0800��Mifare_Pro(X))
 *                   = 0x4403��Mifare_DESFire
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdRequest ( u8 ucReq_code, u8 * pTagType )
{
    char cStatus;  
    u8 ucComMF522Buf [ MAXRLEN ]; 
    u32 ulLen;
 
    ClearBitMask ( Status2Reg, 0x08 );	//����ָʾMIFARECyptol��Ԫ��ͨ�Լ����п�������ͨ�ű����ܵ����
    WriteRawRC ( BitFramingReg, 0x07 );	//	���͵����һ���ֽڵ� ��λ
    SetBitMask ( TxControlReg, 0x03 );	//TX1,TX2�ܽŵ�����źŴ��ݾ����͵��Ƶ�13.56�������ز��ź�
 
    ucComMF522Buf [ 0 ] = ucReq_code;		//���� ��Ƭ������
 
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE,	ucComMF522Buf, 1, ucComMF522Buf, & ulLen );	//Ѱ��  
 
    if ( ( cStatus == MI_OK ) && ( ulLen == 0x10 ) )	//Ѱ���ɹ����ؿ����� 
    {    
       * pTagType = ucComMF522Buf [ 0 ];
       * ( pTagType + 1 ) = ucComMF522Buf [ 1 ];
    }
     
    else
     cStatus = MI_ERR;
 
    return cStatus;
 
}
 
 
/*
 * ��������PcdAnticoll
 * ����  ������ײ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdAnticoll ( u8 * pSnr )
{
    char cStatus;
    u8 uc, ucSnr_check = 0;
    u8 ucComMF522Buf [ MAXRLEN ]; 
	u32 ulLen;
 
    ClearBitMask ( Status2Reg, 0x08 );		//��MFCryptol Onλ ֻ�гɹ�ִ��MFAuthent����󣬸�λ������λ
    WriteRawRC ( BitFramingReg, 0x00);		//����Ĵ��� ֹͣ�շ�
    ClearBitMask ( CollReg, 0x80 );			//��ValuesAfterColl���н��յ�λ�ڳ�ͻ�����
   
    ucComMF522Buf [ 0 ] = 0x93;	//��Ƭ����ͻ����
    ucComMF522Buf [ 1 ] = 0x20;
   
    cStatus = PcdComMF522 ( PCD_TRANSCEIVE, ucComMF522Buf, 2, ucComMF522Buf, & ulLen);//�뿨Ƭͨ��
	
    if ( cStatus == MI_OK)		//ͨ�ųɹ�
    {
		for ( uc = 0; uc < 4; uc ++ )
        {
            * ( pSnr + uc )  = ucComMF522Buf [ uc ];			//����UID
            ucSnr_check ^= ucComMF522Buf [ uc ];
        }
			
        if ( ucSnr_check != ucComMF522Buf [ uc ] )
        		cStatus = MI_ERR;    
				 
    }
    
    SetBitMask ( CollReg, 0x80 );
 
    return cStatus;
	
}
 
 
/*
 * ��������CalulateCRC
 * ����  ����RC522����CRC16
 * ����  ��pIndata������CRC16������
 *         ucLen������CRC16�������ֽڳ���
 *         pOutData����ż�������ŵ��׵�ַ
 * ����  : ��
 * ����  ���ڲ�����
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
 * ��������PcdSelect
 * ����  ��ѡ����Ƭ
 * ����  ��pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
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
//			printf("ѡ���ɹ�\r\n");}
//    else{
//      ucN = MI_ERR;
//      printf("ѡ��ʧ��\r\n");    
//		}
    return ucN;
	
}
 
 
/*
 * ��������PcdAuthState
 * ����  ����֤��Ƭ����
 * ����  ��ucAuth_mode��������֤ģʽ
 *                     = 0x60����֤A��Կ
 *                     = 0x61����֤B��Կ
 *         u8 ucAddr�����ַ
 *         pKey������
 *         pSnr����Ƭ���кţ�4�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
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
 * ��������PcdWrite
 * ����  ��д���ݵ�M1��һ��
 * ����  ��u8 ucAddr�����ַ
 *         pData��д������ݣ�16�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
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
 * ��������PcdRead
 * ����  ����ȡM1��һ������
 * ����  ��u8 ucAddr�����ַ
 *         pData�����������ݣ�16�ֽ�
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
 */
char PcdRead ( u8 ucAddr, u8 * pData )
{
    char cStatus;
	  u8 uc, ucComMF522Buf [ MAXRLEN ]; 
    u32 ulLen;
 
    ucComMF522Buf [ 0 ] = PICC_READ;
    ucComMF522Buf [ 1 ] = ucAddr;
	  //CRCУ��
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
 * ��������PcdHalt
 * ����  �����Ƭ��������״̬
 * ����  ����
 * ����  : ״ֵ̬
 *         = MI_OK���ɹ�
 * ����  ���ⲿ����
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
 // UIDΪ��Ҫ�޸ĵĿ���UID key_type��0ΪKEYA����0ΪKEYB KEYΪ��Կ RW:1�Ƕ���0��д data_addrΪ�޸ĵĵ�ַ dataΪ��������
void IC_RW ( u8 * UID, u8 key_type, u8 * KEY, u8 RW, u8 data_addr, u8 * data )
{
	char status;
	u8 i = 0;
    u8 ucArray_ID [ 4 ] = { 0 };//�Ⱥ���IC�������ͺ�UID(IC�����к�)
 
    status = PcdRequest ( 0x52, ucArray_ID );//Ѱ��
	if(status == MI_OK)
		ShowID(ucArray_ID);
	else
		return;
 
    status = PcdAnticoll ( ucArray_ID );//����ײ
	if(status != MI_OK)
		return;
 
    status = PcdSelect ( UID );//ѡ����
	if(status != MI_OK)
	{
		printf("UID don't match\r\n");
		return;
	}
		
	if(0 == key_type)
		status = PcdAuthState ( KEYA, data_addr, KEY, UID );//У��
	else
		status = PcdAuthState ( KEYB, data_addr, KEY, UID );//У��
 
	if(status != MI_OK)
	{
		printf("KEY don't match\r\n");
		return;
	}
	
    if ( RW )//��дѡ��1�Ƕ���0��д
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
 
    status = PcdHalt(); //����״̬
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
    u8 ucArray_ID [ 4 ] = { 0 };//�Ⱥ���IC�������ͺ�UID(IC�����к�)
 
    PcdRequest ( 0x52, ucArray_ID );//Ѱ��
 
    PcdAnticoll ( ucArray_ID );//����ײ
 
    PcdSelect ( UID );//ѡ����
 
    PcdAuthState ( 0x60, 0x10, KEY, UID );//У��
 
    if ( RW )//��дѡ��1�Ƕ���0��д
        PcdRead ( 0x10, Dat );

    else 
        PcdWrite ( 0x10, Dat );
     
    PcdHalt ();	 
	 
}


void ShowID(u8 *p)	 //��ʾ���Ŀ��ţ���ʮ��������ʾ
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
//�ȴ����뿪
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
//��    �ܣ��ۿ�ͳ�ֵ
//����˵��: dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//          addr[IN]��Ǯ����ַ
//          pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
//��    ��: �ɹ�����MI_OK
 
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
