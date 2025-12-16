#ifndef _GYRO_SENSOR_H
#define _GYRO_SENSOR_H

#include <stdlib.h>
#include "spi.h"

typedef struct gyro_sensor_driver gyro_sensor_driver_t;

typedef struct gyro_sensor {
    uint16_t id;
    gyro_sensor_driver_t *driver;
    float last_reading;
    int8_t last_reading_result;
    uint32_t last_reading_time;
} gyro_sensor_t;

typedef struct gyro_sensor_driver {
    void (*init)(gyro_sensor_t *dev);
    int (*read)(gyro_sensor_t *dev, float *val);
    int (*read_async)(gyro_sensor_t *dev);
    int (*read_immediate)(gyro_sensor_t *dev, float *val);
} gyro_sensor_driver_t;


#endif