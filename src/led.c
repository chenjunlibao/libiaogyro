#include "led.h"
#include "cmsis_os.h"

void led_on(led_t *led)
{
    led->driver->on(led);
}

void led_off(led_t *led)
{
    led->driver->off(led);
}

void led_toggle(led_t *led)
{
    led->driver->toggle(led);
}

void led_blink(led_t *led)
{
    led_on(led);
    osDelay(100);
    led_off(led);
    osDelay(100);
}
