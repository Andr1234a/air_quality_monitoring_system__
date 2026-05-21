/**
 * @file mh_z19b.h
 * @brief High-level API for the MH-Z19B NDIR CO2 sensor.
 * @details This module implements the application programming interface (API) for
 *          interacting with the Intelligent Infrared CO2 Sensor (MH-Z19B) via UART.
 *          It handles raw packet transmission, response frame validation,
 *          strict 8-bit checksum verification, and hardware timeout monitoring.
 * @author Your Name / Project Team
 * @date 2026-05-19
 */

#ifndef MH_Z19B_H
#define MH_Z19B_H

#include <stdint.h>

/**
 * @brief MH-Z19B API execution status and error codes.
 */
typedef enum
{
    MHZ19_OK = 0,             /**< Success. Data received, verified, and parsed correctly. */
    MHZ19_ERR_TIMEOUT,        /**< RX Timeout. No response from the sensor (possible wire disconnect). */
    MHZ19_ERR_INVALID_HEADER, /**< Frame Error. Unexpected start byte or command echo detected. */
    MHZ19_ERR_CHECKSUM        /**< Data Corruption. Calculated 8-bit checksum mismatch due to noise. */
} mhz19_status_t;

/**
 * @brief Initializes the MH-Z19B UART communication peripheral.
 * @details Configures the hardware UART peripheral to meet the MH-Z19B sensor
 *          requirements (9600 baud rate, 8 data bits, no parity, 1 stop bit).
 * @param[in] f_cpu Current CPU master clock frequency in Hz to compute BRR registers.
 */
void mhz19_init(unsigned long f_cpu);

/**
 * @brief Reads the current CO2 concentration level from the sensor.
 * @details Transmits a static 9-byte request frame (Command 0x86) to the sensor.
 *          Flushes the hardware RX buffer before transmission, waits for the 9-byte
 *          response with a software timeout loop, verifies the header structure,
 *          and validates the payload using an 8-bit checksum algorithm.
 * @param[out] co2_value Pointer to a 16-bit unsigned variable where the parsed
 *                       CO2 concentration (in PPM) will be stored upon success.
 * @return mhz19_status_t Execution status code from the #mhz19_status_t enum.
 * @note Outward data parsing reconstructs the 16-bit integer from two distinct
 *       8-bit data bytes: (HIGH_BYTE << 8) | LOW_BYTE.
 */
mhz19_status_t mhz19_read_co2(uint16_t *co2_value);

#endif /* MH_Z19B_H */