/**
 * @file interrupt_vector.c
 * @brief Interrupt vector table for STM8 microcontrollers.
 *
 * This file defines the interrupt vector table used by the STM8 CPU.
 * Each entry contains an interrupt instruction opcode and a pointer
 * to the corresponding interrupt service routine (ISR).
 *
 * Unused interrupts are mapped to a default handler to prevent
 * undefined behavior.
 *
 * @note The vector table must be located at the beginning of flash
 *       memory as required by the STM8 architecture.
 *
 * @author
 * @date
 */

#include "stm8_s.h"

/**
 * @brief Type definition for interrupt handler function pointer.
 *
 * All interrupt handlers must use the @interrupt attribute
 * and be located in far memory.
 */
typedef void @far (*interrupt_handler_t)(void);

/**
 * @brief Interrupt vector table entry structure.
 *
 * Each interrupt vector consists of:
 *  - interrupt_instruction: CPU opcode (typically 0x82 = JMPF)
 *  - interrupt_handler: pointer to interrupt service routine
 */
struct interrupt_vector {
    unsigned char interrupt_instruction;
    interrupt_handler_t interrupt_handler;
};

/**
 * @brief Default handler for unhandled interrupts.
 *
 * This function is called when an interrupt occurs for which
 * no specific handler has been installed.
 *
 * @note The handler intentionally does nothing and simply returns.
 *       This prevents unexpected jumps to random memory.
 */
@far @interrupt void NonHandledInterrupt(void)
{
    return;
}

/**
 * @brief Reset handler entry point.
 *
 * Defined by the linker script.
 * This symbol points to the start of the program code.
 */
extern void _stext(void);

/**
 * @brief External interrupt handler for PORTD.
 *
 * This handler must be implemented elsewhere in the project.
 */
extern @far @interrupt void EXTI_PORTD_IRQHandler(void);
// extern @far @interrupt void EXTI_PORTC_IRQHandler(void);

/**
 * @brief Interrupt vector table.
 *
 * The vector table maps all STM8 interrupt sources to their
 * corresponding handlers.
 *
 * Opcode 0x82 corresponds to the JMPF (Jump Far) instruction,
 * which allows jumping to far memory addresses.
 *
 * Interrupt mapping (STM8S series):
 *  - Vector 0: Reset
 *  - Vector 1: Trap
 *  - Vector 6: External interrupt PORTD
 *  - All other vectors use the default handler
 */
struct interrupt_vector const _vectab[] = {
    {0x82, (interrupt_handler_t)_stext},                /**< Reset */
    {0x82, NonHandledInterrupt},                        /**< Trap */
    {0x82, NonHandledInterrupt},                        /**< IRQ0 */
    {0x82, NonHandledInterrupt},                        /**< IRQ1 */
    {0x82, NonHandledInterrupt},                        /**< IRQ2 */
    {0x82, NonHandledInterrupt},                        /**< IRQ3  PORTA */
    {0x82, NonHandledInterrupt},                        /**< IRQ4  PORTB */
    {0x82, NonHandledInterrupt},                        /**< IRQ5  PORTC */
    {0x82, (interrupt_handler_t)EXTI_PORTD_IRQHandler}, /**< IRQ6  PORTD */
    {0x82, NonHandledInterrupt},                        /**< IRQ7  PORTE */
    {0x82, NonHandledInterrupt},                        /**< IRQ8 */
    {0x82, NonHandledInterrupt},                        /**< IRQ9 */
    {0x82, NonHandledInterrupt},                        /**< IRQ10 */
    {0x82, NonHandledInterrupt},                        /**< IRQ11 */
    {0x82, NonHandledInterrupt},                        /**< IRQ12 */
    {0x82, NonHandledInterrupt},                        /**< IRQ13 */
    {0x82, NonHandledInterrupt},                        /**< IRQ14 */
    {0x82, NonHandledInterrupt},                        /**< IRQ15 */
    {0x82, NonHandledInterrupt},                        /**< IRQ16 */
    {0x82, NonHandledInterrupt},                        /**< IRQ17 */
    {0x82, NonHandledInterrupt},                        /**< IRQ18 */
    {0x82, NonHandledInterrupt},                        /**< IRQ19 */
    {0x82, NonHandledInterrupt},                        /**< IRQ20 */
    {0x82, NonHandledInterrupt},                        /**< IRQ21 */
    {0x82, NonHandledInterrupt},                        /**< IRQ22 */
    {0x82, NonHandledInterrupt},                        /**< IRQ23 */
    {0x82, NonHandledInterrupt},                        /**< IRQ24 */
    {0x82, NonHandledInterrupt},                        /**< IRQ25 */
    {0x82, NonHandledInterrupt},                        /**< IRQ26 */
    {0x82, NonHandledInterrupt},                        /**< IRQ27 */
    {0x82, NonHandledInterrupt},                        /**< IRQ28 */
    {0x82, NonHandledInterrupt},                        /**< IRQ29 */
};
