#include "can_comm_task_fun.h"

void can_comm_task_fun(void *argument) {
  /* USER CODE BEGIN can_comm_task_fun */
  // ? FD CAN Setup should be done in main.c, and the handle should be declared
  // in main.h.
  uint32_t tick;

  // Send any messages.
  uint8_t msg[] = {1, 0, 0, 0};

  FDCAN_TxHeaderTypeDef tx_header = {.Identifier = REMOTE_ID,
                                     .IdType = FDCAN_STANDARD_ID,
                                     .TxFrameType = FDCAN_DATA_FRAME,
                                     .DataLength = FDCAN_DLC_BYTES_4};

  // Transmit the message.
  if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_header, msg) != HAL_OK) {
    Error_Handler();
  }
  /* Infinite loop */
  for (;;) {
    tick = osKernelGetTickCount();

    osDelayUntil(tick + 1000);
  }
  /* USER CODE END can_comm_task_fun */
}

// ! RxFIFO0 Management.
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan,
                               uint32_t RxFifo0ITs) {
  if (RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
    FDCAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[4]; // Max data length for CAN FD is 64 bytes.

    if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, rx_data) !=
        HAL_OK) {
      Error_Handler();
    }

    for (int i = 0; i < 4; i++) {
      recv_buf[i] = rx_data[i];
    }

    // Process the received message (for example, print it or toggle an LED).
  }
}