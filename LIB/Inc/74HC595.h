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
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"

#define VERSION_74595_LIB_asString Ver0.1

//#define LSB_FIRST false
//#define MSB_FIRST true

typedef enum {
	LSB_FIRST 	= 0u,
	MSB_FIRST 	= 1u
} IC74595_ShiftFirstTypedef;

typedef enum{
	IC74595_SER 	= 0u,
	IC74595_SRCLK 	= 1u,
	IC74595_RCLK  	= 2u,
	IC74595_OE		= 3u
}IC74595_pinNameTypedef;

typedef enum{
	IC74595_OK = 0u,
	IC74595_ERROR,
	IC74595_INVALID_ARG,
//	IC74595_BEYOND_MAX_CASCADE,
}IC74595_statusTypedef;

typedef struct {
    GPIO_TypeDef *GPIO_port;
    uint16_t GPIO_pin;
} IC74595_pinInfoTypedef;

//object
typedef struct {
	IC74595_pinInfoTypedef SER, OE, SRCLK, RCLK;
	uint8_t *data;
	uint8_t numberOfIC;
	IC74595_ShiftFirstTypedef shift;
} IC74595_HandelTypedef;


IC74595_statusTypedef IC74595_Init(IC74595_HandelTypedef *IC74595_Object, const uint8_t numberOfIC, const IC74595_ShiftFirstTypedef shift, const GPIO_TypeDef *SERPort, const uint16_t SERPin, const GPIO_TypeDef *SRCLKPort, const uint16_t SRCLKPin, const GPIO_TypeDef *RCLKPort, const uint16_t RCLKPin, const GPIO_TypeDef *OEPort, const uint16_t OEPin);
IC74595_statusTypedef IC74595_SetOject(const IC74595_HandelTypedef *IC74595_Object);
IC74595_statusTypedef IC74595_ShiftOut();
IC74595_statusTypedef IC74595_Disenable();
IC74595_statusTypedef IC74595_Enable();



IC74595_statusTypedef IC74595_SetPin(const uint8_t orderfOfIC, const uint8_t bit);
IC74595_statusTypedef IC74595_SetByte(const uint8_t orderfOfIC, const uint8_t byte);
IC74595_statusTypedef IC74595_SetAllOutIC(const uint8_t *data);

IC74595_statusTypedef IC74595_ResetPin(const uint8_t orderfOfIC, const uint8_t bit);
IC74595_statusTypedef IC74595_ReseByte(const uint8_t orderfOfIC);
IC74595_statusTypedef IC74595_ResetAllOutIC();


IC74595_statusTypedef IC74595_GetNumberOfIC(uint8_t *numberOfIC);
IC74595_statusTypedef IC74595_GetByte(const uint8_t orderfOfIC, uint8_t *byte);
IC74595_statusTypedef IC74595_GetAssignPin(const IC74595_pinNameTypedef choosePin, IC74595_pinInfoTypedef *pin);
IC74595_statusTypedef IC74595_CheckToggleAllPin(const uint32_t timeSwitch, uint8_t repeat);
IC74595_statusTypedef IC74595_CheckOneByOneAllPin(const uint16_t timeSwitchPin, uint8_t repeat);




#endif // end _74595_H_
