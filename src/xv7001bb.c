#include "xv7001bb.h"
#include <string.h>
#include "platform.h"
#include "cmsis_os.h"

enum {
    REG_ANGULAR_V_READ = 0x0a,
    REG_TMP_READ = 0x08,
    REG_DSP_1 = 0x01,
    REG_DSP_2 = 0x02,
    REG_DSP_3 = 0x03,
    REG_STATUS = 0x04,
    RED_RESET = 0x05,
    REG_STANDBY = 0x07,
    REG_SWRST = 0x09,/*软件复位（只复用户寄存器）*/
    REG_OUTCTL1 = 0x0b,
    REG_AUTOC = 0x0c,/*零偏校准*/
    REG_DSPRES = 0x0d,
    REG_MEMLOAD = 0x1b,
    REG_TSDATA_FORMAT = 0x1c,/*温度格式*/
    REG_IFCTL = 0x1f
};

static osThreadId xv700TaskHandle;
static void xv700_thread(const void *arg);
static void cal_angle(xv7001bb_dev_t * dev);
static float normalize_angle_180(float angle_deg);
static void xv700_calibrate_bias(xv7001bb_dev_t *dev);

//data size 2
static void xv7001bb_write_reg(xv7001bb_dev_t * dev, uint8_t * data)
{
    // 单从机：A[6:5] = 00, MSB=0 -> 写
    data[0] = (data[0] & 0x1F);   // bit7=0 写, bit6..5=0, bit4..0=地址

    spi_ss_off(&dev->ss_pin);
    spi_write(dev->spi, data, sizeof(data));
    spi_ss_on(&dev->ss_pin);

}

static void xv7001bb_read_reg(xv7001bb_dev_t * dev, uint8_t data, uint8_t * rcdata)
{
    data = 0x80 | (data & 0x1F);   // MSB=1 -> 读
    spi_ss_off(&dev->ss_pin);
    spi_write(dev->spi, &data, sizeof(data));
    spi_read(dev->spi, rcdata, sizeof(rcdata));
    spi_ss_on(&dev->ss_pin);
}

void xv7001bb_init(xv7001bb_dev_t * dev)
{
    HAL_Delay(5);

    uint8_t data[2];
    data[0] = REG_SWRST;
    data[1] = 0;
    xv7001bb_write_reg(dev, data);
    HAL_Delay(2);

    //set spi 4
    data[0] = REG_IFCTL;
    data[1] = 0;
    xv7001bb_write_reg(dev, data);
    HAL_Delay(2);

    //open ANGULAR_V out
    data[0] = REG_OUTCTL1;
    data[1] = 0x01;
    xv7001bb_write_reg(dev, data);
    HAL_Delay(2);

    //set tmp format 12bit
    data[0] = REG_TSDATA_FORMAT;
    data[1] = 0x40;
    xv7001bb_write_reg(dev, data);
    HAL_Delay(2);

    osThreadDef(xv700Task, xv700_thread, osPriorityNormal, 0, 128 * 4);
    xv700TaskHandle = osThreadCreate(osThread(xv700Task), dev);
}

void xv7007bb_read_angle16(xv7001bb_dev_t *dev)
{
    uint8_t data = REG_ANGULAR_V_READ;
    uint8_t rcdata[2];
    memset(rcdata, 0, sizeof(rcdata));

    xv7001bb_read_reg(dev, data, rcdata);

    int16_t raw = (rcdata[0] << 8) | rcdata[1];

    dev->base.angle_speed = (float)raw / XV700_SF_16BIT;
}

/*TODO*/
void xv7007bb_read_angle24(xv7001bb_dev_t *dev)
{

}

void xv7007bb_read_temp12(xv7001bb_dev_t *dev)
{
    uint8_t buf[2];
    uint8_t reg = REG_TMP_READ;

    xv7001bb_read_reg(dev, reg, buf);


    int16_t raw12 = (int16_t)(((uint16_t)buf[0] << 4) | (buf[1] >> 4));

    // 补码符号扩展（12bit -> 16bit）
    if (raw12 & 0x800) {        // 若 bit11 为1，表示负数
        raw12 |= 0xF000;        // 高4位补1
    }

    // 按典型温度特性近似换算：
    // 25℃ 时大约 400 LSB
    // 约 16 LSB/℃，也就是 Ts ≈16

    dev->base.temperature = 25.0f + ((float)raw12 - XV700_CODE_AT_25) / XV700_LSB_PER_DEG;

}

void gyro_init(xv7001bb_dev_t *dev)
{
    dev->base.driver->init(dev);
}

void xv700send_temp(xv7001bb_dev_t *dev)
{
    uint8_t *data = (uint8_t *) &dev->base.temperature;
    dev->can->id = TEMP_ID;
    can_send(dev->can, data, sizeof(dev->base.temperature));
}

void xv700send_angle(xv7001bb_dev_t *dev)
{
    uint8_t *data = (uint8_t *) &dev->base.angle;
    dev->can->id = ANGLE_ID;
    can_send(dev->can, data, sizeof(dev->base.angle));
}

gyro_sensor_driver_t xv7001bb_driver = {
    .init = xv7001bb_init,
    .read_angle16 = xv7007bb_read_angle16,
    .read_angle24 = xv7007bb_read_angle24,
    .read_temp = xv7007bb_read_temp12,
};

static float normalize_angle_180(float angle_deg)
{
    while (angle_deg > 180.0f)  angle_deg -= 360.0f;
    while (angle_deg <= -180.0f) angle_deg += 360.0f;
    return angle_deg;
}

static void xv700_thread(const void *arg)
{
    xv7001bb_dev_t *dev = (xv7001bb_dev_t *) arg;
    //Handle zero offset
    xv700_calibrate_bias(dev);
    while (1)
    {
        xv7007bb_read_angle16(dev);
        cal_angle(dev);
        xv700send_angle(dev);

        xv7007bb_read_temp12(dev);
        xv700send_temp(dev);
        osDelay(SAMPL_T);
        
    }
}

static void cal_angle(xv7001bb_dev_t *dev)
{
    if(dev->base.angle_speed < ANSPEED_THRESHOLD && dev->base.angle_speed > -ANSPEED_THRESHOLD)
    {
        dev->base.angle += 0;
    }
    else
    {
        float omega = dev->base.angle_speed;
        float omega_corr = omega - dev->base.bias;

        dev->base.angle += omega_corr * (float)SAMPL_T / 1000;
    }

    dev->base.angle = normalize_angle_180(dev->base.angle);
}

static void xv700_calibrate_bias(xv7001bb_dev_t *dev)
{
    const int N = 200;        // 200 次 × 10ms = 2 秒
    float sum = 0.0f;

    dev->base.bias = 0.0f;
    dev->base.angle = 0.0f;

    for (int i = 0; i < N; i++)
    {
        xv7007bb_read_angle16(dev);
        sum += dev->base.angle_speed;
        osDelay(SAMPL_T);
    }

    dev->base.bias = sum / (float)N;   // 静止平均角速度 = 零偏
    dev->base.angle = 0.0f;        // 校准完从 0 度开始记

    // 以后积分时统一用 (angle_speed - bias)
}