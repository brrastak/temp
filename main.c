// STM32F103
#include "stm32f10x.h"

// cstd
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//user
#include "main.h"
#include "init.h"
#include "gpio.h"


// I2C flags -> GPIO
void ShowFlags(void);

uint8_t addr = 0xAF;
uint8_t data[20] = {0xA0};


int main()
{    
    // System init
    InitRcc();
    InitGpio();

    
    // Init I2C
    // Transmitter in master mode
    I2C1->CR2 = 36;                     // RCC 36MHz
    I2C1->CCR |= 180;                   // 100kHz
    I2C1->TRISE = 0x55;                 // 1000ns
    I2C1->CR1 |= I2C_CR1_PE;            // peripheral enable
    
    ShowFlags();
    
    // Generate start condition
    I2C1->CR1 |= I2C_CR1_START;
    while ((I2C1->SR1 | I2C_SR1_ADDR) == 0) {
        
        ShowFlags();
    }
    
    ShowFlags();
    
    // Send addr
    (void)I2C1->SR1;                // clear flags
    I2C1->DR = addr;                // send I2C address
    while ((I2C1->SR1 | I2C_SR1_ADDR) == 0) {
        
        ShowFlags();
    }
    
    ShowFlags();
    
    // Send data
    (void)I2C1->SR1;                // clear flags
    (void)I2C1->SR2;
    I2C1->DR = data[0];
    while ((I2C1->SR1 | I2C_SR1_TXE) == 0) {
        
        ShowFlags();
    }
    
    ShowFlags();
    
    // End of transmission
    
    
    while (true)
        ;
    
}

// I2C flags -> GPIO
void ShowFlags(void)
{
    // TxE
    if ((I2C1->SR1 | I2C_SR1_TXE) != 0)
        SetPin(TXE_PIN);
    else
        ResetPin(TXE_PIN);
    // BTF
    if ((I2C1->SR1 | I2C_SR1_BTF) != 0)
        SetPin(BTF_PIN);
    else
        ResetPin(BTF_PIN);
}



