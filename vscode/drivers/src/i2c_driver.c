#include "i2c_driver.h"
#include "stm8_s.h"

typedef unsigned long timeout_t;

/* I2C Peripheral Clock Controls */
#define I2C_CLK_ENABLE_MASK 0x01u

/* Frequency Calculation Constants */
#define HZ_TO_MHZ_DIVISOR 1000000UL
#define I2C_MIN_CPU_MHZ 1
#define I2C_STANDARD_MODE_DIV 2UL

/* I2C CCR Register Limits & Masks */
#define I2C_CCR_MIN_VALUE 4UL
#define I2C_CCR_MAX_VALUE 0x0FFFUL
#define I2C_CCRL_MASK 0xFFu
#define I2C_CCRH_MSB_MASK 0x0Fu
#define I2C_CCRH_SHIFT 8

/* Bus Operations & Shifts */
#define I2C_ADDR_SHIFT 1
#define I2C_DIR_BIT_MASK 1u
#define I2C_WRITE_DIRECTION 0

/* API Error Codes */
#define I2C_SUCCESS 0
#define I2C_ERR_TIMEOUT -1
#define I2C_ERR_NACK -2

/**
 * @brief Clears the I2C acknowledge failure (AF) flag.
 *
 * This function clears the AF (Acknowledge Failure) flag in the I2C
 * status register SR2. The AF flag is typically set when a NACK is
 * received from the slave device.
 *
 * @note The AF flag is cleared by writing 0 to it.
 */
static void i2c_clear_af(void) { I2C_SR2 = (uint8_t)(~I2C_SR2_AF); }

// Initializes the I2C peripheral in master mode (standard mode)
void i2c_master_init(uint32_t cpu_hz, uint32_t i2c_hz)
{
    unsigned long ccr;
    uint8_t cpu_mhz;
    uint8_t tmp;

    CLK_PCKENR1 |= I2C_CLK_ENABLE_MASK; // Enable peripheral clock for I2C

    I2C_CR1 &= (uint8_t)(~I2C_CR1_PE); // Disable peripheral while configuring

    // program input clock (MHz) into FREQR
    cpu_mhz = (uint8_t)(cpu_hz / HZ_TO_MHZ_DIVISOR);
    if (cpu_mhz == 0)
        cpu_mhz = I2C_MIN_CPU_MHZ;
    I2C_FREQR = cpu_mhz;

    // compute CCR for standard mode: CCR = fPCLK / (2 * fSCL)
    ccr = (unsigned long)(cpu_hz / (I2C_STANDARD_MODE_DIV * i2c_hz));
    if (ccr < I2C_CCR_MIN_VALUE)
        ccr = I2C_CCR_MIN_VALUE;
    if (ccr > I2C_CCR_MAX_VALUE)
        ccr = I2C_CCR_MAX_VALUE; // 12-bit

    I2C_CCRL = (uint8_t)(ccr & I2C_CCRL_MASK);

    // CCRH low nibble stores MSBs of CCR in standard mode; clear fast-mode bit
    tmp = (uint8_t)((ccr >> I2C_CCRH_SHIFT) & I2C_CCRH_MSB_MASK);
    I2C_CCRH = tmp; // F/S = 0 (standard mode), duty = 0

    // TRISE = InputClockMHz + 1 (standard mode)
    I2C_TRISER = (uint8_t)(cpu_mhz + 1u);

    // enable ACK by default
    I2C_CR2 |= I2C_CR2_ACK;

    // Enable peripheral
    I2C_CR1 |= I2C_CR1_PE;
}

// Generates an I2C START condition and waits for it to be sent
int i2c_master_start(void)
{
    timeout_t t = I2C_TIMEOUT_MAX;

    I2C_CR2 |= I2C_CR2_START;
    while ((I2C_SR1 & I2C_SR1_SB) == 0u)
    {
        if (--t == 0u)
            return I2C_ERR_TIMEOUT;
    }
    return I2C_SUCCESS;
}

// Generates an I2C STOP condition
int i2c_master_stop(void)
{
    I2C_CR2 |= I2C_CR2_STOP;
    return I2C_SUCCESS;
}

// Sends a 7-bit slave address with direction bit on the I2C bus
int i2c_master_send_addr(uint8_t addr7, uint8_t dir)
{
    volatile uint8_t tmp;
    timeout_t t = I2C_TIMEOUT_MAX;

    I2C_DR = (uint8_t)((addr7 << I2C_ADDR_SHIFT) | (dir & I2C_DIR_BIT_MASK)); // write address to DR (7-bit << 1)

    // wait for ADDR flag
    while ((I2C_SR1 & I2C_SR1_ADDR) == 0u)
    {
        // check ACK failure
        if (I2C_SR2 & I2C_SR2_AF)
        {
            // clear AF and return NACK
            i2c_clear_af();
            return I2C_ERR_NACK;
        }
        if (--t == 0u)
            return I2C_ERR_TIMEOUT;
    }

    // clear ADDR by reading SR1 then SR3 (as required by HW)
    tmp = I2C_SR1;
    tmp = I2C_SR3;
    (void)tmp;

    return I2C_SUCCESS;
}

// Transmits one data byte over the I2C bus
int i2c_master_write_byte(uint8_t data)
{
    timeout_t t = I2C_TIMEOUT_MAX;

    I2C_DR = data;
    while ((I2C_SR1 & I2C_SR1_TXE) == 0u)
    {
        if (I2C_SR2 & I2C_SR2_AF)
        {
            // clear AF and signal nack
            i2c_clear_af();
            return I2C_ERR_NACK;
        }
        if (--t == 0u)
            return I2C_ERR_TIMEOUT;
    }
    return I2C_SUCCESS;
}

// Receives one data byte from the I2C bus
int i2c_master_read_byte(uint8_t ack)
{
    timeout_t t = I2C_TIMEOUT_MAX;
    uint8_t data;

    if (ack)
    {
        I2C_CR2 |= I2C_CR2_ACK;
    }
    else
    {
        I2C_CR2 &= (uint8_t)(~I2C_CR2_ACK);
    }

    // wait for RXNE (data available)
    while ((I2C_SR1 & I2C_SR1_RXNE) == 0u)
    {
        if (--t == 0u)
            return I2C_ERR_TIMEOUT;
    }

    if (!ack)
    {
        // last byte from slave: generate STOP
        I2C_CR2 |= I2C_CR2_STOP;
    }

    data = I2C_DR;
    return (int)data;
}

// Transmits a data buffer to an I2C slave device
int i2c_master_transmit(uint8_t addr7, const uint8_t *data, uint16_t size)
{
    int res;
    uint16_t i;

    // START condition
    res = i2c_master_start();
    if (res != I2C_SUCCESS)
        return res;

    // Send slave address (write = 0)
    res = i2c_master_send_addr(addr7, I2C_WRITE_DIRECTION);
    if (res != I2C_SUCCESS)
        return res;

    // Send all bytes
    for (i = 0; i < size; i++)
    {
        res = i2c_master_write_byte(data[i]);
        if (res != I2C_SUCCESS)
            return res; // error or NACK
    }

    // STOP condition
    i2c_master_stop();

    return I2C_SUCCESS;
}