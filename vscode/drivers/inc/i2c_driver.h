/**
 * @file i2c_driver.h
 * @brief I2C master driver for STM8 microcontrollers.
 *
 * This file provides a blocking I2C master-mode driver implementation
 * for STM8 microcontrollers using direct register access.
 *
 * Supported features:
 *  - I2C master initialization (standard mode, ≤ 100 kHz)
 *  - START and STOP condition generation
 *  - 7-bit slave addressing
 *  - Byte-wise transmit and receive operations
 *  - ACK / NACK handling
 *  - Timeout protection to avoid bus lockup
 *
 * The driver is intended for simple sensor and peripheral communication
 * (e.g. EEPROMs, temperature sensors, RTCs).
 *
 * @note Only standard mode (Sm, 100 kHz max) is supported.
 *
 * 
 * @date 2026-02-03
 */

#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>


/**
 * @brief Initializes the I2C peripheral in master mode (standard mode).
 *
 * This function configures the I2C peripheral timing registers according
 * to the provided CPU clock and desired I2C bus frequency.
 * The peripheral is configured for standard mode (up to 100 kHz).
 *
 * Configuration steps:
 * - Enables the I2C peripheral clock
 * - Disables the I2C peripheral during configuration
 * - Programs the input clock frequency (FREQR)
 * - Computes and sets the clock control register (CCR)
 * - Configures the rise time register (TRISER)
 * - Enables ACK by default
 * - Enables the I2C peripheral
 *
 * @param[in] cpu_hz  CPU clock frequency in Hz.
 * @param[in] i2c_hz  Desired I2C bus frequency in Hz.
 *
 * @note This implementation supports only standard mode (Sm, ≤ 100 kHz).
 * @note The CCR value is limited to the 12-bit range as required by hardware.
 */
void i2c_master_init(uint32_t cpu_hz, uint32_t i2c_hz);


/**
 * @brief Generates an I2C START condition and waits for it to be sent.
 *
 * This function sets the START bit in the I2C control register (CR2)
 * and waits until the START condition is generated, which is indicated
 * by the SB (Start Bit) flag in the SR1 register.
 *
 * A timeout mechanism is used to prevent blocking in case of a bus error.
 *
 * @retval  0  START condition successfully generated.
 * @retval -1  Timeout occurred while waiting for START condition.
 */
int  i2c_master_start(void);

/**
 * @brief Generates an I2C STOP condition.
 * 
 * This function sets the STOP bit in the I2C control register (CR2),
 * forcing the I2C master to generate a STOP condition on the bus.
 * The function does not wait for the STOP condition to complete.
 * 
 * @return Always returns 0
 */
int  i2c_master_stop(void);

/**
 * @brief Sends a 7-bit slave address with direction bit on the I2C bus.
 *
 * This function writes the 7-bit slave address and transfer direction
 * (read/write) to the I2C data register and waits until the address is
 * acknowledged by the slave device.
 *
 * The function monitors the ADDR flag to detect successful address
 * transmission and checks the AF (Acknowledge Failure) flag to detect
 * a NACK from the slave.
 *
 * @param[in] addr7  7-bit slave address (without R/W bit).
 * @param[in] dir    Transfer direction:
 *                   - 0: Write
 *                   - 1: Read
 *
 * @retval  0   Address successfully acknowledged by the slave.
 * @retval -1   Timeout occurred while waiting for address acknowledge.
 * @retval -2   Slave did not acknowledge the address (NACK received).
 *
 * @note The ADDR flag is cleared by reading SR1 followed by SR3,
 *       as required by the I2C hardware.
 */
int  i2c_master_send_addr(uint8_t addr7, uint8_t dir);

/**
 * @brief Transmits one data byte over the I2C bus.
 *
 * This function writes a single data byte to the I2C data register
 * and waits until the transmit buffer becomes empty (TXE flag set),
 * indicating that the byte has been shifted out.
 *
 * The function monitors the AF (Acknowledge Failure) flag to detect
 * a NACK from the slave device and uses a timeout to avoid blocking.
 *
 * @param[in] data  Data byte to be transmitted.
 *
 * @retval  0   Data byte successfully transmitted and acknowledged.
 * @retval -1   Timeout occurred while waiting for transmission to complete.
 * @retval -2   Slave did not acknowledge the data byte (NACK received).
 */
int  i2c_master_write_byte(uint8_t data);

/**
 * @brief Receives one data byte from the I2C bus.
 *
 * This function reads a single byte from the I2C slave device.
 * Before receiving the byte, the ACK bit is configured to control
 * whether the master acknowledges the received byte.
 *
 * If ACK is disabled, a STOP condition is generated after the byte
 * is received, indicating the end of the transfer.
 *
 * @param[in] ack  Acknowledge control:
 *                 - 1: Send ACK after receiving the byte (more data expected)
 *                 - 0: Send NACK after receiving the byte (last byte)
 *
 * @retval >=0  Received data byte (0–255).
 * @retval -1   Timeout occurred while waiting for data.
 *
 * @note For the last byte of a reception, ACK must be cleared
 *       before reading the data register, and a STOP condition
 *       should be generated.
 */
int  i2c_master_read_byte(uint8_t ack); 

/**
 * @brief Transmits a data buffer to an I2C slave device.
 *
 * This function performs a complete I2C master write transaction:
 * START condition, slave address transmission (write mode),
 * data bytes transmission, and STOP condition.
 *
 * Lower-level I2C functions are used to handle each step and
 * provide timeout and NACK detection.
 *
 * @param[in] addr7  7-bit slave address.
 * @param[in] data   Pointer to the data buffer to transmit.
 * @param[in] size   Number of bytes to transmit.
 *
 * @retval  0   Transmission completed successfully.
 * @retval <0   Error occurred during START, address, or data phase:
 *              - -1: Timeout
 *              - -2: NACK received
 *
 * @note The STOP condition is always generated at the end of
 *       a successful transmission.
 */
int i2c_master_transmit(uint8_t addr7, const uint8_t *data, uint16_t size);

#endif /* I2C_DRIVER_H */