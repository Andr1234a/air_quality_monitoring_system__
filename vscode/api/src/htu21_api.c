#include "i2c_driver.h"
#include "htu21_api.h"
#include "stm8_s.h"


/**
 * @brief Cached last valid temperature value.
 *
 * Contains the most recent temperature measurement that was
 * successfully read from the sensor.
 *
 * Initialized to an invalid sentinel value.
 */
static float last_temp = -1000.0f;

/**
 * @brief Cached last valid humidity value.
 *
 * Contains the most recent humidity measurement that was
 * successfully read from the sensor.
 *
 * Initialized to an invalid sentinel value.
 */
static float last_hum  = -1000.0f;


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
    if (rc != 0) return -1;

    rc = i2c_master_send_addr(HTU21_I2C_ADDR, 0); // write 
    if (rc != 0) {
        i2c_master_stop();
        return -1;
    }

    rc = i2c_master_write_byte(cmd);
    if (rc != 0) {
        i2c_master_stop();
        return -1;
    }

    i2c_master_stop();

    delay_ms(50);

    rc = i2c_master_start();
    if (rc != 0) return -1;

    rc = i2c_master_send_addr(HTU21_I2C_ADDR, 1); // read 
    if (rc != 0) {
        i2c_master_stop();
        return -1;
    }

    for (i = 0; i < len; i++) {
        d = i2c_master_read_byte((i < (len - 1)) ? 1 : 0);
        if (d < 0) {
            i2c_master_stop();
            return -1;
        }
        buf[i] = (unsigned char)d;
    }

    return 0;
}



//Reads the temperature from the HTU21 sensor.
float htu21_read_temperature(void)
{
    unsigned char buf[3];
    unsigned int t;
    float temp;

    if (htu21_read_bytes(HTU21_READTEMP, buf, 3) != 0)
        return -1000.0f;

    t = ((unsigned int)buf[0] << 8) | (buf[1] & 0xFC);

    temp = (float)t;
    temp *= 175.72f;
    temp /= 65536.0f;
    temp -= 46.85f;

    last_temp = temp;
    return temp;
}



//Reads the relative humidity from the HTU21 sensor.
float htu21_read_humidity(void)
{
    unsigned char buf[3];
    unsigned int h;
    float hum;

    if (htu21_read_bytes(HTU21_READHUM, buf, 3) != 0)
        return -1000.0f;

    h = ((unsigned int)buf[0] << 8) | (buf[1] & 0xFC);

    hum = (float)h;
    hum *= 125.0f;
    hum /= 65536.0f;
    hum -= 6.0f;

    last_hum = hum;
    return hum;
}


//Returns the last successfully read temperature from HTU21.
float htu21_last_temperature(void) { return last_temp; }


//Returns the last successfully read relative humidity from HTU21.
float htu21_last_humidity(void)    { return last_hum;  }
