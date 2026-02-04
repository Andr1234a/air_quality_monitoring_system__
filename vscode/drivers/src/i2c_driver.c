#include "i2c_driver.h"
#include "stm8_s.h"

typedef unsigned long timeout_t;


/**
 * @brief Clears the I2C acknowledge failure (AF) flag.
 *
 * This function clears the AF (Acknowledge Failure) flag in the I2C
 * status register SR2. The AF flag is typically set when a NACK is
 * received from the slave device.
 *
 * @note The AF flag is cleared by writing 0 to it.
 */
static void i2c_clear_af(void) {I2C_SR2 = (uint8_t)(~I2C_SR2_AF);}


//Initializes the I2C peripheral in master mode (standard mode)
void i2c_master_init(uint32_t cpu_hz, uint32_t i2c_hz)
{
    unsigned long ccr;
    uint8_t cpu_mhz;
    uint8_t tmp;

    CLK_PCKENR1 |= 0x01u; //Enable peripheral clock for I2C

    I2C_CR1 &= (uint8_t)(~I2C_CR1_PE); //Disable peripheral while configuring

    // program input clock (MHz) into FREQR 
    cpu_mhz = (uint8_t)(cpu_hz / 1000000UL);
    if (cpu_mhz == 0) cpu_mhz = 1;
    I2C_FREQR = cpu_mhz;

    // compute CCR for standard mode: CCR = fPCLK / (2 * fSCL) 
    ccr = (unsigned long)(cpu_hz / (2UL * i2c_hz));
    if (ccr < 4UL) ccr = 4UL;
    if (ccr > 0x0FFFUL) ccr = 0x0FFFUL; // 12-bit 

    I2C_CCRL = (uint8_t)(ccr & 0xFFu);

    // CCRH low nibble stores MSBs of CCR in standard mode; clear fast-mode bit 
    tmp = (uint8_t)((ccr >> 8) & 0x0Fu);
    I2C_CCRH = tmp; // F/S = 0 (standard mode), duty = 0 

    // TRISE = InputClockMHz + 1 (standard mode) 
    I2C_TRISER = (uint8_t)(cpu_mhz + 1u);

    // enable ACK by default 
    I2C_CR2 |= I2C_CR2_ACK;

    // Enable peripheral 
    I2C_CR1 |= I2C_CR1_PE;
}


//Generates an I2C START condition and waits for it to be sent
int i2c_master_start(void)
{
    timeout_t t = I2C_TIMEOUT_MAX;

    I2C_CR2 |= I2C_CR2_START;
    while ((I2C_SR1 & I2C_SR1_SB) == 0u)
    {
        if (--t == 0u) return -1;
    }
    return 0;
}


//Generates an I2C STOP condition
int i2c_master_stop(void)
{
    I2C_CR2 |= I2C_CR2_STOP;
    return 0;
}


//Sends a 7-bit slave address with direction bit on the I2C bus
int i2c_master_send_addr(uint8_t addr7, uint8_t dir)
{
    volatile uint8_t tmp;
    timeout_t t = I2C_TIMEOUT_MAX;

    I2C_DR = (uint8_t)((addr7 << 1) | (dir & 1u));//write address to DR (7-bit << 1)

    // wait for ADDR flag 
    while ((I2C_SR1 & I2C_SR1_ADDR) == 0u)
    {
        // check ACK failure 
        if (I2C_SR2 & I2C_SR2_AF)
        {
            // clear AF and return NACK 
            i2c_clear_af();
            return -2;
        }
        if (--t == 0u) return -1;
    }

    // clear ADDR by reading SR1 then SR3 (as required by HW) 
    tmp = I2C_SR1;
    tmp = I2C_SR3;
    (void)tmp;

    return 0;
}

//Transmits one data byte over the I2C bus
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
            return -2;
        }
        if (--t == 0u) return -1;
    }
    return 0;
}


//Receives one data byte from the I2C bus
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
        if (--t == 0u) return -1;
    }

    if (!ack)
    {
        // last byte from slave: generate STOP 
        I2C_CR2 |= I2C_CR2_STOP;
    }

    data = I2C_DR;
    return (int)data;
}


//Transmits a data buffer to an I2C slave device
int i2c_master_transmit(uint8_t addr7, const uint8_t *data, uint16_t size)
{
    int res;
    uint16_t i;

    // START condition 
    res = i2c_master_start();
    if (res != 0) return res;

    // Send slave address (write = 0) 
    res = i2c_master_send_addr(addr7, 0);
    if (res != 0) return res;

    // Send all bytes 
    for (i = 0; i < size; i++)
    {
        res = i2c_master_write_byte(data[i]);
        if (res != 0) return res;  // error or NACK 
    }

    // STOP condition 
    i2c_master_stop();

    return 0; 
}

