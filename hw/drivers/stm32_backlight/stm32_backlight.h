/*
 * stm32_backlight.h
 * API for backlight control
 * RebbleOS
 *
 * Barry Carter <barry.carter@gmail.com>
 * Thomas England <thomas.c.england@gmail.com>
 */

#ifndef __STM32_BACKLIGHT_H
#define __STM32_BACKLIGHT_H

typedef struct {
  TIM_TypeDef* tim;
  uint16_t pin;
  uint8_t pin_source;
  uint32_t port;
  uint8_t af;
  uint32_t rcc_tim;
} stm32_backlight_config_t;

void _hw_backlight_init(stm32_backlight_config_t *blc);
void hw_backlight_init(void);
void _hw_backlight_set(uint16_t val, stm32_backlight_config_t *blc);
void hw_backlight_set(uint16_t val);

#endif
