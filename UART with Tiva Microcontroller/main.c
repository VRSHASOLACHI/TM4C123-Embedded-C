#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "PLL.h"
#include "string.h"
char string[20];
#define CR 0x0D
#define LF 0x0A
#define BS 0x08
void UART_Init(void) {
    SYSCTL_RCGC1_R |= 0x01;
    SYSCTL_RCGC2_R |= 0x01;
    UART0_CTL_R &= ~(0x01);
    UART0_IBRD_R = 43;
    UART0_FBRD_R = 26;
    UART0_LCRH_R = 0x70;
    UART0_CTL_R |= 0x01;
    GPIO_PORTA_AFSEL_R |= 0x03;
    GPIO_PORTA_DEN_R |= 0x03;
    GPIO_PORTA_PCTL_R = 0x00000011;
    GPIO_PORTA_AMSEL_R &= ~0x03;}
void UART_OutChar(unsigned char data) {
    while ((UART0_FR_R & 0x20) != 0);
    UART0_DR_R = data;}
void UART_OutString(char *pt) {
    while (*pt) {
        UART_OutChar(*pt);
        pt++;}}
unsigned char UART_InChar(void) {
    while ((UART0_FR_R & 0x10) != 0);
    return (unsigned char)(UART0_DR_R & 0xFF);}
void UART_InString(char *bufPt, unsigned short max) {
    int length = 0;
    char character;
    character = UART_InChar();
    while (character != CR) {
        if (character == BS) {
            if (length) {
                bufPt--;
                length--;
                UART_OutChar(BS);}}
        else if (length < max) {
            *bufPt = character;
            bufPt++;
            length++;
            UART_OutChar(character);}
        character = UART_InChar();}
    *bufPt = 0;}
void PortF_Init(void) {unsigned long delay;
    SYSCTL_RCGC2_R |= 0x20;
    delay = SYSCTL_RCGC2_R;
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R = 0x1F;
    GPIO_PORTF_AMSEL_R = 0x00;
    GPIO_PORTF_PCTL_R = 0x00;
    GPIO_PORTF_DIR_R = 0x0E;
    GPIO_PORTF_AFSEL_R = 0x00;
    GPIO_PORTF_PUR_R = 0x11;
    GPIO_PORTF_DEN_R = 0x1F;}
int main(void) 
    PLL_Init();
    UART_Init();
    PortF_Init();
    while (1) {
        UART_OutString("Enter colour: ");
        UART_InString(string, 19);
        if (strcmp(string, "red") == 0) {
            GPIO_PORTF_DATA_R = 0x02;
            UART_OutChar(LF);
            UART_OutString("Red LED glowing");}
        else if (strcmp(string, "blue") == 0) {
            GPIO_PORTF_DATA_R = 0x04;
            UART_OutChar(LF);
            UART_OutString("Blue LED glowing");}
        else if (strcmp(string, "green") == 0) {
            GPIO_PORTF_DATA_R = 0x08;
            UART_OutChar(LF);
            UART_OutString("Green LED glowing");}
        else if (strcmp(string, "yellow") == 0) {
            GPIO_PORTF_DATA_R = 0x0A;
            UART_OutChar(LF);
            UART_OutString("Yellow LED glowing");}
        else if (strcmp(string, "cyan") == 0) {
            GPIO_PORTF_DATA_R = 0x0C;
            UART_OutChar(LF);
            UART_OutString("Cyan LED glowing");}
        else if (strcmp(string, "pink") == 0) {
            GPIO_PORTF_DATA_R = 0x06;
            UART_OutChar(LF);
            UART_OutString("Pink LED glowing");}
        else if (strcmp(string, "white") == 0) {
            GPIO_PORTF_DATA_R = 0x0E;
            UART_OutChar(LF);
            UART_OutString("White LED glowing");}
        else {GPIO_PORTF_DATA_R = 0x00;
            UART_OutChar(LF);
            UART_OutString("NO LED glowing");}
        UART_OutChar(LF);}
}
