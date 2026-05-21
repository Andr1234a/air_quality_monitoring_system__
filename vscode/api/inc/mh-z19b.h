/**
 * @file mh_z19b.h
 * @brief High-level MH-Z19B CO2 sensor API.
 *
 * This file implements an application programming interface (API) for
 * interacting with the MH-Z19B NDIR carbon dioxide (CO2) sensor via UART.
 * It provides robust error-handling mechanisms for serial communications.
 *
 * This module relies on external UART driver primitives and handles
 * communication protocol constraints like frame validation and checksums.
 *
 * @date 2026-05-19
 */

#ifndef MH_Z19B_H
#define MH_Z19B_H

#include <stdint.h>

/**
 * @brief MH-Z19B API execution status codes.
 */
typedef enum
{
    MHZ19_OK = 0,             /**< Success. Data read and validated correctly. */
    MHZ19_ERR_TIMEOUT,        /**< Sensor timeout. No response received (wire disconnection). */
    MHZ19_ERR_INVALID_HEADER, /**< Invalid frame header. Unexpected bytes detected in line. */
    MHZ19_ERR_CHECKSUM        /**< Checksum mismatch. Data corrupted due to noise/interference. */
} mhz19_status_t;

/**
 * @brief Initializes the MH-Z19B UART communication peripheral.
 *
 * Configures the required baud rate and transmission parameters for the
 * hardware UART channel interface dedicated to the sensor.
 *
 * @param[in] f_cpu Current CPU master clock frequency in Hz.
 */
void mhz19_init(unsigned long f_cpu);

/**
 * @brief Reads the current CO2 concentration level from the sensor.
 *
 * Transmits a 9-byte read command block to the MH-Z19B sensor, awaits
 * the 9-byte incoming data block, validates packet boundaries, checks
 * correctness using an 8-bit checksum algorithm, and parses the PPM value.
 *
 * @param[out] co2_value Pointer to an integer variable where the parsed
 * CO2 concentration (in PPM) will be saved.
 *
 * @return mhz19_status_t Execution result code matching the status enum.
 * * @note Outward data verification uses strict type-masking logic to align
 * properly with 8-bit MCU accumulator register operations.
 */
mhz19_status_t mhz19_read_co2(int *co2_value);

#endif