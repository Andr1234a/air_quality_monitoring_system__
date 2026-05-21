#include "uart_driver.h"
#include "stm8_s.h"


//Initializes UART1 peripheral
void UART1_Init(unsigned long f_cpu, unsigned long baudrate)
{
    unsigned int uart_div;

    
    uart_div = (unsigned int)(f_cpu / baudrate);

    UART1_BRR2 = (unsigned char)(((uart_div >> 8) & 0xF0) | (uart_div & 0x0F));
    UART1_BRR1 = (unsigned char)(uart_div >> 4);

    
    UART1_CR1 = 0x00;

    UART1_CR2 = UART1_CR2_TEN | UART1_CR2_REN;

    UART1_CR3 = 0x00;
}

//Sends a single character via UART1
void UART1_SendChar(char c)
{
    while (!(UART1_SR & UART1_SR_TXE));
    UART1_DR = (unsigned char)c;
}

//Sends a null-terminated string via UART1
void UART1_SendString(const char *s)
{
    while (*s)
    {
        UART1_SendChar(*s++);
    }
}

//Receives a single character via UART1
char UART1_ReceiveChar(void)
{
    while (!(UART1_SR & UART1_SR_RXNE));
    return (char)UART1_DR;
}

//Checks if UART1 has received data
unsigned char UART1_DataReady(void)
{
    if (UART1_SR & UART1_SR_RXNE) return 1;
    return 0;
}

//Sends a signed integer value via UART1
void UART1_SendInt(int value)
{
    char buf[11]; 
    char *p;
    unsigned int v;

    p = buf + sizeof(buf) - 1;
    *p = '\0';

    if (value < 0) v = (unsigned int)(-value);
    else v = (unsigned int)value;

    do {
        *--p = (v % 10) + '0';
        v /= 10;
    } while (v != 0);

    if (value < 0) *--p = '-';

    UART1_SendString(p);
}

//Sends a floating-point value via UART1 (simple format)
void UART1_SendFloatSimple(float val)
{
    int whole;
    int frac;

    whole = (int)val;
    frac = (int)((val - whole) * 100.0f); 

    if (frac < 0) frac = -frac;

    UART1_SendInt(whole);
    UART1_SendChar('.');
    UART1_SendInt(frac);
}