/*************************************************************************
	filename	LCD.c
	designer	��չ��
	date		2004/12/31
*************************************************************************/
#include "DSP281x_Device.h"
#include "LCD.h"

#ifndef bool
#define bool unsigned short
#define FALSE 0
#define TRUE  1
#endif

/***************************************************************************
*	����:	void wr_data(unsigned int data)
*	Ŀ�ģ�	дLCD���ݲ���,�ж�0��1λ
*	���룺	dat1 ������Ԫ
*	�����	��
*	������	status�ֲ������������洢LCD��״̬��
***************************************************************************/

void wr_data(unsigned int dat1)
{
	unsigned int status;
	do
	{	
		status = *c_addr & 0x03;		/* ����status��2~15λΪ0 */
	}while(status != 0x03);
	*d_addr = dat1;
}

/***************************************************************************
*	����:	void wr_data1(unsigned int dat1)
*	Ŀ�ģ�	дLCD���ݲ���,�ж�3λ
*	���룺	dat1 ������Ԫ
*	�����	��
*	������	status�ֲ������������洢LCD��״̬��
***************************************************************************/

void wr_data1(unsigned int dat1)
{
	unsigned int status;
	do
	{
		status = *c_addr & 0x08;		/* ����status��0~2��3~15λΪ0 */
	}while(status != 0x08);
	*d_addr = dat1;
}

/***************************************************************************
*	����:	void wr_com(WORD com)
*	Ŀ�ģ�	дLCDָ�����
*	���룺	com ָ�Ԫ
*	�����	��
*	������	status�ֲ������������洢LCD��״̬��
***************************************************************************/

void wr_com(unsigned int com)
{
	unsigned int status;
	do
	{	
		status = *c_addr & 0x03;
	}while(status != 0x03);
	*c_addr = com;
}

/***************************************************************************
*	����:	extern void GUILCD_init(void)

*	Ŀ�ģ�	��ʼ��LCD��ʾ��������ʾ��ʽΪͼ�η�ʽ������ʾ

*	���룺	��

*	�����	��
****************************************************************************/

extern void GUILCD_init(void)
{
	wr_data(0x00);		/*����ͼ����ʾ�����׵�ַ*/	
	wr_data(0x00);		/*��Ϊ�ı����������׵�ַ*/
	wr_com(0x42);
	
	wr_data(0x20);		/*����ͼ����ʾ�������*/
	wr_data(0x00);		/*��Ϊ�ı������������*/
	wr_com(0x43);
	
	wr_com(0xa0);		/*�����״����*/
	
	wr_com(0x81);		/*��ʾ��ʽ���ã��߼���ϳ�*/
	
	wr_com(0x9b);		/*��ʾ�������ã����ı�����ʾ*/
	
}

/***************************************************************************
*	����:	extern void GUILCD_clear(void)

*	Ŀ�ģ�	��LCD�������Զ���ʽ����LCD����Ϊ����

*	���룺	��

*	�����	��

	������	page0�ֲ�����
***************************************************************************/

extern void GUILCD_clear(void)
{
	int page0;

	wr_data(0x00);		/*������ʾRAM�׵�ַ*/	
	wr_data(0x00);
	wr_com(0x24);
	
	wr_com(0xb0);		/*�����Զ�д��ʽ*/
	
	for(page0 = 0x2000; page0 >= 0; page0--)
	{
		wr_data1(0x00);		/* ��0 */
	}
	
	wr_com(0xb2);		/* �Զ�д���� */
}




/***************************************************************************
*	����:	void wr_hex(unsigned int code,unsigned int o_y,unsigned int o_x,bool fanxian)
*	Ŀ�ģ�	д���֣���������λ�����е���д��X��Yλ�ã���������Ϊ����
*	���룺	code	������λ����
		o_y	y���꣬��Χ0~7
		o_x	x���꣬��Χ0~14
		fanxian ���� 0���� 1������
*	�����	��
***************************************************************************/
void wr_hex(unsigned int code,unsigned int o_y,unsigned int o_x,unsigned short fanxian)
{
	unsigned int dat1_temp,dat2_temp;
	unsigned int *code_temp;
	int i1,i2,code_temp1, index;
	int count = 2;
	unsigned int zimu_conv[32];
	unsigned int hex_code[32];
	

	for(i1 = 0; i1 <32; i1++)
	{
		zimu_conv[i1] = zimu[32*code + i1];
	}
	
	for(i2 = 0; i2 < 32; i2++)
	{
		if(fanxian == FALSE)			/* �Ƿ��� */
		{
			hex_code[i2] = zimu_conv[i2];
		}
		else
		{
			hex_code[i2] = (~zimu_conv[i2]) & 0xff;
		}
	}
	
	i1 = o_y * 0x20;
	i1 = i1 + o_x;
	dat1_temp = i1 & 0xff;
	dat1_temp = dat1_temp ;
	dat2_temp = (i1>>8) & 0xff;
	dat2_temp = dat2_temp;
	code_temp = &hex_code[0];
	
	for(i2 = 0; i2 < 16 ; i2++)
	{
		wr_data(dat1_temp);			/* ���ֵ�����LCD�е�λ�� */
		wr_data(dat2_temp);
		wr_com(0x24);
			
		for (index = 0; index < count; ++index) {
			wr_data(*(code_temp+i2*2+index));
			wr_com(0xc0);
		}
		
		//code_temp++;
		i1 = i1 + 0x20;
		dat1_temp = i1 & 0xff;
		dat2_temp = (i1>>8) & 0xff;		/* �޸ĺ��ֵ�����LCD�е�λ�� */
	}
}

void wr_hex2(unsigned int code,unsigned int o_y,unsigned int o_x,unsigned short fanxian)
/**   
 *  @brief ��ʾͼ��
 * 
 *  @param ͼ�����
 *  @param ������
 *  @param ������
 *  @param ����ʹ��
 */  
{
	unsigned int dat1_temp,dat2_temp;
	unsigned int *code_temp;
	int i1,i2,code_temp1,index;
	unsigned int zimu_conv[32*30];
	unsigned int hex_code[32*30];


	for(i1 = 0; i1 <16*30; i1++)
	{
		zimu_conv[i1] = zimu[32*code + i1];
	}

	for(i2 = 0; i2 < 16*30; i2++)
	{
		if(fanxian == FALSE)			/* �Ƿ��� */
		{
			hex_code[i2] = zimu_conv[i2];
		}
		else
		{
			hex_code[i2] = (~zimu_conv[i2]) & 0xff;
		}
	}

	i1 = o_y *16*2;
	i1 = i1 + o_x;
	dat1_temp = i1 & 0xff;
	dat1_temp = dat1_temp ;
	dat2_temp = (i1>>8) & 0xff;
	dat2_temp = dat2_temp;
	code_temp = &hex_code[0];
	for(i2 = 0; i2 < 16*30/8 ; i2++)
	{
		wr_data(dat1_temp);			/* ���ֵ�����LCD�е�λ�� */
		wr_data(dat2_temp);
		wr_com(0x24);

		for(index=0;index<8;index++){
		wr_data(*(code_temp+i2*8+index));			/* д�뺺�ֵ��� */
		wr_com(0xc0);
		}

		i1 = i1 + 0x20;
		dat1_temp = i1 & 0xff;
		dat2_temp = (i1>>8) & 0xff;		/* �޸ĺ��ֵ�����LCD�е�λ�� */
	}
}

/***************************************************************************
*	����:	extern void GUILCD_writeCharStr(unsigned int Row, unsigned int Column, unsigned char *cString ,bool fanxian)
*	Ŀ�ģ�	д�����ַ��������������ݵ��ַ�������LCD����ROW�к�COLUMN�е�λ����ʾ
			�Զ�д����ֱ���ַ���β���ж�Ϊ0ֹͣ�����ݱ���fanxi�Ƿ�Ϊ0��
			������ǰ�ַ����Ƿ��ԡ���ʾλ�ô�LCD����ROW�к�COLUMN�е�λ�ÿ�ʼ��
			Ϊ����ʾ����ROW���䣬COLUMN��1�仯��
*	���룺	//string	�����ַ���
		location	������hanzi[]�е�λ��
		ROW	�����У���Χ(0��7),�����ַ�����ʼYλ��
		COLUMN	�����У���Χ(0��14)�������ַ�����ʼXλ��
		fanxian ���� 0���� ��0������
*	�����	��
***************************************************************************/
extern void GUILCD_writeCharStr(unsigned int Row, unsigned int Column, unsigned int location ,unsigned short fanxian)
{
	unsigned int ii1,ii4;
	ii4 = 0;
	//ii1 = *cString;
	//while(ii1 != 0)				/* �ж��ַ����Ƿ���� */
	{
		wr_hex(location,Row*0x10,Column*2,fanxian);	/* д���� */
		//Column++;				/* ��λ��+1 */
		//ii4++;
		//ii1 = *(cString + ii4);		/* ���ַ����ڵ�ֵ */
	}
}


extern void GUILCD_writeCharStr2(unsigned int Row, unsigned int Column, unsigned int location ,unsigned short fanxian)
{
	unsigned int ii1,ii4;
	ii4 = 0;
	//ii1 = *cString;
	//while(ii1 != 0)				/* �ж��ַ����Ƿ���� */
	{
		wr_hex2(location,Row*0x30,Column*2,fanxian);	/* д���� */
		//Column++;				/* ��λ��+1 */
		//ii4++;
		//ii1 = *(cString + ii4);		/* ���ַ����ڵ�ֵ */
	}
}





/***************************************************************************
*	����:	extern void GUILCD_onLed(void)
*	Ŀ�ģ�	�������
			
*	���룺	��
			
*	�����	��
*	������	��ȫ�ֱ���
***************************************************************************/
extern void GUILCD_onLed(void)
{
//	asm(" ssbx XF");
}

/***************************************************************************
*	����:	extern void GUILCD_offLed(void)
*	Ŀ�ģ�	�ر����
			
*	���룺	��
			
*	�����	��
*	������	��ȫ�ֱ���
***************************************************************************/
extern void GUILCD_offLed(void)
{
//	asm(" rsbx XF");
}


extern void menu_2(void)
{
	GUILCD_writeCharStr(0x00,0x00,10,FALSE);//��ѡ��
	GUILCD_writeCharStr(0x00,0x01,11,FALSE);
	GUILCD_writeCharStr(0x00,0x02,12,FALSE);

	GUILCD_writeCharStr(0x03,0x00,1,FALSE);//1
	GUILCD_writeCharStr(0x03,0x02,13,FALSE);//fir
	GUILCD_writeCharStr(0x03,0x03,14,FALSE);
	GUILCD_writeCharStr(0x04,0x00,2,FALSE);//2
	GUILCD_writeCharStr(0x04,0x02,15,FALSE);//iir
	GUILCD_writeCharStr(0x04,0x03,16,FALSE);
	GUILCD_writeCharStr(0x05,0x00,3,FALSE);//3
	GUILCD_writeCharStr(0x05,0x02,17,FALSE);//fft
	GUILCD_writeCharStr(0x05,0x03,18,FALSE);
	GUILCD_writeCharStr(0x06,0x00,4,FALSE);//4
	GUILCD_writeCharStr(0x06,0x02,55,FALSE);//AD
	GUILCD_writeCharStr(0x06,0x03,56,FALSE);


}

extern void menu_3(void)
{
    GUILCD_writeCharStr(0x00,0x00,10,FALSE);//��ѡ��
	GUILCD_writeCharStr(0x00,0x01,11,FALSE);
	GUILCD_writeCharStr(0x00,0x02,12,FALSE);
	
    GUILCD_writeCharStr(0x02,0x00,1,FALSE);//1
    GUILCD_writeCharStr(0x02,0x02,24,FALSE);//��ͨ
    GUILCD_writeCharStr(0x02,0x03,25,FALSE);
    GUILCD_writeCharStr(0x02,0x05,23,FALSE);//wn
    GUILCD_writeCharStr(0x02,0x06,28,FALSE);//=
    GUILCD_writeCharStr(0x02,0x07,33,FALSE);//0.
    GUILCD_writeCharStr(0x02,0x08,3,FALSE);//3
    
    GUILCD_writeCharStr(0x03,0x00,2,FALSE);//2
    GUILCD_writeCharStr(0x03,0x02,26,FALSE);//��ͨ
    GUILCD_writeCharStr(0x03,0x03,25,FALSE);
    GUILCD_writeCharStr(0x03,0x05,23,FALSE);//wn
    GUILCD_writeCharStr(0x03,0x06,28,FALSE);//=
    GUILCD_writeCharStr(0x03,0x07,33,FALSE);//0.
    GUILCD_writeCharStr(0x03,0x08,2,FALSE);//2
    
    GUILCD_writeCharStr(0x04,0x00,3,FALSE);//3
    GUILCD_writeCharStr(0x04,0x02,27,FALSE);//��ͨ
    GUILCD_writeCharStr(0x04,0x03,25,FALSE);
    GUILCD_writeCharStr(0x04,0x05,23,FALSE);//wn
    GUILCD_writeCharStr(0x04,0x06,28,FALSE);//=
    GUILCD_writeCharStr(0x04,0x07,33,FALSE);//0.
    GUILCD_writeCharStr(0x04,0x08,36,FALSE);//05
    GUILCD_writeCharStr(0x04,0x09,35,FALSE);//--
    GUILCD_writeCharStr(0x04,0x0a,33,FALSE);//0.
    GUILCD_writeCharStr(0x04,0x0b,2,FALSE);//2
}

extern void menu_1(void)
{
    GUILCD_writeCharStr(0x00,0x00,10,FALSE);//��ѡ��
	GUILCD_writeCharStr(0x00,0x01,11,FALSE);
	GUILCD_writeCharStr(0x00,0x02,12,FALSE);
	GUILCD_writeCharStr(0x02,0x00,1,FALSE);//1
    GUILCD_writeCharStr(0x02,0x02,29,FALSE);//fs
    GUILCD_writeCharStr(0x02,0x03,28,FALSE);//=
    GUILCD_writeCharStr(0x02,0x04,30,FALSE);//20
    GUILCD_writeCharStr(0x02,0x05,31,FALSE);//k
    GUILCD_writeCharStr(0x03,0x00,2,FALSE);//2
    GUILCD_writeCharStr(0x03,0x02,29,FALSE);//fs
    GUILCD_writeCharStr(0x03,0x03,28,FALSE);//=
    GUILCD_writeCharStr(0x03,0x04,43,FALSE);//27
    GUILCD_writeCharStr(0x03,0x05,44,FALSE);//.9
    GUILCD_writeCharStr(0x03,0x06,31,FALSE);//k
}





extern void menu_4(void)
{
    GUILCD_writeCharStr(0x00,0x00,19,FALSE);//��
	GUILCD_writeCharStr(0x00,0x01,20,FALSE);//��
	GUILCD_writeCharStr(0x00,0x02,21,FALSE);//ִ
	GUILCD_writeCharStr(0x00,0x03,22,FALSE);//��
	GUILCD_writeCharStr(0x02,0x01,37,FALSE);//��
    GUILCD_writeCharStr(0x02,0x02,6,FALSE);//6
    GUILCD_writeCharStr(0x02,0x03,38,FALSE);//��
    GUILCD_writeCharStr(0x02,0x04,39,FALSE);//��
    GUILCD_writeCharStr(0x02,0x05,40,FALSE);//��
    GUILCD_writeCharStr(0x02,0x06,41,FALSE);//��
    GUILCD_writeCharStr(0x02,0x07,42,FALSE);//��
}


extern void menu_5(void)//change top
{
    GUILCD_writeCharStr(0x00,0x00,10,FALSE);//��
	GUILCD_writeCharStr(0x00,0x01,46,FALSE);//��
	GUILCD_writeCharStr(0x00,0x02,47,FALSE);//��
	GUILCD_writeCharStr(0x00,0x03,4,FALSE);//4
	GUILCD_writeCharStr(0x00,0x04,57,FALSE);//λ
	GUILCD_writeCharStr(0x00,0x05,48,FALSE);//��
    GUILCD_writeCharStr(0x00,0x06,49,FALSE);//��
    GUILCD_writeCharStr(0x00,0x07,50,FALSE);//Ƶ
    GUILCD_writeCharStr(0x00,0x08,51,FALSE);//��
	GUILCD_writeCharStr(0x03,0x00,58,FALSE);
	GUILCD_writeCharStr(0x03,0x01,59,FALSE);
	GUILCD_writeCharStr(0x03,0x02,60,FALSE);
	GUILCD_writeCharStr(0x03,0x03,61,FALSE);
	GUILCD_writeCharStr(0x03,0x04,62,FALSE);
	GUILCD_writeCharStr(0x03,0x05,63,FALSE);
/*	wr_hex(0,0x010,0x05+3,1);
	wr_hex(0,0x010,0x05+6,FALSE);
	wr_hex(0,0x010,0x05+9,FALSE);
	//wr_hex(31,0x010,0x05+12,FALSE);
     wr_hex(0,0x010,0x05+12,FALSE);
*/
    
    wr_hex(0,0x010,0x05+12,1);
	wr_hex(0,0x010,0x05+9,FALSE);
	wr_hex(0,0x010,0x05+6,FALSE);
	//wr_hex(31,0x010,0x05+12,FALSE);
    wr_hex(0,0x010,0x05+3,FALSE);

     	
 
}//change end

extern void menu_6(void)
{
	GUILCD_writeCharStr(0x00,0x00,64,FALSE);//��ѡ��
	GUILCD_writeCharStr(0x00,0x01,65,FALSE);
	GUILCD_writeCharStr(0x00,0x02,66,FALSE);
	GUILCD_writeCharStr(0x00,0x03,8,FALSE);
	GUILCD_writeCharStr(0x00,0x04,8,FALSE);
    
	GUILCD_writeCharStr2(0x01,0x00,67,FALSE);
}

extern void menu_7(void)
{
	GUILCD_writeCharStr(0x00,0x00,10,FALSE);//��ѡ��
	GUILCD_writeCharStr(0x00,0x01,11,FALSE);
	GUILCD_writeCharStr(0x00,0x02,12,FALSE);
	GUILCD_writeCharStr(0x02,0x00,1,FALSE);//1
    GUILCD_writeCharStr(0x02,0x02,53,FALSE);//��
    GUILCD_writeCharStr(0x02,0x03,54,FALSE);//��
    GUILCD_writeCharStr(0x03,0x00,2,FALSE);//2
    GUILCD_writeCharStr(0x03,0x02,52,FALSE);//��
    GUILCD_writeCharStr(0x03,0x03,53,FALSE);//��
    GUILCD_writeCharStr(0x03,0x04,54,FALSE);//��
}
