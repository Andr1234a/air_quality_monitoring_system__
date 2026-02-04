#ifndef TIM1_DRIVER_H
#define TIM1_DRIVER_H

#include <stdint.h>



void TIM1_InitPWM(uint8_t channel, uint16_t period, uint16_t prescaler);
void TIM1_PWM_SetDuty(uint8_t channel, uint16_t duty);
void TIM1_PWM_SetFrequency(uint8_t channel, uint32_t duty, uint32_t freq_hz);
void TIM1_Start(void);
void TIM1_Stop(void); 
void TIM1_Encoder_Init(void);
int16_t TIM1_Encoder_Get(void);

#endif