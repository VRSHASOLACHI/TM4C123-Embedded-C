#include "PLL.h"
#include "tm4c123gh6pm.h"
#include "stdint.h"
void PortD_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x08;          // 1) activate Port D
  delay = SYSCTL_RCGC2_R;          // allow time for clock to stabilize                                  
  GPIO_PORTD_AMSEL_R &= ~0x09;     // 3) disable analog functionality on PD3 and PD0
  GPIO_PORTD_PCTL_R &= ~0x00F000F;  // 4) configure PD3 and PD0 as GPIO
  GPIO_PORTD_DIR_R |= 0x09;        // 5) make PD3 and PD0 out
  GPIO_PORTD_AFSEL_R &= ~0x09;     // 6) disable alt funct on PD4 and PD0
  GPIO_PORTD_DEN_R |= 0x09;        // 7) enable digital I/O on PD4 and PD0
}
void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	//NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;// priority 0
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}
void SysLoad(unsigned long period){
NVIC_ST_RELOAD_R = period-1;  // number of counts to wait
NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }

}

int main(void){
	/*Initialize ports and timers*/
		int i;
	PLL_Init(); // 80 MHz
	SysInit();  	
	PortD_Init();
	while(1)
	{
		GPIO_PORTD_DATA_R |= (0x01); 
		SysLoad(400000);  // wait 5ms
				
		GPIO_PORTD_DATA_R &= ~(0x01); 
		SysLoad(400000);  // wait 5ms
	}
}
