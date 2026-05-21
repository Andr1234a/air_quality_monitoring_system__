#include "delay.h"
#include "stm8_s.h"

//Creates a blocking delay in milliseconds
void delay_ms(uint16_t ms)
{
    volatile uint32_t i;
    for(i=0; i<((F_CPU/18000UL)*ms); i++);
}

//Creates a blocking delay in microseconds
void delay_us(uint16_t us)
{
    volatile uint32_t i;
    for (i = 0; i < ((F_CPU / 18000000UL) * us); i++);
}