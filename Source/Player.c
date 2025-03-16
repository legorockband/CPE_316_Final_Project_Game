/*
 * Player.c
 *
 *  Created on: Mar 8, 2025
 *      Author: pokem
 */

#include "main.h"
#include "Terminal.h"
#include "Level.h"
#include "Player.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern ADC_HandleTypeDef hadc1;

// Position for the player to start at
uint8_t x = 78;
uint8_t y = 27;

char player_ascii[6][2] =
	{
	 {0x4F, 0},  	// Head or O
	 {0x7C, 0}, 	// Body or |
	 {0x2D, 0},		// Arm
	 {0x5C, 0},		// Right leg
	 {0x2F, 0},		// Left Leg
	 {0x20, 0}		// Space Character clear used for later
	};

void createPlayer(uint8_t x, uint8_t y){
	goto_send(x,y,player_ascii[1]);			// Print body
	goto_send(x,y-1,player_ascii[0]);		// Print head
	goto_send(x+1, y, player_ascii[2]);		// Print left arm
	goto_send(x-1, y, player_ascii[2]);		// Print right arm
	goto_send(x-1, y+1, player_ascii[4]);	// Print left leg
	goto_send(x+1, y+1, player_ascii[3]);	// Print left leg
}

//TODO: Lock overlap doesn't work yet
void updatePlayer(){
	bool* wallColl = wallCollision();

	uint8_t* lockPosition = getLockPos();
	uint8_t* playerPosition = getPlayerPos();

	// Extended the range of the person, so if the outer part is touching the lock, then don't do anything
	bool xOverlapLock = isOverlapping(playerPosition[0],lockPosition[0], 1, 4);	// If the x values are in a range of 3 (person) and 9 (key) (extra area around)
	bool yOverlapLock = isOverlapping(playerPosition[1],lockPosition[1], 1, 3);	// If the x values are in a range of 3 (person) and 7 (key) (extra area around)

//  // Print debug info to check the wall position relative to the player
//  char debug[50];
//  sprintf(debug, "Walls: L-%d R-%d U-%d D-%d", wallColl[0], wallColl[1], wallColl[2], wallColl[3]);
//  goto_send(10, 17, debug);

//	char playerPos[20];
//	sprintf(playerPos, "Pos: %d,%d\n", x, y);
//	goto_send(10, 10, playerPos);

	// The values for y are slightly off
	// Get the x and y values for the joy stick
	uint32_t xValue = readADC(&hadc1, ADC_CHANNEL_5); // X-axis on ADC1_IN5
    uint32_t yValue = readADC(&hadc1, ADC_CHANNEL_6); // Y-axis on ADC1_IN6

    // TODO: Edit ranges for joy stick for good ranges
    // Neutral Position
    if ((3100 < xValue && xValue < 3300) &&
    	(3000 < yValue && yValue < 3200)) {
    	createPlayer(x,y);
    }

    // Right Position
    else if ((0 <= xValue && xValue <= 500) &&
    		(0 <= yValue && yValue <= 4095) &&
    		((x+2) <= 156) && 						// Check if the right arm is touching the right boarder, if it is, don't move
			(wallColl[1] == false) &&
			(!xOverlapLock && !yOverlapLock)
			) {
    	x++;
    	createPlayer(x,y);
    }
    // Up Position
    else if ((0 <= xValue && xValue <= 4095) &&
    		(2925 < yValue && yValue <= 4095) &&
			((y-2) >= 2) &&
			(wallColl[2] == false) &&
			(!xOverlapLock && !yOverlapLock)
			) {								// Check if the head is touching the top boarder, if it is, don't move
    	y--;
    	createPlayer(x,y);
    }

    // Down Position
    else if ((0 <= xValue && xValue <= 4095)  &&
    		(0 == yValue) &&
    		((y+2) <= 47) &&
			(wallColl[3] == false) &&
			(!xOverlapLock && !yOverlapLock)
			) {							// Check if the legs are touching the bottom boarder, if it is, don't move
    	y++;
    	createPlayer(x, y);
    }

    // Left Position
    else if ((2940 <= xValue && xValue <= 4095) &&
    		(0 <= yValue && yValue <= 4095) &&
    		((x-2) >= 3) &&
			(wallColl[0] == false) &&
			(!xOverlapLock && !yOverlapLock)
			) {								// Check if the left arm is touching the left boarder, if it is, don't move
    	x--;
    	createPlayer(x,y);
    }

    // Prevent movement if the player is at the lock without the key
    if (xOverlapLock && yOverlapLock && (getKeyStatus() == 0)) {
        goto_send(10, 11, "Need a key!");
        createPlayer(x, y);  					// Redraw player to block movement
    }

    if(wallColl[0] || wallColl[1] || wallColl[2] || wallColl[3] ){
    	createPlayer(x,y);
    }

    if( ((x+2) >= 156) || ((x-2) <= 3) ||		// If the right side or left side of the person is touching the wall redraw at the same position continuously
    	((y+2) >= 47)  || ((y-2) <= 2)){		// If bottom or top of person is touching or beyond the boarder, redraw at the same position continuously

    	createPlayer(x,y);
    }

    // This delay is added for movement speed to be reasonable
    HAL_Delay(80);
    clearPlayer(x,y);
}

void clearPlayer(uint8_t x, uint8_t y){
	goto_send(x, y, player_ascii[5]);	  // Clear the previous body
	goto_send(x, y-1, player_ascii[5]);   // Clear the previous head
	goto_send(x+1, y, player_ascii[5]);   // Clear the previous left arm
	goto_send(x-1, y, player_ascii[5]);   // Clear the previous right arm
	goto_send(x-1, y+1, player_ascii[5]); // Clear the previous left leg
	goto_send(x+1, y+1, player_ascii[5]); // Clear the previous right leg
}

uint32_t readADC(ADC_HandleTypeDef* hadc, uint32_t channel) {
    // Get the correct channel configs for the specific ADC channel
	ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
   	sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;

    HAL_ADC_ConfigChannel(hadc, &sConfig);

    // Start ADC conversion
    HAL_ADC_Start(hadc);

    // Poll for ADC conversion completion
    if (HAL_ADC_PollForConversion(hadc, 10) == HAL_OK) {
        // Check for overrun and clear flag if set
        if (__HAL_ADC_GET_FLAG(hadc, ADC_FLAG_OVR)) {
            __HAL_ADC_CLEAR_FLAG(hadc, ADC_FLAG_OVR);
        }

        // Read the ADC value and mask to 12 bits
        uint32_t value = HAL_ADC_GetValue(hadc) & 0x0FFF;

        // Stop ADC
        HAL_ADC_Stop(hadc);

        return value;
    }

    HAL_ADC_Stop(hadc);
    return 0; // Return 0 if ADC conversion failed
}

uint8_t* getPlayerPos(){
	static uint8_t pos[2];
	pos[0] = x;
	pos[1] = y;
	return pos;
}


