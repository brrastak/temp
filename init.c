// STM32F103
#include "stm32f10x.h"

// user
#include "init.h"
#include "main.h"
#include "gpio.h"

void InitRcc(void)
{
    // port A
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    // port B
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    // port C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // TIM1
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    // TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // TIM4
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    // I2C2
    RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
}
void InitInterrupt(void)
{
    // I2C2
    NVIC_EnableIRQ(I2C2_EV_IRQn);
}
void InitGpio(void)
{
    // I2C2
    SetGpioMode(I2C2_SCL, PIN_MODE_2MHZ_AFOD);
    SetGpioMode(I2C2_SDA, PIN_MODE_2MHZ_AFOD);
    // Flags indication
    SetGpioMode(TXE_PIN, PIN_MODE_2MHZ_OPP);
    SetGpioMode(BTF_PIN, PIN_MODE_2MHZ_OPP);

}
