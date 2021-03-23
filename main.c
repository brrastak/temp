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
#include "debug_led.h"


// I2C flags -> GPIO
void ShowFlags(void);


uint8_t addr = (0x3C << 1);     // SSD1306
uint8_t data[20];


int main()
{    
    // System init
    InitRcc();
    InitGpio();

    TurnLedOff();
    
    
    
    // Init I2C
    // Transmitter in master mode
    I2C2->CR2 = 36;                     // RCC 36MHz
    I2C2->CCR |= 180;                   // 100kHz
    I2C2->TRISE = 0x55;                 // 1000ns
    I2C2->CR1 |= I2C_CR1_PE;            // peripheral enable
    
    ShowFlags();
    
    // Generate start condition
    I2C2->CR1 |= I2C_CR1_START;
    while ((I2C2->SR1 & I2C_SR1_SB) == 0) {
        
        ShowFlags();
    }
    
    ShowFlags();
    
    
    // Send addr
    (void)I2C2->SR1;                // clear flags
    I2C2->DR = addr;                // send I2C address
    while ((I2C2->SR1 & I2C_SR1_ADDR) == 0) {
        
        ShowFlags();
    }
    
    ShowFlags();
    
    
    // Send data
    (void)I2C2->SR1;                // clear flags
    (void)I2C2->SR2;
    
    for (int i = 0; i < 5; i++) {
    
        I2C2->DR = i+'a';
        while ((I2C2->SR1 & I2C_SR1_TXE) == 0) {
        
            ShowFlags();
        }
    }
    
    ShowFlags();
    
    while ((I2C2->SR1 & I2C_SR1_BTF) == 0) {
        
        ShowFlags();
    }
    ShowFlags();
    
    // End of transmission
    I2C2->CR1 |= I2C_CR1_STOP;  // generate stop condition
    
    while (true)
        ShowFlags();
    
}

// I2C flags -> GPIO
void ShowFlags(void)
{
    // TxE
    if ((I2C2->SR1 & I2C_SR1_TXE) != 0)
        SetPin(TXE_PIN);
    else
        ResetPin(TXE_PIN);
    // BTF
    if ((I2C2->SR1 & I2C_SR1_BTF) != 0)
        SetPin(BTF_PIN);
    else
        ResetPin(BTF_PIN);
}



