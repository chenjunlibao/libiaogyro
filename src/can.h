#ifndef _CAN_H
#define _CAN_H

#include "hal.h"
#include "cmsis_os.h"

enum {
    CAN_MAX_FRAME_SIZE = 8,
    CAN_RX_SIGNAL = 0x01,
};

typedef struct {
    CAN_HandleTypeDef *handle;
    uint8_t filter_bank;
    char *name;
    osMutexId tx_mutex_id;
} can_dev_t;

/* CAN request frame */
struct can_frame {
    uint8_t sub_id;
    uint8_t code;   /* function code */
    union {
        uint8_t u8;
        int8_t i8;
        uint16_t u16;
        uint32_t u32;
        float f32;
        uint8_t data[0];
    } param;
} __attribute__((__packed__));


/* CAN reply frame */
struct can_reply_frame {
//    uint8_t src;  /* source device ID */
//    int8_t status;
    union {
        uint8_t u8;
        int8_t i8;
        uint16_t u16;
        uint32_t u32;
        float f32;
        uint8_t data[0];
    } param;
} __attribute__((__packed__));

struct can_read_row {
    float integration_time;
    uint8_t row;
} __attribute__((__packed__));



void can_init(can_dev_t *can);
int8_t can_send(can_dev_t *can, uint8_t *data, uint8_t len);
//int8_t can_recv(can_dev_t *can, uint16_t src, uint16_t *dst, uint8_t *data, uint8_t size, uint32_t timeout);
int8_t can_raw_send(can_dev_t *can, uint8_t *data, uint8_t len);
//int8_t can_txrx(can_dev_t *can, uint8_t type, uint16_t addr, uint8_t *txdata, uint8_t txlen, uint16_t *dst, uint8_t *rxdata, uint8_t rxlen, uint32_t timeout);
void can_lock(can_dev_t *can);
void can_unlock(can_dev_t *can);

#endif
