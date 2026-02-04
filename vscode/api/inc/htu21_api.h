/**
 * @file htu21_api.h
 * @brief High-level HTU21 sensor API.
 *
 * This file implements a high-level application programming interface
 * (API) for accessing temperature and humidity measurements from the
 * HTU21 sensor.
 *
 * The API:
 *  - Provides ready-to-use physical values (°C, %RH)
 *  - Hides low-level I2C transactions and timing requirements
 *  - Caches the last successfully read values
 *  - Can be used directly by application-level code
 *
 * This module depends on an external I2C master driver, but does not
 * access hardware registers directly.
 *
 *
 * @note Sensor CRC byte is currently ignored.
 * @note Blocking delays are used during measurement.
 *
 * @date 2026-02-04
 * 
 * 
 */

#ifndef HTU21_API_H
#define HTU21_API_H

#include "i2c_driver.h"

/**
 * @brief Reads the temperature from the HTU21 sensor.
 *
 * This function performs a temperature measurement by sending the
 * appropriate command to the HTU21 sensor and reading the resulting
 * 16-bit raw value. The raw value is then converted to degrees Celsius
 * according to the sensor's datasheet formula.
 *
 * @retval float  Measured temperature in °C.
 * @retval -1000.0f  Error occurred during I2C communication.
 *
 * @note The conversion formula is:
 *       Temperature [°C] = -46.85 + 175.72 * (raw / 2^16)
 * @note The last successfully read temperature is stored in `last_temp`.
 */
float htu21_read_temperature(void);

/**
 * @brief Reads the relative humidity from the HTU21 sensor.
 *
 * This function performs a humidity measurement by sending the
 * appropriate command to the HTU21 sensor and reading the resulting
 * 16-bit raw value. The raw value is then converted to relative
 * humidity (%) according to the sensor's datasheet formula.
 *
 * @retval float  Measured relative humidity in %RH.
 * @retval -1000.0f  Error occurred during I2C communication.
 *
 * @note The conversion formula is:
 *       Humidity [%RH] = -6 + 125 * (raw / 2^16)
 * @note The last successfully read humidity value is stored in `last_hum`.
 */
float htu21_read_humidity(void);

/**
 * @brief Returns the last successfully read temperature from HTU21.
 *
 * This function returns the most recent temperature value obtained
 * from the HTU21 sensor. It does not perform a new measurement.
 *
 * @retval float  Last measured temperature in °C.
 *
 * @note The value is updated each time `htu21_read_temperature()` is called.
 */
float htu21_last_temperature(void);

/**
 * @brief Returns the last successfully read relative humidity from HTU21.
 *
 * This function returns the most recent humidity value obtained
 * from the HTU21 sensor. It does not perform a new measurement.
 *
 * @retval float  Last measured relative humidity in %RH.
 *
 * @note The value is updated each time `htu21_read_humidity()` is called.
 */
float htu21_last_humidity(void);

#endif