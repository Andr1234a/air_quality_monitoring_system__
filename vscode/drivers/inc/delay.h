/**
 * @file delay.h
 * @brief Blocking delay driver based on CPU clock frequency.
 *
 * This driver implements simple blocking delays using busy-wait loops.
 * Delay duration is calculated from the CPU clock frequency (F_CPU).
 *
 * The driver does not use hardware timers and therefore blocks CPU
 * execution for the entire delay period.
 *
 * @note Intended for simple timing needs during initialization,
 *       debugging, or low-priority delays.
 * 
 * @date 2026-02-04
 * 
 */

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**
 * @brief Creates a blocking delay in milliseconds.
 *
 * This function generates a software delay by executing a calibrated
 * busy-wait loop. The delay length depends on the CPU clock frequency.
 *
 * @param ms  Delay time in milliseconds.
 *
 * @warning This function blocks CPU execution.
 * @warning Delay accuracy depends on compiler optimization and F_CPU value.
 */
void delay_ms(uint16_t ms);

/**
 * @brief Creates a blocking delay in microseconds.
 *
 * This function generates a short software delay using a busy-wait loop.
 *
 * @param us  Delay time in microseconds.
 *
 * @warning This function blocks CPU execution.
 * @warning Intended only for short, non-critical delays.
 */
void delay_us(uint16_t us);

#endif