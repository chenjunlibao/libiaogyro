#ifndef _XV7001BB_H
#define _XV7001BB_H

#include <stdlib.h>
#include "spi.h"
#include "gyro_sensor.h"

typedef struct {
    gyro_sensor_t base;
    spi_t *spi;
    uint8_t addr; /* address */
} xv7001bb_dev_t;



#endif