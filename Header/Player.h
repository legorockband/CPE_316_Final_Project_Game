/*
 * Player.h
 *
 *  Created on: Mar 8, 2025
 *      Author: pokem
 */

#ifndef INC_PLAYER_H_
#define INC_PLAYER_H_

#ifdef _PLAYER_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

SCOPE void createPlayer(uint8_t, uint8_t);
SCOPE void updatePlayer();
SCOPE void clearPlayer(uint8_t, uint8_t);
SCOPE void goto_send(uint8_t, uint8_t, char*);
SCOPE uint32_t readADC(ADC_HandleTypeDef*, uint32_t);

#undef SCOPE
#endif /* INC_PLAYER_H_ */
