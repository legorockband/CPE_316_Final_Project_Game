/*
 * Level.c
 *
 *  Created on: Mar 1, 2025
 *      Author: pokem
 */


/*
 * Level.c
 *
 *  Created on: Mar 1, 2025
 *      Author: pokem
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"

char boarder_char[] = {0XA9, 0};

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

void boarder(void){
	int y,x,n;

	// Double the Thickness for boarder
	for(n = 0; n < 2; n++){
		// Right Boarder
		for(y = 0; y < 55; y++){
			char Goto[10];
			snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", y, n*2); // Format cursor position string
			UART_send(&huart2, Goto);

			// Send an extended block character
			UART_send(&huart2, boarder_char);
		}
	}

	// Double the Thickness for boarder
	for(n = 0; n < 2; n++){
		// Left Boarder
		for(y = 0; y < 55; y++){
			char Goto[10];
			snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", y, 157+n*2); // Format cursor position string
			UART_send(&huart2, Goto);

			// Send an extended block character
			UART_send(&huart2, boarder_char);
		}
	}

	// Top boarder
	for(x = 0; x < 157; x++){
		char Goto[10];
		snprintf(Goto, sizeof(Goto), "\x1B[0;%dH", x); // Format cursor position string
		UART_send(&huart2, Goto);

		// Send an extended block character
		UART_send(&huart2, boarder_char);
	}

	// Bottom boarder
	for(x = 0; x < 157; x++){
		char Goto[10];
		snprintf(Goto, sizeof(Goto), "\x1B[55;%dH", x); // Format cursor position string
		UART_send(&huart2, Goto);

		// Send an extended block character
		UART_send(&huart2, boarder_char);
	}
}


void stage1(void){
	boarder();

}


void key(void){



}

void door(void){





}
