#ifndef EXTI_DRIVER_H
#define EXTI_DRIVER_H

#include <stdint.h>
#include "stm8_s.h"

/**
  * @brief  Deinitializes the external interrupt control registers to their default reset value.
  * @param  None
  * @retval None
  */
void EXTI_DeInit(void);

/**
  * @brief  Set the external interrupt sensitivity of the selected port.
  * @warning
  * - The modification of external interrupt sensitivity is only possible when the interrupts are disabled.
  * - The normal behavior is to disable the interrupts before calling this function, and re-enable them after.
  * @param[in]   Port The port number to access.
  * @param[in]   SensitivityValue The external interrupt sensitivity value to set.
  * @par Required preconditions:
  * Global interrupts must be disabled before calling this function.
  */
void EXTI_SetExtIntSensitivity(EXTI_Port_TypeDef Port, EXTI_Sensitivity_TypeDef SensitivityValue);

/**
  * @brief  Set the TLI interrupt sensitivity.
  * @param[in]   SensitivityValue The TLI interrupt sensitivity value.
  * @par Required preconditions:
  * Global interrupts must be disabled before calling this function.
  */
void EXTI_SetTLISensitivity(EXTI_TLISensitivity_TypeDef SensitivityValue);

/**
  * @brief  Get the external interrupt sensitivity of the selected port.
  * @param[in]   Port The port number to access.
  * @retval EXTI_Sensitivity_TypeDef The external interrupt sensitivity of the selected port.
  */
EXTI_Sensitivity_TypeDef EXTI_GetExtIntSensitivity(EXTI_Port_TypeDef Port);

/**
  * @brief  Get the TLI interrupt sensitivity.
  * @param  None
  * @retval EXTI_TLISensitivity_TypeDef The TLI interrupt sensitivity read.
  */
EXTI_TLISensitivity_TypeDef EXTI_GetTLISensitivity(void);


#endif