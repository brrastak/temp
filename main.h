// STM32F103
#include "stm32f10x.h"

// cstd
#include <stdbool.h>

#ifndef __MAIN_H__
#define __MAIN_H__



// I2C
#define I2C2_SCL                GPIOB, 10
#define I2C2_SDA                GPIOB, 11
// Flags indication
#define TXE_PIN                 GPIOB, 1
#define BTF_PIN                 GPIOB, 0



#endif
