#include "tim1_driver.h"
#include "gpio_driver.h"
#include "stm8_s.h"

/* CCMR Register Input/Output mode configurations */
#define TIM1_CCMR_INPUT_CHANNEL_1 0x01
#define TIM1_CCMR_PWM_MODE_1 0x60

/* CCER Register Channel Enables */
#define TIM1_CCER_CH1_CH3_ENABLE 0x01
#define TIM1_CCER_CH2_CH4_ENABLE 0x10

/* BKR & CR1 Register Bit Masks */
#define TIM1_BKR_MOE 0x80
#define TIM1_CR1_CEN 0x01

/* GPIO Magic Numbers */
#define GPIO_PIN_4 4

/* General Math & Limit Constants */
#define TIM1_REG_SHIFT_8 8
#define TIM1_REG_LOW_BYTE_MASK 0xFF
#define TIM1_ARR_MAX_VALUE 65535UL
#define TIM1_PRESCALER_MAX_VALUE 65536UL

void TIM1_DeInit(void)
{
    TIM1_CR1 = TIM1_CR1_RESET_VALUE;
    TIM1_CR2 = TIM1_CR2_RESET_VALUE;
    TIM1_SMCR = TIM1_SMCR_RESET_VALUE;
    TIM1_ETR = TIM1_ETR_RESET_VALUE;
    TIM1_IER = TIM1_IER_RESET_VALUE;
    TIM1_SR2 = TIM1_SR2_RESET_VALUE;
    /* Disable channels */
    TIM1_CCER1 = TIM1_CCER1_RESET_VALUE;
    TIM1_CCER2 = TIM1_CCER2_RESET_VALUE;
    /* Configure channels as inputs: it is necessary if lock level is equal to 2 or 3 */
    TIM1_CCMR1 = TIM1_CCMR_INPUT_CHANNEL_1;
    TIM1_CCMR2 = TIM1_CCMR_INPUT_CHANNEL_1;
    TIM1_CCMR3 = TIM1_CCMR_INPUT_CHANNEL_1;
    TIM1_CCMR4 = TIM1_CCMR_INPUT_CHANNEL_1;
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
    TIM1_ARRH = TIM1_ARRH_RESET_VALUE;
    TIM1_ARRL = TIM1_ARRL_RESET_VALUE;
    TIM1_CCR1H = TIM1_CCR1H_RESET_VALUE;
    TIM1_CCR1L = TIM1_CCR1L_RESET_VALUE;
    TIM1_CCR2H = TIM1_CCR2H_RESET_VALUE;
    TIM1_CCR2L = TIM1_CCR2L_RESET_VALUE;
    TIM1_CCR3H = TIM1_CCR3H_RESET_VALUE;
    TIM1_CCR3L = TIM1_CCR3L_RESET_VALUE;
    TIM1_CCR4H = TIM1_CCR4H_RESET_VALUE;
    TIM1_CCR4L = TIM1_CCR4L_RESET_VALUE;
    TIM1_OISR = TIM1_OISR_RESET_VALUE;
    TIM1_EGR = TIM1_EGR_UG;
    TIM1_DTR = TIM1_DTR_RESET_VALUE;
    TIM1_BKR = TIM1_BKR_RESET_VALUE;
    TIM1_RCR = TIM1_RCR_RESET_VALUE;
    TIM1_SR1 = TIM1_SR1_RESET_VALUE;
}

void TIM1_InitPWM(uint8_t channel, uint16_t period, uint16_t prescaler)
{
    TIM1_DeInit();

    TIM1_PSCRH = (uint8_t)(prescaler >> TIM1_REG_SHIFT_8);
    TIM1_PSCRL = (uint8_t)(prescaler & TIM1_REG_LOW_BYTE_MASK);
    TIM1_ARRH = (uint8_t)(period >> TIM1_REG_SHIFT_8);
    TIM1_ARRL = (uint8_t)(period & TIM1_REG_LOW_BYTE_MASK);

    PC_DDR |= (1 << GPIO_PIN_4);
    PC_CR1 |= (1 << GPIO_PIN_4);
    PC_CR2 |= (1 << GPIO_PIN_4);

    switch (channel)
    {
    case 1:
        TIM1_CCMR1 = TIM1_CCMR_PWM_MODE_1;
        TIM1_CCER1 |= TIM1_CCER_CH1_CH3_ENABLE;
        break;
    case 2:
        TIM1_CCMR2 = TIM1_CCMR_PWM_MODE_1;
        TIM1_CCER1 |= TIM1_CCER_CH2_CH4_ENABLE;
        break;
    case 3:
        TIM1_CCMR3 = TIM1_CCMR_PWM_MODE_1;
        TIM1_CCER2 |= TIM1_CCER_CH1_CH3_ENABLE;
        break;
    case 4:
        TIM1_CCMR4 = TIM1_CCMR_PWM_MODE_1;
        TIM1_CCER2 |= TIM1_CCER_CH2_CH4_ENABLE;
        break;
    default:
        return;
    }

    TIM1_BKR |= TIM1_BKR_MOE;
}

void TIM1_PWM_SetDuty(uint8_t channel, uint16_t duty)
{
    uint16_t arr = ((uint16_t)TIM1_ARRH << TIM1_REG_SHIFT_8) | TIM1_ARRL;
    if (duty > arr)
        duty = arr;

    switch (channel)
    {
    case 1:
        TIM1_CCR1H = duty >> TIM1_REG_SHIFT_8;
        TIM1_CCR1L = duty;
        break;
    case 2:
        TIM1_CCR2H = duty >> TIM1_REG_SHIFT_8;
        TIM1_CCR2L = duty;
        break;
    case 3:
        TIM1_CCR3H = duty >> TIM1_REG_SHIFT_8;
        TIM1_CCR3L = duty;
        break;
    case 4:
        TIM1_CCR4H = duty >> TIM1_REG_SHIFT_8;
        TIM1_CCR4L = duty;
        break;
    default:
        break;
    }
}

void TIM1_PWM_SetFrequency(uint8_t channel, uint32_t duty, uint32_t freq_hz)
{
    uint32_t prescaler = 1;
    uint32_t arr = (F_CPU / freq_hz) - 1;

    while (arr > TIM1_ARR_MAX_VALUE && prescaler < TIM1_PRESCALER_MAX_VALUE)
    {
        prescaler++;
        arr = (F_CPU / (prescaler * freq_hz)) - 1;
    }
    if (arr > TIM1_ARR_MAX_VALUE)
        arr = TIM1_ARR_MAX_VALUE;

    TIM1_PSCRH = (uint8_t)(prescaler >> TIM1_REG_SHIFT_8);
    TIM1_PSCRL = (uint8_t)(prescaler & TIM1_REG_LOW_BYTE_MASK);
    TIM1_ARRH = (uint8_t)(arr >> TIM1_REG_SHIFT_8);
    TIM1_ARRL = (uint8_t)(arr & TIM1_REG_LOW_BYTE_MASK);

    TIM1_EGR |= TIM1_EGR_UG;
    TIM1_PWM_SetDuty(channel, duty);
}

void TIM1_Start(void) { TIM1_CR1 |= TIM1_CR1_CEN; }
void TIM1_Stop(void) { TIM1_CR1 &= (uint8_t)(~TIM1_CR1_CEN); }