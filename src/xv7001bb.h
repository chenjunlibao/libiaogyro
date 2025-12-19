#ifndef _XV7001BB_H
#define _XV7001BB_H

#include <stdlib.h>
#include "spi.h"
#include "gpio.h"
#include "can.h"

//Angular velocity calculation constant
#define XV700_SF_16BIT         70.0f
#define XV700_SF_24BIT         71680.0f
#define ANSPEED_THRESHOLD      0.1f

#define ANGLE_ID               0x123
#define TEMP_ID                0x124

//Temperature calculation constant
#define XV700_LSB_PER_DEG      16.0f
#define XV700_CODE_AT_25       400.0f

#define SAMPL_T                10
typedef struct gyro_sensor_driver gyro_sensor_driver_t;
typedef struct xv7001bb_dev xv7001bb_dev_t;

typedef struct gyro_sensor {
    gyro_sensor_driver_t *driver;
    float temperature;
    float angle;
    float angle_speed;
    float bias;
} gyro_sensor_t;

typedef struct gyro_sensor_driver {
    void (*init)(xv7001bb_dev_t *dev);
    void (*read_angle16)(xv7001bb_dev_t *dev);
    void (*read_angle24)(xv7001bb_dev_t *dev);
    void (*read_temp)(xv7001bb_dev_t *dev);
} gyro_sensor_driver_t;

typedef struct xv7001bb_dev{
    gyro_sensor_t base;
    spi_t *spi;
    gpio_t ss_pin;
    can_dev_t *can;
} xv7001bb_dev_t;

extern gyro_sensor_driver_t xv7001bb_driver;

void xv7001bb_init(xv7001bb_dev_t * dev);
void xv7007bb_read_angle16(xv7001bb_dev_t * dev);
void xv7007bb_read_angle24(xv7001bb_dev_t * dev);
void xv7007bb_read_temp12(xv7001bb_dev_t * dev);
void gyro_init(xv7001bb_dev_t * dev);
void xv700send_temp(xv7001bb_dev_t * dev);
void xv700send_angle(xv7001bb_dev_t * dev);



#endif