#include<LPC21xx.h>
#include "adc_define.h"
#include "adc.h"
#include "type.h"
#include <LPC21xx.h>
#include "pin_con.h"
#include "delay.h"
#include "adc_define.h"
#include "adc.h"
void Init_adc(void)
{
  cfgportpin(0,28,ANO_INPUT_PIN);
  ADCR=(1<<PDN_BITS)|(CLKDIV<<CLKDIV_BITS);		
  //	AD0CR=(1<<PDN_BITS)|(CLKDIV<<CLKDIV_BITS);						//for 2148
}


void read_adc(u8 chno,f32 *eAR,u32 *dVAL)
{
  ADCR &=0xFFFFFF00;
//	AD0CR &=0xFFFFFF00;
  ADCR|=(1<<ADC_START_BITS)|(1<<chno);
//	AD0CR|=(1<<ADC_START_BITS)|(1<<chno);
  delay_us(3);
  while(((ADDR>>DONE_BITS)&1)==0);
  ADCR&=~(1<<ADC_START_BITS);
  *dVAL=((ADDR>>RESULT_BITS)&1023);
  *eAR=(*dVAL  *  (3.3 / 1023));
}

