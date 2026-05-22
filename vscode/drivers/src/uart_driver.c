#include "uart_driver.h"
#include "stm8_s.h"

/* UART Baud Rate Register (BRR) Masks & Shifts */
#define UART1_BRR2_HIGH_NIBBLE_MASK 0xF0u
#define UART1_BRR2_LOW_NIBBLE_MASK 0x0Fu
#define UART1_BRR_SHIFT_4 4
#define UART1_BRR_SHIFT_8 8

/* Control Register Reset values */
#define UART1_CR_DEFAULT 0x00

/* Integer to ASCII Conversion Constants */
#define INT_BUFFER_SIZE 11
#define DECIMAL_BASE 10

/* Simple Float Conversion Precision */
#define FLOAT_PRECISION_MULTIPLIER 100.0f

// Initializes UART1 peripheral
void UART1_Init(unsigned long f_cpu, unsigned long baudrate)
{
    unsigned int uart_div;

    uart_div = (unsigned int)(f_cpu / baudrate);

    UART1_BRR2 = (unsigned char)(((uart_div >> UART1_BRR_SHIFT_8) & UART1_BRR2_HIGH_NIBBLE_MASK) | (uart_div & UART1_BRR2_LOW_NIBBLE_MASK));
    UART1_BRR1 = (unsigned char)(uart_div >> UART1_BRR_SHIFT_4);

    UART1_CR1 = UART1_CR_DEFAULT;

    UART1_CR2 = UART1_CR2_TEN | UART1_CR2_REN;

    UART1_CR3 = UART1_CR_DEFAULT;
}

// Sends a single character via UART1
void UART1_SendChar(char c)
{
    while (!(UART1_SR & UART1_SR_TXE))
        ;
    UART1_DR = (unsigned char)c;
}

// Sends a null-terminated string via UART1
void UART1_SendString(const char *s)
{
    while (*s)
    {
        UART1_SendChar(*s++);
    }
}

// Receives a single character via UART1
char UART1_ReceiveChar(void)
{
    while (!(UART1_SR & UART1_SR_RXNE))
        ;
    return (char)UART1_DR;
}

// Checks if UART1 has received data
unsigned char UART1_DataReady(void)
{
    if (UART1_SR & UART1_SR_RXNE)
        return 1;
    return 0;
}

// Sends a signed integer value via UART1
void UART1_SendInt(int value)
{
    char buf[INT_BUFFER_SIZE];
    char *p;
    unsigned int v;

    p = buf + sizeof(buf) - 1;
    *p = '\0';

    if (value < 0)
        v = (unsigned int)(-value);
    else
        v = (unsigned int)value;

    do
    {
        *--p = (char)((v % DECIMAL_BASE) + '0');
        v /= DECIMAL_BASE;
    } while (v != 0);

    if (value < 0)
        *--p = '-';

    UART1_SendString(p);
}

// Sends a floating-point value via UART1 (simple format)
void UART1_SendFloatSimple(float val)
{
    int whole;
    int frac;

    whole = (int)val;
    frac = (int)((val - whole) * FLOAT_PRECISION_MULTIPLIER);

    if (frac < 0)
        frac = -frac;

    UART1_SendInt(whole);
    UART1_SendChar('.');
    UART1_SendInt(frac);
}