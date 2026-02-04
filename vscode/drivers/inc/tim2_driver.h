#ifndef TIM2_DRIVER_H
#define TIM2_DRIVER_H

#include <stdint.h>



#define TIM2_PSCR  (*(volatile uint8_t*)0x530E)
#define TIM2_ARRH  (*(volatile uint8_t*)0x530F)
#define TIM2_ARRL  (*(volatile uint8_t*)0x5310)
#define TIM2_EGR   (*(volatile uint8_t*)0x5306)
#define TIM2_CR1   (*(volatile uint8_t*)0x5300)
#define TIM2_IER   (*(volatile uint8_t*)0x5303)
#define TIM2_CCER1 (*(volatile uint8_t*)0x530A)
#define TIM2_CCER2 (*(volatile uint8_t*)0x530B)
#define TIM2_SR1   (*(volatile uint8_t*)0x5304)
#define TIM2_SR2   (*(volatile uint8_t*)0x5305)
#define TIM2_CCR3H (*(volatile uint8_t*)0x5315)
#define TIM2_CCR3L (*(volatile uint8_t*)0x5316)
#define TIM2_CCMR1 (*(volatile uint8_t*)0x5307)
#define TIM2_CCMR2 (*(volatile uint8_t*)0x5308)
#define TIM2_CCMR3 (*(volatile uint8_t*)0x5309)
#define TIM2_CNTRH (*(volatile uint8_t*)0x530C)
#define TIM2_CNTRL (*(volatile uint8_t*)0x530D)

#define TIM2_CCR1H (*(volatile uint8_t*)0x5311)
#define TIM2_CCR1L (*(volatile uint8_t*)0x5312)
#define TIM2_CCR2H (*(volatile uint8_t*)0x5313)
#define TIM2_CCR2L (*(volatile uint8_t*)0x5314)

#define TIM2_CR1_RESET_VALUE   ((uint8_t)0x00)
#define TIM2_IER_RESET_VALUE   ((uint8_t)0x00)
#define TIM2_SR1_RESET_VALUE   ((uint8_t)0x00)
#define TIM2_SR2_RESET_VALUE   ((uint8_t)0x00)
#define TIM2_EGR_RESET_VALUE   ((uint8_t)0x00)
#define TIM2_CCMR1_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCMR2_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCMR3_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCER1_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCER2_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CNTRH_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CNTRL_RESET_VALUE ((uint8_t)0x00)
#define TIM2_PSCR_RESET_VALUE  ((uint8_t)0x00)
#define TIM2_ARRH_RESET_VALUE  ((uint8_t)0xFF)
#define TIM2_ARRL_RESET_VALUE  ((uint8_t)0xFF)
#define TIM2_CCR1H_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR1L_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR2H_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR2L_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR3H_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR3L_RESET_VALUE ((uint8_t)0x00)

#define TIM2_CR1_CEN     ((uint8_t)0x01)

#define TIM2_CCER1_CC1E  ((uint8_t)0x01) /*!< Capture/Compare 1 output enable mask. */
#define TIM2_CCER1_CC1P  ((uint8_t)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM2_CCER1_CC2E  ((uint8_t)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM2_CCER1_CC2P  ((uint8_t)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM2_CCER2_CC3E  ((uint8_t)0x01) /*!< Capture/Compare 3 output enable mask. */
#define TIM2_CCER2_CC3P  ((uint8_t)0x02) /*!< Capture/Compare 3 output Polarity mask. */

#define TIM2_CCMR_ICxPSC ((uint8_t)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM2_CCMR_CCxS   ((uint8_t)0x03) /*!< Capture/Compare x Selection mask. */
#define TIM2_CCMR_ICxF   ((uint8_t)0xF0) /*!< Input Capture x Filter mask. */


typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

/** TIM2 Prescaler Reload Mode */
typedef enum
{
  TIM2_PSCRELOADMODE_UPDATE          = ((uint8_t)0x00),
  TIM2_PSCRELOADMODE_IMMEDIATE       = ((uint8_t)0x01)
}TIM2_PSCReloadMode_TypeDef;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, BitStatus, BitAction;
/** TIM2 Prescaler */
typedef enum
{
  TIM2_PRESCALER_1  = ((uint8_t)0x00),
  TIM2_PRESCALER_2    = ((uint8_t)0x01),
  TIM2_PRESCALER_4    = ((uint8_t)0x02),
  TIM2_PRESCALER_8     = ((uint8_t)0x03),
  TIM2_PRESCALER_16   = ((uint8_t)0x04),
  TIM2_PRESCALER_32     = ((uint8_t)0x05),
  TIM2_PRESCALER_64    = ((uint8_t)0x06),
  TIM2_PRESCALER_128   = ((uint8_t)0x07),
  TIM2_PRESCALER_256   = ((uint8_t)0x08),
  TIM2_PRESCALER_512   = ((uint8_t)0x09),
  TIM2_PRESCALER_1024  = ((uint8_t)0x0A),
  TIM2_PRESCALER_2048 = ((uint8_t)0x0B),
  TIM2_PRESCALER_4096   = ((uint8_t)0x0C),
  TIM2_PRESCALER_8192 = ((uint8_t)0x0D),
  TIM2_PRESCALER_16384 = ((uint8_t)0x0E),
  TIM2_PRESCALER_32768 = ((uint8_t)0x0F)
}TIM2_Prescaler_TypeDef;

/** TIM2 Channel */
typedef enum
{
  TIM2_CHANNEL_1                     = ((uint8_t)0x00),
  TIM2_CHANNEL_2                     = ((uint8_t)0x01),
  TIM2_CHANNEL_3                     = ((uint8_t)0x02)
}TIM2_Channel_TypeDef;


/** TIM2 Input Capture Polarity */
typedef enum
{
  TIM2_ICPOLARITY_RISING            = ((uint8_t)0x00),
  TIM2_ICPOLARITY_FALLING           = ((uint8_t)0x44)
}TIM2_ICPolarity_TypeDef;


/** TIM2 Input Capture Selection */
typedef enum
{
  TIM2_ICSELECTION_DIRECTTI          = ((uint8_t)0x01),
  TIM2_ICSELECTION_INDIRECTTI        = ((uint8_t)0x02),
  TIM2_ICSELECTION_TRGI              = ((uint8_t)0x03)
}TIM2_ICSelection_TypeDef;

/** TIM2 Input Capture Prescaler */
typedef enum
{
  TIM2_ICPSC_DIV1                    = ((uint8_t)0x00),
  TIM2_ICPSC_DIV2                    = ((uint8_t)0x04),
  TIM2_ICPSC_DIV4                    = ((uint8_t)0x08),
  TIM2_ICPSC_DIV8                    = ((uint8_t)0x0C)
}TIM2_ICPSC_TypeDef;

/** TIM2 interrupt sources */
typedef enum
{
  TIM2_IT_UPDATE                     = ((uint8_t)0x01),
  TIM2_IT_CC1                        = ((uint8_t)0x02),
  TIM2_IT_CC2                        = ((uint8_t)0x04),
  TIM2_IT_CC3                        = ((uint8_t)0x08)
}TIM2_IT_TypeDef;

void TIM2_DeInit(void);
void TIM2_TimeBaseInit(TIM2_Prescaler_TypeDef TIM2_Prescaler, uint16_t TIM2_Period);
void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler,TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode);
void TIM2_Cmd(FunctionalState NewState);
void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel,
                 TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                 TIM2_ICSelection_TypeDef TIM2_ICSelection,
                 TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                 uint8_t TIM2_ICFilter);
void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState);
ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT);
void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT);
uint16_t TIM2_GetCapture3(void);
void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler);
void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler);
void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler);



#endif
