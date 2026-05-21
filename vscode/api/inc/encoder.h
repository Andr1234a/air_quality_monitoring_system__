/**
 * @file encoder.h
 * @brief Hardware encoder driver interface using TIM1 for STM8S.
 * @details This module provides an interface to configure and read a rotary encoder
 *          using the hardware Encoder Mode 3 of the 16-bit Timer 1 (TIM1).
 * @date 2026
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "stm8_s.h"

/**
 * @brief Initializes and enables the hardware encoder mode on TIM1.
 * @details Configures TIM1 in Encoder Mode 3 (counting on both TI1 and TI2 edges)
 *          to achieve maximum resolution and robustness against noise.
 *          Disables all TIM1 interrupts to prevent unhandled ISR execution,
 *          sets input capture channels, and initializes the Auto-Reload Register (ARR)
 *          to allow counting up to a predefined limit (e.g., 300).
 * @note This function stops the timer before reconfiguring and resets the counter to 0.
 */
void Encoder_Enable(void);

/**
 * @brief Retrieves the current 16-bit unsigned value from the encoder counter.
 * @details Reads the hardware counter registers of TIM1. To guarantee data atomic
 *          integrity, the high byte register (TIM1_CNTRH) is read first, which latches
 *          the low byte register (TIM1_CNTRL) automatically into a shadow buffer
 *          as required by the STM8S Reference Manual (RM0016).
 * @return uint16_t The combined 16-bit absolute position of the encoder.
 */
uint16_t Encoder_GetValue(void);

/**
 * @brief Sets the encoder counter to a specific 16-bit unsigned value.
 * @details Overwrites the current TIM1 counter registers with the provided value.
 *          The write sequence starts with the high byte (TIM1_CNTRH) to ensure
 *          the internal hardware buffering updates both bytes correctly.
 * @param[in] val The 16-bit unsigned value to be loaded into the encoder counter.
 */
void Encoder_SetValue(uint16_t val);

#endif /* ENCODER_H */