/**
 * @file gpio_driver.h
 * @brief GPIO driver implementation for STM8 microcontrollers.
 *
 * This file contains low-level GPIO control functions such as
 * pin configuration, write, toggle, and read operations.
 *
 * The driver works with abstract GPIO_Pin structures that map
 * GPIO registers (DDR, CR1, ODR, IDR) to a specific physical pin.
 *
 * Supported features:
 *  - Input / Output configuration
 *  - Push-pull / Open-drain output modes
 *  - Pull-up / Floating input modes
 *  - Pin state caching
 * 
 * @date 2026-02-03
 * 
 */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>
#include "stm8_s.h"

/**
 * @brief Initializes the GPIO according to the specified parameters.
 * 
 * @param[in] pin: this is a pointer to a structure that describes a specific physical GPIO pin.  
 * @param[in] dir: This parameter contains the pin direction.
 * @param[in] mode: This parameter contains the pin mode. 
 */
void GPIO_Config(GPIO_Pin *pin, GPIO_Direction dir, GPIO_Mode mode);

/**
 * @brief Writes data to the specified GPIO data port.
 * 
 * @param[in] pin: This is a pointer to a structure that describes a specific physical GPIO pin. 
 * @param[in] value: Specifies the value to be written to the port output
  *         data register  
 */
void GPIO_Write(GPIO_Pin *pin, GPIO_State value);

/**
 * @brief Toggles the GPIO output pin state
 * 
 * @param[in] pin: This is a pointer to a structure that describes a specific physical GPIO pin. 
 */
void GPIO_Toggle(GPIO_Pin *pin);

/**
 * @brief Returns the cached GPIO pin state.
 * 
 * @param[in] pin: This is a pointer to a structure that describes a specific physical GPIO pin.
 * @return GPIO_State: Cached pin state (GPIO_HIGH or GPIO_LOW). 
 */
GPIO_State GPIO_ReadState(GPIO_Pin *pin);

/**
 * @brief Reads the current logic level of the GPIO pin.
 * 
 * @param[in] pin: This is a pointer to a structure that describes a specific physical GPIO pin. 
 * @return uint8_t: Pin logic level 
 */
uint8_t GPIO_Read(GPIO_Pin *pin);

#endif
