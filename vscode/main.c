#include "stm8_s.h"
#include "i2c_driver.h"
#include "lcd_api.h"
#include "delay.h"
#include "htu21_api.h"
#include "eeprom.h"
#include "pwm.h"
#include "encoder.h"
#include "exti_driver.h"
#include "gpio_driver.h"
#include "mh-z19b.h"
#include <stdint.h>

#define FREQUENCY 1000
#define DUTY_CYCLE 8000
#define DURATION_ON 100
#define DURATION_OFF 100
#define BUZZER_TIM1_CHANNEL 4
#define TIM1_PERIOD_INIT 1000
#define TIM1_PRESCALER_INIT 128

#define LCD_UPDATE_INTERVAL_MS 200
#define DOUBLE_CLICK_WINDOW_MS 400
#define CO2_DISPLAY_DIVIDER 10

#define BUTTON_PIN_NUM 3
#define ENCODER_A_PIN_NUM 6
#define ENCODER_B_PIN_NUM 7

typedef enum
{
    MODE_MONITOR,
    MODE_MENU
} system_mode_t;

volatile system_mode_t sys_mode = MODE_MONITOR;
volatile uint8_t menu_step = 0;
volatile uint8_t button_pressed = 0;

float temp, hum, co2;

void draw_hum_temp(void);
void play_alarm(void);
void save_threshold(float t, float h, float c);
void read_threshold(float *dest);

int main(void)
{
    float thresholds[3];
    int8_t encoder_val = 0;
    int8_t last_encoder_val = 0xFF;
    system_mode_t last_mode = MODE_MONITOR;
    uint8_t last_step = 0xFF;

    uint16_t double_click_timer = 0;
    uint8_t click_count = 0;
    int lcd_timer = 0;

    GPIO_Pin btn_pin;
    GPIO_Pin enc_a_pin;
    GPIO_Pin enc_b_pin;

    CPU16Hz_Init();

    GPIO_PIN_INIT(btn_pin, GPIOC, BUTTON_PIN_NUM);
    GPIO_PIN_INIT(enc_a_pin, GPIOC, ENCODER_A_PIN_NUM);
    GPIO_PIN_INIT(enc_b_pin, GPIOC, ENCODER_B_PIN_NUM);

    GPIO_Config(&btn_pin, GPIO_INPUT, GPIO_PULLUP);
    GPIO_Config(&enc_a_pin, GPIO_INPUT, GPIO_PULLUP);
    GPIO_Config(&enc_b_pin, GPIO_INPUT, GPIO_PULLUP);

    (*(btn_pin.CR2)) &= ~((1 << ENCODER_A_PIN_NUM) | (1 << ENCODER_B_PIN_NUM));
    (*(btn_pin.CR2)) |= (1 << BUTTON_PIN_NUM);

    i2c_master_init(F_CPU, F_I2S_100KHz);
    lcd_init();
    Buzzer_Init(BUZZER_TIM1_CHANNEL, TIM1_PERIOD_INIT, TIM1_PRESCALER_INIT);

    save_threshold(38.0f, 60.0f, 1000.0f);

    EXTI_CR1 = (EXTI_CR1 & EXTI_PORTC_MASK) | EXTI_PORTC_FALLING;

    enableInterrupts();

    read_threshold(thresholds);

    while (1)
    {
        if (button_pressed)
        {
            button_pressed = 0;

            if (sys_mode == MODE_MONITOR)
            {
                click_count++;
                if (click_count == 1)
                {
                    double_click_timer = DOUBLE_CLICK_WINDOW_MS;
                }
                else if (click_count >= 2)
                {
                    sys_mode = MODE_MENU;
                    menu_step = 0;
                    click_count = 0;
                    double_click_timer = 0;
                }
            }
            else
            {
                menu_step++;
                if (menu_step > 2)
                {
                    save_threshold(thresholds[0], thresholds[1], thresholds[2]);
                    sys_mode = MODE_MONITOR;
                }
            }
        }

        if (double_click_timer > 0)
        {
            double_click_timer--;
            if (double_click_timer == 0)
            {
                click_count = 0;
            }
        }

        if (sys_mode == MODE_MONITOR)
        {
            if (last_mode != MODE_MONITOR)
            {
                Buzzer_Init(BUZZER_TIM1_CHANNEL, TIM1_PERIOD_INIT, TIM1_PRESCALER_INIT);
                last_mode = MODE_MONITOR;
                lcd_clear();
                lcd_timer = 0;
            }

            Buzzer_Update();

            lcd_timer++;
            if (lcd_timer >= LCD_UPDATE_INTERVAL_MS)
            {
                draw_hum_temp();
                play_alarm();
                lcd_timer = 0;
            }
        }
        else
        {
            if (last_mode != MODE_MENU)
            {
                Buzzer_Stop();
                Encoder_Enable();

                last_mode = MODE_MENU;
                last_step = 0xFF;
                last_encoder_val = 0xFF;
                read_threshold(thresholds);
            }

            encoder_val = Encoder_GetValue();

            if (menu_step != last_step)
            {
                lcd_clear();
                lcd_put_cur(0, 0);
                if (menu_step == 0)
                    lcd_send_string("Set T Thr:");
                else if (menu_step == 1)
                    lcd_send_string("Set H Thr:");
                else
                    lcd_send_string("Set CO2 Thr:");

                if (menu_step == 0)
                    Encoder_SetValue((uint8_t)thresholds[0]);
                else if (menu_step == 1)
                    Encoder_SetValue((uint8_t)thresholds[1]);
                else
                    Encoder_SetValue((uint8_t)(thresholds[2] / CO2_DISPLAY_DIVIDER));

                encoder_val = Encoder_GetValue();
                last_step = menu_step;
                last_encoder_val = 0xFF;
            }

            if (encoder_val != last_encoder_val)
            {
                lcd_put_cur(1, 0);
                lcd_send_string("                ");
                lcd_put_cur(1, 0);

                if (menu_step == 0)
                {
                    thresholds[0] = (float)encoder_val;
                    lcd_send_float(thresholds[0]);
                }
                else if (menu_step == 1)
                {
                    thresholds[1] = (float)encoder_val;
                    lcd_send_float(thresholds[1]);
                }
                else
                {
                    thresholds[2] = (float)encoder_val * CO2_DISPLAY_DIVIDER;
                    lcd_send_int((int)thresholds[2]);
                }
                last_encoder_val = encoder_val;
            }
        }

        delay_ms(1);
    }
}

@far @interrupt void EXTI_PORTC_IRQHandler(void)
{
    button_pressed = 1;
}

void draw_hum_temp(void)
{
    int co2_val;
    mhz19_status_t co2_status;

    co2_val = 0;

    if (htu21_read_temperature(&temp) == 0 && htu21_read_humidity(&hum) == 0)
    {
        // co2_status = mhz19_read_co2(&co2_val);
        if (co2_status == MHZ19_OK)
        {
            co2 = (float)co2_val;
        }
        else
        {
            co2_val = 0;
        }

        lcd_clear();
        lcd_put_cur(0, 1);
        lcd_send_string("T");
        lcd_put_cur(1, 0);
        lcd_send_float(temp);

        lcd_put_cur(0, 7);
        lcd_send_string("H");
        lcd_put_cur(1, 6);
        lcd_send_float(hum);

        lcd_put_cur(0, 13);
        lcd_send_string("CO");
        lcd_put_cur(1, 12);
        lcd_send_int(co2_val);
    }
    else
    {
        lcd_clear();
        lcd_put_cur(0, 1);
        lcd_send_string("ERROR HTU21");
    }
}

void play_alarm(void)
{
    float thresholds[3];
    read_threshold(thresholds);

    if (temp > thresholds[0])
        Buzzer_Start(frequency, duty_cycle, duration_on, duration_off);
    else if (hum > thresholds[1])
        Buzzer_Start(frequency, duty_cycle, duration_on, duration_off);
    else if (co2 > thresholds[2])
        Buzzer_Start(frequency, duty_cycle, duration_on, duration_off);
    else
        Buzzer_Stop();
}

void save_threshold(float t, float h, float c)
{
    float data[3];
    data[0] = t;
    data[1] = h;
    data[2] = c;
    eeprom_write_buff(EEPROM_START_ADDR, (uint8_t *)data, sizeof(data));
}

void read_threshold(float *dest)
{
    eeprom_read_buff(EEPROM_START_ADDR, (uint8_t *)dest, sizeof(float) * 3);
}