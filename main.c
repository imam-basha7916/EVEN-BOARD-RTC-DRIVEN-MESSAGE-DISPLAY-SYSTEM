#include <lpc214x.h>
#include<string.h>
#include<stdio.h>

//include required header files
#include "type.h"
#include "lcd.h"
#include "lcd_define.h"
#include "delay.h"
#include "rtc.h"
#include "lcd.h"
#include "lm35.h"
#include "adc.h"
#include "key1.h"
#include "key_define.h"

#define TOTAL_MESSAGES 10
#define red_led         24
#define green_led       25
#define yellow_led      26
typedef struct {
    u8 hour;
    u8 minute;
    char text[80];
    u8 enabled;  	// 1 = show, 0 = skip (user controlled )
}Message;
Message messageList[TOTAL_MESSAGES] = {
    {7, 45, "Good Morning! Classes Start Soon ", 1},
    {13, 45, "C Programming Session in Class room number 2 ", 1},
    {10, 15, "C module theory exam in 4th floor lab1 ", 1},
    {10, 15, "C module lab exam in 4th floor lab2 and middle lab at 10:30AM ", 1},
    {12, 45, "Lunch Break from 1PM – 2PM ", 1},
    {9, 45, "ARM Workshop on external interrupts in LAB1 at 10AM ", 1},
    {9, 45, "ARM kit issue time from 10AM – 10:30AM in middle lab ", 1},
    {15, 15, "Only 15 mins break time for next ARM session ", 1},
    {17, 00, " Revise today’s Class programs at home! ", 1},
    {17, 45, " End of Day – See You Tomorrow! ", 1}
};
int flag=0;
int result=0;
void msg_scrolling(char * ,int );
void	temperature_checking(void);
void external_interrupt(void);
void password_management(void);
s32 date,sec,day,hour,min,month,year;
// Main function
int main()
{
	int i,p,Break,var;
	char c,v,e,d;
  // Initialize RTC 
   RTC_Init();	
  // Initialize the LCD
	initLCD();
	//Initialize the keypad
	 Initkey();
	//setting direcction to the red led at po.5
	IODIR1|=1<<red_led;
	IODIR1|=1<<green_led;
	IODIR1|=1<<yellow_led;
	//IODIR0|=1<<sw;
	IOSET1=1<<red_led;
	IOSET1=1<<green_led;
	IOSET1=1<<yellow_led;
	CMDLCD(GOTO_LINE1_POS0);
	strLCD("WELCOME TO* * * ");
	CMDLCD(GOTO_LINE2_POS0);
	strLCD("* * *VECTOR!....");
	//delay_s(1);
	delay_ms(1000);
	CMDLCD(CLEAR_LCD);
	hour=13;
	min=44;
	sec=59;
	date=15;
	month=5;
	year=25;
	day=3;
	
  // SET the initial time (hours, minutes, seconds)
   SetRTCTimeInfo(hour,min,sec);
	SetRTCDateInfo(date,month,year);
	SetRTCDay(day);
	external_interrupt();
    while (1) 
		{
			IOCLR1=1<<red_led;
			IOSET1=1<<green_led;
	    IOSET1=1<<yellow_led;			
       //Get and display the current time on LCD
			GetRTCTimeInfo(&hour,&min,&sec);
			DisplayRTCTime(hour,min,sec);
			 //Get and display the current date,month,year on LCD
	   	GetRTCDateInfo(&date,&month,&year);
			DisplayRTCDate(date,month,year);
			 //Get and display the current day on LCD
			GetRTCDay(&day);
			DisplayRTCDay(day);
			//displaying current room temperature 
			temperature_checking();
			//indicates that ,current time ,date ,month,year , date and temperature 
			//was dispaying on lcd
       
         if (flag == 1)
        {
					IOCLR1=1<<yellow_led;
					IOSET1=1<<red_led;
					IOSET1=1<<green_led;
        CMDLCD(CLEAR_LCD);
        CMDLCD(GOTO_LINE1_POS0);
        strLCD("@@.WELCOME TO....");
        CMDLCD(GOTO_LINE2_POS0);
        strLCD("..ADMINE MODE.@@");
        delay_ms(500);
        CMDLCD(CLEAR_LCD);
        password_management(); 
          IOCLR1=1<<yellow_led;
					IOSET1=1<<red_led;
					IOSET1=1<<green_led;					// Global 'result' is set inside this
				while(1)
				{					
        if (result == 1)
        {   IOCLR1=1<<yellow_led;
            CMDLCD(CLEAR_LCD);
            CMDLCD(GOTO_LINE1_POS0);
            strLCD(" TE:1 ME:2 DE:3");
            CMDLCD(GOTO_LINE2_POS0);
					  strLCD("          EXIT:0");
            CMDLCD(GOTO_LINE2_POS0);
            c = keyscan();  // user choice
            charLCD(c);
            delay_ms(200);

            switch (c)
            {
							case '0':IOCLR1=1<<yellow_led;
								       Break=1;
								       flag=0; 
								       break;
                case '1':  // Edit Time
									   IOCLR1=1<<yellow_led;
                    CMDLCD(CLEAR_LCD);
                    strLCD(" EDIT TIME ");
                    delay_ms(150);

                    CMDLCD(CLEAR_LCD);
                    CMDLCD(GOTO_LINE1_POS0);
                    strLCD("hour min sec");

                    hour = readnew(); delay_ms(20);
                    min = readnew();  delay_ms(20);
                    sec = readnew();  delay_ms(20);
                    //day = keyscan();  delay_ms(20);

                    CMDLCD(GOTO_LINE2_POS0);
                    U32LCD(hour); charLCD(':');
                    U32LCD(min);  charLCD(':');
                    U32LCD(sec);

                    SetRTCTimeInfo(hour, min, sec);
                    flag = 0;
                    delay_ms(200);
                    CMDLCD(CLEAR_LCD);
                    break;

                case '2':  // Enable/Disable message
									  IOCLR1=1<<yellow_led;
                    CMDLCD(CLEAR_LCD);
                    CMDLCD(GOTO_LINE1_POS0);
                    strLCD("MSG ENABLE EDIT");
                    delay_ms(500);

                    CMDLCD(CLEAR_LCD);
                    CMDLCD(GOTO_LINE1_POS0);
                    strLCD("1 2 3 4 5 6 7 8");
                    CMDLCD(GOTO_LINE2_POS0);
                    strLCD("     9 10 ");

                    v = keyscan(); // e.g., '0' to '9'

                    // Convert char to index (0 to 9)
								     p=v-'0';

                    if (p < 0 || p >= TOTAL_MESSAGES) {
                        CMDLCD(CLEAR_LCD);
                        strLCD("Invalid Index");
                        delay_ms(1000);
                        CMDLCD(CLEAR_LCD);
                        break;
                    }

                    CMDLCD(CLEAR_LCD);
                    CMDLCD(GOTO_LINE1_POS0);
                    strLCD("ENBL:1 DISBL:0");
                    CMDLCD(GOTO_LINE2_POS0);
                    U32LCD(p + 1); // show message number

                    CMDLCD(GOTO_LINE2_POS0 + 4);
                    e = keyscan();
                    charLCD(e);
                    delay_ms(500);

                    messageList[p].enabled = e - '0';

                    CMDLCD(CLEAR_LCD);
                    CMDLCD(GOTO_LINE1_POS0);
                    strLCD("Updated To:");
                    CMDLCD(GOTO_LINE2_POS0);
                    U32LCD(messageList[p].enabled);
                    delay_ms(1000);

                    CMDLCD(CLEAR_LCD);
                    flag = 0;
                    break;

                case '3':  // Edit Date and Day
									  IOCLR1=1<<yellow_led;
                    CMDLCD(CLEAR_LCD);
                    CMDLCD(GOTO_LINE1_POS0);
                    strLCD(" EDIT DATE DAY ");
								
                    date = readnew();  delay_ms(50);
                    month = readnew(); delay_ms(50);
                    year = readnew();  delay_ms(50);
                    d = keyscan();     delay_ms(50);
								
                    CMDLCD(GOTO_LINE2_POS0);
                    U32LCD(date); charLCD('/');
                    U32LCD(month); charLCD('/');
                    U32LCD(year);

                    CMDLCD(GOTO_LINE2_POS0 + 12);
                    charLCD(d);

                    day = d - '0';
                    SetRTCDateInfo(date, month, year);
                    SetRTCDay(day);
                    delay_ms(200);

                    CMDLCD(CLEAR_LCD);
                    flag = 0;
                    break;

                default:
									  IOCLR1=1<<yellow_led;
                    CMDLCD(CLEAR_LCD);
                    strLCD("Invalid Option");
                    delay_ms(1000);
                    CMDLCD(CLEAR_LCD);
                    break;
            }

            delay_ms(100);
				 }
				else
				{
					flag=0;
					break;
				}
				if(Break==1)
				{
					CMDLCD(CLEAR_LCD);
					break;
				}
				
			}
        
        
  }
				
					
					
				
			 
			//displaying messages based on time
			for(i=0;i<TOTAL_MESSAGES;i++)
			{
				//checking wether hours,min are matched with messagelist hour and min
				//if conditon is true than it goes to message scrolling operation
				if ((messageList[i].hour == hour) && (messageList[i].minute == min) && (messageList[i].enabled==1))
				{
					IOSET1=1<<red_led;
					IOSET1=1<<yellow_led;
					IOCLR1=1<<green_led;
					//if(flag==1)
						//break;
					var=15*60*1000;
				
					
					msg_scrolling(messageList[i].text,var);
					CMDLCD(CLEAR_LCD);
					if(flag==1)
						break;
					
				 
					
				}
			}
		}
	
}



/*
void msg_scrolling(char * m, int len)
{
    int i, a;
    char shiftdata[17];
   // char scroll[80];
    char last;
    int count = 0;

    // Make a local copy of message to protect original
   // strcpy(scroll, m);
    //strcat(scroll, "                ");  // Padding for scrolling
   // len = strlen(scroll);

    CMDLCD(CLEAR_LCD);

    // Scroll for about 15 seconds (adjust as needed)
    while (count < 150)  // ~150 * 100ms = 15s
    {
        last = m[0];
        for (a = 0; a < len - 1; a++)
        {
            m[a] = m[a + 1];
        }
        m[len - 1] = last;

        for (i = 0; i < 16; i++)
        {
            shiftdata[i] = m[i];
        }
        shiftdata[16] = '\0';

        CMDLCD(GOTO_LINE1_POS0);
        strLCD(shiftdata);

        CMDLCD(GOTO_LINE2_POS0);
        strLCD("   EVEN BOARD  ");
				if(flag==1)
					break;

        delay_ms(100);  // Delay for human-readable speed
        count++;
    }
}
void msg_scrolling(char *msg, int duration_ms)
{
    char window[17];   // 16 characters + null terminator
    int i, j;
    int delay_time = 10;
    int steps = duration_ms / delay_time;
    int msg_len = 0;
    CMDLCD(CLEAR_LCD);
    // Calculate message length
    while (msg[msg_len] != '\0')
        msg_len++;
while(1)
{
    for (i = 0; i < steps; i++)
    {
        for (j = 0; j < 16; j++)
        {
            int idx = i + j - 16;  // Start with -16 to pad spaces at beginning

            if (idx < 0)
                window[j] = ' ';   // leading spaces
            else if (idx < msg_len)
                window[j] = msg[idx];  // actual message
            else
                window[j] = ' ';   // trailing spaces
        }

        window[16] = '\0';  // null-terminate the string

        CMDLCD(GOTO_LINE1_POS0);
        strLCD(window);

        CMDLCD(GOTO_LINE2_POS0);
        strLCD("   EVEN BOARD  ");

        if (flag == 1)
            break;

        delay_ms(delay_time);

    }
	}
		
		
}*/


void msg_scrolling(char *msg, int duration_ms)   // Function to scroll a given message on LCD for given duration (ms)
{
	  
    char window[17];  // Buffer to hold 16 characters (LCD width) + 1 null terminator
    int i, j, total_len;  
    int delay_time = 100;   // Each step assumed to be 100ms
    int steps = duration_ms / delay_time;   // Total number of steps for scrolling based on duration
    int msg_len = 0;    // Variable to store message length

    // Calculate message length manually (like strlen)
    while (msg[msg_len] != '\0')
        msg_len++;

    // Total scrollable length = message length + 16 leading blanks + 16 trailing blanks
    total_len = msg_len + 32;
    CMDLCD(CLEAR_LCD);
    // Loop through each step of scrolling
    for (i = 0; i < steps; i++)
    {
        // Fill one LCD window (16 characters) for this step
        for (j = 0; j < 16; j++)
        {
            // Calculate index of message character (with leading blanks shift)
            int idx = (i + j - 16) % total_len;  

            // If index is outside message range ? put blank space
            if (idx < 0 || idx >= msg_len)
                window[j] = ' ';
            else
                window[j] = msg[idx];   // Otherwise copy character from message
        }

        window[16] = '\0';   // Null-terminate the 16-char window (to print as string)

        CMDLCD(GOTO_LINE1_POS0);   // Move cursor to beginning of line 1
        strLCD(window);            // Print the 16-char window on line 1

        CMDLCD(GOTO_LINE2_POS0);   // Move cursor to beginning of line 2
        strLCD("   EVEN BOARD  "); // Always display fixed text on line 2

        if (flag == 1)   // If external stop flag is set, break the scrolling
            break;

        delay_ms(10);   // Small delay before next step (smooth scrolling)
    }
}




void temperature_checking(void)
{
	int num;
	
      num=read_LM35();
			CMDLCD(GOTO_LINE2_POS0+11);
			strLCD("TP:");
			U32LCD(num);
			delay_s(2);
}
			



		
		
		
	
