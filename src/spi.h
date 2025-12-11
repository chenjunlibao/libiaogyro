#ifndef _SPI_H
#define _SPI_H

#include "hal.h"
#include "cmsis_os.h"
#include "gpio.h"

typedef struct {
    osMutexId mutex;
    SPI_HandleTypeDef *hspi; /* SPI Handle */
} spi_t;

void spi_init(spi_t *spi);
void spi_ss_on(gpio_t *pin);
void spi_ss_off(gpio_t *pin);
void spi_read(spi_t *spi, uint8_t *data, uint16_t len);
void spi_write(spi_t *spi, uint8_t *data, uint16_t len);

#endif