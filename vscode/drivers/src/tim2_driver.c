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
    TIM2_ARRH  = (uint8_t)TIM2_ARRH_RESET_VALUE;
    TIM2_ARRL  = (uint8_t)TIM2_ARRL_RESET_VALUE;
    TIM2_CCR1H = (uint8_t)TIM2_CCR1H_RESET_VALUE;
    TIM2_CCR1L = (uint8_t)TIM2_CCR1L_RESET_VALUE;
    TIM2_CCR2H = (uint8_t)TIM2_CCR2H_RESET_VALUE;
    TIM2_CCR2L = (uint8_t)TIM2_CCR2L_RESET_VALUE;
    TIM2_CCR3H = (uint8_t)TIM2_CCR3H_RESET_VALUE;
    TIM2_CCR3L = (uint8_t)TIM2_CCR3L_RESET_VALUE;
    TIM2_SR1 = (uint8_t)TIM2_SR1_RESET_VALUE;
}


/**
  * @brief  Initializes the TIM2 Time Base Unit according to the specified parameters.
  * @param    TIM2_Prescaler specifies the Prescaler from TIM2_Prescaler_TypeDef.
  * @param    TIM2_Period specifies the Period value.
  * @retval None
  */
void TIM2_TimeBaseInit( TIM2_Prescaler_TypeDef TIM2_Prescaler,
                        uint16_t TIM2_Period)
{
    /* Set the Prescaler value */
    TIM2_PSCR = (uint8_t)(TIM2_Prescaler);
    /* Set the Autoreload value */
    TIM2_ARRH = (uint8_t)(TIM2_Period >> 8);
    TIM2_ARRL = (uint8_t)(TIM2_Period);
}

/**
  * @brief  Configures the TIM2 Prescaler.
  * @param   Prescaler specifies the Prescaler Register value
  * This parameter can be one of the following values
  *                       -  TIM2_PRESCALER_1
  *                       -  TIM2_PRESCALER_2
  *                       -  TIM2_PRESCALER_4
  *                       -  TIM2_PRESCALER_8
  *                       -  TIM2_PRESCALER_16
  *                       -  TIM2_PRESCALER_32
  *                       -  TIM2_PRESCALER_64
  *                       -  TIM2_PRESCALER_128
  *                       -  TIM2_PRESCALER_256
  *                       -  TIM2_PRESCALER_512
  *                       -  TIM2_PRESCALER_1024
  *                       -  TIM2_PRESCALER_2048
  *                       -  TIM2_PRESCALER_4096
  *                       -  TIM2_PRESCALER_8192
  *                       -  TIM2_PRESCALER_16384
  *                       -  TIM2_PRESCALER_32768
  * @param   TIM2_PSCReloadMode specifies the TIM2 Prescaler Reload mode.
  * This parameter can be one of the following values
  *                       - TIM2_PSCRELOADMODE_IMMEDIATE: The Prescaler is loaded
  *                         immediately.
  *                       - TIM2_PSCRELOADMODE_UPDATE: The Prescaler is loaded at
  *                         the update event.
  * @retval None
  */
void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler,
                          TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode)
{

    /* Set the Prescaler value */
    TIM2_PSCR = (uint8_t)Prescaler;

    /* Set or reset the UG Bit */
    TIM2_EGR = (uint8_t)TIM2_PSCReloadMode;
}

/**
  * @brief  Enables or disables the TIM2 peripheral.
  * @param   NewState new state of the TIM2 peripheral. This parameter can
  * be ENABLE or DISABLE.
  * @retval None
  */
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
    TIM2_CCMR1  = (uint8_t)((uint8_t)(TIM2_CCMR1 & (uint8_t)(~(uint8_t)( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF )))
                             | (uint8_t)(((TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));

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

static void TI2_Config(uint8_t TIM2_ICPolarity,
                       uint8_t TIM2_ICSelection,
                       uint8_t TIM2_ICFilter)
{
    /* Disable the Channel 2: Reset the CCE Bit */
    TIM2_CCER1 &= (uint8_t)(~TIM2_CCER1_CC2E);

    /* Select the Input and set the filter */
    TIM2_CCMR2 = (uint8_t)((uint8_t)(TIM2_CCMR2 & (uint8_t)(~(uint8_t)( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF ))) 
                            | (uint8_t)(( (TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));


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
    TIM2_CCER2 &=  (uint8_t)(~TIM2_CCER2_CC3E);

    /* Select the Input and set the filter */
    TIM2_CCMR3 = (uint8_t)((uint8_t)(TIM2_CCMR3 & (uint8_t)(~( TIM2_CCMR_CCxS | TIM2_CCMR_ICxF))) 
                            | (uint8_t)(( (TIM2_ICSelection)) | ((uint8_t)( TIM2_ICFilter << 4))));


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

/**
  * @brief  Initializes the TIM2 peripheral according to the specified parameters.
  * @param    TIM2_Channel specifies the Input Capture Channel from @ref TIM2_Channel_TypeDef.
  * @param   TIM2_ICPolarity specifies the Input Capture Polarity from @ref TIM2_ICPolarity_TypeDef.
  * @param   TIM2_ICSelection specifies the Input Capture Selection from @ref TIM2_ICSelection_TypeDef.
  * @param   TIM2_ICPrescaler specifies the Input Capture Prescaler from @ref TIM2_ICPSC_TypeDef.
  * @param   TIM2_ICFilter specifies the Input Capture Filter value (value can be an integer from 0x00 to 0x0F).
  * @retval None
  */
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

/**
  * @brief  Enables or disables the specified TIM2 interrupts.
  * @param   NewState new state of the TIM2 peripheral.
  * This parameter can be: ENABLE or DISABLE.
  * @param   TIM2_IT specifies the TIM2 interrupts sources to be enabled or disabled.
  * This parameter can be any combination of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @param   NewState new state of the TIM2 peripheral.
  * @retval None
  */
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


/**
  * @brief  Checks whether the TIM2 interrupt has occurred or not.
  * @param   TIM2_IT specifies the TIM2 interrupt source to check.
  * This parameter can be one of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @retval ITStatus The new state of the TIM2_IT(SET or RESET).
  */

ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT)
{
    ITStatus bitstatus = RESET;
    uint8_t TIM2_itStatus = 0, TIM2_itEnable = 0;


    TIM2_itStatus = (uint8_t)(TIM2_SR1 & TIM2_IT);

    TIM2_itEnable = (uint8_t)(TIM2_IER & TIM2_IT);

    if ((TIM2_itStatus != (uint8_t)RESET ) && (TIM2_itEnable != (uint8_t)RESET ))
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }
    return (ITStatus)(bitstatus);
}

/**
  * @brief  Clears the TIM2's interrupt pending bits.
  * @param   TIM2_IT specifies the pending bit to clear.
  * This parameter can be one of the following values:
  *                       - TIM2_IT_UPDATE: TIM2 update Interrupt source
  *                       - TIM2_IT_CC1: TIM2 Capture Compare 1 Interrupt source
  *                       - TIM2_IT_CC2: TIM2 Capture Compare 2 Interrupt source
  *                       - TIM2_IT_CC3: TIM2 Capture Compare 3 Interrupt source
  * @retval None.
  */
void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT)
{

    /* Clear the IT pending Bit */
    TIM2_SR1 = (uint8_t)(~TIM2_IT);
}

/**
  * @brief  Gets the TIM2 Input Capture 3 value.
  * @param  None
  * @retval Capture Compare 3 Register value.
  */
uint16_t TIM2_GetCapture3(void)
{
    /* Get the Capture 3 Register value */
    uint16_t tmpccr3 = 0;
    uint8_t tmpccr3l=0, tmpccr3h=0;

    tmpccr3h = TIM2_CCR3H;
    tmpccr3l = TIM2_CCR3L;

    tmpccr3 = (uint16_t)(tmpccr3l);
    tmpccr3 |= (uint16_t)((uint16_t)tmpccr3h << 8);
    /* Get the Capture 3 Register value */
    return (uint16_t)tmpccr3;
}

/**
  * @brief  Sets the TIM2 Input Capture 1 Prescaler.
  * @param   TIM2_IC1Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler)
{


    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2_CCMR1 = (uint8_t)((uint8_t)(TIM2_CCMR1 & (uint8_t)(~TIM2_CCMR_ICxPSC))
                            | (uint8_t)TIM2_IC1Prescaler);
}

/**
  * @brief  Sets the TIM2 Input Capture 2 prescaler.
  * @param   TIM2_IC2Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler)
{


    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2_CCMR2 = (uint8_t)((uint8_t)(TIM2_CCMR2 & (uint8_t)(~TIM2_CCMR_ICxPSC))
                            | (uint8_t)TIM2_IC2Prescaler);
}

/**
  * @brief  Sets the TIM2 Input Capture 3 prescaler.
  * @param   TIM2_IC3Prescaler specifies the Input Capture prescaler new value
  * This parameter can be one of the following values:
  *                       - TIM2_ICPSC_DIV1: no prescaler
  *                       - TIM2_ICPSC_DIV2: capture is done once every 2 events
  *                       - TIM2_ICPSC_DIV4: capture is done once every 4 events
  *                       - TIM2_ICPSC_DIV8: capture is done once every 8 events
  * @retval None
  */
void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler)
{

    /* Reset the IC1PSC Bits &Set the IC1PSC value */
    TIM2_CCMR3 = (uint8_t)((uint8_t)(TIM2_CCMR3 & (uint8_t)(~TIM2_CCMR_ICxPSC))
                            | (uint8_t)TIM2_IC3Prescaler);
}
