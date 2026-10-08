#include "tm4c123gh6pm.h"
#include "PLL.h"
#include <stdint.h>
void PortD_Init(void)
{
    unsigned long volatile delay;

    SYSCTL_RCGC2_R |= 0x08;      // Enable clock for Port D
    delay = SYSCTL_RCGC2_R;      // Wait for clock to stabilize

    GPIO_PORTD_DIR_R |= 0x01;    // PD0 as output
    GPIO_PORTD_AFSEL_R &= ~0x01; // Disable alternate function on PD0
    GPIO_PORTD_AMSEL_R &= ~0x01; // Disable analog on PD0
    GPIO_PORTD_PCTL_R &= ~0x0000000F; // Clear PCTL for PD0
    GPIO_PORTD_DEN_R |= 0x01;    // Enable digital I/O on PD0
}
void SysInit(void)
{
    NVIC_ST_CTRL_R = 0;          // Disable SysTick during setup
    NVIC_ST_CURRENT_R = 0;       // Any write clears current value
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF); // Priority 0
    NVIC_ST_CTRL_R = 0x00000005; // Enable SysTick with core clock
}
void SysLoad(unsigned long period)
{
    NVIC_ST_RELOAD_R = period - 1; // Load reload value
    NVIC_ST_CURRENT_R = 0;         // Clear current value

    while ((NVIC_ST_CTRL_R & 0x00010000) == 0)
    {
        // Wait for COUNT flag
    }
}
int main(void)
{
    PLL_Init();     // Set system clock
    PortD_Init();   // Initialize PD0
    SysInit();      // Initialize SysTick
    while (1)
    {
        GPIO_PORTD_DATA_R = 0x01; // PD0 HIGH
        SysLoad(80000);

        GPIO_PORTD_DATA_R = 0x00; // PD0 LOW
        SysLoad(80000);
    }
}
