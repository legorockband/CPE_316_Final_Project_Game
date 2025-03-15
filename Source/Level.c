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
#include "Player.h"

uint8_t x_lock_pos = 40;
uint8_t y_lock_pos = 40;

uint8_t all_key_pos[3][2] = {
		{70, 27},				// Stage 1 Key Position
		{30, 30},				// Stage 2 Key Position
		{30, 30}				// Stage 3 Key Position
};

uint8_t all_lock_pos[3][2] = {
		{40, 40},				// Stage 1 Lock
		{40, 40},				// Stage 2 Lock
		{40, 40}				// Stage 3 Lock
};


uint8_t currentStage = 0;
uint8_t gotKey = 0;

char boarder_char[2] = {0XA9, 0};

char clear_ascii_art[7][2]={
		{0x20, 0},
		{0x20, 0},
		{0x20, 0},
		{0x20, 0},
		{0x20, 0},
		{0x20, 0},
		{0x20, 0}
};

char lock_ascii_art[7][2]=
{
		{0x5F, 0},		// "_"
		{0x2F, 0},		// "/"
		{0x5C, 0},		// "\"
		{0x28, 0},		// "("
		{0x29, 0},		// ")"
		{0x7C, 0},		// "|"
		{0x6F, 0}		// "o"
};

char key_ascii_art[4][2] =
{
		{0x5C, 0},		// "\"
		{0x2F, 0},		// "/"
		{0x7C, 0},		// "|"
		{0x5F, 0}		// "_"
};

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
	currentStage = 1;
	boarder();
	key(all_key_pos[0][0], all_key_pos[0][1], key_ascii_art);
	lock(all_lock_pos[0][0], all_lock_pos[0][1], lock_ascii_art);
}

void stage2(){

}

void key(uint8_t x, uint8_t y, char key_ascii[][2]){
	/*
	/\___		Art for key character
	\/ ||
	*/

	goto_send(x-2, y, key_ascii[1]);	// Create the top left of handle
	goto_send(x-1, y, key_ascii[0]);	// Create the top right of handle
	goto_send(x, y, key_ascii[3]); 		// Create the first part of the shaft
	goto_send(x+1, y, key_ascii[3]); 	// Create the second part of the shaft
	goto_send(x+2, y, key_ascii[3]); 	// Create the third part of the shaft

	goto_send(x-2, y+1, key_ascii[0]);	// Create the bottom right of handle
	goto_send(x-1, y+1, key_ascii[1]);	// Create the bottom left of handle
	goto_send(x+1, y+1, key_ascii[2]); 	// Create the left tooth
	goto_send(x+2, y+1, key_ascii[2]); 	// Create the right tooth
}

void lock(uint8_t x_lock, uint8_t y_lock, char lock_ascii[][2]){
	/*
	   ___
	 _/___\_
	 | (o) |
	 | /|\ |
	 |_____|
	 */

	// Top line
	goto_send(x_lock-1, y_lock-2, lock_ascii[0]);	// Top (left) of handle "_"
	goto_send(x_lock, y_lock-2, lock_ascii[0]);		// Top (middle) of handle "_"
	goto_send(x_lock+1, y_lock-2, lock_ascii[0]);	// Top (right) of handle "_"

	// Second line
	goto_send(x_lock-3, y_lock-1, lock_ascii[0]);	// Top right corner "_"
	goto_send(x_lock-2, y_lock-1, lock_ascii[1]);	// "/" for lock handle
	goto_send(x_lock-1, y_lock-1, lock_ascii[0]); 	// Above the "(", "_"
	goto_send(x_lock, y_lock-1, lock_ascii[0]);		// Above the "o", "_"
	goto_send(x_lock+1, y_lock-1, lock_ascii[0]);	// Above the ")", "_"
	goto_send(x_lock+2, y_lock-1, lock_ascii[2]);	// "\" for lock handle
	goto_send(x_lock+3, y_lock-1, lock_ascii[0]);	// Top left corner "_"

	// Third Line

	goto_send(x_lock-3, y_lock, lock_ascii[5]); 	// Left of Center "|"
	goto_send(x_lock-1, y_lock, lock_ascii[3]); 	// Left of the "o", "("
	goto_send(x_lock, y_lock, lock_ascii[6]);		// Center, "o"
	goto_send(x_lock+1, y_lock, lock_ascii[4]);		// Right of "o", ")"
	goto_send(x_lock+3, y_lock, lock_ascii[5]);		// Right edge from center "|"

	// Fourth Line
	goto_send(x_lock-3, y_lock+1, lock_ascii[5]);	// Left edge "|"
	goto_send(x_lock-1, y_lock+1, lock_ascii[1]);	// Under the "(", "/"
	goto_send(x_lock, y_lock+1, lock_ascii[5]);		// Under the "o"
	goto_send(x_lock+1, y_lock+1, lock_ascii[2]);	// Under the ")", "\"
	goto_send(x_lock+3, y_lock+1, lock_ascii[5]);	// Right edge "|"

	// Last Line
	goto_send(x_lock-3, y_lock+2, lock_ascii[5]);	// Bottom Left "|"
	goto_send(x_lock-2, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock-1, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock, y_lock+2, lock_ascii[0]);		// Bottom "_"
	goto_send(x_lock+1, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock+2, y_lock+2, lock_ascii[0]);	// Bottom "_"
	goto_send(x_lock+3, y_lock+2, lock_ascii[5]);	// Bottom Right "|"
}

void checkPlayerPos(){
	// If the player is on the key, give them the key and allow the door to open
	// TODO: Change the collect area of the key
	// TODO: Might need to change how these are compared
	uint8_t* playerPosition = getPlayerPos();				// Get the player's position, index 0 is the x value, index 1 is the y value
	uint8_t* keyPosition = getKeyPos();						// Get the key's position, index 0 is the x value, index 1 is the y value
	uint8_t* lockPosition = getLockPos();					// Get the lock's position, index 0 is the x value, index 1 is the y value

	bool xOverlapKey = isOverlapping(playerPosition[0],keyPosition[0], 1, 2);	// Checks if the x values are in a range of 3 (person) and 5 (key) since a key is 5x2
	bool yOverlapKey = isOverlapping(playerPosition[1],keyPosition[1], 1, 1);	// Checks if the x values are in a range of 3 (person) and 2 (key) since a key is 5x2

	bool xOverlapLock = isOverlapping(playerPosition[0],lockPosition[0], 1, 3);	// Checks if the x values are in a range of 3 (person) and 7 (key) since a lock is 7x5
	bool yOverlapLock = isOverlapping(playerPosition[1],lockPosition[1], 1, 2);	// Checks if the x values are in a range of 3 (person) and 5 (key) since a key is 7x5


	if(xOverlapKey && yOverlapKey && (gotKey == 0)){
        goto_send(60, 10, "Key Got");
        key(keyPosition[0], keyPosition[1], clear_ascii_art);		// Clear the key
        gotKey = 1;					// The player has a key
	}

	else if(xOverlapLock && yOverlapLock && (gotKey == 1)){
		goto_send(60, 11, "Lock Open");
        lock(lockPosition[0], lockPosition[1], clear_ascii_art);	// Clear the lock
        gotKey = 0;
	}

}

bool isOverlapping(uint8_t playerPos, uint8_t itemPos, uint8_t playerRange, uint8_t itemRange) {
    return (playerPos + playerRange >= itemPos - itemRange) &&
           (playerPos - playerRange <= itemPos + itemRange);
}

uint8_t* getKeyPos(){
	static uint8_t pos[2];
	pos[0] = all_key_pos[currentStage - 1][0];		// Get the x value of the key in the current stage
	pos[1] = all_key_pos[currentStage - 1][1];		// Get the y value of the key in the current stage
	return pos;
}

uint8_t* getLockPos(){
	static uint8_t pos[2];
	pos[0] = all_lock_pos[currentStage - 1][0];		// Get the x value of the lock in the current stage
	pos[1] = all_lock_pos[currentStage - 1][1];		// Get the y value of the lock in the current stage
	return pos;
}



