#ifndef __STM8S_H
#define __STM8S_H

#include <stdint.h>

#define _MEM_(mem_addr)         (*(volatile uint8_t *)(mem_addr))
#define _SFR_(mem_addr)         (*(volatile uint8_t *)(0x5000 + (mem_addr)))

#define F_CPU 16000000UL

#define enableInterrupts()    {_asm("rim\n");}
#define disableInterrupts()   {_asm("sim\n");}

//-----------------------------Clock control (CLK)--------------------------
#define CLK_CKDIVR  _SFR_(0xC6)
#define CLK_PCKENR1 _SFR_(0x07)/**< peripheral clock enable register 1*/ 


//______________________API___________________________________

//---------------HTU21_API(htu21_api.h)-------------
#define HTU21_I2C_ADDR   0x40  /* 7-bit адреса HTU21D */
#define HTU21_READTEMP   0xE3
#define HTU21_READHUM    0xE5


//-------------LCD_API(lcd_api.h)--------------------

#define PIN_RS  (1<<0)   // P0
#define PIN_RW  (1<<1)   // P1
#define PIN_EN  (1<<2)   // P2
#define PIN_BL  (1<<3)   // P3 (LED K, активний 1)
#define PIN_D4  (1<<4)   // P4
#define PIN_D5  (1<<5)   // P5
#define PIN_D6  (1<<6)   // P6
#define PIN_D7  (1<<7)   // P7

#define LCD_ADDR 0x27


#define LCD_COLS 16
#define LCD_ROWS 2

//______________DRIVER________________________________________

//---------------GPIO(gpio_driver.h)--------
/**
 * @brief Macro for initializing GPIO pins
 * 
 */
#define GPIO_PIN_INIT(pin_var, PORT, NUM)    \
    do {                                    \
        (pin_var).pin   = (NUM);            \
        (pin_var).DDR   = &(PORT)->DDR;     \
        (pin_var).CR1   = &(PORT)->CR1;     \
        (pin_var).CR2   = &(PORT)->CR2;     \
        (pin_var).ODR   = &(PORT)->ODR;     \
        (pin_var).IDR   = &(PORT)->IDR;     \
        (pin_var).state = GPIO_LOW;          \
    } while (0)

/**
 * @brief Structure for working with GPIO
 * 
 */
typedef struct {
    volatile uint8_t ODR;   
    volatile uint8_t IDR;   
    volatile uint8_t DDR;   
    volatile uint8_t CR1;   
    volatile uint8_t CR2;   
} GPIO_TypeDef;




#define GPIOA ((GPIO_TypeDef *)0x5000)
#define GPIOB ((GPIO_TypeDef *)0x5005)
#define GPIOC ((GPIO_TypeDef *)0x500A)
#define GPIOD ((GPIO_TypeDef *)0x500F)
#define GPIOE ((GPIO_TypeDef *)0x5014)


/**
 * @brief Enum for GPIO pin state
 * 
 */
typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH
} GPIO_State;

/**
 * @brief Enum for GPIO pin direction
 * 
 */
typedef enum {
    GPIO_INPUT = 0,
    GPIO_OUTPUT
} GPIO_Direction;

/**
 * @brief Enum for GPIO pin mode
 * 
 */
typedef enum {
    GPIO_FLOATING = 0,
    GPIO_PULLUP,
    GPIO_PUSHPULL,
    GPIO_OPENDRAIN
} GPIO_Mode;

/**
 * @brief Structure for working with a single GPIO pin 
 * 
 */
typedef struct {
    uint8_t pin;                  
    volatile uint8_t *DDR;
    volatile uint8_t *CR1;
    volatile uint8_t *CR2;
    volatile uint8_t *ODR;
    volatile uint8_t *IDR;
    GPIO_State state;
} GPIO_Pin;


//-------i2c----------
#define I2C_BASE   0x5210
#define I2C_CR1    (*(volatile uint8_t *)(I2C_BASE + 0x00))
#define I2C_CR2    (*(volatile uint8_t *)(I2C_BASE + 0x01))
#define I2C_FREQR  (*(volatile uint8_t *)(I2C_BASE + 0x02))
#define I2C_OARL   (*(volatile uint8_t *)(I2C_BASE + 0x03))
#define I2C_OARH   (*(volatile uint8_t *)(I2C_BASE + 0x04))
#define I2C_DR     (*(volatile uint8_t *)(I2C_BASE + 0x06))
#define I2C_SR1    (*(volatile uint8_t *)(I2C_BASE + 0x07))
#define I2C_SR2    (*(volatile uint8_t *)(I2C_BASE + 0x08))
#define I2C_SR3    (*(volatile uint8_t *)(I2C_BASE + 0x09))
#define I2C_CCRL   (*(volatile uint8_t *)(I2C_BASE + 0x0B))
#define I2C_CCRH   (*(volatile uint8_t *)(I2C_BASE + 0x0C))
#define I2C_TRISER (*(volatile uint8_t *)(I2C_BASE + 0x0D))

/* CR1/CR2 bits */
#define I2C_CR1_PE      ((uint8_t)0x01)
#define I2C_CR2_START   ((uint8_t)0x01)
#define I2C_CR2_STOP    ((uint8_t)0x08)
#define I2C_CR2_ACK     ((uint8_t)0x10)

/* SR1 bits */
#define I2C_SR1_SB      ((uint8_t)0x01)
#define I2C_SR1_ADDR    ((uint8_t)0x02)
#define I2C_SR1_RXNE    ((uint8_t)0x40)
#define I2C_SR1_TXE     ((uint8_t)0x80)

/* SR2 bits */
#define I2C_SR2_AF      ((uint8_t)0x04)  /* Acknowledge failure */

/* direction for send_addr */
#define I2C_DIR_WRITE 0
#define I2C_DIR_READ  1

#define I2C_TIMEOUT_MAX ((timeout_t)30000UL)

//--------------UART(uart_driver.h)--------------------------

/* Біти керування */
#define UART1_CR2_TEN ((uint8_t)0x08)
#define UART1_CR2_REN ((uint8_t)0x04)
#define UART1_SR_TXE  ((uint8_t)0x80)
#define UART1_SR_RXNE ((uint8_t)0x20)
#define UART1_SR_BSY  ((uint8_t)0x40)

#define UART1_SR   (*(volatile unsigned char *)0x5230)
#define UART1_DR   (*(volatile unsigned char *)0x5231)
#define UART1_BRR1 (*(volatile unsigned char *)0x5232)
#define UART1_BRR2 (*(volatile unsigned char *)0x5233)
#define UART1_CR1  (*(volatile unsigned char *)0x5234)
#define UART1_CR2  (*(volatile unsigned char *)0x5235)
#define UART1_CR3  (*(volatile unsigned char *)0x5236)


//---------------EEPROM(eeprom.h)-------------------
#define FLASH_PUKR_KEY1         0x56
#define FLASH_PUKR_KEY2         0xAE

#define FLASH_DUKR              _SFR_(0x64)
#define FLASH_DUKR_KEY1         FLASH_PUKR_KEY2
#define FLASH_DUKR_KEY2         FLASH_PUKR_KEY1
#define FLASH_IAPSR             _SFR_(0x5F)
#define FLASH_IAPSR_DUL         3

#define FLASH_CR2               _SFR_(0x5B)
#define FLASH_CR2_OPT           7

#define FLASH_NCR2              _SFR_(0x5C)
#define FLASH_NCR2_NOPT         7

#define FLASH_IAPSR_EOP         2

#define EEPROM_START_ADDR      0x4000
#define EEPROM_END_ADDR        0x407F

/* Option bytes */
#define OPT0                   _MEM_(0x4800)
#define OPT1                   _MEM_(0x4801)
#define NOPT1                  _MEM_(0x4802)
#define OPT2                   _MEM_(0x4803)
#define NOPT2                  _MEM_(0x4804)
#define OPT3                   _MEM_(0x4805)
#define NOPT3                  _MEM_(0x4806)
#define OPT4                   _MEM_(0x4807)
#define NOPT4                  _MEM_(0x4808)
#define OPT5                   _MEM_(0x4809)
#define NOPT5                  _MEM_(0x480A)

//-------------EXTI(exti_driver.h)---------

#define EXTI_CR1   (*(volatile uint8_t*)0x50A0)
#define EXTI_CR2   (*(volatile uint8_t*)0x50A1)

#define EXTI_CR1_RESET_VALUE ((uint8_t)0x00)
#define EXTI_CR2_RESET_VALUE ((uint8_t)0x00)

#define EXTI_CR1_PDIS ((uint8_t)0xC0) /*!< PORTD external interrupt sensitivity bits mask */
#define EXTI_CR1_PCIS ((uint8_t)0x30) /*!< PORTC external interrupt sensitivity bits mask */
#define EXTI_CR1_PBIS ((uint8_t)0x0C) /*!< PORTB external interrupt sensitivity bits mask */
#define EXTI_CR1_PAIS ((uint8_t)0x03) /*!< PORTA external interrupt sensitivity bits mask */

#define EXTI_CR2_TLIS ((uint8_t)0x04) /*!< Top level interrupt sensitivity bit mask */
#define EXTI_CR2_PEIS ((uint8_t)0x03) /*!< PORTE external interrupt sensitivity bits mask */


/**
  * @brief  EXTI Sensitivity values for PORTA to PORTE
  */
typedef enum {
  EXTI_SENSITIVITY_FALL_LOW  = 0x00, /*!< Interrupt on Falling edge and Low level */
  EXTI_SENSITIVITY_RISE_ONLY = 0x01, /*!< Interrupt on Rising edge only */
  EXTI_SENSITIVITY_FALL_ONLY = 0x02, /*!< Interrupt on Falling edge only */
  EXTI_SENSITIVITY_RISE_FALL = 0x03  /*!< Interrupt on Rising and Falling edges */
} EXTI_Sensitivity_TypeDef;

/**
  * @brief  EXTI Sensitivity values for TLI
  */
typedef enum {
  EXTI_TLISENSITIVITY_FALL_ONLY = 0x00, /*!< Top Level Interrupt on Falling edge only */
  EXTI_TLISENSITIVITY_RISE_ONLY = 0x04  /*!< Top Level Interrupt on Rising edge only */
} EXTI_TLISensitivity_TypeDef;

/**
  * @brief  EXTI PortNum possible values
  */
typedef enum {
  EXTI_PORT_GPIOA = 0x00, /*!< GPIO Port A */
  EXTI_PORT_GPIOB = 0x01, /*!< GPIO Port B */
  EXTI_PORT_GPIOC = 0x02, /*!< GPIO Port C */
  EXTI_PORT_GPIOD = 0x03, /*!< GPIO Port D */
  EXTI_PORT_GPIOE = 0x04  /*!< GPIO Port E */
} EXTI_Port_TypeDef;


/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for PORTA to PORTE.
  */
#define IS_EXTI_SENSITIVITY_OK(SensitivityValue) \
  (((SensitivityValue) == EXTI_SENSITIVITY_FALL_LOW) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_RISE_ONLY) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_FALL_ONLY) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_RISE_FALL))

/**
  * @brief  Macro used by the assert function in order to check the different sensitivity values for TLI.
  */
#define IS_EXTI_TLISENSITIVITY_OK(SensitivityValue) \
  (((SensitivityValue) == EXTI_TLISENSITIVITY_FALL_ONLY) || \
   ((SensitivityValue) == EXTI_TLISENSITIVITY_RISE_ONLY))

/**
  * @brief  Macro used by the assert function in order to check the different Port values
  */
#define IS_EXTI_PORT_OK(PORT) \
  (((PORT) == EXTI_PORT_GPIOA) ||\
   ((PORT) == EXTI_PORT_GPIOB) ||\
   ((PORT) == EXTI_PORT_GPIOC) ||\
   ((PORT) == EXTI_PORT_GPIOD) ||\
   ((PORT) == EXTI_PORT_GPIOE))

/**
  * @brief  Macro used by the assert function in order to check the different values of the EXTI PinMask
  */
#define IS_EXTI_PINMASK_OK(PinMask) ((((PinMask) & (uint8_t)0x00) == (uint8_t)0x00) && ((PinMask) != (uint8_t)0x00))

//-----------------------TIM1(tim1_driver.h)----------

#define TIM1_CR1_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_CR2_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_SMCR_RESET_VALUE  ((uint8_t)0x00)
#define TIM1_ETR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_IER_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_SR1_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_SR2_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_EGR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_CCMR1_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCMR2_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCMR3_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCMR4_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCER1_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCER2_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CNTRH_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CNTRL_RESET_VALUE ((uint8_t)0x00)
#define TIM1_PSCRH_RESET_VALUE ((uint8_t)0x00)
#define TIM1_PSCRL_RESET_VALUE ((uint8_t)0x00)
#define TIM1_ARRH_RESET_VALUE  ((uint8_t)0xFF)
#define TIM1_ARRL_RESET_VALUE  ((uint8_t)0xFF)
#define TIM1_RCR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_CCR1H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR1L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR2H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR2L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR3H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR3L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR4H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR4L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_BKR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_DTR_RESET_VALUE   ((uint8_t)0x00)
#define TIM1_OISR_RESET_VALUE  ((uint8_t)0x00)


#define TIM1_CR1   (*(volatile uint8_t*)0x5250)
#define TIM1_CR2   (*(volatile uint8_t*)0x5251)
#define TIM1_SMCR  (*(volatile uint8_t*)0x5252)
#define TIM1_ETR   (*(volatile uint8_t*)0x5253)
#define TIM1_IER   (*(volatile uint8_t*)0x5254)
#define TIM1_SR1   (*(volatile uint8_t*)0x5255)
#define TIM1_SR2   (*(volatile uint8_t*)0x5256)
#define TIM1_CCMR1 (*(volatile uint8_t*)0x5258)
#define TIM1_CCMR2 (*(volatile uint8_t*)0x5259)
#define TIM1_CCMR3 (*(volatile uint8_t*)0x525A)
#define TIM1_CCMR4 (*(volatile uint8_t*)0x525B)
#define TIM1_CCER1 (*(volatile uint8_t*)0x525C)
#define TIM1_CCER2 (*(volatile uint8_t*)0x525D)
#define TIM1_CNTRH (*(volatile uint8_t*)0x525E)
#define TIM1_CNTRL (*(volatile uint8_t*)0x525F)
#define TIM1_PSCRH (*(volatile uint8_t*)0x5260)
#define TIM1_PSCRL (*(volatile uint8_t*)0x5261)
#define TIM1_ARRH  (*(volatile uint8_t*)0x5262)
#define TIM1_ARRL  (*(volatile uint8_t*)0x5263)
#define TIM1_RCR   (*(volatile uint8_t*)0x5264)
#define TIM1_CCR3H (*(volatile uint8_t*)0x5269)
#define TIM1_CCR3L (*(volatile uint8_t*)0x526A)
#define TIM1_CCR4H (*(volatile uint8_t*)0x526B)
#define TIM1_CCR4L (*(volatile uint8_t*)0x526C)
#define TIM1_BKR   (*(volatile uint8_t*)0x526D)
#define TIM1_DTR   (*(volatile uint8_t*)0x526E)
#define TIM1_OISR  (*(volatile uint8_t*)0x526F)
#define TIM1_EGR   (*(volatile uint8_t*)0x5257)
#define TIM1_EGR_UG (1 << 0)

// PWM compare registers
#define TIM1_CCR1H (*(volatile uint8_t*)0x5265)
#define TIM1_CCR1L (*(volatile uint8_t*)0x5266)
#define TIM1_CCR2H (*(volatile uint8_t*)0x5267)
#define TIM1_CCR2L (*(volatile uint8_t*)0x5268)

#define PC_DDR (*(volatile uint8_t*)0x500C)
#define PC_CR1 (*(volatile uint8_t*)0x500D)
#define PC_CR2 (*(volatile uint8_t*)0x500E)


#endif