#include "encoder.h"

#define TIM1_CR1_CEN ((uint8_t)0x01) /**< Counter Enable bit */

#define TIM1_IER_DISABLE_ALL ((uint8_t)0x00)  /**< Disable all TIM1 interrupts */
#define TIM1_SR1_CLEAR_ALL ((uint8_t)0x00)    /**< Clear all status flags */
#define TIM1_CCER_DISABLE_ALL ((uint8_t)0x00) /**< Disable all capture/compare outputs */

#define TIM1_SMCR_SMS_TRIGGER ((uint8_t)0x03) /**< Slave Mode Selection: Trigger Mode */
#define TIM1_CCMR_CCxS_INPUT ((uint8_t)0x01)  /**< CCx channel configured as input, mapped on TIx */

#define TIM1_ARR_VALUE 300                 /**< Auto-reload period value */
#define TIM1_CNTR_RESET 0                  /**< Value to reset counter registers */
#define DATA_LOW_BYTE_MASK ((uint8_t)0xFF) /**< Mask to extract the lower 8 bits */
#define SHIFT_ONE_BYTE 8                   /**< Shift value to access the upper byte of a 16-bit word */

void Encoder_Enable(void)
{

    TIM1_CR1 &= (uint8_t)(~TIM1_CR1_CEN);

    TIM1_IER = TIM1_IER_DISABLE_ALL;
    TIM1_SR1 = TIM1_SR1_CLEAR_ALL;

    TIM1_CCER1 = TIM1_CCER_DISABLE_ALL;
    TIM1_CCER2 = TIM1_CCER_DISABLE_ALL;

    TIM1_SMCR = TIM1_SMCR_SMS_TRIGGER;

    TIM1_CCMR1 = TIM1_CCMR_CCxS_INPUT;
    TIM1_CCMR2 = TIM1_CCMR_CCxS_INPUT;

    TIM1_ARRH = (uint8_t)(TIM1_ARR_VALUE >> SHIFT_ONE_BYTE);
    TIM1_ARRL = (uint8_t)(TIM1_ARR_VALUE & DATA_LOW_BYTE_MASK);

    TIM1_CNTRH = TIM1_CNTR_RESET;
    TIM1_CNTRL = TIM1_CNTR_RESET;

    TIM1_CR1 |= TIM1_CR1_CEN;
}

uint16_t Encoder_GetValue(void)
{
    volatile uint8_t high = TIM1_CNTRH;
    volatile uint8_t low = TIM1_CNTRL;

    return (uint16_t)((high << SHIFT_ONE_BYTE) | low);
}

void Encoder_SetValue(uint16_t val)
{
    TIM1_CNTRH = (uint8_t)(val >> SHIFT_ONE_BYTE);
    TIM1_CNTRL = (uint8_t)(val & DATA_LOW_BYTE_MASK);
}