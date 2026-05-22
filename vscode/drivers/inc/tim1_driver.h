/**
 * @file tim1_driver.h
 * @brief TIM1 Advanced Control Timer driver for STM8S with PWM support.
 * @version 1.1
 * @date 2026-05-22
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef TIM1_DRIVER_H
#define TIM1_DRIVER_H

#include <stdint.h>

/**
 * @brief Deinitializes the TIM1 peripheral registers to their default reset values.
 *
 * Clears all control registers, channel configurations, and counter values.
 * It is recommended to call this function before reinitializing the timer.
 */
void TIM1_DeInit(void);

/**
 * @brief Initializes a specific TIM1 channel in Pulse Width Modulation (PWM) mode.
 *
 * Configures the timer base unit timebase and sets up the corresponding
 * microcontroller pin to operate as an alternate function PWM output.
 *
 * @param[in] channel   TIM1 channel number (typically 1 to 4).
 * @param[in] period    Auto-reload register (ARR) value. Defines the PWM period/resolution.
 * @param[in] prescaler Clock prescaler 16-bit divider value.
 *
 * @note This function only sets up the configuration. Call TIM1_Start() to begin signal generation.
 * @pre The master CPU clock (f_CPU) must be initialized prior to calling this function.
 */
void TIM1_InitPWM(uint8_t channel, uint16_t period, uint16_t prescaler);

/**
 * @brief Sets the duty cycle (pulse width) for the selected TIM1 channel.
 *
 * Modifies the Capture/Compare Register (CCR) value to adjust output power,
 * brightness, or buzzer volume.
 *
 * @param[in] channel TIM1 channel number (1 to 4).
 * @param[in] duty    Compare match value. Must be less than or equal to the 'period'
 *                    value defined during initialization.
 *
 * @warning If `duty > period`, the PWM output will lock to a constant high or low level
 *          depending on the channel's configured polarity.
 */
void TIM1_PWM_SetDuty(uint8_t channel, uint16_t duty);

/**
 * @brief Dynamically updates the PWM frequency on the fly.
 *
 * Recalculates and overrides the prescaler and auto-reload (ARR) registers
 * to achieve the target frequency in Hertz, while maintaining a proportional duty cycle.
 *
 * @param[in] channel TIM1 channel number (1 to 4).
 * @param[in] duty    The base or updated duty cycle value.
 * @param[in] freq_hz Target frequency of the output signal in Hertz (Hz).
 *
 * @note Extremely useful for driving sound buzzers to generate different musical pitches/notes.
 */
void TIM1_PWM_SetFrequency(uint8_t channel, uint32_t duty, uint32_t freq_hz);

/**
 * @brief Enables the TIM1 counter and activates signal generation.
 *
 * Sets the CEN (Counter Enable) bit in the TIM1_CR1 register and handles
 * the Main Output Enable (MOE bit in TIM1_BKR), which is required for STM8 advanced timers.
 */
void TIM1_Start(void);

/**
 * @brief Stops the TIM1 counter.
 *
 * Suspends PWM signal generation across all channels. The current counter value is preserved.
 */
void TIM1_Stop(void);

#endif /* TIM1_DRIVER_H */