/* stm32_backlight.c
 * Backlight control for stm32
 * RebbleOS
 *
 * Barry Carter <barry.carter@gmail.com>
 * Thomas England <thomas.c.england@gmail.com>
 */

#include "stdio.h"
#include "string.h"
#include "display.h"
#include "backlight.h"
#include "stm32_backlight.h"
#include "stm32_power.h"
#include "log.h"
#include "platform_config.h"

#if defined(STM32F4XX)
#    include "snowy_display.h"
#    include <stm32f4xx_spi.h>
#    include <stm32f4xx_tim.h>
#elif defined(STM32F2XX)
#    include "stm32f2xx.h"
#    include "stm32f2xx_spi.h"
#    include "stm32f2xx_tim.h"
#    include "stm32f2xx_gpio.h"
#    include "stm32f2xx_rcc.h"
#    include "stm32f2xx_usart.h"
#else
#    error "I have no idea what kind of stm32 this is; sorry"
#endif
#define _TIM_Func(CH, Func) TIM_OC ## CH ## Func
#define TIM_Func(CH, Func) _TIM_Func(CH, Func)
/*** backlight init ***/

static uint8_t _backlight_clocks_on = 0;
void _hw_backlight_init(stm32_backlight_config_t *blc) {
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitDef;

    GPIO_InitDef.GPIO_Pin = blc->pin;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitDef);

    /* And multivac pronounced "and let there be backlight" */
    GPIO_SetBits(GPIOB, blc->pin);

}

void _hw_backlight_set(uint16_t val, stm32_backlight_config_t *blc) {
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;
    TIM_OCInitTypeDef TIM_OCStruct;

    if (!_backlight_clocks_on)
        stm32_power_request(STM32_POWER_APB1, blc->rcc_tim);

    TIM_BaseStruct.TIM_Prescaler = 0;
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 9999; // 10khz calculated by clock dividers
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(blc->tim, &TIM_BaseStruct);

    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;  // set on compare
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OCStruct.TIM_Pulse = val;

    TIM_Func(BL_TIM_CH, Init)(blc->tim, &TIM_OCStruct);
    TIM_Func(BL_TIM_CH, PreloadConfig)(blc->tim, TIM_OCPreload_Enable);

    TIM_Cmd(blc->tim, ENABLE);
    TIM_CtrlPWMOutputs(blc->tim, ENABLE);

    GPIO_PinAFConfig(GPIOB, blc->pin_source, blc->af);

    _backlight_clocks_on = val > 0;
    if (!_backlight_clocks_on)
        stm32_power_release(STM32_POWER_APB1, blc->rcc_tim);
}
