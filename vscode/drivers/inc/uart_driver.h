/**
 * @file uart_driver.h
 * @brief UART1 driver for STM8 microcontrollers.
 *
 * This file provides a simple blocking driver for UART1 on STM8.
 * It supports basic initialization, character and string transmission,
 * character reception, and simple number formatting.
 *
 * Features:
 *  - UART1 initialization (8N1)
 *  - Blocking transmit and receive
 *  - String transmission
 *  - Integer and simple float output
 *
 * Intended for debugging, logging, and communication with a PC
 * terminal (e.g. via USB-UART converter).
 *
 * @note Interrupts and DMA are not used.
 * 
 * 
 * @date 2026-02-04
 */

#ifndef UART_DRIVER_H
#define UART_DRIVER_H


/**
 * @brief Initializes UART1 peripheral.
 *
 * Configures UART1 baud rate and frame format,
 * and enables transmitter and receiver.
 *
 * UART configuration:
 *  - 8 data bits
 *  - No parity
 *  - 1 stop bit
 *
 * @param[in] f_cpu     CPU clock frequency in Hz.
 * @param[in] baudrate  Desired UART baud rate (e.g. 9600, 115200).
 *
 * @note Baud rate divider is calculated as f_cpu / baudrate.
 */
void UART1_Init(unsigned long f_cpu, unsigned long baudrate);

/**
 * @brief Sends a single character via UART1.
 *
 * Waits until the transmit data register is empty (TXE flag),
 * then writes the character to the data register.
 *
 * @param[in] c Character to send.
 */
void UART1_SendChar(char c);

/**
 * @brief Sends a null-terminated string via UART1.
 *
 * Characters are transmitted one by one until
 * the terminating null character is reached.
 *
 * @param[in] s Pointer to null-terminated string.
 */
void UART1_SendString(const char *s);

/**
 * @brief Receives a single character via UART1.
 *
 * Blocks execution until a character is received
 * (RXNE flag set), then returns the received byte.
 *
 * @return Received character.
 */
char UART1_ReceiveChar(void);

/**
 * @brief Checks if UART1 has received data.
 *
 * Tests the RXNE flag without blocking.
 *
 * @retval 1  Data is available in the receive register.
 * @retval 0  No data available.
 */
unsigned char UART1_DataReady(void);

/**
 * @brief Sends a signed integer value via UART1.
 *
 * Converts the integer value to its ASCII representation
 * and transmits it as a string.
 *
 * Supports negative numbers.
 *
 * @param[in] value Integer value to send.
 */
void UART1_SendInt(int value);

/**
 * @brief Sends a floating-point value via UART1 (simple format).
 *
 * The number is transmitted in the format:
 *   xx.yy
 *
 * Only two digits after the decimal point are supported.
 *
 * @param[in] val Floating-point value to send.
 *
 * @note This function does not handle rounding or scientific notation.
 */
void UART1_SendFloatSimple(float val);

#endif