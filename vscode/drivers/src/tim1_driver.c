#include "tim1_driver.h"
#include "gpio_driver.h"
#include "stm8_s.h"


void TIM1_DeInit(void){
    TIM1_CR1  = TIM1_CR1_RESET_VALUE;
    TIM1_CR2  = TIM1_CR2_RESET_VALUE;
    TIM1_SMCR = TIM1_SMCR_RESET_VALUE;
    TIM1_ETR  = TIM1_ETR_RESET_VALUE;
    TIM1_IER  = TIM1_IER_RESET_VALUE;
    TIM1_SR2  = TIM1_SR2_RESET_VALUE;
    /* Disable channels */
    TIM1_CCER1 = TIM1_CCER1_RESET_VALUE;
    TIM1_CCER2 = TIM1_CCER2_RESET_VALUE;
    /* Configure channels as inputs: it is necessary if lock level is equal to 2 or 3 */
    TIM1_CCMR1 = 0x01;
    TIM1_CCMR2 = 0x01;
    TIM1_CCMR3 = 0x01;
    TIM1_CCMR4 = 0x01;
    /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
    TIM1_CCER1 = TIM1_CCER1_RESET_VALUE;
    TIM1_CCER2 = TIM1_CCER2_RESET_VALUE;
    TIM1_CCMR1 = TIM1_CCMR1_RESET_VALUE;
    TIM1_CCMR2 = TIM1_CCMR2_RESET_VALUE;
    TIM1_CCMR3 = TIM1_CCMR3_RESET_VALUE;
    TIM1_CCMR4 = TIM1_CCMR4_RESET_VALUE;
    TIM1_CNTRH = TIM1_CNTRH_RESET_VALUE;
    TIM1_CNTRL = TIM1_CNTRL_RESET_VALUE;
    TIM1_PSCRH = TIM1_PSCRH_RESET_VALUE;
    TIM1_PSCRL = TIM1_PSCRL_RESET_VALUE;
    TIM1_ARRH  = TIM1_ARRH_RESET_VALUE;
    TIM1_ARRL  = TIM1_ARRL_RESET_VALUE;
    TIM1_CCR1H = TIM1_CCR1H_RESET_VALUE;
    TIM1_CCR1L = TIM1_CCR1L_RESET_VALUE;
    TIM1_CCR2H = TIM1_CCR2H_RESET_VALUE;
    TIM1_CCR2L = TIM1_CCR2L_RESET_VALUE;
    TIM1_CCR3H = TIM1_CCR3H_RESET_VALUE;
    TIM1_CCR3L = TIM1_CCR3L_RESET_VALUE;
    TIM1_CCR4H = TIM1_CCR4H_RESET_VALUE;
    TIM1_CCR4L = TIM1_CCR4L_RESET_VALUE;
    TIM1_OISR  = TIM1_OISR_RESET_VALUE;
    TIM1_EGR   = 0x01; /* TIM1_EGR_UG */
    TIM1_DTR   = TIM1_DTR_RESET_VALUE;
    TIM1_BKR   = TIM1_BKR_RESET_VALUE;
    TIM1_RCR   = TIM1_RCR_RESET_VALUE;
    TIM1_SR1   = TIM1_SR1_RESET_VALUE;  
}
void TIM1_InitPWM(uint8_t channel, uint16_t period, uint16_t prescaler)
{
    TIM1_DeInit();

    TIM1_PSCRH = (uint8_t)(prescaler >> 8);
    TIM1_PSCRL = (uint8_t)(prescaler & 0xFF);
    TIM1_ARRH  = (uint8_t)(period >> 8);
    TIM1_ARRL  = (uint8_t)(period & 0xFF);

    switch(channel)
    {
        case 1: TIM1_CCMR1 = 0x60; TIM1_CCER1 |= 0x01; break;
        case 2: TIM1_CCMR2 = 0x60; TIM1_CCER1 |= 0x10; break;
        case 3: TIM1_CCMR3 = 0x60; TIM1_CCER2 |= 0x01; break;
        case 4: TIM1_CCMR4 = 0x60; TIM1_CCER2 |= 0x10; break;
        default: return;
    }

    TIM1_BKR |= 0x80; 
}

void TIM1_PWM_SetDuty(uint8_t channel, uint16_t duty)
{
    uint16_t arr = ((uint16_t)TIM1_ARRH << 8) | TIM1_ARRL;
    if(duty > arr) duty = arr;

    switch(channel)
    {
        case 1: TIM1_CCR1H = duty >> 8; TIM1_CCR1L = duty; break;
        case 2: TIM1_CCR2H = duty >> 8; TIM1_CCR2L = duty; break;
        case 3: TIM1_CCR3H = duty >> 8; TIM1_CCR3L = duty; break;
        case 4: TIM1_CCR4H = duty >> 8; TIM1_CCR4L = duty; break;
        default: break;
    }
}

void TIM1_PWM_SetFrequency(uint8_t channel, uint32_t duty, uint32_t freq_hz)
{
    uint32_t prescaler = 1;
    uint32_t arr = (F_CPU/freq_hz)-1;

    while(arr > 65535 && prescaler<65536)
    {
        prescaler++;
        arr = (F_CPU/(prescaler*freq_hz))-1;
    }
    if(arr>65535) arr=65535;

    TIM1_PSCRH = (uint8_t)(prescaler>>8);
    TIM1_PSCRL = (uint8_t)(prescaler&0xFF);
    TIM1_ARRH  = (uint8_t)(arr>>8);
    TIM1_ARRL  = (uint8_t)(arr&0xFF);

    TIM1_EGR |= TIM1_EGR_UG;
    TIM1_PWM_SetDuty(channel,duty);
}

void TIM1_Start(void) { TIM1_CR1 |= 0x01; }
void TIM1_Stop(void)  { TIM1_CR1 &= ~0x01; }


void TIM1_Encoder_Init(void)
{
    TIM1_DeInit();
    CLK_PCKENR1 |= (1 << 7);

    /* PC6, PC7 input pull-up */
    PC_DDR &= ~((1<<6) | (1<<7));
    PC_CR1 |=  ((1<<6) | (1<<7));
    PC_CR2 &= ~((1<<6) | (1<<7));

    TIM1_CCMR1 = 0x01;
    TIM1_CCMR2 = 0x01;

    TIM1_CCER1 = (1 << 0) | (1 << 4);
    TIM1_SMCR  = 0x03;

    TIM1_ARRH = 0xFF;
    TIM1_ARRL = 0xFF;

    TIM1_BKR |= (1 << 7); // MOE

    TIM1_CNTRH = 0;
    TIM1_CNTRL = 0;

    TIM1_CR1 |= 0x01;
}


int16_t TIM1_Encoder_Get(void)
{
    uint8_t h = TIM1_CNTRH;
    uint8_t l = TIM1_CNTRL;
    return (int16_t)((h << 8) | l);
}