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
#include "timer.h"
#include "display.h"
#include "UART.h"
#include "SPI.h"
#include "MCP4921.h"
#include "encoder.h"
#include "button.h"

// Onboard LED
void LEDOn(void);
void LEDOff(void);
void LEDBlink(void);

bool keyboard_enable = false;


int main()
{    
    // System init
    InitRCC();
    InitGPIO();
    InitSysTick();
    //InitUART();
    InitSPI();
    InitTIM2();
    InitInterrupt();
    
    //delay_ms(200);
    InitDisp(); 
    
    // Hardware init
    LEDOff();  
    
    
    // Software init    
    char buf[16];
    pattern = 1;
    volume = ValueEncoder();
    
    btn_t    butn = {BUTN0_PIN, 0, 20, false, false, false},
            relay = {BUTN1_PIN, 0,  1, false, false, false};
    
    // Timings
    uint32_t ButnCheckCounter   = sys_tick;
    uint32_t DispCounter        = sys_tick;
    const int DispTime = 200;        // ms
    const int DispClrScrTime = 2;   // ms
    bool to_clear = false;
    
    while (true) {
        
        if (SPItransmitted())
            ChipDeselectSPI();
        
        if ((sys_tick - DispCounter > DispTime)&& !to_clear) {
            DispCounter = sys_tick;
            
            sprintf(buf, " Pattern: %u", pattern);
            WriteLine(buf);

            //WriteLine("Hello! =)");
            
            MoveToLine2();
            sprintf(buf, "  Volume: %u", volume);
            WriteLine(buf);
            
            to_clear = true;
        }
        if ((sys_tick - DispCounter > DispTime - DispClrScrTime)&& to_clear) {
            ClrScr();
            to_clear = false;
        }
        
        if (sys_tick - ButnCheckCounter > 1) {
            ButnCheckCounter = sys_tick;
            
            GetEncoder();
            volume = ValueEncoder();
            
            BtnCheck(&butn);
            BtnCheck(&relay);
        }
        
        if (butn.was_pressed) {
            butn.was_pressed = false;
            pattern++;
            if (pattern > 8)
                pattern = 1;
        }
        
        if (butn.is_pressed) {
            LEDOn();
        }
        else {
            LEDOff();
        }
    }
}


// Onboard LED
void LEDOn(void)
{
    PinReset(LED_PIN);
}
void LEDOff(void)
{
    PinSet(LED_PIN);
}
void LEDBlink(void)
{
    PinSwitch(LED_PIN);
}
