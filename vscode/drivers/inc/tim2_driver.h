/**
 * @file tim2_driver.h
 * @brief TIM2 General Purpose Timer driver for STM8S with Input Capture support.
 * @version 1.0
 * @date 2026-05-22
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef TIM2_DRIVER_H
#define TIM2_DRIVER_H

#include <stdint.h>
#include "stm8_s.h"

/**
 * @brief Deinitializes the TIM2 peripheral registers to their default reset values.
 *
 * Completely resets the TIM2 peripheral, turning off counters, interrupts,
 * and clearing channel routing.
 */
void TIM2_DeInit(void);

/**
 * @brief Initializes the TIM2 Time Base unit according to the specified parameters.
 *
 * Configures the timer clock divider (prescaler) and the maximum value up to
 * which the timer counts (period) before wrapping around.
 *
 * @param[in] TIM2_Prescaler Clock prescaler factor (expressed as standard SPL enum).
 * @param[in] TIM2_Period    Auto-reload register (ARR) value. Controls the overflow step.
 */
void TIM2_TimeBaseInit(TIM2_Prescaler_TypeDef TIM2_Prescaler, uint16_t TIM2_Period);

/**
 * @brief Configures the TIM2 clock prescaler dynamically.
 *
 * Allows changing the clock division factor at runtime and defines when the new
 * divider takes effect.
 *
 * @param[in] Prescaler           The new prescaler value to apply.
 * @param[in] TIM2_PSCReloadMode  Specifies whether the change occurs immediately or
 *                                after the next update event (overflow).
 */
void TIM2_PrescalerConfig(TIM2_Prescaler_TypeDef Prescaler, TIM2_PSCReloadMode_TypeDef TIM2_PSCReloadMode);

/**
 * @brief Enables or disables the TIM2 peripheral counter.
 *
 * @param[in] NewState New state of the counter. Can be:
 *                     @arg ENABLE: Starts the timer clocking and counting.
 *                     @arg DISABLE: Pauses the timer counting.
 */
void TIM2_Cmd(FunctionalState NewState);

/**
 * @brief Initializes a chosen TIM2 channel for Input Capture (IC) operations.
 *
 * Configures a hardware channel to measure outside digital waveforms, ideal for
 * applications like reading rotary encoders or infrared signals.
 *
 * @param[in] TIM2_Channel     Target timer channel (e.g., TIM2_CHANNEL_1, TIM2_CHANNEL_2).
 * @param[in] TIM2_ICPolarity  Active edge selection (Rising, Falling, etc.).
 * @param[in] TIM2_ICSelection Input routing configuration (Direct, Indirect, or TRGI).
 * @param[in] TIM2_ICPrescaler Input capture event prescaler (divides external event pulses).
 * @param[in] TIM2_ICFilter    Digital input filter coefficient. Must be a 4-bit value (0x00 to 0x0F).
 */
void TIM2_ICInit(TIM2_Channel_TypeDef TIM2_Channel,
                 TIM2_ICPolarity_TypeDef TIM2_ICPolarity,
                 TIM2_ICSelection_TypeDef TIM2_ICSelection,
                 TIM2_ICPSC_TypeDef TIM2_ICPrescaler,
                 uint8_t TIM2_ICFilter);

/**
 * @brief Enables or disables specific TIM2 interrupt sources.
 *
 * @param[in] TIM2_IT   Target interrupt flag configuration (e.g., TIM2_IT_UPDATE, TIM2_IT_CC1).
 * @param[in] NewState  New activation state (ENABLE or DISABLE).
 */
void TIM2_ITConfig(TIM2_IT_TypeDef TIM2_IT, FunctionalState NewState);

/**
 * @brief Checks whether a specific TIM2 interrupt pending flag has occurred.
 *
 * @param[in] TIM2_IT The interrupt source status bit to check.
 * @return ITStatus Current state of the checked flag (SET or RESET).
 */
ITStatus TIM2_GetITStatus(TIM2_IT_TypeDef TIM2_IT);

/**
 * @brief Clears the interrupt pending (flag) bit for a specific TIM2 event.
 *
 * Call this inside the Interrupt Service Routine (ISR) to acknowledge the event
 * and prevent execution loops.
 *
 * @param[in] TIM2_IT The interrupt flag source to clear.
 */
void TIM2_ClearITPendingBit(TIM2_IT_TypeDef TIM2_IT);

/**
 * @brief Reads the raw latch value from Capture/Compare Register 3 (CCR3).
 *
 * Typically used to extract the recorded timestamp when an edge occurs on Channel 3.
 *
 * @return uint16_t The captured 16-bit timer tick value.
 */
uint16_t TIM2_GetCapture3(void);

/**
 * @brief Sets the input capture divider specifically for Channel 1.
 *
 * @param[in] TIM2_IC1Prescaler New prescaler mapping for Channel 1.
 */
void TIM2_SetIC1Prescaler(TIM2_ICPSC_TypeDef TIM2_IC1Prescaler);

/**
 * @brief Sets the input capture divider specifically for Channel 2.
 *
 * @param[in] TIM2_IC2Prescaler New prescaler mapping for Channel 2.
 */
void TIM2_SetIC2Prescaler(TIM2_ICPSC_TypeDef TIM2_IC2Prescaler);

/**
 * @brief Sets the input capture divider specifically for Channel 3.
 *
 * @param[in] TIM2_IC3Prescaler New prescaler mapping for Channel 3.
 */
void TIM2_SetIC3Prescaler(TIM2_ICPSC_TypeDef TIM2_IC3Prescaler);

#endif /* TIM2_DRIVER_H */