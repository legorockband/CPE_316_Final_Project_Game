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
#include "Level.h"
#include "Terminal.h"

char boarder_char[2] = {0XA9, 0};

char key_ascii[3][2] =
{
		{0x4F, 0},		// O, Key Handle
		{0x2D, 0},		// -
		{0x7C, 0}		// |
};

extern UART_HandleTypeDef huart2;

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
	key();
}


void key(void){
	/*
	O--		Art for key character
	 ||
	*/

	uint8_t x = 30;
	uint8_t y = 30;

	goto_send(x, y, key_ascii[1]); 		// Create the first part of the shaft
	goto_send(x+1, y , key_ascii[1]);	// Create the second part of the shaft
	goto_send(x-1, y, key_ascii[0]);	// Create the handle
	goto_send(x,y+1, key_ascii[2]);		// Create first tooth
	goto_send(x+1, y+1, key_ascii[2]);	// Create second tooth
}

void door(void){



}
