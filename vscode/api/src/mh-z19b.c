#include "mh-z19b.h"
#include "stm8_s.h"
#include "exti_driver.h"
#include "tim2_driver.h"

#define INTERRUPT @far @interrupt
#define INTERRUPT_HANDLER(a,b) @far @interrupt void a(void)

INTERRUPT void EXTI_PORTB_IRQHandler(void);
/* ================= CONFIG ================= */
#define MHZ19_PWM_PORT   GPIOD
#define MHZ19_PWM_PIN    3
#define MHZ19_EXTI_PORT  EXTI_PORT_GPIOD

/* ================= STATE ================= */
typedef struct {
    volatile uint16_t lastRise;
    volatile uint16_t lastFall;
    volatile uint16_t tHigh;
    volatile uint16_t tLow;
    volatile uint8_t  ready;
} MHZ19_PWM_State_t;

static MHZ19_PWM_State_t pwm;

/* ================= INIT ================= */
void MHZ19_PWM_Init(void)
{
    pwm.lastRise = 0;
    pwm.lastFall = 0;
    pwm.tHigh    = 0;
    pwm.tLow     = 0;
    pwm.ready    = 0;

    /* TIM2 = timebase (~1 µs @16 MHz) */
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 0xFFFF);
    TIM2_Cmd(ENABLE);

    /* GPIO input + EXTI */
    MHZ19_PWM_PORT->DDR &= ~(1 << MHZ19_PWM_PIN);
    MHZ19_PWM_PORT->CR1 &= ~(1 << MHZ19_PWM_PIN);
    MHZ19_PWM_PORT->CR2 |=  (1 << MHZ19_PWM_PIN);

    disableInterrupts();
    EXTI_SetExtIntSensitivity(MHZ19_EXTI_PORT, EXTI_SENSITIVITY_RISE_FALL);
    enableInterrupts();
}

/* ================= CO2 ================= */
uint16_t MHZ19_PWM_GetPPM(void)
{
    uint32_t Th;
    uint32_t Tl;
    uint32_t ppm;

    if (!pwm.ready)
        return 0;

    disableInterrupts();
    Th = pwm.tHigh;
    Tl = pwm.tLow;
    pwm.ready = 0;
    enableInterrupts();

    if (Th < 2000 || (Th + Tl) < 4000)
        return 0;

    ppm = 5000UL * (Th - 2000) / (Th + Tl - 4000);

    if (ppm > 5000)
        ppm = 5000;

    return (uint16_t)ppm;
}

/* ================= EXTI ISR ================= */
INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
    uint16_t now = ((uint16_t)TIM2_CNTRH << 8) | TIM2_CNTRL;

    if (MHZ19_PWM_PORT->IDR & (1 << MHZ19_PWM_PIN))
    {
        pwm.tLow     = now - pwm.lastFall;
        pwm.lastRise = now;
    }
    else
    {
        pwm.tHigh    = now - pwm.lastRise;
        pwm.lastFall = now;
        pwm.ready    = 1;
    }
}
