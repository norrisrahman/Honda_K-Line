/*
 * kline.h
 *
 *  Created on: May 15, 2024
 *      Author: Sapuangin
 */

#ifndef INC_KLINE_H_
#define INC_KLINE_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdlib.h>
#include "nbt.h"


//static void MX_GPIO_Init(void);
// extern MX_USART2_UART_Init(void);

extern UART_HandleTypeDef huart2;

extern nbt_t reqData;

extern uint8_t RxBuffer[];
extern uint8_t TxBuffer[];
extern uint8_t ECU_WAKEUP_MESSAGE[];
extern uint8_t ECU_INIT_MESSAGE[];
extern uint8_t T17[];
extern uint8_t numDataBytes;
extern int isDataValid;
extern uint8_t valchk;
extern uint8_t *filteredData;
extern int needToInit;

uint8_t calcChecksum(const uint8_t *data, uint8_t len);
uint8_t* filterData(const uint8_t *data, uint8_t len);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size);
void initEcu(void);
void pingECU(void);
void UART_TURN_OFF(void);
void UART_TURN_ON(void);
void main_loop(void);
void resetGpio(void);

#endif /* INC_KLINE_H_ */
