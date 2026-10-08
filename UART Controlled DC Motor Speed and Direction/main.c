#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
#include "string.h"
char string[20];
#define CR 0x0D
#define LF 0x0A
#define BS 0x08
void UART_Init(void){
    SYSCTL_RCGC1_R|=0x01; SYSCTL_RCGC2_R|=0x01;
    UART0_CTL_R&=~0x01;
    UART0_IBRD_R=43; UART0_FBRD_R=26;
    UART0_LCRH_R=0x70; UART0_CTL_R|=0x01;
    GPIO_PORTA_AFSEL_R|=0x03; GPIO_PORTA_DEN_R|=0x03;
    GPIO_PORTA_PCTL_R=0x00000011;
    GPIO_PORTA_AMSEL_R&=~0x03;}
void UART_OutChar(unsigned char d){ while(UART0_FR_R&0x20); UART0_DR_R=d; }
void UART_OutString(char *p){ while(*p) UART_OutChar(*p++); }
unsigned char UART_InChar(void){ while(UART0_FR_R&0x10); return UART0_DR_R&0xFF; }
void UART_InString(char *b,unsigned short m){
    int l=0; char c=UART_InChar();
    while(c!=CR){
        if(c==BS){ if(l){ b--; l--; UART_OutChar(BS);} }
        else if(l<m){ *b++=c; l++; UART_OutChar(c);}
        c=UART_InChar();}
    *b=0;}
void PortD_Init(void){volatile unsigned long d;
    SYSCTL_RCGC2_R|=0x08; d=SYSCTL_RCGC2_R;
    GPIO_PORTD_LOCK_R=0x4C4F434B; GPIO_PORTD_CR_R=0x03;
    GPIO_PORTD_AMSEL_R=0; GPIO_PORTD_PCTL_R=0;
    GPIO_PORTD_DIR_R=0x03; GPIO_PORTD_AFSEL_R=0;
    GPIO_PORTD_PUR_R=0x03; GPIO_PORTD_DEN_R=0x03;}
void SysInit(void){
    NVIC_ST_CTRL_R=0; NVIC_ST_CURRENT_R=0;
    NVIC_ST_CTRL_R=0x05;}
void SysLoad(unsigned long p){
    NVIC_ST_RELOAD_R=p-1; NVIC_ST_CURRENT_R=0;
    while((NVIC_ST_CTRL_R&0x10000)==0);}
void SysFun(void){ NVIC_ST_CTRL_R=0; NVIC_ST_CTRL_R=0x05; }
int main(void){
    PLL_Init(); UART_Init(); PortD_Init(); SysFun(); SysInit();
    while(1){
        UART_OutString("Enter Speed and Direction: ");
        UART_InString(string,19);
        if(!strcmp(string,"50 clock")){
            UART_OutChar(LF);
            UART_OutString("Motor at 50% CW");
            while(UART0_FR_R&0x10){
                GPIO_PORTD_DATA_R=0x01; SysLoad(400000);
                GPIO_PORTD_DATA_R=0x00; SysLoad(400000);}}
        else if(!strcmp(string,"50 anticlock")){
            UART_OutChar(LF);
            UART_OutString("Motor at 50% CCW");
            while(UART0_FR_R&0x10){
                GPIO_PORTD_DATA_R=0x02; SysLoad(400000);
                GPIO_PORTD_DATA_R=0x00; SysLoad(400000);}}
        else if(!strcmp(string,"75 clock")){
            UART_OutChar(LF);
            UART_OutString("Motor at 75% CW");
            while(UART0_FR_R&0x10){
                GPIO_PORTD_DATA_R=0x01; SysLoad(600000);
                GPIO_PORTD_DATA_R=0x00; SysLoad(200000);}}
        else if(!strcmp(string,"90 clock")){
            UART_OutChar(LF);
            UART_OutString("Motor at 90% CW");
            while(UART0_FR_R&0x10){
                GPIO_PORTD_DATA_R=0x01; SysLoad(720000);
                GPIO_PORTD_DATA_R=0x00; SysLoad(80000);}}
        else if(!strcmp(string,"90 anticlock")){
            UART_OutChar(LF);
            UART_OutString("Motor at 90% CCW");
            while(UART0_FR_R&0x10){
                GPIO_PORTD_DATA_R=0x02; SysLoad(720000);
                GPIO_PORTD_DATA_R=0x00; SysLoad(80000);}}
        else{
            GPIO_PORTD_DATA_R=0x00;
            UART_OutChar(LF);
            UART_OutString("No operation");}
        UART_OutChar(LF);}}
