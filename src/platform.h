#ifndef _PLATFORM_H
#define _PLATFORM_H

/*
 * Hardware Platform Definition for Three MCU board - CIS chip.
 *
 *   This is the configuration file for a specific PCR hardware platform.
 *   Hardware(GPIO/SPI/I2C/UART ..) related defines are all place in this file,
 *   and the `.c' file with the same name.
 *
 * */

#include "cmsis_os.h"
#include "hal.h"
#include "gpio.h"
#include "can.h"
#include "spi.h"
#include "led_io.h"
#include "led.h"
#include "xv7001bb.h"

/*SPI*/
extern SPI_HandleTypeDef hspi2;
extern spi_t spi_dev;

/*LED*/
extern led_t led_dev;

/*CAN*/
extern CAN_HandleTypeDef hcan;
extern can_dev_t can_dev;

/*XV700*/
extern xv7001bb_dev_t xv700_dev;

#endif

