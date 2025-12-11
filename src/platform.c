#include "platform.h"

spi_t spi_dev = {
    .hspi = &hspi2,
};

led_t led_dev = {
    .led_io = {GPIOB, GPIO_PIN_9},
    .driver = &led_io_driver,
};