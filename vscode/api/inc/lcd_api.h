/**
 * @file lcd_api.h
 * @brief High-level API for HD44780-compatible LCD via I2C (PCF8574).
 *
 * This file implements a high-level LCD API for controlling
 * an HD44780-compatible character LCD using a PCF8574 I2C I/O expander.
 *
 * The API provides convenient functions for:
 * - LCD initialization
 * - Sending commands and data
 * - Printing strings, integers, and floating-point numbers
 * - Cursor positioning and display clearing
 *
 * Low-level I2C communication is handled by the i2c_driver module.
 *
 * @note LCD operates in 4-bit mode through PCF8574.
 * @note Default I2C address of the PCF8574 is 0x27.
 * 
 * @date 2026-02-04
 * 
 */

#ifndef LCD_API_H
#define LCD_API_H

#include <stdint.h>

/**
 * @brief Sends a command byte to the LCD via PCF8574 I2C I/O expander.
 *
 * This function sends an LCD command through a PCF8574 I2C GPIO expander.
 * The 8-bit command is transferred in 4-bit mode by splitting it into
 * upper and lower nibbles.
 *
 * The PCF8574 output pins are used to control the LCD signals:
 * - RS = 0 (command mode)
 * - EN is toggled to latch each nibble
 * - Backlight output is kept enabled
 *
 * @param[in] cmd  LCD command byte to be sent.
 *
 * @note This function assumes a PCF8574-based LCD adapter
 *       with I2C address 0x27.
 */
void lcd_send_cmd(char cmd);

/**
 * @brief Sends a data byte to the LCD via PCF8574 I2C I/O expander.
 *
 * This function writes a single data byte (character) to an LCD using
 * a PCF8574 I2C GPIO expander in 4-bit mode. The byte is split into
 * upper and lower nibbles and transmitted sequentially.
 *
 * The PCF8574 output pins are used to control the LCD signals:
 * - RS = 1 (data mode)
 * - EN is toggled to latch each nibble
 * - Backlight output is kept enabled
 *
 * @param[in] data  The data byte (character) to send to the LCD.
 *
 * @note This function assumes a PCF8574-based LCD adapter
 *       with I2C address 0x27.
 */
void lcd_send_data (char data);

/**
 * @brief Initializes the LCD via PCF8574 I2C I/O expander.
 *
 * This function performs the standard initialization sequence for
 * an LCD using a PCF8574 I2C GPIO expander in 4-bit mode.
 * The sequence ensures the LCD is properly configured for:
 * - 4-bit interface
 * - 2-line display
 * - Display ON, cursor OFF, blink OFF
 * - Entry mode set (increment, no shift)
 *
 * Timing delays are applied according to LCD power-up requirements.
 *
 * @note This function assumes the LCD is connected through a
 *       PCF8574 I2C adapter at address 0x27.
 * @note Backlight is enabled by default in the command/data functions.
 */
void lcd_init(void);

/**
 * @brief Sends a null-terminated string to the LCD via PCF8574.
 *
 * This function transmits a sequence of characters to the LCD by
 * repeatedly calling `lcd_send_data()`. The string must be
 * null-terminated.
 *
 * @param[in] str  Pointer to the null-terminated string to display.
 *
 * @note Each character is sent in 4-bit mode through the PCF8574 I2C
 *       expander. Backlight is assumed to be enabled.
 * @note The string will be displayed starting from the current cursor
 *       position; it does not automatically wrap lines.
 */
void lcd_send_string (char *str);

/**
 * @brief Sets the cursor position on the LCD.
 *
 * This function moves the LCD cursor to the specified row and column.
 * The position is converted into the corresponding DDRAM address
 * and sent as a command via `lcd_send_cmd()`.
 *
 * @param[in] row  Row index (0 or 1 for 2-line display).
 * @param[in] col  Column index (0–15 for 16x2 LCD).
 *
 * @note The function assumes a standard 16x2 LCD connected through
 *       a PCF8574 I2C expander at address 0x27.
 * @note If the specified row exceeds the number of rows, the function
 *       defaults to the first row.
 */
void lcd_put_cur(int row, int col);

/**
 * @brief Sends an integer number to the LCD as a string.
 *
 * This function converts a non-negative integer into a decimal
 * string representation and sends it to the LCD using
 * `lcd_send_string()`.
 *
 * @param[in] num  Integer number to display (assumes non-negative).
 *
 * @note The function supports only non-negative integers.
 * @note Maximum number of digits is 10 (fits in `buf[10]`).
 * @note The number is displayed starting from the current cursor
 *       position; it does not handle line wrapping.
 */
void lcd_send_int(int num);

/**
 * @brief Clears the LCD display and resets the cursor to home.
 *
 * This function sends the clear display command to the LCD via
 * `lcd_send_cmd()`. After clearing, the cursor is set to the
 * home position (row 0, column 0).
 *
 * @note A delay of 2 ms is applied after the command to allow the
 *       LCD to complete the clear operation.
 * @note This function uses a PCF8574 I2C GPIO expander at address 0x27.
 */
void lcd_clear(void);

/**
 * @brief Sends a floating-point number to the LCD as a string.
 *
 * This function converts a floating-point number into a decimal string
 * with one digit after the decimal point and sends it to the LCD
 * using `lcd_send_string()`.
 *
 * @param[in] num  Floating-point number to display.
 *
 * @note The function rounds the number to one decimal place.
 * @note Negative numbers are handled, with a '-' printed before the number.
 * @note Maximum displayable integer part is 3 digits (up to 999.9).
 * @note The number is displayed starting from the current cursor position.
 */
void lcd_send_float(float num);

#endif
