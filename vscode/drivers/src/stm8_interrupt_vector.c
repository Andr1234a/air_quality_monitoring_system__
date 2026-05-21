/**
 * @file stm8_interrupt_vector.c
 * @brief Таблиця векторів переривань для мікроконтролера STM8.
 *
 * Файл містить структуру векторів переривань, що необхідна для роботи
 * компілятора Cosmic. Всі обробники, що реалізовані в інших модулях,
 * оголошені як extern.
 *
 * @author А. Р. Бондар
 * @date 18.05.2026
 */

#include "stm8_s.h"

/** * @brief Тип для покажчика на функцію обробника переривання
 */
typedef void @far (*interrupt_handler_t)(void);

/** * @brief Структура елемента таблиці векторів
 */
struct interrupt_vector
{
    unsigned char interrupt_instruction;   /**< Опкод інструкції (0x82 для JMPF) */
    interrupt_handler_t interrupt_handler; /**< Адреса обробника */
};

/**
 * @brief Стандартний обробник для невикористаних переривань.
 */
@far @interrupt void NonHandledInterrupt(void)
{
    return;
}

/** * @brief Точка входу в програму (визначається лінкером)
 */
extern void _stext(void);

/** * @brief Обробник переривання Port C (реалізований у main.c)
 */
extern @far @interrupt void EXTI_PORTC_IRQHandler(void);

/** * @brief Обробник переривання Port D (якщо використовується)
 */
extern @far @interrupt void EXTI_PORTD_IRQHandler(void);

/** * @brief Обробник переривання таймера TIM4 (реалізований у main.c)
 */
extern @far @interrupt void TIM4_UPD_IRQHandler(void);

/**
 * @brief Таблиця векторів переривань.
 * Масив структур, що розташовується за адресою 0x8000.
 * Кожен запис містить команду переходу (0x82) та адресу функції.
 */
struct interrupt_vector const _vectab[] = {
    {0x82, (interrupt_handler_t)_stext}, /**< 00: Reset */
    {0x82, NonHandledInterrupt},         /**< 01: Trap  */
    {0x82, NonHandledInterrupt},         /**< 02: TLI   */
    {0x82, NonHandledInterrupt},         /**< 03: AWU   */
    {0x82, NonHandledInterrupt},         /**< 04: CLK   */
    {0x82, NonHandledInterrupt},         /**< 05: IRQ3  PORTA */
    {0x82, NonHandledInterrupt},         /**< 06: IRQ4  PORTB */

    /* Вектор IRQ5 для Port C (Кнопка енкодера) */
    {0x82, (interrupt_handler_t)EXTI_PORTC_IRQHandler}, /**< 07: IRQ5  PORTC */

    {0x82, NonHandledInterrupt}, /**< 08: IRQ6  PORTD */
    {0x82, NonHandledInterrupt}, /**< 09: IRQ7  PORTE */
    {0x82, NonHandledInterrupt}, /**< 10: IRQ8  CAN_RX */
    {0x82, NonHandledInterrupt}, /**< 11: IRQ9  CAN_TX */
    {0x82, NonHandledInterrupt}, /**< 12: IRQ10 SPI */
    {0x82, NonHandledInterrupt}, /**< 13: IRQ11 TIM1_UPD */
    {0x82, NonHandledInterrupt}, /**< 14: IRQ12 TIM1_CAP */
    {0x82, NonHandledInterrupt}, /**< 15: IRQ13 TIM2_UPD */
    {0x82, NonHandledInterrupt}, /**< 16: IRQ14 TIM2_CAP */
    {0x82, NonHandledInterrupt}, /**< 17: IRQ15 TIM3_UPD */
    {0x82, NonHandledInterrupt}, /**< 18: IRQ16 TIM3_CAP */
    {0x82, NonHandledInterrupt}, /**< 19: IRQ17 UART1_TX */
    {0x82, NonHandledInterrupt}, /**< 20: IRQ18 UART1_RX */
    {0x82, NonHandledInterrupt}, /**< 21: IRQ19 I2C */
    {0x82, NonHandledInterrupt}, /**< 22: IRQ20 UART2_TX / UART3_TX */
    {0x82, NonHandledInterrupt}, /**< 23: IRQ21 UART2_RX / UART3_RX */
    {0x82, NonHandledInterrupt}, /**< 24: IRQ22 ADC1 / ADC2 */

    /* Вектор IRQ23 для таймера TIM4 (Апаратний відлік мілісекунд) */
    {0x82, NonHandledInterrupt}, /**< 25: IRQ23 TIM4 */
    {0x82, NonHandledInterrupt}, /**< 26: IRQ24 FLASH */
    {0x82, NonHandledInterrupt}, /**< 27: IRQ25 Reserved */
    {0x82, NonHandledInterrupt}, /**< 28: IRQ26 Reserved */
    {0x82, NonHandledInterrupt}, /**< 29: IRQ27 Reserved */
    {0x82, NonHandledInterrupt}, /**< 30: IRQ28 Reserved */
    {0x82, NonHandledInterrupt}, /**< 31: IRQ29 Reserved */
};