#include "spi.h"
#include <string.h>

osMutexDef(spi_mutex);

void spi_init(spi_t *spi)
{
    spi->mutex = osMutexCreate(osMutex(spi_mutex)); 
}

void spi_ss_on(gpio_t *pin)
{
    gpio_set(pin);
}

void spi_ss_off(gpio_t *pin)
{
    gpio_reset(pin);
}

void spi_read(spi_t *spi, uint8_t *data, uint16_t len)
{
    memset(data, 0, len);
    osMutexWait(spi->mutex, osWaitForever);
    HAL_SPI_Receive(spi->hspi, data, len, 100);
    osMutexRelease(spi->mutex);
}

void spi_write(spi_t *spi, uint8_t *data, uint16_t len)
{
    osMutexWait(spi->mutex, osWaitForever);
	HAL_SPI_Transmit(spi->hspi, data, len, 100);
    osMutexRelease(spi->mutex);
}
