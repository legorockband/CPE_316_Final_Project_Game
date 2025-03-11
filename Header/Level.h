/*
 * Level.h
 *
 *  Created on: Mar 1, 2025
 *      Author: pokem
 */

#ifndef INC_LEVEL_H_
#define INC_LEVEL_H_

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

SCOPE void key(uint8_t, uint8_t);
SCOPE void lock(uint8_t, uint8_t);
SCOPE void enemy(void);

SCOPE uint8_t* getKeyPos(void);
SCOPE void checkPlayerPos(void);

#undef SCOPE
#endif /* INC_LEVEL_H_ */
