#include "gpio.h"

void gpio_set(gpio_t *gpio)
{
    HAL_GPIO_WritePin(gpio->port, gpio->pin, GPIO_PIN_SET);
}

void gpio_reset(gpio_t *gpio)
{
    HAL_GPIO_WritePin(gpio->port, gpio->pin, GPIO_PIN_RESET);
}

void gpio_toggle(gpio_t *gpio)
{
    HAL_GPIO_TogglePin(gpio->port, gpio->pin);
}

uint8_t gpio_get(gpio_t *gpio)
{
    return HAL_GPIO_ReadPin(gpio->port, gpio->pin);
}

void gpio_set_input(gpio_t *gpio)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = gpio->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(gpio->port, &GPIO_InitStruct);
}

void gpio_set_output(gpio_t *gpio)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = gpio->pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(gpio->port, &GPIO_InitStruct);
}
