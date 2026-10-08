#include "PLL.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

void PortD_Init(void);
void SysTick_Init(void);
void SysTick_Wait(uint32_t delay);
void SysTick_Wait10ms(uint32_t time);
int main(void){
    uint32_t i;
    PLL_Init();          
    PortD_Init();        
    SysTick_Init();      
    for(i = 0; i < 12; i++){

        GPIO_PORTD_DATA_R = 0x05;   
        SysTick_Wait10ms(1);

        GPIO_PORTD_DATA_R = 0x06;   
        SysTick_Wait10ms(1);

        GPIO_PORTD_DATA_R = 0x0A;   
        SysTick_Wait10ms(1);

        GPIO_PORTD_DATA_R = 0x09;   
        SysTick_Wait10ms(1);
    }

    GPIO_PORTD_DATA_R = 0x05;   
    SysTick_Wait10ms(1);
    GPIO_PORTD_DATA_R = 0x06;   
    SysTick_Wait10ms(1);
}
void PortD_Init(void){
    volatile uint32_t delay;
    
    SYSCTL_RCGCGPIO_R |= 0x08;   
    delay = SYSCTL_RCGCGPIO_R;   
    
    GPIO_PORTD_AMSEL_R &= ~0x0F; 
    GPIO_PORTD_PCTL_R &= ~0x0000FFFF; 
    GPIO_PORTD_DIR_R |= 0x0F;    
    GPIO_PORTD_AFSEL_R &= ~0x0F; 
    GPIO_PORTD_DEN_R |= 0x0F;    
}
void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0;          
    NVIC_ST_CURRENT_R = 0;       
    NVIC_ST_CTRL_R = 0x00000005; 
}
void SysTick_Wait(uint32_t delay){
    NVIC_ST_RELOAD_R = delay - 1;
    NVIC_ST_CURRENT_R = 0;
    while((NVIC_ST_CTRL_R & 0x00010000) == 0);
}

void SysTick_Wait10ms(uint32_t time){
    uint32_t i;
    for(i = 0; i < time; i++){
        SysTick_Wait(800000);  
    }
}
