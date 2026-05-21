#include "mh-z19b.h"
#include "uart_driver.h"

#define PACKET_SIZE 9

mhz19_status_t mhz19_read_co2(int *co2_value)
{
    // Масиви оголошені як static, щоб не перевантажувати обмежений стек STM8
    static uint8_t cmd[PACKET_SIZE] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    static uint8_t resp[PACKET_SIZE];

    uint16_t calc_checksum = 0;
    uint8_t i;
    uint8_t bytes_received;
    volatile uint16_t timeout;

    // 1. Розрахунок контрольної суми для команди (суворо як в Arduino)
    for (i = 1; i < 8; i++)
    {
        calc_checksum += cmd[i];
    }
    calc_checksum = calc_checksum & 0xFF; // Залишаємо тільки молодший байт (імітуємо переповнення)
    cmd[8] = (uint8_t)((0xFF - calc_checksum) + 1);

    // 2. Очищення вхідного буфера UART від старого сміття
    bytes_received = 0;
    while (UART1_DataReady() && bytes_received < 20)
    {
        UART1_ReceiveChar();
        bytes_received++;
    }

    // 3. Відправка команди в датчик
    for (i = 0; i < PACKET_SIZE; i++)
    {
        UART1_SendChar((char)cmd[i]);
    }

    // 4. Прийом відповіді (9 байт) з безпечним таймаутом
    for (bytes_received = 0; bytes_received < PACKET_SIZE; bytes_received++)
    {
        timeout = 40000; // Програмний таймаут для 16 МГц без delay_ms
        while (!UART1_DataReady())
        {
            timeout--;
            if (timeout == 0)
            {
                return MHZ19_ERR_TIMEOUT;
            }
        }
        // Приведення типу до uint8_t обов'язкове, бо UART1_ReceiveChar повертає знаковий char
        resp[bytes_received] = (uint8_t)UART1_ReceiveChar();
    }

    // 5. Перевірка заголовка відповіді
    if (resp[0] != 0xFF || resp[1] != 0x86)
    {
        return MHZ19_ERR_INVALID_HEADER;
    }

    // 6. Перевірка контрольної суми отриманого пакету
    calc_checksum = 0;
    for (i = 1; i < 8; i++)
    {
        calc_checksum += resp[i];
    }
    calc_checksum = calc_checksum & 0xFF; // Маскування для 8-бітного переповнення
    calc_checksum = (uint8_t)((0xFF - calc_checksum) + 1);

    if (resp[8] != calc_checksum)
    {
        return MHZ19_ERR_CHECKSUM;
    }

    // 7. Розрахунок результату CO2 (HIGH_BYTE * 256 + LOW_BYTE)
    *co2_value = ((int)resp[2] << 8) | (int)resp[3];
    return MHZ19_OK;
}