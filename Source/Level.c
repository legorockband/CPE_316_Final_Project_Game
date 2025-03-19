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

#define MAX_WALLS 100

Wall walls[MAX_WALLS];
uint8_t total_walls = 0;

void (*stages[3])(void) = {stage1, stage2, stage3};

uint8_t all_key_pos[3][2] = {
		{70, 27},				// Stage 1 Key Position
		{145, 5},				// Stage 2 Key Position
		{30, 30}				// Stage 3 Key Position
};

uint8_t all_lock_pos[3][2] = {
		{40, 40},				// Stage 1 Lock
		{15, 32},				// Stage 2 Lock
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

char k_ascii[6][9][2] = {
    {{0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x20,0}, {0x20,0}},  // "  _  __  "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}},  // " | |/ /  "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " | ' /   "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x3C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " |  <    "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x2E,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " | . \   "
    {{0x20,0}, {0x7C,0}, {0x5F,0}, {0x7C,0}, {0x5C,0}, {0x5F,0}, {0x5C,0}, {0x20,0}, {0x20,0}}   // " |_|\_\  "
};



char e_ascii[6][9][2] = {
    {{0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}},  // "  ______  "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x7C,0}, {0x20,0}},  // " |  ____| "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}},  // " | |____  "
    {{0x20,0}, {0x7C,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x7C,0}, {0x20,0}},  // " |  ____| "
    {{0x20,0}, {0x7C,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}},  // " | |_____ "
    {{0x20,0}, {0x7C,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x7C,0}, {0x20,0}}   // " |______| "
};

char y_ascii[6][9][2] = {
    {{0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}},  // "__   __ "
    {{0x5C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x2F,0}, {0x20,0}},  // "\ \ / / "
    {{0x20,0}, {0x5C,0}, {0x20,0}, {0x5C,0}, {0x2F,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}},  // " \ V /  "
    {{0x20,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  \ /   "
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   | |  "
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}}   // "   |_|  "
};

char m_ascii[5][13][2] = {
    {{0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}},  // "__       __ "
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x7C,0}},  // "|  \     /  |"
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}},  // "| | \   / | |"
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}},  // "| |  \_/  | |"
    {{0x7C,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x7C,0}}   // "|_|       |_|"
};



char a_ascii[5][11][2] = {
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "     _    "
    {{0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "    / \   "
    {{0x20,0}, {0x2F,0}, {0x20,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   / _ \  "
    {{0x2F,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  / ___ \ "
    {{0x5F,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5C,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}}   // " /_/   \_\"
};

char s_ascii[6][11][2] = {
    {{0x20,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   _____   "
    {{0x20,0}, {0x2F,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  / ____|  "
    {{0x7C,0}, {0x20,0}, {0x28,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " | (_____  "
    {{0x20,0}, {0x5C,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  \_____ \ "
    {{0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x29,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  _____) | "
    {{0x7C,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}}   // " |______/  "
};

char t_ascii[6][8][2] = {
    {{0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}},  // "  _____  "
    {{0x7C,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x7C,0}, {0x20,0}},  // " |_   _| "
    {{0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   | |   "
    {{0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   | |   "
    {{0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   | |   "
    {{0x20,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}}   // "   |_|   "
};

char r_ascii[6][10][2] = {
    {{0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  _____   "
    {{0x7C,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " |  __ \  "
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x29,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " | |__) | "
    {{0x7C,0}, {0x20,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " |  _  /  "
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // " | | \ \  "
    {{0x7C,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x5C,0}, {0x5F,0}, {0x5C,0}, {0x20,0}, {0x20,0}}   // " |_|  \_\ "
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

void makewall(uint8_t x_start, uint8_t y_start, uint8_t length, char direction) {
    for (uint8_t i = 0; i < length; i++) {
        uint8_t x = x_start;
        uint8_t y = y_start;
        // always start from 3 to account for 2 walls on x-axis
        // always start from 1 to account for 1 walls on y-axis
        switch (direction) {
            case 'L': x = x_start - i; break; // Move left
            case 'R': x = x_start + i; break; // Move right
            case 'U': y = y_start - i; break; // Move up
            case 'D': y = y_start + i; break; // Move down
            default: return;
        }

        if (total_walls < MAX_WALLS) {
            walls[total_walls].x = x;
            walls[total_walls].y = y;
            total_walls++;
        }

        goto_send(x, y, boarder_char);
    }
}

void resetWalls(){
	// Reset the wall positions
	for (uint8_t i = 0; i < total_walls; i++){
		walls[i].x = 0;
		walls[i].y = 0;
	}

	// Reset the total number of walls
	total_walls = 0;
}

bool* wallCollision(){
	uint8_t* playerPos = getPlayerPos();
    uint8_t player_x = playerPos[0];
    uint8_t player_y = playerPos[1];

    // Set an array of bool values to false for each direction
    static bool wallCheck[4] = {false, false, false, false};		// [0] is Left, [1] is Right, [2] is Up, [3] is Down

    // Reset wallColl[] each time it's called
    for (int i = 0; i < 4; i++) {
    	wallCheck[i] = false;
    }

    // Check each position with all of the possible walls compared to the players position
    for (uint8_t i = 0; i < total_walls; i++) {
        if (walls[i].x == player_x - 2 &&
           (walls[i].y == player_y || walls[i].y == player_y - 1 || walls[i].y == player_y + 1)) {
        	wallCheck[0] = true;	// Left wall has collision
        }
        if ((walls[i].x == player_x + 2) &&
            (walls[i].y == player_y || walls[i].y == player_y - 1 || walls[i].y == player_y + 1)) {
        	wallCheck[1] = true;	// Right wall has collision
        }
        if ((walls[i].x == player_x || walls[i].x == player_x - 1 || walls[i].x == player_x + 1) &&
        	(walls[i].y == player_y - 2)) {
        	wallCheck[2] = true;	// Up wall has collision
        }
        if ((walls[i].x == player_x || walls[i].x == player_x - 1 || walls[i].x == player_x + 1) &&
        	(walls[i].y == player_y + 2)) {
        	wallCheck[3] = true;	// Down wall has collision
        }
    }

    return wallCheck;
}

bool* lockCollision(uint8_t stage){
	uint8_t* playerPos = getPlayerPos();
    uint8_t player_x = playerPos[0];
    uint8_t player_y = playerPos[1];

    // Set an array of bool values to false for each direction
    static bool lockCheck[4] = {false, false, false, false};		// [0] is Left, [1] is Right, [2] is Up, [3] is Down

    // Reset lockColl[] each time it's called
    for (int i = 0; i < 4; i++) {
    	lockCheck[i] = false;
    }

    // Get current lock position based on the current stage
    uint8_t lock_x = all_lock_pos[stage - 1][0];
    uint8_t lock_y = all_lock_pos[stage - 1][1];

    // Define lock boundaries
    uint8_t lock_left = lock_x - 4;
    uint8_t lock_right = lock_x + 4;
    uint8_t lock_top = lock_y - 3;
    uint8_t lock_bottom = lock_y + 3;

    // Left of Lock collision, Player’s right side touches lock’s left side
    if ((player_x + 1 == lock_left) &&
        (player_y + 1 >= lock_top && player_y - 1 <= lock_bottom)) {
        lockCheck[1] = true;
    }
    // Right of Lock collision – Player’s left side touches lock’s right side
    if (player_x - 1 == lock_right  &&
        (player_y + 1 >= lock_top && player_y - 1 <= lock_bottom)) {
        lockCheck[0] = true;
    }

    // Top of Lock collision – Player’s top side touches lock’s bottom side
    if (player_y + 1 == lock_top &&
        (player_x >= lock_left && player_x <= lock_right)) {
        lockCheck[3] = true;
    }

    // Bottom of lock collision – Player’s bottom side touches lock’s top side
    if (player_y - 1 == lock_bottom  &&
        (player_x >= lock_left && player_x <= lock_right)) {
        lockCheck[2] = true;
    }

	char debug[50];
	sprintf(debug, "Lock: L-%d R-%d U-%d D-%d", lockCheck[0], lockCheck[1], lockCheck[2], lockCheck[3]);
	goto_send(10, 17, debug);

    return lockCheck;
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

void stage2(void) {
	currentStage = 2;
	boarder();
    key(all_key_pos[1][0], all_key_pos[1][1], key_ascii_art);
    lock(all_lock_pos[1][0], all_lock_pos[1][1], lock_ascii_art);
    makewall(30, 32, 12, 'L');  // Up works(how far over[x],how far down[y],how many  )
    makewall(30, 32, 19, 'D');  // Left works
    makewall(3,  32, 9, 'R');
}

void stage3(void){
	currentStage = 3;
	boarder();
	resetWalls();

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
	uint8_t* playerPosition = getPlayerPos();				// Get the player's position, index 0 is the x value, index 1 is the y value
	uint8_t* keyPosition = getKeyPos();						// Get the key's position, index 0 is the x value, index 1 is the y value
	uint8_t* lockPosition = getLockPos();					// Get the lock's position, index 0 is the x value, index 1 is the y value

	bool xOverlapKey = isOverlapping(playerPosition[0],keyPosition[0], 1, 2);	// Checks if the x values are in a range of 3 (person) and 5 (key) since a key is 5x2
	bool yOverlapKey = isOverlapping(playerPosition[1],keyPosition[1], 1, 1);	// Checks if the x values are in a range of 3 (person) and 2 (key) since a key is 5x2

	bool xOverlapLock = isOverlapping(playerPosition[0],lockPosition[0], 1, 3);	// Checks if the x values are in a range of 3 (person) and 7 (key) since a lock is 7x5
	bool yOverlapLock = isOverlapping(playerPosition[1],lockPosition[1], 1, 2);	// Checks if the x values are in a range of 3 (person) and 5 (key) since a key is 7x5

	// If the player is over a key, pick up the key
	if(xOverlapKey && yOverlapKey){
        goto_send(60, 10, "Key Got");
        key(keyPosition[0], keyPosition[1], clear_ascii_art);		// Clear the key
        gotKey = 1;					// The player has a key
	}

	// If you are on top of the lock with the key, go to the next stage
	else if(xOverlapLock && yOverlapLock && (gotKey == 1)){
		char ClearScreen[] = { 0x1B, '[', '2' , 'J',0 }; 	// Clear the screen
		char CursorHome[] = { 0x1B, '[' , 'H' , 0 }; 	// Home the cursor
		goto_send(60, 11, "Lock Open");
        lock(lockPosition[0], lockPosition[1], clear_ascii_art);	// Clear the lock
        gotKey = 0;

        currentStage++;		// Go to the next stage

    	HAL_UART_Transmit(&huart2, (uint8_t *)ClearScreen, strlen(ClearScreen), HAL_MAX_DELAY);
    	HAL_UART_Transmit(&huart2, (uint8_t *)CursorHome, strlen(CursorHome), HAL_MAX_DELAY);

        stages[currentStage - 1]();			// Draw the next stage
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

uint8_t getKeyStatus(){
	return gotKey;
}

uint8_t getCurrentStage(){
	return currentStage;
}

void title(uint8_t x_title, uint8_t y_title) {
    // Print "K"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 0; col < 9; col++) {
            goto_send(x_title + 50 + col, y_title - 10 + row, k_ascii[row][col]);
        }
    }

    // Print "E"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 0; col < 9; col++) {
            goto_send(x_title + 58 + col, y_title - 10 + row, e_ascii[row][col]);
        }
    }

    // Print "Y"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 0; col < 9; col++) {
            goto_send(x_title + 67 + col, y_title - 10 + row, y_ascii[row][col]);
        }
    }

    // Print "M"
    for (uint8_t row = 0; row < 5; row++) {
        for (uint8_t col = 0; col < 13; col++) {
            goto_send(x_title + 76 + col, y_title - 9 + row, m_ascii[row][col]);
        }
    }

    // Print "A"
    for (uint8_t row = 0; row < 5; row++) {
        for (uint8_t col = 11; col < 22; col++) {
            goto_send(x_title + 79 + col, y_title - 9 + row, a_ascii[row][col - 11]);
        }
    }

    // Print "S"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 22; col < 33; col++) {
            goto_send(x_title + 76 + col, y_title - 10 + row, s_ascii[row][col - 22]);
        }
    }

    // Print "T"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 33; col < 41; col++) {
            goto_send(x_title + 74 + col, y_title - 10 + row, t_ascii[row][col - 33]);
        }
    }

    // Print "E"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 41; col < 50; col++) {
            goto_send(x_title + 72 + col, y_title - 10 + row, e_ascii[row][col - 41]);
        }
    }

    // Print "R"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 50; col < 58; col++) {
            goto_send(x_title + 72 + col, y_title - 10 + row, r_ascii[row][col - 50]);
        }
    }
    // Description text
    char *description[] = {
        "In Key Master, players must collect all the visible keys",
        "scattered across each level to unlock the exit and advance.",
        "while the keys are in plain sight, reaching them won't be easy—",
        "players must navigate tricky platforming sections, dodge enemies,",
        "and solve environmental challenges to progress.",
        "Each level introduces new mechanics, from moving platforms",
        "to dangerous traps, requiring skill and quick thinking.",
        "Can you grab all the keys and make it to the final level?"
    };

    uint8_t desc_x = x_title + 59;  // Adjust position based on the title width
    uint8_t desc_y = y_title - 2;   // Place the description just below the title

    // Print each line of the description below the title
    for (uint8_t i = 0; i < 8; i++) {
        char Goto[10];
        snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", desc_y + i, desc_x);
        UART_send(&huart2, Goto);
        UART_send(&huart2, description[i]);
    }
}

