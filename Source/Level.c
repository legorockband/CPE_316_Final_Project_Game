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

uint8_t x_lock = 40;
uint8_t y_lock = 40;

char boarder_char[2] = {0XA9, 0};

char key_ascii[3][2] =
{
		{0x4F, 0},		// O, Key Handle
		{0x2D, 0},		// -
		{0x7C, 0}		// |
};

char lock_ascii[6][2]=
{
		{0x5F, 0},		// "_"
		{0x2F, 0},		// "/"
		{0x5C, 0},		// "\"
		{0x28, 0},		// "("
		{0x29, 0},		// ")"
		{0x7C, 0}		// "|"
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

void title(void){

	//TODO: Create a title for the top of the screen using the ASCII text generator



}

void stage1(void){
	boarder();
	key();
	lock();
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

void lock(void){
	/*
	   __
	 _/__\_
	 | () |
	 | /\ |
	 |____|
	 */

	// Top line
	goto_send(x_lock, y_lock-2, lock_ascii[0]);		// Top (left) of handle "_"
	goto_send(x_lock+1, y_lock-2, lock_ascii[0]);	// Top (right) of handle "_"

	// Second line
	goto_send(x_lock-2, y_lock-1, lock_ascii[0]);	// Top right corner "_"
	goto_send(x_lock-1, y_lock-1, lock_ascii[1]);	// "/" for lock handle
	goto_send(x_lock, y_lock-1, lock_ascii[0]);		// Above the "(", "_"
	goto_send(x_lock+1, y_lock-1, lock_ascii[0]);	// Above the ")", "_"
	goto_send(x_lock+2, y_lock-1, lock_ascii[2]);	// "\" for lock handle
	goto_send(x_lock+3, y_lock-1, lock_ascii[0]);	// Top left corner "_"

	// Third Line
	goto_send(x_lock-2, y_lock, lock_ascii[5]); 	// Left of Center "|"
	goto_send(x_lock, y_lock, lock_ascii[3]); 		// Center of the lock "("
	goto_send(x_lock+1, y_lock, lock_ascii[4]);		// The other ")"
	goto_send(x_lock+3, y_lock, lock_ascii[5]);		// Right edge from center "|"

	// Fourth Line
	goto_send(x_lock-2, y_lock+1, lock_ascii[5]);	// Left edge "|"
	goto_send(x_lock, y_lock+1, lock_ascii[1]);		// Under the "(", "/"
	goto_send(x_lock+1, y_lock+1, lock_ascii[2]);	// Under the ")", "\"
	goto_send(x_lock+3, y_lock+1, lock_ascii[5]);	// Right edge "|"

	// Last Line
	goto_send(x_lock-2, y_lock+2, lock_ascii[5]);	// Bottom Left "|"
	goto_send(x_lock-1, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock, y_lock+2, lock_ascii[0]);		// Bottom "_"
	goto_send(x_lock+1, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock+2, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock+3, y_lock+2, lock_ascii[5]);	// Bottom Right "|"


}
