/**
 * @file eeprom.h
 * @brief EEPROM (Data EEPROM) access functions for STM8 microcontrollers.
 *
 * This module provides a low-level interface for working with the internal
 * Data EEPROM of STM8 microcontrollers. It includes functions for:
 *  - unlocking and locking EEPROM for write access;
 *  - waiting for EEPROM write completion;
 *  - writing a buffer of bytes to EEPROM;
 *  - reading a buffer of bytes from EEPROM.
 *
 * The implementation directly accesses FLASH control registers.
 *
 * @date 2026-01-31
 */

#ifndef EEPROM_H
#define EEPROM_H

#include "stdint.h"


/**
 * @brief Unlock Data EEPROM for write access.
 *
 * This function unlocks the Data EEPROM by writing the required
 * key sequence to the FLASH_DUKR register. It then waits until
 * the DUL (Data Unlock) flag is set in the FLASH_IAPSR register.
 *
 * This function must be called before any EEPROM write operation.
 */
void eeprom_unlock(void);


/**
 * @brief Unlock option bytes memory.
 *
 * Enables write access to option bytes by configuring
 * FLASH_CR2 and FLASH_NCR2 registers.
 *
 * @note This function is not required for standard EEPROM
 *       read/write operations. Use with caution, as option
 *       bytes affect MCU configuration.
 */
void option_bytes_unlock(void);


/**
 * @brief Lock Data EEPROM after write operations.
 *
 * Clears the DUL (Data Unlock) flag in FLASH_IAPSR,
 * disabling further write access to the EEPROM.
 *
 * It is recommended to call this function after
 * completing EEPROM write operations.
 */
void eeprom_lock(void);

/**
 * @brief Wait until EEPROM write operation is completed.
 *
 * This function blocks execution until the EOP
 * (End Of Programming) flag is set in FLASH_IAPSR,
 * indicating that the current write operation has finished.
 */
void eeprom_wait_busy(void);


/**
 * @brief Write a buffer of bytes to EEPROM.
 *
 * Writes `len` bytes from the data buffer to EEPROM,
 * starting from the specified EEPROM address.
 *
 * EEPROM is automatically unlocked before writing
 * and locked again after the operation is complete.
 *
 * @param[in] start_addr Starting EEPROM address.
 * @param[in] data Pointer to the data buffer to be written.
 * @param[in] len Number of bytes to write.
 */
void eeprom_write_buff(uint16_t start_addr, const uint8_t *data, uint16_t len);


/**
 * @brief Read a buffer of bytes from EEPROM.
 *
 * Reads `len` bytes from EEPROM starting at the specified
 * address and stores them into the provided buffer.
 *
 * @param[in] start_addr Starting EEPROM address.
 * @param[in] buf Pointer to the buffer where read data will be stored.
 * @param[in] len Number of bytes to read.
 */
void eeprom_read_buff(uint16_t start_addr, uint8_t *buf, uint16_t len);


#endif