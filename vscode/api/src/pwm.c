#include "pwm.h"
#include "tim1_driver.h"

static uint8_t buzzer_state=0;//<Current buzzer state
static uint32_t buzzer_timer=0;//<Internal timing counter
static uint16_t buzzer_on_ms=0;//<Buzzer ON duration in milliseconds
static uint16_t buzzer_off_ms=0;//<Buzzer OFF duration in milliseconds
static uint16_t buzzer_duty=0;//<PWM duty cycle used when the buzzer is ON

//Initializes the buzzer PWM subsystem
void Buzzer_Init(uint8_t channel, uint16_t period, uint16_t prescaler){
    TIM1_InitPWM(channel, period, prescaler);//base 4,1000, 128 
    TIM1_PWM_SetDuty(channel,0); 
    TIM1_Start();
}

//Starts periodic buzzer operation
void Buzzer_Start(uint16_t freq, uint16_t duty, uint16_t on_ms, uint16_t off_ms)
{
    buzzer_state = 0;
    buzzer_timer = 0;
    buzzer_duty = duty;
    buzzer_on_ms = on_ms;
    buzzer_off_ms = off_ms;

    TIM1_PWM_SetFrequency(4, 0, freq);
    TIM1_PWM_SetDuty(4, 0);
    TIM1_Start();
}

//Updates the buzzer state machine
void Buzzer_Update(void)
{
    buzzer_timer++;

    if(buzzer_state==0)
    {
        if(buzzer_timer>=buzzer_off_ms)
        {
            TIM1_PWM_SetDuty(4, buzzer_duty);
            buzzer_timer=0;
            buzzer_state=1;
        }
    }
    else
    {
        if(buzzer_timer>=buzzer_on_ms)
        {
            TIM1_PWM_SetDuty(4, 0);
            buzzer_timer=0;
            buzzer_state=0;
        }
    }
}

//Stops buzzer operation
void Buzzer_Stop(void)
{
    TIM1_PWM_SetDuty(4,0);
    TIM1_Stop();
    buzzer_state=0;
}