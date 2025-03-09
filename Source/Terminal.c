/*
 * Terminal.c
 *
 *  Created on: Mar 8, 2025
 *      Author: pokem
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "Terminal.h"

extern UART_HandleTypeDef huart2;

void UART_send(UART_HandleTypeDef *huart, char buffer[])
{
    HAL_UART_Transmit(huart, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);
}

/* USER CODE BEGIN 0 */
void UART_send_newline(UART_HandleTypeDef *huart)
{
      HAL_UART_Transmit(huart, (uint8_t *)"\n\r", 2, HAL_MAX_DELAY);
}
