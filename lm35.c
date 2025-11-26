#include "type.h"
#include "adc_define.h"
#include "adc.h"
#include "lcd_define.h"
#include "lcd.h"
u32 read_LM35(void)
{
       f32 eAR;u32 dVAL;static u32 flag;
       u32 tdegc;
       if(flag==0)
	   {
       Init_adc();
        flag=1;
       }
	   read_adc(1,&eAR,&dVAL);
	   tdegc=eAR*100;
	   return tdegc;
}
