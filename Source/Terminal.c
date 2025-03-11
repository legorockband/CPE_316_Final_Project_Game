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

void goto_send(uint8_t x, uint8_t y, char *player_ascii){
	char Goto[12];
	// Position for Person
	snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", y, x); // Format cursor position string
	UART_send(&huart2, Goto);

	// Send an Body to the center of the screen
	UART_send(&huart2, player_ascii);
}
