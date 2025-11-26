#include<stdio.h>
#include<stdlib.h>
#include "type.h"
#include "key_define.h"
#include "lcd_define.h"
#include "lcd.h"
#include<LPC21xx.h>
#include "delay.h"
u8 keyLUT[4][4]=
{
 {'7','8','9','/'},
 {'4','5','6','*'},
 {'1','2','3','-'},
 {'#','0','=','+'}
 };
 void Initkey(void)
 {
 IODIR1|=15<<row0;
 }


 u32 colscan(void)
 {
 u32 t;
 t=(((IOPIN1>>col0)&15)<15)?0:1;
  return t;
 }


u32 rowcheck(void)
{
  u32 r;
  for(r=0;r<4;r++)
  {
  IOPIN1=((IOPIN1 & ~(15<<row0)) | ((~(1<<r))<<row0));
  if(colscan()==0)
  {
  	break;
	}
  }
  IOCLR1=15<<row0;
  return r;
  
}


u32 colcheck(void)
{
  u32 c;
  for(c=0;c<4;c++)
  {
  if(((IOPIN1>>(col0)+c)&1)==0)
  {
  break;
  }

 }
  return c;

}




u8 keyscan(void)
{

    u32 r,c;
	u8 kv;
	while(colscan());
	r=rowcheck();
	c=colcheck();
	kv=keyLUT[r][c];
	while(colscan()==0);
	return kv;
}
/*
u32 readnew(void)
{
   u32 sum=0;
   s32 i=0;
   char num;
   while(1)
   {
       
        num=keyscan();
        while(colscan()==0);
        CMDLCD(GOTO_LINE2_POS0  +8);
        charLCD(num);
    		delay_ms(50);
        if(num>='0' && num<='9')
        {
		  sum=sum*10+(num-'0');
		  i++;
		  CMDLCD(GOTO_LINE2_POS0);
	      U32LCD(sum);
        }
		 else if(num=='#')
		    {
		      CMDLCD(GOTO_LINE2_POS0+(i--));
		       strLCD(" ");
		       sum/=10;
		    }
		else if(num=='=')
		{
		break;
		}
    }

	return sum;
}
*/

u32 readnew(void)
{
    char input[12];   // buffer for digits (max 10 digits + null)
    s32 i = 0;
    u32 sum = 0;
    char num;

    input[0] = '\0';  // start with empty string

    while (1)
    {
        num = keyscan();
        while (colscan() == 0);
        delay_ms(20);

        if (num >= '0' && num <= '9')
        {
            if (i < 10)   // prevent overflow
            {
                input[i++] = num;
                input[i] = '\0';

                CMDLCD(GOTO_LINE2_POS0);
                strLCD(input);   // print full string with leading zeros
            }
        }
        else if (num == '#')   // backspace
        {
            if (i > 0)
            {
                i--;
                input[i] = '\0';

                CMDLCD(GOTO_LINE2_POS0);
                strLCD("          ");   // clear line
                CMDLCD(GOTO_LINE2_POS0);
                strLCD(input);          // reprint trimmed input
							//  strLCD("          ");
            }
        }
        else if (num == '=')   // end input
        {
					  CMDLCD(GOTO_LINE2_POS0);
					  strLCD("          ");
            break;
        }
    }

    // convert string to number
    sum = atoi(input);
    return sum;
}





