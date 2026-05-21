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

mhz19_status_t mhz19_read_co2(int *co2_value)
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

    uint8_t calc_checksum = 0; /* Changed to uint8_t to handle overflow naturally */
    uint8_t i;
    uint8_t bytes_received;
    volatile uint16_t timeout;

    bytes_received = 0;
    while (UART1_DataReady() && bytes_received < MHZ19_FLUSH_MAX)
    {
        UART1_ReceiveChar();
        bytes_received++;
    }

    for (i = 0; i < PACKET_SIZE; i++)
    {
        UART1_SendChar((char)MHZ19_CMD_READ_CO2_PKT[i]);
    }

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

    if (resp[0] != MHZ19_RESP_START_BYTE || resp[1] != MHZ19_RESP_CMD_CO2)
    {
        return MHZ19_ERR_INVALID_HEADER;
    }

    calc_checksum = 0;
    for (i = 1; i < 8; i++)
    {
        calc_checksum += resp[i];
    }
    /* 0xFF stands for 8-bit mask here (customary operation) */
    calc_checksum = (uint8_t)((0xFF - calc_checksum) + 1);

    if (resp[8] != calc_checksum)
    {
        return MHZ19_ERR_CHECKSUM;
    }

    // 6. Розрахунок результату CO2 (HIGH_BYTE * 256 + LOW_BYTE)
    *co2_value = ((int)resp[2] << 8) | (int)resp[3];
    return MHZ19_OK;
}