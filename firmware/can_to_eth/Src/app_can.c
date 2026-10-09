#include "app_can.h"

volatile app_can_frame_t app_can_last_frame;
volatile uint32_t app_can_rx_count;
volatile uint32_t app_can_error_count;
volatile uint32_t app_can_last_error;

HAL_StatusTypeDef app_can_start(CAN_HandleTypeDef *hcan)
{
  CAN_FilterTypeDef filter = {0};

  /* Bank 0, 32-bit mask mode: identifier and mask both zero accept every frame. */
  filter.FilterBank = 0U;
  filter.FilterMode = CAN_FILTERMODE_IDMASK;
  filter.FilterScale = CAN_FILTERSCALE_32BIT;
  filter.FilterIdHigh = 0U;
  filter.FilterIdLow = 0U;
  filter.FilterMaskIdHigh = 0U;
  filter.FilterMaskIdLow = 0U;
  filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  filter.FilterActivation = ENABLE;
  filter.SlaveStartFilterBank = 14U;

  if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
  {
    return HAL_ERROR;
  }

  if (HAL_CAN_Start(hcan) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_CAN_ActivateNotification(hcan,
                                      CAN_IT_RX_FIFO0_MSG_PENDING |
                                      CAN_IT_ERROR_WARNING |
                                      CAN_IT_ERROR_PASSIVE |
                                      CAN_IT_BUSOFF |
                                      CAN_IT_LAST_ERROR_CODE |
                                      CAN_IT_ERROR);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  CAN_RxHeaderTypeDef header;
  uint8_t data[8];
  uint32_t index;

  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK)
  {
    app_can_last_error = HAL_CAN_GetError(hcan);
    app_can_error_count++;
    return;
  }

  app_can_last_frame.std_id = header.StdId;
  app_can_last_frame.ext_id = header.ExtId;
  app_can_last_frame.ide = header.IDE;
  app_can_last_frame.rtr = header.RTR;
  app_can_last_frame.dlc = header.DLC;
  for (index = 0U; index < 8U; index++)
  {
    app_can_last_frame.data[index] = data[index];
  }
  app_can_rx_count++;
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  app_can_last_error = HAL_CAN_GetError(hcan);
  app_can_error_count++;
}
