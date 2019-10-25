// STM32F103
#include "stm32f10x.h"

//user
#include "main.h"
#include "timer.h"
#include "keyboard.h"

volatile uint32_t sys_tick = 0;

// TIM1 update interrupt
void TIM1_IRQHandler(void)
{
}
void InitTIM1(void)
{
    // Timer
    TIM1->CR1 =     TIM_CR1_CKD_0       * 0 |   // clock division for dead time : DTS = CK_INT
                    TIM_CR1_CKD_1       * 0 |
                    TIM_CR1_ARPE        * 0 |   // auto-reload preload enable
                    TIM_CR1_CMS_0       * 0 |   // center-aligned mode selection : edge-aligned mode
                    TIM_CR1_CMS_1       * 0 |
                    TIM_CR1_DIR         * 0;    // direction : upcounter
    
    TIM1->PSC = 35999;                  // CK_CNT = 2kHz
    TIM1->ARR = 39;                     // 50Hz
    
    // PWM
    TIM1->CCMR1 =   TIM_CCMR1_CC1S_0    * 0 |   // capture/compare 1 selection: compare
                    TIM_CCMR1_CC1S_1    * 0 |
                    TIM_CCMR1_OC1PE     * 0 |   // output compare 1 preload enable
                    TIM_CCMR1_OC1M_0    * 0 |   // output compare 1 mode: PWM mode 1 (active->inactive)
                    TIM_CCMR1_OC1M_1    * 1 |
                    TIM_CCMR1_OC1M_2    * 1 |
                        
                    TIM_CCMR1_CC2S_0    * 0 |   // capture/compare 2 selection: compare
                    TIM_CCMR1_CC2S_1    * 0 |
                    TIM_CCMR1_OC2PE     * 0 |   // output compare 2 preload enable
                    TIM_CCMR1_OC2M_0    * 0 |   // output compare 2 mode: PWM mode 1 (active->inactive)
                    TIM_CCMR1_OC2M_1    * 1 |
                    TIM_CCMR1_OC2M_2    * 1;
    
    TIM1->CCER =    TIM_CCER_CC1E       * 1 |   // capture/compare 1 output enable
                    TIM_CCER_CC1P       * 0 |   // capture/compare 1 output polarity: active high
                    TIM_CCER_CC1NE      * 1 |   // capture/compare 1 complementary output enable
                    TIM_CCER_CC1NP      * 0 |   // capture/compare 1 complementary output polarity: active high
                        
                    TIM_CCER_CC2E       * 1 |   // capture/compare 2 output enable
                    TIM_CCER_CC2P       * 0 |   // capture/compare 2 output polarity: active high
                    TIM_CCER_CC2NE      * 1 |   // capture/compare 2 complementary output enable
                    TIM_CCER_CC2NP      * 0;    // capture/compare 2 complementary output polarity: active high
        
    TIM1->CCR1 = 20;       // channel 1 duty time = 50%
    TIM1->CCR2 = 20;       // channel 2 duty time = 50%
    
    TIM1->BDTR =    0xff |                      // dead time
                    TIM_BDTR_MOE        * 1;    // main output enable
    
    // Generate update event
    TIM1->EGR = TIM_EGR_UG;
    
    // Interrupt
    //TIM1->DIER |= TIM_DIER_UIE;         // update interrupt enable
    
    // Start!
    TIM1->CR1 |= TIM_CR1_CEN;
}

// SysTick timer interrupt
void SysTick_Handler(void)
{
    static bool div2;
    
    if (div2)           // every 1ms
        sys_tick++;
    div2 = !div2;
    
    // Keyboard
    /*if (keyboard_enable)
        KeyCheck();*/
}
// TIM2 update interrupt
void TIM2_IRQHandler(void)
{   
    if (!(TIM2->SR & TIM_SR_UIF)) return;
    TIM2->SR &= ~TIM_SR_UIF;            // clear flag

    SendSine();
}
void InitSysTick(void)
{
    SysTick->LOAD = SYS_TIMER_TICK;
    SysTick->VAL = 0;
    SysTick->CTRL =	SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |    // interrupt enable
                    SysTick_CTRL_ENABLE_Msk;
}
void InitTIM2(void)
{
    // Timer
    TIM2->CR1 = TIM_CR1_ARPE    * 1 |   // Auto-reload preload enable
                TIM_CR1_CMS_0   * 0 |   // Center-aligned mode selection : edge-aligned mode
                TIM_CR1_CMS_1   * 0 |
                TIM_CR1_DIR     * 0;    // Direction : upcounter
    TIM2->PSC = 0;                      // CK_CNT = 72MHz
    TIM2->ARR = 359;                    // 200kHz
    TIM2->EGR = TIM_EGR_UG;             // update
    // PWM
    /*TIM2->CCMR1 = 0x60;                 // PWM mode 1
    TIM2->CCER |= 0x01;                 // out enable & active high
    TIM2->CCR1 = 0;                     // duty time = 50%*/
    // Interrupt
    TIM2->DIER |= TIM_DIER_UIE;         // update interrupt enable
    TIM2->CR1 |= TIM_CR1_CEN;           // start
}
void PWMEnable(void)
{
    TIM2->CR1 |= TIM_CR1_CEN;
}
void PWMDisable(void)
{
    TIM2->CR1 &= ~TIM_CR1_CEN;
}
void InitADCTimer(void)
{
    // Timer
    TIM3->CR1 = TIM_CR1_ARPE    * 1 |   // Auto-reload preload enable
                TIM_CR1_CMS_0   * 0 |   // Center-aligned mode selection : edge-aligned mode
                TIM_CR1_CMS_1   * 0 |
                TIM_CR1_DIR     * 0;    // Direction : upcounter
    TIM3->CR2 |= 0x20;                  // TRGO on update event
    TIM3->PSC = 3599;                   // CK_CNT = 20kHz
    TIM3->ARR = 399;                    // 50 Hz
    TIM3->EGR = TIM_EGR_UG;             // update event enable
    // Interrupt
    //TIM3->DIER |= TIM_DIER_UIE;         // update
    // Start!
    TIM3->CR1 |= TIM_CR1_CEN;
}

// Delay TIM4
void delay_ms(int time)
{
    TIM4->CR1 = 0;
    TIM4->SR = 0;
    TIM4->PSC = 35999;                  // 2kHz
    TIM4->ARR = time * 2 - 1;
    TIM4->EGR = TIM_EGR_UG;
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->CR1 |= TIM_CR1_CEN;
    while (!(TIM4->SR & TIM_SR_UIF))
        ;
    TIM4->CR1 &= ~TIM_CR1_CEN;
}
void delay_us(int time)
{
    TIM4->CR1 = 0;
    TIM4->SR = 0;
    TIM4->PSC = 35;                     // 2MHz
    TIM4->ARR = time * 2 - 1;
    TIM4->EGR = TIM_EGR_UG;
    TIM4->SR &= ~TIM_SR_UIF;
    TIM4->CR1 |= TIM_CR1_CEN;
    while (!(TIM4->SR & TIM_SR_UIF))
        ;
    TIM4->CR1 &= ~TIM_CR1_CEN;
}