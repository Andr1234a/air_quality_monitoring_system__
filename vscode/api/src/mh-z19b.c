#include "mh-z19b.h"
#include "uart_driver.h"

#define PACKET_SIZE 9
#define MHZ19_FLUSH_MAX 20
#define MHZ19_RX_TIMEOUT_TICKS 40000

/* Protocol bytes from Intelligent Infrared CO2 Sensor MH-Z19B Datasheet */
#define MHZ19_START_BYTE 0xFF
#define MHZ19_SENSOR_ADDR 0x01
#define MHZ19_CMD_READ_CO2 0x86

#define MHZ19_RESP_START_BYTE 0xFF
#define MHZ19_RESP_CMD_CO2 0x86

#define MHZ19_UNUSED_DATA_BYTES 0x00, 0x00, 0x00, 0x00, 0x00
#define MHZ19_CMD_READ_CO2_CHKSUM 0x79

void mhz19_init(unsigned long f_cpu)
{
    /*
     * Calculate Baud Rate registers (BRR1 & BRR2) for 9600 baud.
     * Divider = f_cpu / 9600
     */
    unsigned long div = f_cpu / 9600;

    /* Reset UART1 control registers to initial state */
    UART1_CR1 = 0x00;
    UART1_CR2 = 0x00;
    UART1_CR3 = 0x00;

    /*
     * Write division factor to BRR1 and BRR2.
     * STM8-specific bit arrangement:
     * BRR2 gets the 4 MSB bits and 4 LSB bits of the divider.
     * BRR1 gets the middle 8 bits.
     */
    UART1_BRR2 = (uint8_t)(((div >> 8) & 0xF0) | (div & 0x0F));
    UART1_BRR1 = (uint8_t)(div >> 4);

    /* Enable Transmitter (TE) and Receiver (RE) */
    UART1_CR2 |= (uint8_t)(0x08 | 0x04);
}

mhz19_status_t mhz19_read_co2(uint16_t *co2_value)
{
    /*
     * Command array definition for "Read CO2 measurement" (Command 0x86).
     * Structure according to Datasheet (Table 4 - Request Chamber):
     * [0] Start Byte (0xFF)
     * [1] Sensor Address (0x01)
     * [2] Command (0x86)
     * [3]-[7] Reserved/Data (0x00)
     * [8] Checksum (0x79 - calculated statically to save CPU cycles)
     */
    static const uint8_t MHZ19_CMD_READ_CO2_PKT[PACKET_SIZE] = {
        MHZ19_START_BYTE, MHZ19_SENSOR_ADDR, MHZ19_CMD_READ_CO2,
        MHZ19_UNUSED_DATA_BYTES, MHZ19_CMD_READ_CO2_CHKSUM};

    /* Static allocation to avoid overflowing the limited STM8 stack */
    static uint8_t resp[PACKET_SIZE];

    uint8_t calc_checksum = 0;
    uint8_t i;
    uint8_t bytes_received;
    volatile uint16_t timeout;

    /* 1. Flush the RX buffer to remove residual or obsolete data */
    bytes_received = 0;
    while (UART1_DataReady() && bytes_received < MHZ19_FLUSH_MAX)
    {
        UART1_ReceiveChar();
        bytes_received++;
    }

    /* 2. Send the 9-byte request frame to the sensor */
    for (i = 0; i < PACKET_SIZE; i++)
    {
        UART1_SendChar((char)MHZ19_CMD_READ_CO2_PKT[i]);
    }

    /* 3. Receive the 9-byte response frame with timeout monitoring */
    for (bytes_received = 0; bytes_received < PACKET_SIZE; bytes_received++)
    {
        timeout = MHZ19_RX_TIMEOUT_TICKS;
        while (!UART1_DataReady())
        {
            timeout--;
            if (timeout == 0)
            {
                return MHZ19_ERR_TIMEOUT;
            }
        }
        resp[bytes_received] = (uint8_t)UART1_ReceiveChar();
    }

    /* 4. Validate response frame structure */
    if (resp[0] != MHZ19_RESP_START_BYTE || resp[1] != MHZ19_RESP_CMD_CO2)
    {
        return MHZ19_ERR_INVALID_HEADER;
    }

    /* 5. Verify the 8-bit checksum */
    calc_checksum = 0;
    for (i = 1; i < 8; i++)
    {
        calc_checksum += resp[i];
    }
    calc_checksum = (uint8_t)((0xFF - calc_checksum) + 1);

    if (resp[8] != calc_checksum)
    {
        return MHZ19_ERR_CHECKSUM;
    }

    /* 6. Reconstruct and store the 16-bit CO2 PPM value (HIGH_BYTE * 256 + LOW_BYTE) */
    *co2_value = ((uint16_t)resp[2] << 8) | (uint16_t)resp[3];

    return MHZ19_OK;
}