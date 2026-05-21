#include "stm8_s.h"
#include "encoder.h"

void Encoder_Enable(void)
{

    TIM1_CR1 &= (uint8_t)(~0x01); // Зупиняємо таймер перед налаштуванням

    // ВИМИКАЄМО всі переривання TIM1, щоб уникнути зависання у NonHandledInterrupt!
    TIM1_IER = 0x00;
    TIM1_SR1 = 0x00;

    // Скидаємо конфігурацію каналів
    TIM1_CCER1 = 0x00;
    TIM1_CCER2 = 0x00;

    // Режим енкодера (Encoder mode 3)
    TIM1_SMCR = 0x03;

    // Налаштовуємо канали 1 та 2 на вхід
    TIM1_CCMR1 = 0x01;
    TIM1_CCMR2 = 0x01;

    TIM1_ARRH = 0;
    TIM1_ARRL = 100;

    // Скидаємо лічильник
    TIM1_CNTRH = 0;
    TIM1_CNTRL = 0;

    TIM1_CR1 |= 0x01; // Запуск таймера
}

int8_t Encoder_GetValue(void)
{
    // ЗА ВИМОГОЮ RM0016: ОБОВ'ЯЗКОВО ЧИТАЄМО СТАРШИЙ БАЙТ ПЕРШИМ!
    volatile uint8_t high = TIM1_CNTRH;
    volatile uint8_t low = TIM1_CNTRL;
    return (int8_t)low;
}

void Encoder_SetValue(uint8_t val)
{
    // Запис теж бажано робити парою, починаючи з High
    TIM1_CNTRH = 0;
    TIM1_CNTRL = val;
}