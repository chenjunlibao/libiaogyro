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
    .id = 0x123,
};

xv7001bb_dev_t xv700_dev = {
    .spi = &spi_dev,
    .can = &can_dev,
    .base = {
        .angle = 0,
        .angle_speed = 0,
        .temperature = 0,
        .driver = &xv7001bb_driver,
        .bias = 0,
    },
    .ss_pin = {GPIOB, GPIO_PIN_12},
};