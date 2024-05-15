/*
 * kline.c
 *
 *  Created on: May 15, 2024
 *      Author: Sapuangin
 */

#include "kline.h"
#include "main.h"
#include "usbd_cdc_if.h"

UART_HandleTypeDef huart2;

nbt_t reqData;

#define RxBuffer_Size 32
#define TxBuffer_Size 32

uint8_t RxBuffer[32];
uint8_t TxBuffer[32];
uint8_t ECU_WAKEUP_MESSAGE[] = { 0xFE, 0x04, 0x72, 0x8C };
uint8_t ECU_INIT_MESSAGE[] = { 0x72, 0x05, 0x00, 0xF0, 0x99 };
uint8_t T17[] = { 0x72, 0x05, 0x71, 0x13, 0x05 };

uint8_t numDataBytes = 0;
int isDataValid = 0;
uint8_t valchk;
uint8_t *filteredData;
int needToInit = 0;

uint8_t calcChecksum(const uint8_t *data, uint8_t len) {
	uint8_t cksum = 0;
	for (int i = 0; i < len; ++i) {
		cksum -= data[i];
	}
	return cksum;
}

uint8_t* filterData(const uint8_t *data, uint8_t len) {
	if (len <= data[1]) {
		return NULL;
	}
	uint8_t *filteredData = (uint8_t*) malloc((len - data[1]) * sizeof(uint8_t));
	if (filteredData == NULL) {
		return NULL;
	}
	for (int i = data[1]; i <= len; i++) {
		filteredData[i - data[1]] = data[i];
	}
	return filteredData;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
	numDataBytes = Size;
	filteredData = filterData(RxBuffer, numDataBytes);
	needToInit = (filteredData == NULL) ? 1 : 0;
	HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxBuffer, sizeof(RxBuffer));
}

void initEcu(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
	HAL_Delay(70);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
	HAL_Delay(130);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

void pingECU(void) {
	HAL_UART_Transmit_IT(&huart2, ECU_WAKEUP_MESSAGE, sizeof(ECU_WAKEUP_MESSAGE));
}

void UART_TURN_OFF(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	HAL_UART_MspDeInit(&huart2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void UART_TURN_ON(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
	HAL_UART_MspInit(&huart2);
	MX_USART2_UART_Init();
}

void resetGpio(void) {
	UART_TURN_OFF();

	initEcu();

	UART_TURN_ON();

	HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxBuffer, RxBuffer_Size);

	pingECU();

	NBT_init(&reqData, 300);
}

void main_loop(void) {
	if (needToInit == 1) {
		UART_TURN_OFF();

		initEcu();

		UART_TURN_ON();

		HAL_UARTEx_ReceiveToIdle_IT(&huart2, RxBuffer, RxBuffer_Size);

		pingECU();

		needToInit = 0;
	}

	if (NBT_handler(&reqData)) {

		HAL_UART_Transmit(&huart2, T17, T17[1], 100);

		//			CDC_Transmit_FS(filteredData, filteredData[1]);


		char *string_array = (char*) malloc((filteredData[1] * 2 + 1) * sizeof(char));
		//
		for (int i = 0; i < filteredData[1]; i++) {
			sprintf(&string_array[i * 2], "%02X", filteredData[i]);
		}

		CDC_Transmit_FS((uint8_t*) string_array, filteredData[1] * 2);

		free(string_array);
		free(filteredData);

	}
}
