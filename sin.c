// STM32F103
#include "stm32f10x.h"

//user
#include "main.h"
#include "MCP4921.h"

static int16_t const* sine[8] = 
{
    sine_table1,
    sine_table2,
    sine_table3,
    sine_table4,
    sine_table5,
    sine_table6,
    sine_table7,
    sine_table8
};

int pattern;
int volume;

// Every 1 us -> ~1 kHz
void SendSine(void)
{
    static int counter = 0;
    const int max = 1023;
    int16_t value;
    
    value = sine[pattern-1][counter];
    value = value * volume / 40 - 2500;
    SendDAC(value);
    
    counter += 5;
    if (counter > max)
        counter -= max;
}