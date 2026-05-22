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

#define BUZZER_FREQUENCY 1000
#define BUZZER_DUTY_CYCLE 8000
#define BUZZER_TIM1_CHANNEL 4
#define BUZZER_TIM1_PERIOD_INIT 1000
#define BUZZER_TIM1_PRESCALER_INIT 128

#define BUZZER_WARN_DURATION_ON 150
#define BUZZER_WARN_DURATION_OFF 300

#define BUZZER_ALARM_DURATION_ON 100
#define BUZZER_ALARM_DURATION_OFF 100

#define CO2_YELLOW_ZONE_PPM 800.0f
#define CO2_RED_ZONE_PPM 1500.0f

#define LCD_UPDATE_INTERVAL_MS 300

#define DOUBLE_CLICK_WINDOW_MS 400

#define CO2_DISPLAY_DIVIDER 10

#define BUTTON_PIN_NUM 3
#define ENCODER_A_PIN_NUM 6
#define ENCODER_B_PIN_NUM 7
#define ENCODER_SENTINEL 0xFFFF

#define MAIN_LOOP_TICK_MS 1

#define EEPROM_MAGIC_ADDR EEPROM_START_ADDR
#define EEPROM_MAGIC_HIGH 0x5A
#define EEPROM_MAGIC_LOW 0x23
#define EEPROM_MAGIC_SIZE 2
#define EEPROM_THRESHOLDS_ADDR (EEPROM_START_ADDR + EEPROM_MAGIC_SIZE)

#define DEFAULT_THR_TEMP 38.0f
#define DEFAULT_THR_HUM 60.0f
#define DEFAULT_THR_CO2 1000.0f

#define THRESHOLDS_BUF_SIZE 3

#define LCD_COL_MENU_START 0

#define LCD_LINE_LABELS 0
#define LCD_LINE_VALUES 1

#define LCD_COL_TEMP_LABEL 1
#define LCD_COL_TEMP_VALUE 0

#define LCD_COL_HUM_LABEL 7
#define LCD_COL_HUM_VALUE 6

#define LCD_COL_CO2_LABEL 13
#define LCD_COL_CO2_VALUE 12

typedef enum
{
    MODE_MONITOR,
    MODE_MENU
} system_mode_t;

typedef enum
{
    MENU_TEMP = 0,
    MENU_HUM = 1,
    MENU_CO2 = 2,
    MENU_COUNT
} menu_step_t;

volatile system_mode_t sys_mode = MODE_MONITOR;
volatile menu_step_t menu_step = MENU_TEMP;
volatile uint8_t button_pressed = 0;

float temp, hum, co2;

float active_thresholds[THRESHOLDS_BUF_SIZE];

void draw_hum_temp(void);
void play_alarm(void);
void save_threshold(float t, float h, float c);
void read_threshold(float *dest);
void init_eeprom_settings(void);
void update_active_thresholds(void);

int main(void)
{
    float thresholds[THRESHOLDS_BUF_SIZE];
    uint16_t encoder_val = 0;
    uint16_t last_encoder_val = ENCODER_SENTINEL;
    system_mode_t last_mode = MODE_MONITOR;
    uint8_t last_step = ENCODER_SENTINEL;

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
    Buzzer_Init(BUZZER_TIM1_CHANNEL, BUZZER_TIM1_PERIOD_INIT, BUZZER_TIM1_PRESCALER_INIT);

    init_eeprom_settings();

    EXTI_CR1 = (EXTI_CR1 & EXTI_PORTC_MASK) | EXTI_PORTC_FALLING;

    enableInterrupts();

    update_active_thresholds();

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
                    menu_step = MENU_TEMP;
                    click_count = 0;
                    double_click_timer = 0;
                }
            }
            else
            {
                menu_step++;
                if (menu_step >= MENU_COUNT)
                {

                    save_threshold(thresholds[MENU_TEMP], thresholds[MENU_HUM], thresholds[MENU_CO2]);

                    update_active_thresholds();

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
                last_step = ENCODER_SENTINEL;
                last_encoder_val = ENCODER_SENTINEL;
                read_threshold(thresholds);
            }

            encoder_val = Encoder_GetValue();

            if (menu_step != last_step)
            {
                lcd_clear();
                lcd_put_cur(LCD_LINE_LABELS, LCD_COL_MENU_START);
                if (menu_step == MENU_TEMP)
                    lcd_send_string("Set T Thr:");
                else if (menu_step == MENU_HUM)
                    lcd_send_string("Set H Thr:");
                else
                    lcd_send_string("Set CO2 Thr:");

                if (menu_step == MENU_TEMP)
                    Encoder_SetValue((uint16_t)thresholds[MENU_TEMP]);
                else if (menu_step == MENU_HUM)
                    Encoder_SetValue((uint16_t)thresholds[MENU_HUM]);
                else
                    Encoder_SetValue((uint16_t)(thresholds[MENU_CO2] / CO2_DISPLAY_DIVIDER));

                encoder_val = Encoder_GetValue();
                last_step = menu_step;
                last_encoder_val = ENCODER_SENTINEL;
            }

            if (encoder_val != last_encoder_val)
            {
                lcd_clear_line(1);

                if (menu_step == MENU_TEMP)
                {
                    thresholds[MENU_TEMP] = (float)encoder_val;
                    lcd_send_float(thresholds[MENU_TEMP]);
                }
                else if (menu_step == MENU_HUM)
                {
                    thresholds[MENU_HUM] = (float)encoder_val;
                    lcd_send_float(thresholds[MENU_HUM]);
                }
                else
                {
                    thresholds[MENU_CO2] = (float)encoder_val * CO2_DISPLAY_DIVIDER;
                    lcd_send_int((int)thresholds[MENU_CO2]);
                }
                last_encoder_val = encoder_val;
            }
        }

        delay_ms(MAIN_LOOP_TICK_MS);
    }
}

@far @interrupt void EXTI_PORTC_IRQHandler(void)
{
    button_pressed = 1;
}

void init_eeprom_settings(void)
{
    uint8_t magic[EEPROM_MAGIC_SIZE];

    eeprom_read_buff(EEPROM_MAGIC_ADDR, magic, EEPROM_MAGIC_SIZE);

    if (magic[0] != EEPROM_MAGIC_HIGH || magic[1] != EEPROM_MAGIC_LOW)
    {
        save_threshold(DEFAULT_THR_TEMP, DEFAULT_THR_HUM, DEFAULT_THR_CO2);

        magic[0] = EEPROM_MAGIC_HIGH;
        magic[1] = EEPROM_MAGIC_LOW;
        eeprom_write_buff(EEPROM_MAGIC_ADDR, magic, EEPROM_MAGIC_SIZE);
    }
}

void save_threshold(float t, float h, float c)
{
    float data[THRESHOLDS_BUF_SIZE];
    data[MENU_TEMP] = t;
    data[MENU_HUM] = h;
    data[MENU_CO2] = c;
    eeprom_write_buff(EEPROM_THRESHOLDS_ADDR, (uint8_t *)data, sizeof(data));
}

void read_threshold(float *dest)
{
    eeprom_read_buff(EEPROM_THRESHOLDS_ADDR, (uint8_t *)dest, sizeof(float) * THRESHOLDS_BUF_SIZE);
}

void draw_hum_temp(void)
{
    uint16_t co2_val;
    mhz19_status_t co2_status;
    uint8_t htu_success = 0;

    lcd_clear();

    if (htu21_read_temperature(&temp) == 0 && htu21_read_humidity(&hum) == 0)
    {
        htu_success = 1;

        lcd_put_cur(LCD_LINE_LABELS, LCD_COL_TEMP_LABEL);
        lcd_send_string("T");
        lcd_put_cur(LCD_LINE_VALUES, LCD_COL_TEMP_VALUE);
        lcd_send_float(temp);

        lcd_put_cur(LCD_LINE_LABELS, LCD_COL_HUM_LABEL);
        lcd_send_string("H");
        lcd_put_cur(LCD_LINE_VALUES, LCD_COL_HUM_VALUE);
        lcd_send_float(hum);
    }
    else
    {
        lcd_put_cur(LCD_LINE_LABELS, LCD_COL_TEMP_LABEL);
        lcd_send_string("T");
        lcd_put_cur(LCD_LINE_VALUES, LCD_COL_TEMP_VALUE);
        lcd_send_string("Err");

        lcd_put_cur(LCD_LINE_LABELS, LCD_COL_HUM_LABEL);
        lcd_send_string("H");
        lcd_put_cur(LCD_LINE_VALUES, LCD_COL_HUM_VALUE);
        lcd_send_string("Err");
    }

    co2_status = mhz19_read_co2(&co2_val);

    if (co2_status == MHZ19_OK)
    {
        co2 = (float)co2_val;

        lcd_put_cur(LCD_LINE_LABELS, LCD_COL_CO2_LABEL);
        lcd_send_string("CO");
        lcd_put_cur(LCD_LINE_VALUES, LCD_COL_CO2_VALUE);
        lcd_send_int(co2_val);
    }
    else
    {
        lcd_put_cur(LCD_LINE_LABELS, LCD_COL_CO2_LABEL);
        lcd_send_string("CO");
        lcd_put_cur(LCD_LINE_VALUES, LCD_COL_CO2_VALUE);
        lcd_send_string("Err");
    }
}

void update_active_thresholds(void)
{
    read_threshold(active_thresholds);
}

void play_alarm(void)
{
    float co2_threshold;

    co2_threshold = active_thresholds[MENU_CO2];
    if (co2_threshold > CO2_YELLOW_ZONE_PPM)
    {
        co2_threshold = CO2_YELLOW_ZONE_PPM;
    }

    if (co2 > CO2_RED_ZONE_PPM)
    {
        Buzzer_Start(BUZZER_FREQUENCY, BUZZER_DUTY_CYCLE, BUZZER_ALARM_DURATION_ON, BUZZER_ALARM_DURATION_OFF);
    }
    else if (temp > active_thresholds[MENU_TEMP] || hum > active_thresholds[MENU_HUM] || co2 >= co2_threshold)
    {
        Buzzer_Start(BUZZER_FREQUENCY, BUZZER_DUTY_CYCLE, BUZZER_WARN_DURATION_ON, BUZZER_WARN_DURATION_OFF);
    }
    else
    {
        Buzzer_Stop();
    }
}