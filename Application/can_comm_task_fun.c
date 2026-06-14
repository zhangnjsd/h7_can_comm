#include "can_comm_task_fun.h"
#include "main.h"
#include "stm32h7xx_hal_fdcan.h"
#include <stdint.h>

void can_comm_task_fun(void *argument) {
  /* USER CODE BEGIN can_comm_task_fun */
  // ? FD CAN Setup should be done in main.c, and the handle should be declared
  // in main.h.
  uint32_t tick;

  // Send any messages.
  uint8_t msg[] = {0, 0, 0, 0, 0, 0, 0, 0};

  FDCAN_TxHeaderTypeDef tx_header = {.Identifier = REMOTE_ID,
                                     .IdType = FDCAN_STANDARD_ID,
                                     .TxFrameType = FDCAN_DATA_FRAME,
                                     .DataLength = FDCAN_DLC_BYTES_8,
                                     .ErrorStateIndicator = FDCAN_ESI_ACTIVE,
                                     .BitRateSwitch = FDCAN_BRS_OFF,
                                     .FDFormat = FDCAN_CLASSIC_CAN,
                                     .TxEventFifoControl = FDCAN_NO_TX_EVENTS,
                                     .MessageMarker = 0};

  /* Infinite loop */
  for (;;) {
    tick = osKernelGetTickCount();

    int16_t motor_spd = 10000;
    msg[0] = (uint8_t)(motor_spd >> 8);
    msg[1] = (uint8_t)motor_spd;
    msg[2] = (uint8_t)(motor_spd >> 8);
    msg[3] = (uint8_t)motor_spd;
    msg[4] = (uint8_t)(motor_spd >> 8);
    msg[5] = (uint8_t)motor_spd;
    msg[6] = (uint8_t)(motor_spd >> 8);
    msg[7] = (uint8_t)motor_spd;

    // Transmit the message.
    if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_header, msg) != HAL_OK) {
      Error_Handler();
    }
    
    osDelayUntil(tick + 10);
  }
  /* USER CODE END can_comm_task_fun */
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {

  FDCAN_RxHeaderTypeDef rx_header;
  uint8_t rx_data[8];

  // Only process when a message is actually available
  if (HAL_FDCAN_GetRxMessage(&hfdcan1, FDCAN_RX_FIFO0, &rx_header, rx_data) ==
      HAL_OK) {

    // Check if the message is from the remote ID and has the correct data
    // length.
    if (rx_header.Identifier >= 0x205 && rx_header.Identifier <= 0x208) {
      for (int i = 0; i < 8; i++) {
        recv_buf[i] = rx_data[i];
      }
    }
  }

}