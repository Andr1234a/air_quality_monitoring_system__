#ifndef __STM8S_H
#define __STM8S_H

#include <stdint.h>

#define _MEM_(mem_addr) (*(volatile uint8_t *)(mem_addr))
#define _SFR_(mem_addr) (*(volatile uint8_t *)(0x5000 + (mem_addr)))

#define F_CPU 16000000UL

#define enableInterrupts() \
  {                        \
    _asm("rim\n");         \
  }
#define disableInterrupts() \
  {                         \
    _asm("sim\n");          \
  }

//-----------------------------Clock control (CLK)--------------------------
#define CLK_CKDIVR _SFR_(0xC6)
#define CLK_PCKENR1 _SFR_(0x07) /**< Peripheral clock enable register 1 */

#define CPU16Hz_Init() \
  {                    \
    CLK_CKDIVR = 0x00; \
  }

//______________________API___________________________________

//---------------HTU21_API(htu21_api.h)-------------
#define HTU21_I2C_ADDR 0x40
#define HTU21_READTEMP 0xE3
#define HTU21_READHUM 0xE5

//-------------LCD_API(lcd_api.h)--------------------

#define PIN_RS (1 << 0)
#define PIN_RW (1 << 1)
#define PIN_EN (1 << 2)
#define PIN_BL (1 << 3)
#define PIN_D4 (1 << 4)
#define PIN_D5 (1 << 5)
#define PIN_D6 (1 << 6)
#define PIN_D7 (1 << 7)

#define LCD_MASK_NIBBLE 0xF0
#define LCD_CMD_EN_HIGH 0x0C
#define LCD_CMD_EN_LOW 0x08
#define LCD_DATA_EN_HIGH 0x0D
#define LCD_DATA_EN_LOW 0x09

#define LCD_CMD_FUNCTION_SET_8BIT 0x30
#define LCD_CMD_4BIT_MODE 0x20
#define LCD_CMD_FUNCTION_SET_4BIT 0x28
#define LCD_CMD_DISPLAY_OFF 0x08
#define LCD_CMD_CLEAR 0x01
#define LCD_CMD_ENTRY_MODE 0x06
#define LCD_CMD_DISPLAY_ON 0x0C

#define LCD_DDRAM_ROW0_ADDR 0x80
#define LCD_DDRAM_ROW1_ADDR 0xC0

//______________DRIVER________________________________________

//---------------GPIO(gpio_driver.h)--------
/**
 * @brief Macro for initializing GPIO pins
 */
#define GPIO_PIN_INIT(pin_var, PORT, NUM) \
  do                                      \
  {                                       \
    (pin_var).pin = (NUM);                \
    (pin_var).DDR = &(PORT)->DDR;         \
    (pin_var).CR1 = &(PORT)->CR1;         \
    (pin_var).CR2 = &(PORT)->CR2;         \
    (pin_var).ODR = &(PORT)->ODR;         \
    (pin_var).IDR = &(PORT)->IDR;         \
    (pin_var).state = GPIO_LOW;           \
  } while (0)

/**
 * @brief STM8 GPIO hardware registers
 */
typedef struct
{
  volatile uint8_t ODR; /**< Output Data Register */
  volatile uint8_t IDR; /**< Input Data Register */
  volatile uint8_t DDR; /**< Data Direction Register */
  volatile uint8_t CR1; /**< Control Register 1 */
  volatile uint8_t CR2; /**< Control Register 2 */
} GPIO_TypeDef;

#define GPIOA ((GPIO_TypeDef *)0x5000)
#define GPIOB ((GPIO_TypeDef *)0x5005)
#define GPIOC ((GPIO_TypeDef *)0x500A)
#define GPIOD ((GPIO_TypeDef *)0x500F)
#define GPIOE ((GPIO_TypeDef *)0x5014)

/**
 * @brief GPIO pin states
 */
typedef enum
{
  GPIO_LOW = 0, /**< Low logic level */
  GPIO_HIGH     /**< High logic level */
} GPIO_State;

/**
 * @brief GPIO pin direction
 */
typedef enum
{
  GPIO_INPUT = 0, /**< Input mode */
  GPIO_OUTPUT     /**< Output mode */
} GPIO_Direction;

/**
 * @brief GPIO configuration modes
 */
typedef enum
{
  GPIO_FLOATING = 0, /**< Floating input */
  GPIO_PULLUP,       /**< Input with pull-up */
  GPIO_PUSHPULL,     /**< Push-pull output */
  GPIO_OPENDRAIN     /**< Open-drain output */
} GPIO_Mode;

/**
 * @brief GPIO pin control structure
 */
typedef struct
{
  uint8_t pin;           /**< Pin number or mask */
  volatile uint8_t *DDR; /**< Pointer to DDR register */
  volatile uint8_t *CR1; /**< Pointer to CR1 register */
  volatile uint8_t *CR2; /**< Pointer to CR2 register */
  volatile uint8_t *ODR; /**< Pointer to ODR register */
  volatile uint8_t *IDR; /**< Pointer to IDR register */
  GPIO_State state;      /**< Pin logical state */
} GPIO_Pin;

//-------i2c----------

#define F_I2S_100KHz 100000UL

#define I2C_BASE 0x5210
#define I2C_CR1 (*(volatile uint8_t *)(I2C_BASE + 0x00))
#define I2C_CR2 (*(volatile uint8_t *)(I2C_BASE + 0x01))
#define I2C_FREQR (*(volatile uint8_t *)(I2C_BASE + 0x02))
#define I2C_OARL (*(volatile uint8_t *)(I2C_BASE + 0x03))
#define I2C_OARH (*(volatile uint8_t *)(I2C_BASE + 0x04))
#define I2C_DR (*(volatile uint8_t *)(I2C_BASE + 0x06))
#define I2C_SR1 (*(volatile uint8_t *)(I2C_BASE + 0x07))
#define I2C_SR2 (*(volatile uint8_t *)(I2C_BASE + 0x08))
#define I2C_SR3 (*(volatile uint8_t *)(I2C_BASE + 0x09))
#define I2C_CCRL (*(volatile uint8_t *)(I2C_BASE + 0x0B))
#define I2C_CCRH (*(volatile uint8_t *)(I2C_BASE + 0x0C))
#define I2C_TRISER (*(volatile uint8_t *)(I2C_BASE + 0x0D))

#define I2C_CR1_PE ((uint8_t)0x01)
#define I2C_CR2_START ((uint8_t)0x01)
#define I2C_CR2_STOP ((uint8_t)0x08)
#define I2C_CR2_ACK ((uint8_t)0x10)

#define I2C_SR1_SB ((uint8_t)0x01)
#define I2C_SR1_ADDR ((uint8_t)0x02)
#define I2C_SR1_RXNE ((uint8_t)0x40)
#define I2C_SR1_TXE ((uint8_t)0x80)

#define I2C_SR2_AF ((uint8_t)0x04)

#define I2C_DIR_WRITE 0
#define I2C_DIR_READ 1

#define I2C_TIMEOUT_MAX ((timeout_t)30000UL)

//--------------UART(uart_driver.h)--------------------------

#define UART1_CR2_TEN ((uint8_t)0x08)
#define UART1_CR2_REN ((uint8_t)0x04)
#define UART1_SR_TXE ((uint8_t)0x80)
#define UART1_SR_RXNE ((uint8_t)0x20)
#define UART1_SR_BSY ((uint8_t)0x40)

#define UART1_SR (*(volatile unsigned char *)0x5230)
#define UART1_DR (*(volatile unsigned char *)0x5231)
#define UART1_BRR1 (*(volatile unsigned char *)0x5232)
#define UART1_BRR2 (*(volatile unsigned char *)0x5233)
#define UART1_CR1 (*(volatile unsigned char *)0x5234)
#define UART1_CR2 (*(volatile unsigned char *)0x5235)
#define UART1_CR3 (*(volatile unsigned char *)0x5236)

//---------------EEPROM(eeprom.h)-------------------
#define FLASH_PUKR_KEY1 0x56
#define FLASH_PUKR_KEY2 0xAE

#define FLASH_DUKR _SFR_(0x64)
#define FLASH_DUKR_KEY1 FLASH_PUKR_KEY2
#define FLASH_DUKR_KEY2 FLASH_PUKR_KEY1
#define FLASH_IAPSR _SFR_(0x5F)
#define FLASH_IAPSR_DUL 3

#define FLASH_CR2 _SFR_(0x5B)
#define FLASH_CR2_OPT 7

#define FLASH_NCR2 _SFR_(0x5C)
#define FLASH_NCR2_NOPT 7

#define FLASH_IAPSR_EOP 2

#define EEPROM_START_ADDR 0x4000
#define EEPROM_END_ADDR 0x407F

#define OPT0 _MEM_(0x4800)
#define OPT1 _MEM_(0x4801)
#define NOPT1 _MEM_(0x4802)
#define OPT2 _MEM_(0x4803)
#define NOPT2 _MEM_(0x4804)
#define OPT3 _MEM_(0x4805)
#define NOPT3 _MEM_(0x4806)
#define OPT4 _MEM_(0x4807)
#define NOPT4 _MEM_(0x4808)
#define OPT5 _MEM_(0x4809)
#define NOPT5 _MEM_(0x480A)

//-------------EXTI(exti_driver.h)---------

#define EXTI_CR1 (*(volatile uint8_t *)0x50A0)
#define EXTI_CR2 (*(volatile uint8_t *)0x50A1)

#define EXTI_CR1_RESET_VALUE ((uint8_t)0x00)
#define EXTI_CR2_RESET_VALUE ((uint8_t)0x00)

#define EXTI_CR1_PDIS ((uint8_t)0xC0) /**< Port D interrupt sensitivity mask */
#define EXTI_CR1_PCIS ((uint8_t)0x30) /**< Port C interrupt sensitivity mask */
#define EXTI_CR1_PBIS ((uint8_t)0x0C) /**< Port B interrupt sensitivity mask */
#define EXTI_CR1_PAIS ((uint8_t)0x03) /**< Port A interrupt sensitivity mask */

#define EXTI_CR2_TLIS ((uint8_t)0x04) /**< TLI sensitivity mask */
#define EXTI_CR2_PEIS ((uint8_t)0x03) /**< Port E interrupt sensitivity mask */

#define EXTI_PORTC_MASK 0xCF
#define EXTI_PORTC_FALLING 0x20

/**
 * @brief EXTI interrupt sensitivity modes
 */
typedef enum
{
  EXTI_SENSITIVITY_FALL_LOW = 0x00,  /**< Falling edge and low level */
  EXTI_SENSITIVITY_RISE_ONLY = 0x01, /**< Rising edge only */
  EXTI_SENSITIVITY_FALL_ONLY = 0x02, /**< Falling edge only */
  EXTI_SENSITIVITY_RISE_FALL = 0x03  /**< Both rising and falling edges */
} EXTI_Sensitivity_TypeDef;

/**
 * @brief EXTI Top Level Interrupt sensitivity modes
 */
typedef enum
{
  EXTI_TLISENSITIVITY_FALL_ONLY = 0x00, /**< TLI falling edge only */
  EXTI_TLISENSITIVITY_RISE_ONLY = 0x04  /**< TLI rising edge only */
} EXTI_TLISensitivity_TypeDef;

/**
 * @brief EXTI Port identifiers
 */
typedef enum
{
  EXTI_PORT_GPIOA = 0x00, /**< Port A */
  EXTI_PORT_GPIOB = 0x01, /**< Port B */
  EXTI_PORT_GPIOC = 0x02, /**< Port C */
  EXTI_PORT_GPIOD = 0x03, /**< Port D */
  EXTI_PORT_GPIOE = 0x04  /**< Port E */
} EXTI_Port_TypeDef;

#define IS_EXTI_SENSITIVITY_OK(SensitivityValue)         \
  (((SensitivityValue) == EXTI_SENSITIVITY_FALL_LOW) ||  \
   ((SensitivityValue) == EXTI_SENSITIVITY_RISE_ONLY) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_FALL_ONLY) || \
   ((SensitivityValue) == EXTI_SENSITIVITY_RISE_FALL))

#define IS_EXTI_TLISENSITIVITY_OK(SensitivityValue)         \
  (((SensitivityValue) == EXTI_TLISENSITIVITY_FALL_ONLY) || \
   ((SensitivityValue) == EXTI_TLISENSITIVITY_RISE_ONLY))

#define IS_EXTI_PORT_OK(PORT)     \
  (((PORT) == EXTI_PORT_GPIOA) || \
   ((PORT) == EXTI_PORT_GPIOB) || \
   ((PORT) == EXTI_PORT_GPIOC) || \
   ((PORT) == EXTI_PORT_GPIOD) || \
   ((PORT) == EXTI_PORT_GPIOE))

#define IS_EXTI_PINMASK_OK(PinMask) ((((PinMask) & (uint8_t)0x00) == (uint8_t)0x00) && ((PinMask) != (uint8_t)0x00))

//-----------------------TIM1(tim1_driver.h)----------

#define TIM1_CR1_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CR2_RESET_VALUE ((uint8_t)0x00)
#define TIM1_SMCR_RESET_VALUE ((uint8_t)0x00)
#define TIM1_ETR_RESET_VALUE ((uint8_t)0x00)
#define TIM1_IER_RESET_VALUE ((uint8_t)0x00)
#define TIM1_SR1_RESET_VALUE ((uint8_t)0x00)
#define TIM1_SR2_RESET_VALUE ((uint8_t)0x00)
#define TIM1_EGR_RESET_VALUE ((uint8_t)0x00)
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
#define TIM1_ARRH_RESET_VALUE ((uint8_t)0xFF)
#define TIM1_ARRL_RESET_VALUE ((uint8_t)0xFF)
#define TIM1_RCR_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR1H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR1L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR2H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR2L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR3H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR3L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR4H_RESET_VALUE ((uint8_t)0x00)
#define TIM1_CCR4L_RESET_VALUE ((uint8_t)0x00)
#define TIM1_BKR_RESET_VALUE ((uint8_t)0x00)
#define TIM1_DTR_RESET_VALUE ((uint8_t)0x00)
#define TIM1_OISR_RESET_VALUE ((uint8_t)0x00)

#define TIM1_CR1 (*(volatile uint8_t *)0x5250)
#define TIM1_CR2 (*(volatile uint8_t *)0x5251)
#define TIM1_SMCR (*(volatile uint8_t *)0x5252)
#define TIM1_ETR (*(volatile uint8_t *)0x5253)
#define TIM1_IER (*(volatile uint8_t *)0x5254)
#define TIM1_SR1 (*(volatile uint8_t *)0x5255)
#define TIM1_SR2 (*(volatile uint8_t *)0x5256)
#define TIM1_CCMR1 (*(volatile uint8_t *)0x5258)
#define TIM1_CCMR2 (*(volatile uint8_t *)0x5259)
#define TIM1_CCMR3 (*(volatile uint8_t *)0x525A)
#define TIM1_CCMR4 (*(volatile uint8_t *)0x525B)
#define TIM1_CCER1 (*(volatile uint8_t *)0x525C)
#define TIM1_CCER2 (*(volatile uint8_t *)0x525D)
#define TIM1_CNTRH (*(volatile uint8_t *)0x525E)
#define TIM1_CNTRL (*(volatile uint8_t *)0x525F)
#define TIM1_PSCRH (*(volatile uint8_t *)0x5260)
#define TIM1_PSCRL (*(volatile uint8_t *)0x5261)
#define TIM1_ARRH (*(volatile uint8_t *)0x5262)
#define TIM1_ARRL (*(volatile uint8_t *)0x5263)
#define TIM1_RCR (*(volatile uint8_t *)0x5264)
#define TIM1_CCR3H (*(volatile uint8_t *)0x5269)
#define TIM1_CCR3L (*(volatile uint8_t *)0x526A)
#define TIM1_CCR4H (*(volatile uint8_t *)0x526B)
#define TIM1_CCR4L (*(volatile uint8_t *)0x526C)
#define TIM1_BKR (*(volatile uint8_t *)0x526D)
#define TIM1_DTR (*(volatile uint8_t *)0x526E)
#define TIM1_OISR (*(volatile uint8_t *)0x526F)
#define TIM1_EGR (*(volatile uint8_t *)0x5257)
#define TIM1_EGR_UG (1 << 0)

#define TIM1_CCR1H (*(volatile uint8_t *)0x5265)
#define TIM1_CCR1L (*(volatile uint8_t *)0x5266)
#define TIM1_CCR2H (*(volatile uint8_t *)0x5267)
#define TIM1_CCR2L (*(volatile uint8_t *)0x5268)

#define PC_DDR (*(volatile uint8_t *)0x500C)
#define PC_CR1 (*(volatile uint8_t *)0x500D)
#define PC_CR2 (*(volatile uint8_t *)0x500E)

//-----------------------TIM2(tim2_driver.h)----------

#define TIM2_PSCR (*(volatile uint8_t *)0x530E)
#define TIM2_ARRH (*(volatile uint8_t *)0x530F)
#define TIM2_ARRL (*(volatile uint8_t *)0x5310)
#define TIM2_EGR (*(volatile uint8_t *)0x5306)
#define TIM2_CR1 (*(volatile uint8_t *)0x5300)
#define TIM2_IER (*(volatile uint8_t *)0x5303)
#define TIM2_CCER1 (*(volatile uint8_t *)0x530A)
#define TIM2_CCER2 (*(volatile uint8_t *)0x530B)
#define TIM2_SR1 (*(volatile uint8_t *)0x5304)
#define TIM2_SR2 (*(volatile uint8_t *)0x5305)
#define TIM2_CCR3H (*(volatile uint8_t *)0x5315)
#define TIM2_CCR3L (*(volatile uint8_t *)0x5316)
#define TIM2_CCMR1 (*(volatile uint8_t *)0x5307)
#define TIM2_CCMR2 (*(volatile uint8_t *)0x5308)
#define TIM2_CCMR3 (*(volatile uint8_t *)0x5309)
#define TIM2_CNTRH (*(volatile uint8_t *)0x530C)
#define TIM2_CNTRL (*(volatile uint8_t *)0x530D)

#define TIM2_CCR1H (*(volatile uint8_t *)0x5311)
#define TIM2_CCR1L (*(volatile uint8_t *)0x5312)
#define TIM2_CCR2H (*(volatile uint8_t *)0x5313)
#define TIM2_CCR2L (*(volatile uint8_t *)0x5314)

#define TIM2_CR1_RESET_VALUE ((uint8_t)0x00)
#define TIM2_IER_RESET_VALUE ((uint8_t)0x00)
#define TIM2_SR1_RESET_VALUE ((uint8_t)0x00)
#define TIM2_SR2_RESET_VALUE ((uint8_t)0x00)
#define TIM2_EGR_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCMR1_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCMR2_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCMR3_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCER1_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCER2_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CNTRH_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CNTRL_RESET_VALUE ((uint8_t)0x00)
#define TIM2_PSCR_RESET_VALUE ((uint8_t)0x00)
#define TIM2_ARRH_RESET_VALUE ((uint8_t)0xFF)
#define TIM2_ARRL_RESET_VALUE ((uint8_t)0xFF)
#define TIM2_CCR1H_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR1L_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR2H_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR2L_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR3H_RESET_VALUE ((uint8_t)0x00)
#define TIM2_CCR3L_RESET_VALUE ((uint8_t)0x00)

#define TIM2_CR1_CEN ((uint8_t)0x01)

#define TIM2_CCER1_CC1E ((uint8_t)0x01)
#define TIM2_CCER1_CC1P ((uint8_t)0x02)
#define TIM2_CCER1_CC2E ((uint8_t)0x10)
#define TIM2_CCER1_CC2P ((uint8_t)0x20)
#define TIM2_CCER2_CC3E ((uint8_t)0x01)
#define TIM2_CCER2_CC3P ((uint8_t)0x02)

#define TIM2_CCMR_ICxPSC ((uint8_t)0x0C)
#define TIM2_CCMR_CCxS ((uint8_t)0x03)
#define TIM2_CCMR_ICxF ((uint8_t)0xF0)

/**
 * @brief Functional state
 */
typedef enum
{
  DISABLE = 0,      /**< Disabled */
  ENABLE = !DISABLE /**< Enabled */
} FunctionalState;

/**
 * @brief TIM2 Prescaler reload modes
 */
typedef enum
{
  TIM2_PSCRELOADMODE_UPDATE = ((uint8_t)0x00),   /**< Reload on update event */
  TIM2_PSCRELOADMODE_IMMEDIATE = ((uint8_t)0x01) /**< Reload immediately */
} TIM2_PSCReloadMode_TypeDef;

/**
 * @brief Logic and flag status
 */
typedef enum
{
  RESET = 0,   /**< Cleared / Low status */
  SET = !RESET /**< Active / High status */
} FlagStatus,
    ITStatus, BitStatus, BitAction;

/**
 * @brief TIM2 Prescaler division factors
 */
typedef enum
{
  TIM2_PRESCALER_1 = ((uint8_t)0x00),     /**< fCK_CNT = fCK_PSC/1 */
  TIM2_PRESCALER_2 = ((uint8_t)0x01),     /**< fCK_CNT = fCK_PSC/2 */
  TIM2_PRESCALER_4 = ((uint8_t)0x02),     /**< fCK_CNT = fCK_PSC/4 */
  TIM2_PRESCALER_8 = ((uint8_t)0x03),     /**< fCK_CNT = fCK_PSC/8 */
  TIM2_PRESCALER_16 = ((uint8_t)0x04),    /**< fCK_CNT = fCK_PSC/16 */
  TIM2_PRESCALER_32 = ((uint8_t)0x05),    /**< fCK_CNT = fCK_PSC/32 */
  TIM2_PRESCALER_64 = ((uint8_t)0x06),    /**< fCK_CNT = fCK_PSC/64 */
  TIM2_PRESCALER_128 = ((uint8_t)0x07),   /**< fCK_CNT = fCK_PSC/128 */
  TIM2_PRESCALER_256 = ((uint8_t)0x08),   /**< fCK_CNT = fCK_PSC/256 */
  TIM2_PRESCALER_512 = ((uint8_t)0x09),   /**< fCK_CNT = fCK_PSC/512 */
  TIM2_PRESCALER_1024 = ((uint8_t)0x0A),  /**< fCK_CNT = fCK_PSC/1024 */
  TIM2_PRESCALER_2048 = ((uint8_t)0x0B),  /**< fCK_CNT = fCK_PSC/2048 */
  TIM2_PRESCALER_4096 = ((uint8_t)0x0C),  /**< fCK_CNT = fCK_PSC/4096 */
  TIM2_PRESCALER_8192 = ((uint8_t)0x0D),  /**< fCK_CNT = fCK_PSC/8192 */
  TIM2_PRESCALER_16384 = ((uint8_t)0x0E), /**< fCK_CNT = fCK_PSC/16384 */
  TIM2_PRESCALER_32768 = ((uint8_t)0x0F)  /**< fCK_CNT = fCK_PSC/32768 */
} TIM2_Prescaler_TypeDef;

/**
 * @brief TIM2 Channels
 */
typedef enum
{
  TIM2_CHANNEL_1 = ((uint8_t)0x00), /**< Channel 1 */
  TIM2_CHANNEL_2 = ((uint8_t)0x01), /**< Channel 2 */
  TIM2_CHANNEL_3 = ((uint8_t)0x02)  /**< Channel 3 */
} TIM2_Channel_TypeDef;

/**
 * @brief TIM2 Input Capture polarity
 */
typedef enum
{
  TIM2_ICPOLARITY_RISING = ((uint8_t)0x00), /**< Rising edge trigger */
  TIM2_ICPOLARITY_FALLING = ((uint8_t)0x44) /**< Falling edge trigger */
} TIM2_ICPolarity_TypeDef;

/**
 * @brief TIM2 Input Capture selection
 */
typedef enum
{
  TIM2_ICSELECTION_DIRECTTI = ((uint8_t)0x01),   /**< Direct input map */
  TIM2_ICSELECTION_INDIRECTTI = ((uint8_t)0x02), /**< Indirect input map */
  TIM2_ICSELECTION_TRGI = ((uint8_t)0x03)        /**< Internal trigger map */
} TIM2_ICSelection_TypeDef;

/**
 * @brief TIM2 Input Capture prescaler
 */
typedef enum
{
  TIM2_ICPSC_DIV1 = ((uint8_t)0x00), /**< No division */
  TIM2_ICPSC_DIV2 = ((uint8_t)0x04), /**< Division by 2 */
  TIM2_ICPSC_DIV4 = ((uint8_t)0x08), /**< Division by 4 */
  TIM2_ICPSC_DIV8 = ((uint8_t)0x0C)  /**< Division by 8 */
} TIM2_ICPSC_TypeDef;

/**
 * @brief TIM2 Interrupt sources
 */
typedef enum
{
  TIM2_IT_UPDATE = ((uint8_t)0x01), /**< Update interrupt */
  TIM2_IT_CC1 = ((uint8_t)0x02),    /**< Capture/Compare 1 interrupt */
  TIM2_IT_CC2 = ((uint8_t)0x04),    /**< Capture/Compare 2 interrupt */
  TIM2_IT_CC3 = ((uint8_t)0x08)     /**< Capture/Compare 3 interrupt */
} TIM2_IT_TypeDef;

#endif