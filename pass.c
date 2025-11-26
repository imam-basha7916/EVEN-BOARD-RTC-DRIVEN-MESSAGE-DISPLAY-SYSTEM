#include<LPC21xx.h>
#include<string.h>
#include "key1.h"
#include "key_define.h"
#include "lcd.h"
#include "lcd_define.h"
#include "delay.h"
extern int result;
u8 pm[5]="1234";
int password_checking(void);
int currect_password(char * p);
void password_management(void)
{
	u32 rev;
	int count=0;
  initLCD();
  Initkey();
 
  while(1)
  {
  CMDLCD(GOTO_LINE1_POS0);
   strLCD(" ENTER PASSWORD ");
  rev=password_checking();
  
  if(rev)
  {
  	 CMDLCD(CLEAR_LCD);	 
	 CMDLCD(GOTO_LINE1_POS0);
	 strLCD("SUCCESSFULLY....@");
   CMDLCD(GOTO_LINE2_POS0);
   strLCD("@.OPEN.........@");
		result=1;
	 delay_ms(150);
		//CMDLCD(CLEAR_LCD);
	 break;
		
  }
  else
  {	 CMDLCD(CLEAR_LCD);
     CMDLCD(GOTO_LINE1_POS0);
	 strLCD("INVALID PASSWORD");
    CMDLCD(GOTO_LINE2_POS0);
	strLCD("*@.TRY AGAIN.@*");
		result=0;
		count++;
	delay_ms(100);
		if(count==3)
	{
		break;
	}
  CMDLCD(CLEAR_LCD);
 }
	
 }
	if(rev==0)
	{
	CMDLCD(CLEAR_LCD);
	CMDLCD(GOTO_LINE1_POS0);
  strLCD("  TRY IN SOME ");
  CMDLCD(GOTO_LINE2_POS0);
  strLCD("  OTHER TIME  ");
  delay_ms(500);
  CMDLCD(CLEAR_LCD);
	}
}
int password_checking(void)
{
   s32 i=0,result,num=0;
   s8 pass[4];
   while(1)
   {
     num=keyscan();
	 delay_ms(50);

	// num1=num+'0';
	 if(num>='0' && num<='9')
	 {
	     pass[i]=num;
	     CMDLCD(GOTO_LINE2_POS0+i);
       charLCD('*');
		   i++;
		 delay_ms(1);
	 }
	 else if(num=='#')
	 {
	   i--;
	   CMDLCD(GOTO_LINE2_POS0+i);
	   charLCD(' ');
	//	i--;
	 }
	 else if(num=='=')
	 {
	 pass[i]='\0';
	 break;
	 }
	
   }
   result=currect_password(pass);
   return result;
}
int currect_password(char *p)
{
    int i=0,count=0;
    for(i=0;i<4;i++)
	{
	   if(p[i]==pm[i])
	   {  
	      count++;
	   }
	}
	if(count==4)
        	return 1; 
  /*	if(strcmp(p,pm)==0)
	{
	  return 1;
	 }*/
	 else  
	     return 0;
}
	     

