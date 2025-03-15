/*
 * Level.h
 *
 *  Created on: Mar 1, 2025
 *      Author: pokem
 */

#ifndef INC_LEVEL_H_
#define INC_LEVEL_H_

#include <stdbool.h>

#ifdef _LEVEL_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

SCOPE void boarder(void);
SCOPE void title(void);
SCOPE void stage1(void);
SCOPE void stage2(void);
SCOPE void stage3(void);

SCOPE void key(uint8_t, uint8_t, char[][2]);
SCOPE void lock(uint8_t, uint8_t, char[][2]);
SCOPE void enemy(void);

SCOPE uint8_t* getKeyPos(void);
SCOPE uint8_t* getLockPos(void);
SCOPE void checkPlayerPos(void);
SCOPE bool isOverlapping(uint8_t , uint8_t , uint8_t , uint8_t );

#undef SCOPE
#endif /* INC_LEVEL_H_ */
