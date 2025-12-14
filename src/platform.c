#include "platform.h"

spi_t spi_dev = {
    .hspi = &hspi2,
};

led_t led_dev = {
    .led_io = {GPIOB, GPIO_PIN_9},
    .driver = &led_io_driver,
};

can_dev_t can_dev = {
    .filter_bank = 0,
    .handle = &hcan,
    .name = "CAN",
};