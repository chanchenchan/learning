#ifndef APP_CAN_H
#define APP_CAN_H

#include "main.h"

typedef struct
{
  uint32_t std_id;
  uint32_t ext_id;
  uint32_t ide;
  uint32_t rtr;
  uint32_t dlc;
  uint8_t data[8];
} app_can_frame_t;

/* These symbols are deliberately global and volatile for the Keil Watch view. */
extern volatile app_can_frame_t app_can_last_frame;
extern volatile uint32_t app_can_rx_count;
extern volatile uint32_t app_can_error_count;
extern volatile uint32_t app_can_last_error;

HAL_StatusTypeDef app_can_start(CAN_HandleTypeDef *hcan);

#endif /* APP_CAN_H */
