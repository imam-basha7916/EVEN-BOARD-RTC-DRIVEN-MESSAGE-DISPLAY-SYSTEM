#include<LPC21xx.h>
#include "type.h"
#include "lcd_define.h"
#include "delay.h"
#define LCD_DATA 8
#define LCD_RS 16
#define LCD_RW 18
#define LCD_EN 17


void writeLCD(u8 byte)
{
  IOPIN0=((IOPIN0 & ~(255<<LCD_DATA)) |  ((byte & 255)<<LCD_DATA));
  IOCLR0=1<<LCD_RW;
  IOSET0=1<<LCD_EN;
  delay_us(1);
  IOCLR0=1<<LCD_EN;
  delay_ms(2);


}

void CMDLCD(u8 cmd)
{
IOCLR0=1<<LCD_RS;
  writeLCD(cmd);
}


void initLCD(void )
{
IODIR0=((IODIR0 & ~(255<<LCD_DATA)) | (255<<LCD_DATA));
  IODIR0=((IODIR0 & ~(15<<LCD_RS)) | (7<<LCD_RS));
  delay_ms(15);
   CMDLCD(0x30);
   delay_ms(4);
   delay_us(100);
   CMDLCD(0x30);
   delay_us(100);
   CMDLCD(0x30);
   CMDLCD(MODE_8BIT_2LINE);
   CMDLCD(DSP_ON_CUR_OFF);
   CMDLCD(CLEAR_LCD);
   CMDLCD(SHIFT_CUR_RIGHT);
}


void charLCD(u8 asciival)
{
    IOSET0=1<<LCD_RS;
        writeLCD(asciival);
}


void strLCD(s8 *p)
{
   while(*p)
   {
     charLCD(*p++);
   }
}

void U32LCD(u32 n)

{
   u8 a[10];
   s32 i=0;
   if(n==0)
   {
     charLCD('0');
   }
   else
   {
     while(n)
         {
            a[i]=(n%10)+48;
                n/=10;
                i++;
         }
         for(--i;i>=0;i--)
         {
           charLCD(a[i]);
         }
   }
  }

void S32LCD(s32 n)
{
   if(n>0)
   {
     charLCD('-');
         n=-n;
   }
   U32LCD(n);
}
void F32LCD(f32 fn ,u32 ndp)
{
   u32 n,i;
   if(fn<0)
   {
   charLCD('-');
   fn=-fn;
   }
   n=fn;
   U32LCD(n);
   charLCD('.');
   for(i=0;i<ndp;i++)
   {
   fn=(fn-n)*10;
   n=fn;
   charLCD(n+48);
   }
 }

void buildCGRAM(u8 *p,u8 nbyte)
{
  u32 i;
   CMDLCD(GOTO_CGRAM_START);
   IOSET0=1<<LCD_RS;
   for(i=0;i<nbyte;i++)
   {
   writeLCD(p[i]);
   }
   CMDLCD(GOTO_LINE1_POS0);
 }

	
