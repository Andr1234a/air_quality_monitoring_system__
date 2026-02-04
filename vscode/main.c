#include "stm8_s.h"
#include "i2c_driver.h"
#include "lcd_api.h"
#include "delay.h"
#include <stdint.h>

int main(void)
{
    int16_t encoder_value = 0;    // поточне значення енкодера
    int16_t last_value    = 0;    // попереднє значення
    int16_t delta         = 0;    // різниця для обробки
    
    CLK_CKDIVR = 0x00;//16Mhz

    TIM1_Encoder_Init(); // ініціалізація таймера-енкодера

    i2c_master_init(F_CPU, 10000); // ініціалізація i2c 
    lcd_init(); // ініціалізація дисплею


    while(1)
    {
        encoder_value = TIM1_Encoder_Get();      // зчитування значення

        delta = encoder_value - last_value;     // обчислюємо різницю
        last_value = encoder_value;

        lcd_clear();
        lcd_put_cur(0,0);
        lcd_send_int(encoder_value);
        
        delay_ms(500);
    }
}