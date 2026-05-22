#include "lcd_api.h"
#include "i2c_driver.h"
#include "delay.h"
#include "stm8_s.h"

#define LCD_ADDR 0x27

#define LCD_TX_BUFFER_SIZE 4

#define BUF_SIZE_SEND_INT 10
#define BUF_SIZE_SEND_FLOAT 10

#define BUF_SIZE_TEMP_INT 7

#define LCD_COLS 16
#define LCD_ROWS 2

#define LCD_FLOAT_DECIMALS 1

#define LCD_FLOAT_ROUND_HALF 0.05f
#define LCD_FLOAT_MULT_FACTOR 10.0f
#define LCD_FLOAT_MOD_FACTOR 10

#define LCD_DELAY_POWER_ON_MS 50
#define LCD_DELAY_INIT_STAGE1_MS 5
#define LCD_DELAY_INIT_STAGE2_US 200
#define LCD_DELAY_INIT_STAGE3_MS 10

#define LCD_DELAY_COMMAND_MS 1
#define LCD_DELAY_CMD_CLEAR_MS 2

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

    delay_ms(LCD_DELAY_POWER_ON_MS);
    lcd_send_cmd(LCD_CMD_FUNCTION_SET_8BIT);
    delay_ms(LCD_DELAY_INIT_STAGE1_MS);
    lcd_send_cmd(LCD_CMD_FUNCTION_SET_8BIT);
    delay_us(LCD_DELAY_INIT_STAGE2_US);
    lcd_send_cmd(LCD_CMD_FUNCTION_SET_8BIT);
    delay_ms(LCD_DELAY_INIT_STAGE3_MS);
    lcd_send_cmd(LCD_CMD_4BIT_MODE);
    delay_ms(LCD_DELAY_INIT_STAGE3_MS);

    lcd_send_cmd(LCD_CMD_FUNCTION_SET_4BIT);
    delay_ms(LCD_DELAY_COMMAND_MS);
    lcd_send_cmd(LCD_CMD_EN_LOW);
    delay_ms(LCD_DELAY_COMMAND_MS);
    lcd_send_cmd(LCD_CMD_CLEAR);
    delay_ms(LCD_DELAY_CMD_CLEAR_MS);
    lcd_send_cmd(LCD_CMD_ENTRY_MODE);
    delay_ms(LCD_DELAY_COMMAND_MS);
    lcd_send_cmd(LCD_CMD_DISPLAY_ON);
    delay_ms(LCD_DELAY_COMMAND_MS);
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

void lcd_send_float(float num)
{
    char result[BUF_SIZE_SEND_FLOAT];
    char temp_int[BUF_SIZE_TEMP_INT];
    int int_part;
    int frac_part;
    int i = 0;
    int temp_idx = 0;

    if (num < 0)
    {
        num = -num;
        lcd_send_string("-");
    }

    num += LCD_FLOAT_ROUND_HALF;

    int_part = (int)num;

    frac_part = (int)(num * LCD_FLOAT_MULT_FACTOR) % LCD_FLOAT_MOD_FACTOR;

    if (int_part == 0)
    {
        temp_int[temp_idx++] = '0';
    }
    else
    {
        while (int_part > 0 && temp_idx < (BUF_SIZE_TEMP_INT - 1))
        {
            temp_int[temp_idx++] = (int_part % 10) + '0';
            int_part /= 10;
        }
    }
    while (temp_idx > 0)
    {
        result[i++] = temp_int[--temp_idx];
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