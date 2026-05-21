#include "gpio_driver.h"


//Initializes the GPIO according to the specified parameters
void GPIO_Config(GPIO_Pin *pin, GPIO_Direction dir, GPIO_Mode mode)
{
    if (dir == GPIO_OUTPUT) {        // Output
        *(pin->DDR) |= (1 << pin->pin);  

        if (mode == GPIO_PUSHPULL)   
            *(pin->CR1) |= (1 << pin->pin);
        else if (mode == GPIO_OPENDRAIN) 
            *(pin->CR1) &= ~(1 << pin->pin);

    } else {                          // Input
        *(pin->DDR) &= ~(1 << pin->pin);

        if (mode == GPIO_PULLUP)
            *(pin->CR1) |= (1 << pin->pin);
        else 
            *(pin->CR1) &= ~(1 << pin->pin);
    }
}


//Writes data to the specified GPIO data port
void GPIO_Write(GPIO_Pin *pin, GPIO_State value)
{
    if (value == GPIO_HIGH)
        *(pin->ODR) |= (1 << pin->pin);
    else
        *(pin->ODR) &= ~(1 << pin->pin);

    pin->state = value;
}


//Toggles the GPIO output pin state
void GPIO_Toggle(GPIO_Pin *pin)
{
    *(pin->ODR) ^= (1 << pin->pin);
    pin->state = (*(pin->ODR) & (1 << pin->pin)) ? GPIO_HIGH : GPIO_LOW;
}

//@brief Returns the cached GPIO pin state.
GPIO_State GPIO_ReadState(GPIO_Pin *pin)
{
    return pin->state;
}


//Reads the current logic level of the GPIO pin.
uint8_t GPIO_Read(GPIO_Pin *pin)
{
    return (*(pin->IDR) & (1 << pin->pin)) ? 1 : 0;
}
