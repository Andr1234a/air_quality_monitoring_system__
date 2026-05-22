#include "delay.h"
#include "stm8_s.h"

/* Calibration factors for loop-based delay calculation at given F_CPU */
#define DELAY_MS_CALIBRATION_FACTOR 18000UL
#define DELAY_US_CALIBRATION_FACTOR 18000000UL

// Creates a blocking delay in milliseconds
void delay_ms(uint16_t ms)
{
    volatile uint32_t i;
    for (i = 0; i < ((F_CPU / DELAY_MS_CALIBRATION_FACTOR) * ms); i++)
        ;
}

// Creates a blocking delay in microseconds
void delay_us(uint16_t us)
{
    volatile uint32_t i;
    for (i = 0; i < ((F_CPU / DELAY_US_CALIBRATION_FACTOR) * us); i++)
        ;
}