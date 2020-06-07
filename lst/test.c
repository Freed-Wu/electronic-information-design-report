void wr_hex2(unsigned int code,unsigned int o_y,unsigned int o_x,unsigned short fanxian)
/**   
 *  @brief 显示图形
 * 
 *  @param 图形序号
 *  @param 纵坐标
 *  @param 横坐标
 *  @param 反显使能
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
		if(fanxian == FALSE)			/* 是否反显 */
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
		wr_data(dat1_temp);			/* 汉字点阵在LCD中的位置 */
		wr_data(dat2_temp);
		wr_com(0x24);

		for(index=0;index<8;index++){
		wr_data(*(code_temp+i2*8+index));			/* 写入汉字点阵 */
		wr_com(0xc0);
		}

		i1 = i1 + 0x20;
		dat1_temp = i1 & 0xff;
		dat2_temp = (i1>>8) & 0xff;		/* 修改汉字点阵在LCD中的位置 */
	}
}

