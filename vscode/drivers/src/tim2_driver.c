#include "tim2_driver.h"
#include <stdint.h>

void TIM2_DeInit(void)
{

    TIM2_CR1 = (uint8_t)TIM2_CR1_RESET_VALUE;
    TIM2_IER = (uint8_t)TIM2_IER_RESET_VALUE;
    TIM2_SR2 = (uint8_t)TIM2_SR2_RESET_VALUE;

    /* Disable channels */
    TIM2_CCER1 = (uint8_t)TIM2_CCER1_RESET_VALUE;
    TIM2_CCER2 = (uint8_t)TIM2_CCER2_RESET_VALUE;

    /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
    TIM2_CCER1 = (uint8_t)TIM2_CCER1_RESET_VALUE;
    TIM2_CCER2 = (uint8_t)TIM2_CCER2_RESET_VALUE;
    TIM2_CCMR1 = (uint8_t)TIM2_CCMR1_RESET_VALUE;
    TIM2_CCMR2 = (uint8_t)TIM2_CCMR2_RESET_VALUE;
    TIM2_CCMR3 = (uint8_t)TIM2_CCMR3_RESET_VALUE;
    TIM2_CNTRH = (uint8_t)TIM2_CNTRH_RESET_VALUE;
    TIM2_CNTRL = (uint8_t)TIM2_CNTRL_RESET_VALUE;
    TIM2_PSCR = (uint8_t)TIM2_PSCR_RESET_VALUE;
    TIM2_ARRH = (uint8_t)TIM2_ARRH_RESET_VALUE;
    TIM2_ARRL = (uint8_t)TIM2_ARRL_RESET_VALUE;
    TIM2_CCR1H = (uint8_t)TIM2_CCR1H_RESET_VALUE;
    TIM2_CCR1L = (uint8_t)TIM2_CCR1L_RESET_VALUE;
    TIM2_CCR2H = (uint8_t)TIM2_CCR2H_RESET_VALUE;
    TIM2_CCR2L = (uint8_t)TIM2_CCR2L_RESET_VALUE;
    TIM2_CCR3H = (uint8_t)TIM2_CCR3H_RESET_VALUE;
    TIM2_CCR3L = (uint8_t)TIM2_CCR3L_RESET_VALUE;
    TIM2_SR1 = (uint8_t)TIM2_SR1_RESET_VALUE;
}

void TIM2_TimeBaseInit(TIM2_Prescaler_TypeDef TIM2_Prescaler,
                       uint16_t TIM2_Period)
{
    /* Set the Prescaler value */
    TIM2_PSCR = (uint8_t)(TIM2_Prescaler);
    /* Set the Autoreload value */
    TIM2_ARRH = (uint8_t)(TIM2_Period >> 8);
    TIM2_ARRL = (uint8_t)(TIM2_Period);
}

void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler,
                          TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode)
{

    /* Set the Prescaler value */
    TIM2_PSCR = (uint8_t)Prescaler;

    /* Set or reset the UG Bit */
    TIM2_EGR = (uint8_t)TIM2_PSCReloadMode;
}

void TIM2_Cmd(FunctionalState NewState)
{

    /* set or Reset the CEN Bit */
    if (NewState != DISABLE)
    {
        TIM2_CR1 |= (uint8_t)TIM2_CR1_CEN;
    }
    else
    {
        TIM2_CR1 &= (uint8_t)(~TIM2_CR1_CEN);
    }
}

/**
 * @brief  Configure the TI1 as Input.
 * @param   TIM2_ICPolarity  The Input Polarity.
 * This parameter can be one of the following values:
 *                       - TIM2_ICPOLARITY_FALLING
 *                       - TIM2_ICPOLARITY_RISING
 * @param   TIM2_ICSelection specifies the input to be used.
 * This parameter can be one of the following values:
 *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 1 is selected to
 *                         be connected to IC1.
 *                       - TIM2_ICSELECTION_INDIRECTTI: TIM2 Input 1 is selected to
 *                         be connected to IC2.
 * @param   TIM2_ICFilter Specifies the Input Capture Filter.
 * This parameter must be a value between 0x00 and 0x0F.
 * @retval None
 */
static void TI1_Config(uint8_t TIM2_ICPolarity,
                       uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
    /* Disable the Channel 1: Reset the CCE Bit */
    TIM2_CCER1 &= (uint8_t)(~TIM2_CCER1_CC1E);

    /* Select the Input and set the filter */
    TIM2_CCMR1 = (uint8_t)((uint8_t)(TIM2_CCMR1 & (uint8_t)(~(uint8_t)(TIM2_CCMR_CCxS | TIM2_CCMR_ICxF))) | (uint8_t)(((TIM2_ICSelection)) | ((uint8_t)(TIM2_ICFilter << 4))));

    /* Select the Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
    {
        TIM2_CCER1 |= TIM2_CCER1_CC1P;
    }
    else
    {
        TIM2_CCER1 &= (uint8_t)(~TIM2_CCER1_CC1P);
    }
    /* Set the CCE Bit */
    TIM2_CCER1 |= TIM2_CCER1_CC1E;
}

/**
 * @brief  Configures the TIM2 Channel 2 as Input Capture.
 * @note   This is a static internal helper function used by the driver
 *         to encapsulate the hardware register mutations for Channel 2.
 *
 * @param[in] TIM2_ICPolarity   Specifies the active edge for input capture.
 *                              This parameter can be one of the following values:
 *                              @arg TIM2_ICPOLARITY_RISING: Capture triggered by rising edge.
 *                              @arg TIM2_ICPOLARITY_FALLING: Capture triggered by falling edge.
 * @param[in] TIM2_ICSelection  Specifies the input source selection.
 *                              This parameter maps directly to CC2S bits and can be:
 *                              @arg TIM2_ICSELECTION_DIRECTTI: Channel mapped on TI2.
 *                              @arg TIM2_ICSELECTION_INDIRECTTI: Channel mapped on TI1.
 *                              @arg TIM2_ICSELECTION_TRGI: Channel mapped on TRGI.
 * @param[in] TIM2_ICFilter     Specifies the input capture filter coefficient.
 *                              Must be a 4-bit value between 0x00 and 0x0F, which will
 *                              be shifted internally into the IC2F bitfield.
 *
 * @return None
 */
static void TI2_Config(uint8_t TIM2_ICPolarity,
                       uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
    /* Disable the Channel 2: Reset the CCE Bit */
    TIM2_CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);

    /* Select the Input and set the filter */
    TIM2_CCMR2 = (uint8_t)((uint8_t)(TIM2_CCMR2 & (uint8_t)(~(uint8_t)(TIM2_CCMR_CCxS | TIM2_CCMR_ICxF))) | (uint8_t)(((TIM2_ICSelection)) | ((uint8_t)(TIM2_ICFilter << 4))));

    /* Select the Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
    {
        TIM2_CCER1 |= TIM2_CCER1_CC2P;
    }
    else
    {
        TIM2_CCER1 &= (uint8_t)(~TIM2_CCER1_CC2P);
    }

    /* Set the CCE Bit */
    TIM2_CCER1 |= TIM2_CCER1_CC2E;
}

/**
 * @brief  Configure the TI3 as Input.
 * @param   TIM2_ICPolarity  The Input Polarity.
 * This parameter can be one of the following values:
 *                       - TIM2_ICPOLARITY_FALLING
 *                       - TIM2_ICPOLARITY_RISING
 * @param   TIM2_ICSelection specifies the input to be used.
 * This parameter can be one of the following values:
 *                       - TIM2_ICSELECTION_DIRECTTI: TIM2 Input 3 is selected to
 *                         be connected to IC3.
 * @param   TIM2_ICFilter Specifies the Input Capture Filter.
 * This parameter must be a value between 0x00 and 0x0F.
 * @retval None
 */
static void TI3_Config(uint8_t TIM2_ICPolarity, uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
    /* Disable the Channel 3: Reset the CCE Bit */
    TIM2_CCER2 &= (uint8_t)(~TIM2_CCER2_CC3E);

    /* Select the Input and set the filter */
    TIM2_CCMR3 = (uint8_t)((uint8_t)(TIM2_CCMR3 & (uint8_t)(~(TIM2_CCMR_CCxS | TIM2_CCMR_ICxF))) | (uint8_t)(((TIM2_ICSelection)) | ((uint8_t)(TIM2_ICFilter << 4))));

    /* Select the Polarity */
    if (TIM2_ICPolarity != TIM2_ICPOLARITY_RISING)
    {
        TIM2_CCER2 |= TIM2_CCER2_CC3P;
    }
    else
    {
        TIM2_CCER2 &= (uint8_t)(~TIM2_CCER2_CC3P);
    }
    /* Set the CCE Bit */
    TIM2_CCER2 |= TIM2_CCER2_CC3E;
}

void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel,
                 TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                 TIM2_ICSelection_TypeDef TIM2_ICSelection,
                 TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                 uint8_t TIM2_ICFilter)
{

    if (TIM2_Channel == TIM2_CHANNEL_1)
    {
        /* TI1 Configuration */
        TI1_Config((uint8_t)TIM2_ICPolarity,
                   (uint8_t)TIM2_ICSelection,
                   (uint8_t)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC1Prescaler(TIM2_ICPrescaler);
    }
    else if (TIM2_Channel == TIM2_CHANNEL_2)
    {
        /* TI2 Configuration */
        TI2_Config((uint8_t)TIM2_ICPolarity,
                   (uint8_t)TIM2_ICSelection,
                   (uint8_t)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC2Prescaler(TIM2_ICPrescaler);
    }
    else
    {
        /* TI3 Configuration */
        TI3_Config((uint8_t)TIM2_ICPolarity,
                   (uint8_t)TIM2_ICSelection,
                   (uint8_t)TIM2_ICFilter);

        /* Set the Input Capture Prescaler value */
        TIM2_SetIC3Prescaler(TIM2_ICPrescaler);
    }
}

void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState)
{

    if (NewState != DISABLE)
    {
        /* Enable the Interrupt sources */
        TIM2_IER |= (uint8_t)TIM2_IT;
    }
    else
    {
        /* Disable the Interrupt sources */
        TIM2_IER &= (uint8_t)(~TIM2_IT);
    }
}

ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT)
{
    ITStatus bitstatus = RESET;
    uint8_t TIM2_itStatus = 0, TIM2_itEnable = 0;

    TIM2_itStatus = (uint8_t)(TIM2_SR1 & TIM2_IT);

    TIM2_itEnable = (uint8_t)(TIM2_IER & TIM2_IT);

    if ((TIM2_itStatus != (uint8_t)RESET) && (TIM2_itEnable != (uint8_t)RESET))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return (ITStatus)(bitstatus);
}

void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT)
{

    /* Clear the IT pending Bit */
    TIM2_SR1 = (uint8_t)(~TIM2_IT);
}

uint16_t TIM2_GetCapture3(void)
{
    /* Get the Capture 3 Register value */
    uint16_t tmpccr3 = 0;
    uint8_t tmpccr3l = 0, tmpccr3h = 0;

    tmpccr3h = TIM2_CCR3H;
    tmpccr3l = TIM2_CCR3L;

    tmpccr3 = (uint16_t)(tmpccr3l);
    tmpccr3 |= (uint16_t)((uint16_t)tmpccr3h << 8);
    /* Get the Capture 3 Register value */
    return (uint16_t)tmpccr3;
}

void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler)
{

    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2_CCMR1 = (uint8_t)((uint8_t)(TIM2_CCMR1 & (uint8_t)(~TIM2_CCMR_ICxPSC)) | (uint8_t)TIM2_IC1Prescaler);
}

void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler)
{

    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2_CCMR2 = (uint8_t)((uint8_t)(TIM2_CCMR2 & (uint8_t)(~TIM2_CCMR_ICxPSC)) | (uint8_t)TIM2_IC2Prescaler);
}

void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler)
{

    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2_CCMR3 = (uint8_t)((uint8_t)(TIM2_CCMR3 & (uint8_t)(~TIM2_CCMR_ICxPSC)) | (uint8_t)TIM2_IC3Prescaler);
}
