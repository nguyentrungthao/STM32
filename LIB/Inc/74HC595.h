/*
 * 74HC595.h
 *
 *  Created on: Mar 23, 2023
 *      Author: Thao
 *
 */

#ifndef _74595_H_
#define _74595_H_
//#define OPEN_DRAIN
#include "main.h"
#include "string.h"

#define VERSION_74595_LIB_asString Ver0.1

#define LSB_FIRST 0
#define MSB_FIRST 1

typedef struct {
    GPIO_TypeDef *GPIO_Port;
    uint16_t GPIO_pin;
} IC74595_pinInfoTypedef;

//object
typedef struct {
	IC74595_pinInfoTypedef SER, OE, SRCLK, RCLK;
	uint8_t numberOfIC;
} IC74595_typedef;

typedef enum{
	IC74595_LATCH = 0u,
	IC74595_CLK,
	IC74595_DS,
	IC74595_OE
}IC74595_pinNameTypedef;

typedef enum{
	IC74595_OK = 0u,
	IC74595_ERROR,
	IC74595_INVALID_ARG,
//	IC74595_BEYOND_MAX_CASCADE,
}IC74595_statusTypedef;

static IC74595_typedef *pIC74595 = NULL;

IC74595_statusTypedef IC74595_InitPinAndNumberOfIC(IC74595_typedef *IC74595, GPIO_TypeDef *SERPort, uint16_t SERPin, GPIO_TypeDef *SRCLKPort, uint16_t SRCLKPin, GPIO_TypeDef *RCLKPort, uint16_t RCLKPin, GPIO_TypeDef *OEPort, uint16_t OEPin, uint8_t numberOfIC);
IC74595_statusTypedef IC74595_ShiftOut(uint8_t *pData);

IC74595_statusTypedef IC74595_SetPin(uint8_t , uint8_t);
IC74595_statusTypedef IC74595_ResetPin(uint8_t , uint8_t);
IC74595_statusTypedef IC74595_SetByte(uint8_t , uint8_t);
IC74595_statusTypedef IC74595_ReseByte(uint8_t , uint8_t);

void IC74595_SetAllHigh();
void IC74595_SetAllLow();

uint8_t IC74595_GetNumberOfIC();
uint8_t IC74595_GetBit(uint8_t, uint8_t);
uint8_t IC74595_GetByte(uint8_t, uint8_t);
uint8_t *IC74595_GetAll();
IC74595_typedef IC74595_GetAssignPin();

void IC74595_Disenable();
void IC74595_Enable();




#endif // end _74595_H_
