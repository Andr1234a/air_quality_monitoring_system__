#include "lcd_api.h"
#include "i2c_driver.h"
#include "delay.h"
#include "stm8_s.h"



//Sends a command byte to the LCD via PCF8574 I2C I/O expander.
void lcd_send_cmd(char cmd){
    char data_u, data_l;
    uint8_t data_t[4];

    data_u = (cmd & 0xF0);
    data_l = ((cmd << 4) & 0xf0);

    data_t[0] = data_u | 0x0C;   
    data_t[1] = data_u | 0x08;
    data_t[2] = data_l | 0x0C;   
    data_t[3] = data_l | 0x08;

    i2c_master_transmit(0x27, data_t, 4);
}


//Sends a data byte to the LCD via PCF8574 I2C I/O expander
void lcd_send_data (char data)
{
	char data_u, data_l;
	uint8_t data_t[4];
	data_u = (data&0xf0);
	data_l = ((data<<4)&0xf0);
	data_t[0] = data_u|0x0D;  
	data_t[1] = data_u|0x09;  
	data_t[2] = data_l|0x0D;  
	data_t[3] = data_l|0x09;
    i2c_master_transmit(0x27, data_t, 4);
}


//Initializes the LCD via PCF8574 I2C I/O expander
void lcd_init(void){

    
    delay_ms(50);
    lcd_send_cmd(0x30);
    delay_ms(5);
    lcd_send_cmd(0x30);
    delay_us(200);
    lcd_send_cmd(0x30);
    delay_ms(10);
    lcd_send_cmd(0x20);
    delay_ms(10);

     
    lcd_send_cmd (0x28);
    delay_ms(1);
    lcd_send_cmd (0x08);
    delay_ms(1);
    lcd_send_cmd (0x01);
    delay_ms(2);
    lcd_send_cmd (0x06);
    delay_ms(1);
    lcd_send_cmd (0x0C);
}

//ends a null-terminated string to the LCD
void lcd_send_string (char *str)
{
    while (*str) lcd_send_data (*str++);
    
}

//Sets the cursor position on the LCD
void lcd_put_cur(int row, int col)
{
    switch (row)
    {
        case 0:
            col |= 0x80;
            break;
        case 1:
            col |= 0xC0;
            break;
    }
    lcd_send_cmd (col);
}

//Sends an integer number to the LCD as a string
void lcd_send_int(int num){

    char buf[10];    
    int i = 0, j;
    char tmp[10];

    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
    } else {
        while (num > 0) {
            tmp[i++] = (num % 10) + '0';
            num /= 10;
        }
        for (j = 0; j < i; j++) {
            buf[j] = tmp[i - j - 1];
        }
        buf[i] = '\0';
    }

    lcd_send_string (buf);

}

//Clears the LCD display and resets the cursor to home
void lcd_clear(void){
    lcd_send_cmd (0x01);
    delay_ms(2);
}


//Sends a floating-point number to the LCD as a string
void lcd_send_float(float num)
{
    char result[5];   
    int int_part;
    int frac_part;
    int i = 0;

    if (num < 0) {
        num = -num;
        lcd_send_string("-"); 
    }

    num += 0.05f;


    int_part = (int)num;
    frac_part = (int)(num * 10.0f) % 10;

    if (int_part >= 100) {
        result[i++] = (int_part / 100) + '0';
        result[i++] = ((int_part / 10) % 10) + '0';
        result[i++] = (int_part % 10) + '0';
    } else if (int_part >= 10) {
        result[i++] = (int_part / 10) + '0';
        result[i++] = (int_part % 10) + '0';
    } else {
        result[i++] = int_part + '0';
    }

    result[i++] = '.';


    result[i++] = frac_part + '0';

    result[i] = '\0';

    lcd_send_string(result);
}