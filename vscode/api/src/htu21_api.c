#include "i2c_driver.h"
#include "htu21_api.h"
#include "stm8_s.h"

/* Common Configuration & Masks */
#define HTU21_RESPONSE_SIZE 3
#define SHIFT_ONE_BYTE 8
#define HTU21_STATUS_BIT_MASK ((uint8_t)0xFC)
#define HTU21_CONVERSION_DELAY_MS 50

/* I2C Transfer Directions */
#define I2C_DIR_WRITE_MODE 0
#define I2C_DIR_READ_MODE 1

/* I2C Ack/Nack Control for Master Read */
#define I2C_ACK_SIGNAL 1
#define I2C_NACK_SIGNAL 0

/* Sentinel Error Values for Cache Initialization */
#define HTU21_INVALID_TEMP_VALUE -1000.0f
#define HTU21_INVALID_HUM_VALUE -1000.0f

/* Formula Constants for Temperature Calculation */
#define HTU21_TEMP_COEFF_MULTI 175.72f
#define HTU21_TEMP_COEFF_DIV 65536.0f
#define HTU21_TEMP_COEFF_SUB 46.85f

/* Formula Constants for Humidity Calculation */
#define HTU21_HUM_COEFF_MULTI 125.0f
#define HTU21_HUM_COEFF_DIV 65536.0f
#define HTU21_HUM_COEFF_SUB 6.0f

/* API Standard Error Codes */
#define HTU21_SUCCESS 0
#define HTU21_ERROR -1

/**
 * @brief Cached last valid temperature value.
 *
 * Contains the most recent temperature measurement that was
 * successfully read from the sensor.
 *
 * Initialized to an invalid sentinel value.
 */
static float last_temp = HTU21_INVALID_TEMP_VALUE;

/**
 * @brief Cached last valid humidity value.
 *
 * Contains the most recent humidity measurement that was
 * successfully read from the sensor.
 *
 * Initialized to an invalid sentinel value.
 */
static float last_hum = HTU21_INVALID_HUM_VALUE;

/**
 * @brief Reads multiple bytes from the HTU21 sensor via I2C.
 *
 * This function performs a complete I2C transaction to read `len` bytes
 * from the HTU21 sensor. It first sends a command byte, waits for
 * the sensor to perform the measurement, and then reads the data bytes.
 *
 * @param[in]  cmd  Command byte to send to HTU21 (e.g., trigger temperature or humidity measurement).
 * @param[out] buf  Pointer to the buffer where the received data will be stored.
 * @param[in]  len  Number of bytes to read.
 *
 * @retval 0   Data successfully read.
 * @retval -1  I2C communication error occurred (start, address, write, or read failure).
 *
 * @note This function waits ~50 ms after sending the command to allow the HTU21
 *       to perform the conversion.
 * @note The last byte is read with NACK to indicate the end of reading.
 */
static int htu21_read_bytes(unsigned char cmd, unsigned char *buf, unsigned char len)
{
    int rc;
    unsigned char i;
    int d;

    rc = i2c_master_start();
    if (rc != 0)
        return HTU21_ERROR;

    rc = i2c_master_send_addr(HTU21_I2C_ADDR, I2C_DIR_WRITE_MODE); // write
    if (rc != 0)
    {
        i2c_master_stop();
        return HTU21_ERROR;
    }

    rc = i2c_master_write_byte(cmd);
    if (rc != 0)
    {
        i2c_master_stop();
        return HTU21_ERROR;
    }

    i2c_master_stop();

    delay_ms(HTU21_CONVERSION_DELAY_MS);

    rc = i2c_master_start();
    if (rc != 0)
        return HTU21_ERROR;

    rc = i2c_master_send_addr(HTU21_I2C_ADDR, I2C_DIR_READ_MODE); // read
    if (rc != 0)
    {
        i2c_master_stop();
        return HTU21_ERROR;
    }

    for (i = 0; i < len; i++)
    {
        d = i2c_master_read_byte((i < (len - 1)) ? I2C_ACK_SIGNAL : I2C_NACK_SIGNAL);
        if (d < 0)
        {
            i2c_master_stop();
            return HTU21_ERROR;
        }
        buf[i] = (unsigned char)d;
    }

    return HTU21_SUCCESS;
}

// Reads the temperature from the HTU21 sensor.
int htu21_read_temperature(float *out)
{
    unsigned char buf[HTU21_RESPONSE_SIZE];
    unsigned int t;
    float temp;

    if (htu21_read_bytes(HTU21_READTEMP, buf, HTU21_RESPONSE_SIZE) != 0)
        return HTU21_ERROR;

    t = ((unsigned int)buf[0] << SHIFT_ONE_BYTE) | (buf[1] & HTU21_STATUS_BIT_MASK);

    temp = (float)t;
    temp *= HTU21_TEMP_COEFF_MULTI;
    temp /= HTU21_TEMP_COEFF_DIV;
    temp -= HTU21_TEMP_COEFF_SUB;
    last_temp = temp;
    *out = temp;

    return HTU21_SUCCESS;
}

// Reads the relative humidity from the HTU21 sensor.
int htu21_read_humidity(float *out)
{
    unsigned char buf[HTU21_RESPONSE_SIZE];
    unsigned int h;
    float hum;

    if (htu21_read_bytes(HTU21_READHUM, buf, HTU21_RESPONSE_SIZE) != 0)
        return HTU21_ERROR;

    h = ((unsigned int)buf[0] << SHIFT_ONE_BYTE) | (buf[1] & HTU21_STATUS_BIT_MASK);

    hum = (float)h;
    hum *= HTU21_HUM_COEFF_MULTI;
    hum /= HTU21_HUM_COEFF_DIV;
    hum -= HTU21_HUM_COEFF_SUB;

    last_hum = hum;
    *out = hum;

    return HTU21_SUCCESS;
}

// Returns the last successfully read temperature from HTU21.
float htu21_last_temperature(void) { return last_temp; }

// Returns the last successfully read relative humidity from HTU21.
float htu21_last_humidity(void) { return last_hum; }