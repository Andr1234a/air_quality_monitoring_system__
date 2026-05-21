#include "encoder.h"

void Encoder_Enable(void)
{
    TIM1_CR1 &= (uint8_t)(~0x01);

    TIM1_IER = 0x00;
    TIM1_SR1 = 0x00;

    TIM1_CCER1 = 0x00;
    TIM1_CCER2 = 0x00;

    TIM1_SMCR = 0x03;

    TIM1_CCMR1 = 0x01;
    TIM1_CCMR2 = 0x01;

    TIM1_ARRH = (uint8_t)(300 >> 8);
    TIM1_ARRL = (uint8_t)(300 & 0xFF);

    TIM1_CNTRH = 0;
    TIM1_CNTRL = 0;

    TIM1_CR1 |= 0x01;
}

uint16_t Encoder_GetValue(void)
{
    volatile uint8_t high = TIM1_CNTRH;
    volatile uint8_t low = TIM1_CNTRL;

    return (uint16_t)((high << 8) | low);
}

void Encoder_SetValue(uint16_t val)
{
    TIM1_CNTRH = (uint8_t)(val >> 8);
    TIM1_CNTRL = (uint8_t)(val & 0xFF);
}