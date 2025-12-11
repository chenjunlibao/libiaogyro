#ifndef _GPIO_H
#define _GPIO_H

#include "hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} gpio_t;


void gpio_set(gpio_t *gpio);
void gpio_reset(gpio_t *gpio);
void gpio_toggle(gpio_t *gpio);
uint8_t gpio_get(gpio_t *gpio);
void gpio_set_input(gpio_t *gpio);
void gpio_set_output(gpio_t *gpio);


#endif
