#include "led_io.h"

static void led_io_on(led_t *led)
{
    gpio_reset(&led->led_io);
}

static void led_io_off(led_t *led)
{
    gpio_set(&led->led_io);
}

static void led_io_toggle(led_t *led)
{
    gpio_toggle(&led->led_io);
}

led_driver_t led_io_driver = {
    .on = led_io_on,
    .off = led_io_off,
    .toggle = led_io_toggle,
};