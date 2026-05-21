
/**
 * @file encoder.h
 * @brief Incremental encoder driver header file.
 * @details Provides the interface for initializing the encoder, reading its
 *          current counter value, and forcing a preset step value.
 * @date 2026-05-21
 */
#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

/**
 * @brief Initializes and enables the encoder hardware peripheral.
 * @details Configures the required GPIO pins (Phase A and Phase B channels) as
 *          inputs with pull-ups, and sets up external interrupts (EXTI) or
 *          hardware timer encoder mode depending on the implementation.
 */
void Encoder_Enable(void);

/**
 * @brief Retrieves the current accumulated encoder value.
 * @details Reads the internal directional counter step. This value increments
 *          with clockwise rotation and decrements with counter-clockwise rotation.
 * @return The current encoder counter value as a signed 8-bit integer.
 */
int8_t Encoder_GetValue(void);

/**
 * @brief Forces the internal encoder counter to a specific value.
 * @details Typically used to synchronize the hardware encoder state with
 *          software UI menu boundaries (e.g., resetting to 0 when entering a
 *          new screen or clamping to a maximum choice limit).
 * @param[in] val The new value to be loaded into the encoder counter.
 */
void Encoder_SetValue(uint8_t val);

#endif