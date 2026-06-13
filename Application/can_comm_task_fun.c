#include "can_comm_task_fun.h"

void can_comm_task_fun(void *argument)
{
	/* USER CODE BEGIN can_comm_task_fun */
	// ? FD CAN Setup should be done in main.c, and the handle should be declared in main.h. 
	uint32_t tick;
	/* Infinite loop */
	for(;;) {
		tick = osKernelGetTickCount();
		// Send any messages.
		uint8_t msg[] = {1, 0, 0, 0};

		FDCAN_TxHeaderTypeDef tx_header = {
			.Identifier = REMOTE_ID,
			.IdType = FDCAN_STANDARD_ID,
			.TxFrameType = FDCAN_DATA_FRAME,
			.DataLength = FDCAN_DLC_BYTES_4
		};

		// Transmit the message.
		if (HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &tx_header, msg) != HAL_OK) {
			Error_Handler();
		}

		osDelayUntil(tick + 1000);
	}
	/* USER CODE END can_comm_task_fun */
}