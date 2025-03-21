/*
 * Terminal.h
 *
 *  Created on: Mar 8, 2025
 *      Author: pokem
 */

#ifndef INC_TERMINAL_H_
#define INC_TERMINAL_H_

#ifdef _TERMINAL_C
   #define SCOPE
#else
   #define SCOPE extern
#endif

SCOPE UART_HandleTypeDef huart2;

SCOPE void UART_send(UART_HandleTypeDef*, char*);

SCOPE void goto_send(uint8_t, uint8_t, char*);

#endif /* INC_TERMINAL_H_ */
