#include "lcd_api.h"
#include "i2c_driver.h"
#include "delay.h"
#include "stm8_s.h"

// Sends a command byte to the LCD via PCF8574 I2C I/O expander.
void lcd_send_cmd(char cmd)
{
    char data_u, data_l;
    uint8_t data_t[LCD_TX_BUFFER_SIZE];

    data_u = (cmd & LCD_MASK_NIBBLE);
    data_l = ((cmd << 4) & LCD_MASK_NIBBLE);

    data_t[0] = data_u | LCD_CMD_EN_HIGH;
    data_t[1] = data_u | LCD_CMD_EN_LOW;
    data_t[2] = data_l | LCD_CMD_EN_HIGH;
    data_t[3] = data_l | LCD_CMD_EN_LOW;

    i2c_master_transmit(LCD_ADDR, data_t, 4);
}

// Sends a data byte to the LCD via PCF8574 I2C I/O expander
void lcd_send_data(char data)
{
    char data_u, data_l;
    uint8_t data_t[LCD_TX_BUFFER_SIZE];
    data_u = (data & LCD_MASK_NIBBLE);
    data_l = ((data << 4) & LCD_MASK_NIBBLE);
    data_t[0] = data_u | LCD_DATA_EN_HIGH;
    data_t[1] = data_u | LCD_DATA_EN_LOW;
    data_t[2] = data_l | LCD_DATA_EN_HIGH;
    data_t[3] = data_l | LCD_DATA_EN_LOW;
    i2c_master_transmit(LCD_ADDR, data_t, 4);
}

// Initializes the LCD via PCF8574 I2C I/O expander
void lcd_init(void)
{

    delay_ms(50);
    lcd_send_cmd(LCD_CMD_FUNCTION_SET_8BIT);
    delay_ms(5);
    lcd_send_cmd(LCD_CMD_FUNCTION_SET_8BIT);
    delay_us(200);
    lcd_send_cmd(LCD_CMD_FUNCTION_SET_8BIT);
    delay_ms(10);
    lcd_send_cmd(LCD_CMD_4BIT_MODE);
    delay_ms(10);

    lcd_send_cmd(LCD_CMD_FUNCTION_SET_4BIT);
    delay_ms(1);
    lcd_send_cmd(LCD_CMD_EN_LOW);
    delay_ms(1);
    lcd_send_cmd(LCD_CMD_CLEAR);
    delay_ms(2);
    lcd_send_cmd(LCD_CMD_ENTRY_MODE);
    delay_ms(1);
    lcd_send_cmd(LCD_CMD_DISPLAY_ON);
}

// ends a null-terminated string to the LCD
void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}

// Sets the cursor position on the LCD
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
    case 0:
        col |= LCD_DDRAM_ROW0_ADDR;
        break;
    case 1:
        col |= LCD_DDRAM_ROW1_ADDR;
        break;
    }
    lcd_send_cmd(col);
}

// Sends an integer number to the LCD as a string
void lcd_send_int(int num)
{

    char buf[BUF_SIZE_SEND_INT];
    int i = 0, j;
    char tmp[BUF_SIZE_SEND_INT];

    if (num == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
    }
    else
    {
        while (num > 0)
        {
            tmp[i++] = (num % 10) + '0';
            num /= 10;
        }
        for (j = 0; j < i; j++)
        {
            buf[j] = tmp[i - j - 1];
        }
        buf[i] = '\0';
    }

    lcd_send_string(buf);
}

// Clears the LCD display and resets the cursor to home
void lcd_clear(void)
{
    lcd_send_cmd(LCD_CMD_CLEAR);
    delay_ms(2);
}

// Sends a floating-point number to the LCD as a string
void lcd_send_float(float num)
{
    char result[BUF_SIZE_SEND_FLOAT];
    int int_part;
    int frac_part;
    int i = 0;

    if (num < 0)
    {
        num = -num;
        lcd_send_string("-");
    }

    num += LCD_FLOAT_ROUND_HALF;

    int_part = (int)num;
    frac_part = (int)(num * LCD_FLOAT_MULT_FACTOR) % LCD_FLOAT_MOD_FACTOR;

    if (int_part >= 100)
    {
        result[i++] = (int_part / 100) + '0';
        result[i++] = ((int_part / 10) % 10) + '0';
        result[i++] = (int_part % 10) + '0';
    }
    else if (int_part >= 10)
    {
        result[i++] = (int_part / 10) + '0';
        result[i++] = (int_part % 10) + '0';
    }
    else
    {
        result[i++] = int_part + '0';
    }

    result[i++] = '.';

    result[i++] = frac_part + '0';

    result[i] = '\0';

    lcd_send_string(result);
}

void lcd_clear_line(uint8_t row)
{
    uint8_t i;
    lcd_put_cur(row, 0);
    for (i = 0; i < LCD_COLS; i++)
    {
        lcd_send_string(" ");
    }
}