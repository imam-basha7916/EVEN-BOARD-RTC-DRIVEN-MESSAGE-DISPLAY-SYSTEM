#include <LPC21xx.h>
#include "type.h"
#include "pin_con.h"
#include "delay.h"
//#include "interrupt.h"

#define PIN_FUNC 3
#define EINT0_PIN_FUNC PIN_FUNC
#define EINT0_PIN 1
#define EINT0_VIC_CHNO 14
extern u32 flag;
void eint0_isr(void) __irq;
u32 gcount;

void external_interrupt() {
    // Set P0.1 as EINT0 (function 3)
    cfgportpin(0, EINT0_PIN, EINT0_PIN_FUNC);

    // Configure VIC
    VICIntSelect = 0;                                // IRQ (not FIQ)
    VICIntEnable = 1 << EINT0_VIC_CHNO;              // Enable EINT0 interrupt
    VICVectCntl0 = (1 << 5) | EINT0_VIC_CHNO;        // Enable slot 0 for EINT0
    VICVectAddr0 = (u32)eint0_isr;                   // ISR address

    // Configure EINT0
    EXTINT =1<<0;         // Clear any pending EINT0 interrupt
    EXTMODE=1<<0;        // Edge triggered
    EXTPOLAR=1<<0;       // Falling edge
}

// ISR for EINT0
void eint0_isr(void) __irq {
    flag=1;  // Toggle LED
    EXTINT=1<<0;                  // Clear EINT0 flag
    VICVectAddr = 0;	// End of ISR
}
