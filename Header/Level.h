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

typedef struct {
    uint8_t x;
    uint8_t y;
} Wall;

SCOPE void border(void);
SCOPE void title(uint8_t, uint8_t);
SCOPE void thankYou(uint8_t, uint8_t);

SCOPE void stage1(void);
SCOPE void stage2(void);
SCOPE void stage3(void);
SCOPE void congrats(void);

SCOPE void makewall(uint8_t x_start, uint8_t y_start, uint8_t length, char direction);
SCOPE bool* wallCollision(void);
SCOPE void resetWalls(void);

SCOPE void key(uint8_t, uint8_t, char[][2]);
SCOPE void trapKey(void);
SCOPE bool trapStatus(void);
SCOPE void makeMaze(void);

SCOPE void lock(uint8_t, uint8_t, char[][2]);
SCOPE bool* lockCollision(uint8_t, uint8_t);

SCOPE void enemy(void);

SCOPE uint8_t* getKeyPos(void);
SCOPE uint8_t getKeyStatus(void);
SCOPE bool getSkipKey(void);
SCOPE uint8_t* getLockPos(void);
SCOPE uint8_t getCurrentStage(void);

SCOPE bool checkInvalidPos(void);
SCOPE bool isOverlapping(uint8_t , uint8_t , uint8_t , uint8_t );

#undef SCOPE
#endif /* INC_LEVEL_H_ */
