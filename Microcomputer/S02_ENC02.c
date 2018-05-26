/***********************************************************************/
/*                                                                     */
/*  FILE        :ENC01.c                                               */
/*  DATE        :Tue, Jun 14, 2016                                     */
/*  DESCRIPTION :Main Program                                          */
/*  CPU TYPE    :H8/3687                                               */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.16).    */
/*                                                                     */
/***********************************************************************/
                  
#include"iodefine.h"
#include"lcd_func.h"
#include<machine.h>

int count1 = 0;
int count2 = 0;

#define MSK_WKP0 0x01
#define MSK_WKP1 0x02

void main(void);
#ifdef __cplusplus
extern "C" {
void abort(void);
}
#endif

void WKP_func()
{	
	if( IO.PDR5.BIT.B1 ){
		count2++;
		count1--;
		IWPR.BYTE = 0x00;
		return;
	}
	
	if(!IO.PDR5.BIT.B1){
		count1++;
		count2--;
		IWPR.BYTE = 0x00;
		return;
	}
}

void main(void)
{
	set_imask_ccr(1);
	
	lcd_init();
	  
	IO.PCR5 = 0xF0;  
	IO.PUCR5.BYTE = 0xFF;    
	
	IO.PMR5.BYTE = 0x01; //P50,P51���g�p
	IEGR2.BYTE = 0x00;   //����������o
	IENR1.BIT.IENWP = 1; //�O���[�q�����ݔ���������
	IWPR.BYTE = 0x00;    //�t���O���N���A
	
	set_imask_ccr(0);
	
	
	while(1){
		lcd_xy(1,1);
		
		if(count1 >= 0)
			lcd_dataout(count1);
		else{
			lcd_puts("-");
			lcd_dataout(count2);
		}
			
		lcd_puts("  ");
		
	}
}

#ifdef __cplusplus
void abort(void)
{

}
#endif