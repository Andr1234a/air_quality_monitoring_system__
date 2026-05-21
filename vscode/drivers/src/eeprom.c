#include "eeprom.h"
#include "stm8_s.h"

//Unlock Data EEPROM for write access
void eeprom_unlock(void) {
    FLASH_DUKR = FLASH_DUKR_KEY1;
    FLASH_DUKR = FLASH_DUKR_KEY2;
    while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_DUL)));
}

//Unlock option bytes memory
void option_bytes_unlock(void) {
    FLASH_CR2 |= (1 << FLASH_CR2_OPT);
    FLASH_NCR2 &= ~(1 << FLASH_NCR2_NOPT);
}

//Lock Data EEPROM after write operations
void eeprom_lock(void) {
    FLASH_IAPSR &= ~(1 << FLASH_IAPSR_DUL);
}

//Wait until EEPROM write operation is completed
void eeprom_wait_busy(void) {
    while (!(FLASH_IAPSR & (1 << FLASH_IAPSR_EOP)));
}

//Write a buffer of bytes to EEPROM.
void eeprom_write_buff(uint16_t start_addr, const uint8_t *data, uint16_t len){
	uint16_t i = 0;
	uint16_t addr = start_addr;

	eeprom_unlock();
	
	for (i; i<len; i++, addr++){
		_MEM_(addr) = data[i];
		eeprom_wait_busy();
	}
	eeprom_lock();
}

//Read a buffer of bytes from EEPROM
void eeprom_read_buff(uint16_t start_addr, uint8_t *buf, uint16_t len)
{
    uint16_t addr = start_addr;

    while (len--)
    {
        *buf++ = _MEM_(addr++);
    }
}
