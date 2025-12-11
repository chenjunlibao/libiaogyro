#ifndef _LED_H
#define _LED_H

#include "gpio.h"

typedef struct led_driver led_driver_t;

typedef struct {
    gpio_t led_io;
    led_driver_t *driver;
} led_t;

typedef struct led_driver {
    void (*on)(led_t *);
    void (*off)(led_t *);
    void (*toggle)(led_t *);
} led_driver_t;

void led_on(led_t *led);
void led_off(led_t *led);
void led_toggle(led_t *led);
void led_blink(led_t *led);


#endif
