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
 
 SCOPE void UART_send(UART_HandleTypeDef, char);
 SCOPE void UART_send_newline(UART_HandleTypeDef);
 
 SCOPE void boarder(void);
 SCOPE void stage1(void);
 SCOPE void stage2(void);
 SCOPE void stage3(void);
 
 SCOPE void key(void);
 SCOPE void door(void);
 SCOPE void enemy(void);
 
 
 #undef SCOPE
 #endif /* INC_LEVEL_H_ */
