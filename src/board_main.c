#include "gpio.h"
#include "spi.h"
#include "platform.h"
#include "led.h"
#include "can.h"
#include "xv7001bb.h"
#include <stdlib.h>

void board_main()
{
    spi_init(&spi_dev);
    can_init(&can_dev);

    gyro_init(&xv700_dev);

    osThreadTerminate(osThreadGetId());
}