#include "tm4c123gh6pm.h"
#include <stdint.h>
unsigned long Switch;
/* Function to initialize Port F */
void PortF_Init(void)
{
    volatile unsigned long delay;

    SYSCTL_RCGC2_R |= 0x00000020;     // 1) Activate clock for Port F
    delay = SYSCTL_RCGC2_R;           // Allow time for clock to start

    GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) Unlock GPIO Port F
    GPIO_PORTF_CR_R   = 0x1F;         // Allow changes to PF4–PF0

    GPIO_PORTF_AMSEL_R = 0x00;        // 3) Disable analog function on PF
    GPIO_PORTF_PCTL_R  = 0x00000000;  // 4) Configure PF pins as GPIO
    GPIO_PORTF_DIR_R   = 0x0E;        // 5) PF4 & PF0 input, PF3–PF1 output
    GPIO_PORTF_AFSEL_R = 0x00;        // 6) Disable alternate function
    GPIO_PORTF_PUR_R   = 0x11;        // Enable pull-up resistors on PF0 & PF4
    GPIO_PORTF_DEN_R   = 0x1F;        // 7) Enable digital I/O on PF4–PF0
}

int main(void)
{
    PortF_Init();                    // Initialize Port F
    while (1)
    {
        Switch = GPIO_PORTF_DATA_R & 0x11;   // Read switch inputs PF4 & PF0

        if (Switch == 0x11)
        {
            GPIO_PORTF_DATA_R = 0x02;       // Pink / Red LED
        }
        else if (Switch == 0x01)
        {
            GPIO_PORTF_DATA_R = 0x08;       // Yellow LED
        }
        else if (Switch == 0x10)
        {
            GPIO_PORTF_DATA_R = 0x04;       // Sky Blue LED
        }
        else if (Switch == 0x00)
        {
            GPIO_PORTF_DATA_R = 0x0E;       // White LED
        }
        else
        {
            GPIO_PORTF_DATA_R = 0x00;       // Turn OFF all LEDs
        }
    }
}
