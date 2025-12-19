#include "can.h"
#include "led.h"
#include "platform.h"
#include <string.h>

typedef struct {
    uint8_t data[CAN_MAX_FRAME_SIZE];
    uint8_t size;
    CAN_HandleTypeDef *can_handle;
} can_msg_t;

static CAN_TxHeaderTypeDef TxHeader;
static CAN_RxHeaderTypeDef RxHeader;

//only four byte date len in cmsis_os_v1, put poiter
static osMessageQDef(queue, 64, uint32_t);
static osMessageQId queue_handle;
static osMutexDef(cantx_mutex);
static osThreadId canrxTaskHandle;
static osThreadId cantxTaskHandle;

static uint8_t rxdata = {0};
static uint8_t msg_avaliable = 0;


static void can_rx_thread(const void *arg);
static void can_tx_thread(const void *arg);


void can_init(can_dev_t *can)
{
    can->tx_mutex_id = osMutexCreate(osMutex(cantx_mutex));
    queue_handle = osMessageCreate(osMessageQ(queue), NULL);

    // osThreadDef(canrxTask, can_rx_thread, osPriorityNormal, 0, 128 * 4);
    // canrxTaskHandle = osThreadCreate(osThread(canrxTask), can);

    osThreadDef(cantxTask, can_tx_thread, osPriorityNormal, 0, 128 * 4);
    cantxTaskHandle = osThreadCreate(osThread(cantxTask), can);
    
    CAN_FilterTypeDef filter;

    filter.FilterBank = can->filter_bank;
    filter.FilterMode = CAN_FILTERMODE_IDMASK;
    filter.FilterScale = CAN_FILTERSCALE_16BIT;
    filter.SlaveStartFilterBank = 14;

    /* ID 和 MASK 全 0 → 全接收 */
    filter.FilterIdHigh = 0x0000;
    filter.FilterIdLow  = 0x0000;
    filter.FilterMaskIdHigh = 0x0000;
    filter.FilterMaskIdLow  = 0x0000;

    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterActivation = ENABLE;

    HAL_CAN_ConfigFilter(can->handle, &filter);

    HAL_CAN_Start(can->handle);

    //enale fifo0 receive interrupt
    //HAL_CAN_ActivateNotification(can->handle, CAN_IT_RX_FIFO0_MSG_PENDING);
}

int8_t can_send(can_dev_t *can, uint8_t *data, uint8_t len)
{
    int8_t ret;

    can_lock(can);
    ret = can_raw_send(can, data, len);
    can_unlock(can);

    return ret;
}

//send without mutex
int8_t can_raw_send(can_dev_t *can, uint8_t *data, uint8_t len)
{
    uint8_t ret;
    static uint32_t TxMailbox = 0;

    if (len > CAN_MAX_FRAME_SIZE)
    {
        return -1;
    }

    TxHeader.StdId = can->id;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.IDE = CAN_ID_STD;
    TxHeader.DLC = len;
    TxHeader.TransmitGlobalTime = DISABLE;

    ret = HAL_CAN_AddTxMessage(can->handle, &TxHeader, data, &TxMailbox);
    if (ret != HAL_OK)
    {
        return -1;
    }

    return 0;
}

void can_lock(can_dev_t *can)
{
    osMutexWait(can->tx_mutex_id, osWaitForever);
}

void can_unlock(can_dev_t *can)
{
    osMutexRelease(can->tx_mutex_id);
}

//rewrite can callback
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, &rxdata);
    msg_avaliable = 1;
}

static void can_tx_thread(const void *arg)
{
    // while (1)
    // {
    //     osEvent evt = osMessageGet(queue_handle, osWaitForever);
    //     if (evt.status != osEventMessage)
    //         continue;

    //     can_msg_t *msg = (can_msg_t *)evt.value.v;

    //     vPortFree(msg);
    //     osDelay(1);
    // }
    while (1)
    {
        led_blink(&led_dev);
    }
    
}

static void can_rx_thread(const void *arg)
{
    can_dev_t *can = (can_dev_t *)arg;
    uint8_t rxData[8] = {0};
    while (1)
    {
        // while(HAL_CAN_GetRxFifoFillLevel(can->handle, CAN_RX_FIFO0) > 0)
        // {

        //     // can_msg_t *msg = pvPortMalloc(sizeof(can_msg_t));

        //     // HAL_CAN_GetRxMessage(can->handle, CAN_RX_FIFO0,&RxHeader, rxData);
        //     // msg->size = RxHeader.DLC;
        //     // memcpy(msg->data, rxData, RxHeader.DLC);

        //     // osMessagePut(queue_handle, (uint32_t)msg, 0);
        //     HAL_CAN_GetRxMessage(can->handle, CAN_RX_FIFO0,&RxHeader, rxData);
        //     can_send(can, rxData, RxHeader.DLC);
        // }
        // if (msg_avaliable == 1)
        // {
        //     HAL_CAN_GetRxMessage(can->handle, CAN_RX_FIFO0,&RxHeader, rxData);
        //     can_send(can, rxData, RxHeader.DLC);
        //     msg_avaliable = 0;
        // }
        // osDelay(100);
    }
    
}