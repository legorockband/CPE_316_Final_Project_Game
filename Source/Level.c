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

#define MAX_WALLS 705

Wall walls[MAX_WALLS];
uint16_t total_walls = 0;

uint8_t currentStage = 0;
uint8_t gotKey = 0;
uint8_t gotFakeKey = 0;
bool skipKey = false;
bool activateTrap = false;

void (*stages[3])(void) = {stage1, stage2, stage3};

uint8_t all_key_pos[4][2] = {
		{145, 5},				// Stage 1 Key
		{10, 22},				// Stage 2 Key
		{78, 13},				// Stage 3 Key
		{88, 15}
};

uint8_t all_fake_key[3][2] = {
		{104, 18},				// Fake Key between line 25 and 27 of maze
		{71,  35},				// Fake Key between line 12 and 23 of maze
		{104, 27}				// Fake Key between line 40 and 42 of maze
};

uint8_t all_lock_pos[3][2] = {
		{15, 32},				// Stage 1 Lock
		{10, 30},				// Stage 2 Lock
		{145, 25}				// Stage 3 Lock
};

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
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "  | |   "
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}}   // "  |_|   "
};

char m_ascii[6][15][2] = {
    {{0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}},  // "____       ____"
    {{0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}},  // "|   \     /   |"
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x5C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x2F,0}, {0x7C,0}, {0x20,0}, {0x7C,0}},  // "| |\ \   / /| |"
    {{0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x5C,0}, {0x5F,0}, {0x2F,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}},  // "| | \ \_/ / | |"
	{{0x7C,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x5C,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x20,0}, {0x7C,0}},  // "| |  \___/  | |"
	{{0x7C,0}, {0x5F,0}, {0x7C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x7C,0}, {0x5F,0}, {0x7C,0}}   // "|_|         |_|"
};



char a_ascii[6][13][2] = {
	{{0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "      _      "
	{{0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "     / \     "
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "    / _ \    "
    {{0x20,0}, {0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x2F,0}, {0x5F,0}, {0x5C,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}, {0x20,0}},  // "   / /_\ \   "
    {{0x20,0}, {0x20,0}, {0x2F,0}, {0x20,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x5F,0}, {0x20,0}, {0x5C,0}, {0x20,0}, {0x20,0}},  // "  / _____ \  "
    {{0x20,0}, {0x2F,0}, {0x5F,0}, {0x2F,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x20,0}, {0x5C,0}, {0x5F,0}, {0x5C,0}, {0x20,0}}   // " /_/     \_\ "
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

void border(void){
	int y,x,n;

	// Double the Thickness for border
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

	// Double the Thickness for border
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

	// Top border
	for(x = 0; x < 157; x++){
		char Goto[10];
		snprintf(Goto, sizeof(Goto), "\x1B[0;%dH", x); // Format cursor position string
		UART_send(&huart2, Goto);

		// Send an extended block character
		UART_send(&huart2, boarder_char);
	}

	// Bottom border
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

//	char debug[20];
//	sprintf(debug, "Total Walls: %d", total_walls);
//	goto_send(10, 9, debug);
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
    for (uint16_t i = 0; i < total_walls; i++) {
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

//	char debug[50];
//	sprintf(debug, "Walls: L-%d R-%d U-%d D-%d", wallCheck[0], wallCheck[1], wallCheck[2], wallCheck[3]);
//	goto_send(10, 12, debug);

    return wallCheck;
}

bool* lockCollision(uint8_t stage, uint8_t keyNum){
	// Set an array of bool values to false for each direction
	static bool lockCheck[4] = {false, false, false, false};		// [0] is Left, [1] is Right, [2] is Up, [3] is Down

	if(keyNum == 0){
		uint8_t* playerPos = getPlayerPos();
		uint8_t player_x = playerPos[0];
		uint8_t player_y = playerPos[1];

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
	}
    return lockCheck;
}

void stage1(void){
	currentStage = 1;
	border();
	key(all_key_pos[0][0], all_key_pos[0][1], key_ascii_art);
	lock(all_lock_pos[0][0], all_lock_pos[0][1], lock_ascii_art);
    makewall(30, 32, 12, 'L');  // Up works(how far over[x],how far down[y],how many  )
    makewall(30, 32, 19, 'D');  // Left works
    makewall(3,  32, 9, 'R');
}

void stage2(void) {
	currentStage = 2;
	setPlayerPos(78,27);	// Reset the player to the center of the screen
	resetWalls();
	border();

    key(all_key_pos[1][0], all_key_pos[1][1], key_ascii_art);
    lock(all_lock_pos[1][0], all_lock_pos[1][1], lock_ascii_art);

    // Create a T shape on the left edge
    makewall(3,  27, 15, 'R');
    makewall(18, 27, 10, 'U');
    makewall(18, 27, 10, 'D');
}

void stage3(void){
	currentStage = 3;
	activateTrap = false;
	skipKey = true;			// This is used to bypass the key and go straight to the next stage

	setPlayerPos(80,27);	// Reset the player to the center of the screen
	resetWalls();
	border();

    key(all_key_pos[2][0], all_key_pos[2][1], key_ascii_art);
    lock(all_lock_pos[2][0], all_lock_pos[2][1], lock_ascii_art);
}

//void stage4(){
//	curretnStage = 4;
//
//}

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

void fakeKey(uint8_t x, uint8_t y, char key_ascii[][2]){
	/*
	/\___		Art for key character
	\/ |
	*/

	goto_send(x-2, y, key_ascii[1]);	// Create the top left of handle
	goto_send(x-1, y, key_ascii[0]);	// Create the top right of handle
	goto_send(x, y, key_ascii[3]); 		// Create the first part of the shaft
	goto_send(x+1, y, key_ascii[3]); 	// Create the second part of the shaft
	goto_send(x+2, y, key_ascii[3]); 	// Create the third part of the shaft

	goto_send(x-2, y+1, key_ascii[0]);	// Create the bottom right of handle
	goto_send(x-1, y+1, key_ascii[1]);	// Create the bottom left of handle
	goto_send(x+1, y+1, key_ascii[2]); 	// Create the left tooth
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

void fakeLock(uint8_t x_lock, uint8_t y_lock, char lock_ascii[][2]){
	/*
	   ___
	 _/___\_
	 | ( ) |
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

	// First key
	bool xOverlapFakeKey1 = isOverlapping(playerPosition[0], all_fake_key[0][0], 1, 2);
	bool yOverlapFakeKey1 = isOverlapping(playerPosition[1], all_fake_key[0][1], 1, 1);

	// Second key
	bool xOverlapFakeKey2 = isOverlapping(playerPosition[0], all_fake_key[1][0], 1, 2);
	bool yOverlapFakeKey2 = isOverlapping(playerPosition[1], all_fake_key[1][1], 1, 1);

	// Third key
	bool xOverlapFakeKey3 = isOverlapping(playerPosition[0], all_fake_key[2][0], 1, 2);
	bool yOverlapFakeKey3 = isOverlapping(playerPosition[1], all_fake_key[2][1], 1, 1);

	bool xOverlapFakeLock = isOverlapping(playerPosition[0],29, 1, 3);	// Checks if the x values are in a range of 3 (person) and 7 (key) since a lock is 7x5
	bool yOverlapFakeLock = isOverlapping(playerPosition[1],13, 1, 2);	// Checks if the x values are in a range of 3 (person) and 5 (key) since a key is 7x5

	if(gotFakeKey == 3){
		goto_send(5,5, "Did those keys do anything?");
		fakeLock(29, 13, lock_ascii_art);
		gotFakeKey = 0;
	}

	// If the player is over a key, pick up the key
	if(xOverlapKey && yOverlapKey){
        goto_send(5, 3, "Key Got!   ");		// Extra spaces to remove the Need Key text
        key(keyPosition[0], keyPosition[1], clear_ascii_art);		// Clear the key
        gotKey = 1;					// The player has a key
	}

	else if(xOverlapFakeKey1 && yOverlapFakeKey1 && currentStage == 3)
	{
		fakeKey(all_fake_key[0][0], all_fake_key[0][1], clear_ascii_art);
		gotFakeKey = 1;
	}

	else if(xOverlapFakeKey2 && yOverlapFakeKey2 && currentStage == 3)
	{
		fakeKey(all_fake_key[1][0], all_fake_key[1][1], clear_ascii_art);
		gotFakeKey = 2;
	}

	else if(xOverlapFakeKey3 && yOverlapFakeKey3 && currentStage == 3)
	{
		fakeKey(all_fake_key[2][0], all_fake_key[2][1], clear_ascii_art);
		gotFakeKey = 3;
	}


	// If you are overlapping the lock with the key or the is the skipKey flag is true, go to the next stage
	else if(xOverlapLock && yOverlapLock && ((gotKey == 1) || (skipKey))){
		char ClearScreen[] = { 0x1B, '[', '2' , 'J',0 }; 	// Clear the screen
		char CursorHome[] = { 0x1B, '[' , 'H' , 0 }; 	// Home the cursor

//		goto_send(60, 11, "Lock Open");
        lock(lockPosition[0], lockPosition[1], clear_ascii_art);	// Clear the lock
        gotKey = 0;

        currentStage++;		// Go to the next stage

    	HAL_UART_Transmit(&huart2, (uint8_t *)ClearScreen, strlen(ClearScreen), HAL_MAX_DELAY);
    	HAL_UART_Transmit(&huart2, (uint8_t *)CursorHome, strlen(CursorHome), HAL_MAX_DELAY);

        stages[currentStage - 1]();			// Draw the next stage
	}

	else if(xOverlapFakeLock && yOverlapFakeLock && currentStage == 3){
		goto_send(5,6, "Well it seems like those keys did nothing");
		fakeLock(29, 13, clear_ascii_art);
	}

}

void trapKey(){
	switch(currentStage)
	{
	case(0):
			return;
	case(1):
			return;
	case(2):
			if(gotKey == 1 && activateTrap == false){
				makewall(18, 27, 70, 'R');
				goto_send(5,4, "The key activated something ...");
				activateTrap = true;
			}
	case(3):
			if(gotKey == 1 && activateTrap == false){
				makeMaze();

			    fakeKey(all_fake_key[0][0], all_fake_key[0][1], key_ascii_art);
			    fakeKey(all_fake_key[1][0], all_fake_key[1][1], key_ascii_art);
			    fakeKey(all_fake_key[2][0], all_fake_key[2][1], key_ascii_art);


				goto_send(5,4, "but at what cost ...");
				activateTrap = true;
			}
	}
}

void makeMaze(void){
    makewall(66, 18, 26, 'R');		// 1
    makewall(84, 18, 13, 'U');		// 2
    makewall(92, 1,  13, 'D');		// 3

    makewall(76, 12, 20, 'L');		// 4
    makewall(76, 12, 12, 'U');		// 5

    makewall(56, 12, 11, 'D');		// 6
    makewall(56, 23, 10, 'L');		// 7
    makewall(46, 23, 10, 'D');		// 8
    makewall(46, 33,  12, 'L');		// 9

    makewall(56, 28,  6, 'D');		// 10
    makewall(56, 31,  8, 'R');		// 11
    makewall(66, 18, 20, 'D');		// 12

    makewall(34, 33, 17, 'U');		// 13
    makewall(34, 16,  5, 'R');		// 14
    makewall(39, 16,  6, 'U');		// 15
    makewall(39, 10, 20, 'L');		// 16
    makewall(19, 10,  6, 'D');		// 17
    makewall(19, 16,  5, 'R');		// 18
    makewall(24, 16, 17, 'D');		// 19
    makewall(24, 33, 22, 'L');		// 20

    makewall(24, 38, 52, 'R');		// 21
    makewall(24, 38,  5, 'D');		// 22
    makewall(76, 38, 10, 'U');		// 23
    makewall(66, 25, 15, 'R');		// 24

    makewall(92,  18,  3, 'D');		// 25
    makewall(92,  21, 24, 'R');		// 26
    makewall(116, 21,  4, 'U');		// 27
    makewall(116, 17, 18, 'L');		// 28
    makewall(99,  17, 12, 'U');		// 29
    makewall(99,   5, 50, 'R');		// 30
    makewall(149,  5, 14, 'D');		// 31
    makewall(149, 19,  8, 'R');		// 32
    makewall(157, 29, 28, 'L');		// 33
    makewall(129, 29, 16, 'U');		// 34
    makewall(129, 19,  8, 'R');		// 35
    makewall(137, 19, 10, 'U');		// 36
    makewall(137,  9, 30, 'L');		// 37
    makewall(107,  9,  4, 'D');		// 38
    makewall(107, 13, 23, 'R');		// 39

    makewall(92,  53, 28, 'U');		// 40
    makewall(92,  25, 25, 'R');		// 41
    makewall(117, 25, 10, 'D');		// 42
    makewall(117, 35, 20, 'R');		// 43
    makewall(137, 35,  8, 'D');		// 44
    makewall(137, 43, 20, 'L');		// 45


//    key(all_key_pos[3][0], all_key_pos[3][1], key_ascii_art);		//Fake key 1

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

bool getSkipKey(){
	return skipKey;
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
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 0; col < 15; col++) {
            goto_send(x_title + 76 + col, y_title - 10 + row, m_ascii[row][col]);
        }
    }

    // Print "A"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 15; col < 27; col++) {
            goto_send(x_title + 76 + col, y_title - 10 + row, a_ascii[row][col - 15]);
        }
    }

    // Print "S"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 27; col < 38; col++) {
            goto_send(x_title + 76 + col, y_title - 10 + row, s_ascii[row][col - 27]);
        }
    }

    // Print "T"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 38; col < 46; col++) {
            goto_send(x_title + 74 + col, y_title - 10 + row, t_ascii[row][col - 38]);
        }
    }

    // Print "E"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 46; col < 55; col++) {
            goto_send(x_title + 72 + col, y_title - 10 + row, e_ascii[row][col - 46]);
        }
    }

    // Print "R"
    for (uint8_t row = 0; row < 6; row++) {
        for (uint8_t col = 55; col < 63; col++) {
            goto_send(x_title + 72 + col, y_title - 10 + row, r_ascii[row][col - 55]);
        }
    }
    // Description text
    char *description[] = {
        "In Key Master, players must collect all the visible keys",
        "scattered across each level to unlock the exit and advance.",
        "while the keys are in plain sight, reaching them won't be easy—",
        "players must navigate tricky sections, adapt to changing level,",
        "and solve environmental challenges to progress.",
        "Can you grab all the keys and make it to the final level?"
    };

    char *createdBy = "Created By: Troy Renner and Samuel Ramirez";

    uint8_t desc_x = x_title + 59;  // Adjust position based on the title width
    uint8_t desc_y = y_title + 3;   // Place the description just below the title

    // Print each line of the description below the title
    for (uint8_t i = 0; i < 5; i++) {
        char Goto[20];
        snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", desc_y + i, desc_x);
        UART_send(&huart2, Goto);
        UART_send(&huart2, description[i]);

//    	goto_send(desc_y + i, desc_x, description[i]);
    }

    goto_send(desc_x + 6, desc_y + 6, createdBy);
}

