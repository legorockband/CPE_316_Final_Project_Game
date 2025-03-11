/*
 * Player.c
 *
 *  Created on: Mar 8, 2025
 *      Author: pokem
 */

#include "main.h"
#include "Terminal.h"
#include "Player.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart2;
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
	// Go to center of screen and print Body
	goto_send(x,y,player_ascii[1]);

	// Go to head position and print head
	goto_send(x,y-1,player_ascii[0]);

	// Print left arm
	goto_send(x+1, y, player_ascii[2]);

	// Print right arm
	goto_send(x-1, y, player_ascii[2]);

	// Print left leg
	goto_send(x-1, y+1, player_ascii[4]);

	// Print left leg
	goto_send(x+1, y+1, player_ascii[3]);
}

void goto_send(uint8_t x, uint8_t y, char *player_ascii){
	char Goto[12];
	// Position for Person
	snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", y, x); // Format cursor position string
	UART_send(&huart2, Goto);

	// Send an Body to the center of the screen
	UART_send(&huart2, player_ascii);
}

void updatePlayer(){
	// Get the x and y values for the joy stick
	uint32_t xValue = readADC(&hadc1, ADC_CHANNEL_5); // X-axis on ADC1_IN5

    uint32_t yValue = readADC(&hadc1, ADC_CHANNEL_6); // Y-axis on ADC1_IN6

	char Goto[10];
	snprintf(Goto, sizeof(Goto), "\x1B[%d;%dH", 20, 20); // Format cursor position string
	UART_send(&huart2, Goto);

	// TODO: Change ADC pins to see if there is an error in that

    char msg[40];
    sprintf(msg, "X: %lu, Y: %lu", xValue, yValue);
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);


    // Get range for joy stick and x/y acceptable range
    // Neutral Position
    if (2020 < xValue && xValue < 2080 && 2750 < yValue && yValue < 2820) {
    	createPlayer(x,y);
    }
    // Right Position
    else if (0 <= xValue && xValue < 1600 && 1200 < yValue
    		&& ((x+2) <= 156)) {							// Check if the right arm is touching the right boarder, if it is, don't move right
    	x++;
    	createPlayer(x,y);
    }
    // Left Position
    else if (1600 <= xValue && xValue < 4095 && 1200 < yValue
    		&& ((x-2) >= 3)) {
    	x--;
    	createPlayer(x,y);
    }
    // Up Position
    else if (yValue < 2750 && 1950 < xValue && xValue < 2050) {
    	y--;
    	createPlayer(x,y);
    }
    // Down Position
    else if (yValue > 2850 && 1950 < xValue && xValue < 2050) {
    	y++;
    	createPlayer(x, y);
    }

    if( ((x+2) >= 156) || ((x-2) <= 3) ||	// If the right side or left side of the person is touching or beyond the wall don't move him
    	((y+2) >= 55)  || ((y-2) <= 2)){		// If bottom or top of person is touching or beyond the boarder, don't move

    	createPlayer(x,y);
    }

    HAL_Delay(80);
    clearPlayer(x,y);
}

void collisionCheck(){

}

void clearPlayer(uint8_t x, uint8_t y){
	// Clear the previous body
	goto_send(x, y, player_ascii[5]);
	// Clear the previous head
	goto_send(x, y-1, player_ascii[5]);
	// Clear the previous left arm
	goto_send(x+1, y, player_ascii[5]);
	// Clear the previous right arm
	goto_send(x-1, y, player_ascii[5]);
	// Clear the previous left leg
	goto_send(x-1, y+1, player_ascii[5]);
	// Clear the previous right leg
	goto_send(x+1, y+1, player_ascii[5]);
}

uint32_t readADC(ADC_HandleTypeDef* hadc, uint32_t channel) {
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5; // Increased sampling time

    // Configure ADC channel
    HAL_ADC_ConfigChannel(hadc, &sConfig);

    // Start ADC conversion
    HAL_ADC_Start(hadc);

    // Poll for ADC conversion completion
    HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);

    // Read the ADC value and mask to 12 bits
    uint32_t value = HAL_ADC_GetValue(hadc) & 0x0FFF;

    // Stop ADC
    HAL_ADC_Stop(hadc);

    return value;
}
