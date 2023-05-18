#include "74HC595.h"

static IC74595_HandelTypedef *pIC74595 = NULL;

//* static prototypes ----------------------------------------------
static void IC74595_InitPin(IC74595_pinInfoTypedef *IC74595_Pin, GPIO_TypeDef *portPin, const uint16_t pin);
static inline GPIO_PinState IC74595_CaculateOutBit(const uint8_t positionByte, const uint8_t positionBit);
static inline void IC74595_WritePin(IC74595_pinInfoTypedef pin, const GPIO_PinState pinState);

static void IC74595_InitPin(IC74595_pinInfoTypedef *IC74595_Pin, GPIO_TypeDef *portPin, const uint16_t pin){
	IC74595_Pin->GPIO_port = portPin;
	IC74595_Pin->GPIO_pin = pin;
}
static inline GPIO_PinState IC74595_CaculateOutBit(const uint8_t positionByte, const uint8_t positionBit){
	uint8_t dataOut;

	if(pIC74595->shift == MSB_FIRST){
		dataOut = pIC74595->data[1 - positionByte] >> (7 - positionBit);
	}
	else {
		dataOut = pIC74595->data[positionByte] >> (positionBit);
	}

	return dataOut & 0x01;
}
static inline void IC74595_WritePin(IC74595_pinInfoTypedef pin, const GPIO_PinState pinState){
	HAL_GPIO_WritePin(pin.GPIO_port, pin.GPIO_pin, pinState);
}



IC74595_statusTypedef IC74595_Init(IC74595_HandelTypedef *IC74595_Object, const uint8_t numberOfIC, const IC74595_ShiftFirstTypedef shift, const GPIO_TypeDef *SERPort, const uint16_t SERPin, const GPIO_TypeDef *SRCLKPort, const uint16_t SRCLKPin, const GPIO_TypeDef *RCLKPort, const uint16_t RCLKPin, const GPIO_TypeDef *OEPort, const uint16_t OEPin){
//IC74595_statusTypedef IC74595_Init(IC74595_typedef *IC74595_Object, uint8_t numberOfIC, bool shift, GPIO_TypeDef *SERPort, uint16_t SERPin, GPIO_TypeDef *SRCLKPort, uint16_t SRCLKPin, GPIO_TypeDef *RCLKPort, uint16_t RCLKPin, GPIO_TypeDef *OEPort, uint16_t OEPin){

	if(SERPort == NULL || SRCLKPort == NULL || RCLKPort == NULL || OEPort == NULL)
		return IC74595_INVALID_ARG;

	if(IC74595_Object == NULL && pIC74595 == NULL){
		IC74595_Object = (IC74595_HandelTypedef*)malloc(sizeof(IC74595_HandelTypedef));
		if(IC74595_Object == NULL){
			return IC74595_ERROR;
		}
	}

	IC74595_Object->data = (uint8_t*)malloc(numberOfIC * sizeof(uint8_t));
	if(IC74595_Object->data == NULL){
		return IC74595_ERROR;
	}

	IC74595_Object->shift = shift;
	IC74595_Object->numberOfIC = numberOfIC;

	IC74595_InitPin(&IC74595_Object->SER, SERPort, SERPin);
	IC74595_InitPin(&IC74595_Object->SRCLK, SRCLKPort, SRCLKPin);
	IC74595_InitPin(&IC74595_Object->RCLK, RCLKPort,RCLKPin);
	IC74595_InitPin(&IC74595_Object->OE, OEPort, OEPin);

	IC74595_SetOject(IC74595_Object);
	IC74595_Enable();
	IC74595_ResetAllOutIC();
	IC74595_ShiftOut();

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_SetOject(const IC74595_HandelTypedef *IC74595_Object){
	if(IC74595_Object == NULL){
		return IC74595_INVALID_ARG;
	}

	pIC74595 = IC74595_Object;

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_ShiftOut(){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	for(uint8_t orderOfIC = 0; orderOfIC < pIC74595->numberOfIC; orderOfIC++){
		for (uint8_t orderOfBit = 0; orderOfBit < 8; orderOfBit++) {
			IC74595_WritePin(pIC74595->SER, IC74595_CaculateOutBit(orderOfIC, orderOfBit));

			IC74595_WritePin(pIC74595->SRCLK, 0);
			IC74595_WritePin(pIC74595->SRCLK, 1);
		}
	}
	IC74595_WritePin(pIC74595->RCLK, 0);
	IC74595_WritePin(pIC74595->RCLK, 1);

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_Disenable(){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	IC74595_WritePin(pIC74595->OE, 1);

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_Enable(){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	IC74595_WritePin(pIC74595->OE, 0);

	return IC74595_OK;
}



IC74595_statusTypedef IC74595_SetPin(const uint8_t orderfOfIC, const uint8_t bit){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(orderfOfIC > pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}
	if(bit > 7){
		return IC74595_INVALID_ARG;
	}

	pIC74595->data[orderfOfIC] |= (0x01 << bit);

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_SetByte(const uint8_t orderfOfIC, const uint8_t byte){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(orderfOfIC > pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}

	pIC74595->data[orderfOfIC] = byte;
//	*(pIC74595->data + orderfOfIC) = byte;

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_SetAllOutIC(const uint8_t *data){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(data == NULL){
		return IC74595_INVALID_ARG;
	}
	if(strlen((char*)data) >  pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}

	pIC74595->data = data;

	return IC74595_OK;
}

IC74595_statusTypedef IC74595_ResetPin(const uint8_t orderfOfIC, const uint8_t bit){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(orderfOfIC > pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}
	if(bit > 7){
		return IC74595_INVALID_ARG;
	}

	pIC74595->data[orderfOfIC] &= ~(0x01 << bit);

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_ReseByte(const uint8_t orderfOfIC){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(orderfOfIC > pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}

	pIC74595->data[orderfOfIC] = 0x00;
//	*(pIC74595->data + orderfOfIC) = byte;

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_ResetAllOutIC(){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	memset(pIC74595->data, 0, pIC74595->numberOfIC * sizeof(uint8_t));

	return IC74595_OK;
}



IC74595_statusTypedef IC74595_GetNumberOfIC(uint8_t *numberOfIC){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	*numberOfIC = pIC74595->numberOfIC;

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_GetBit(const uint8_t orderfOfIC, const uint8_t chooseBit, uint8_t *bit){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(orderfOfIC > pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}
	if(chooseBit > 7){
		return IC74595_INVALID_ARG;
	}

	*bit = (pIC74595->data[orderfOfIC] >> chooseBit) & 0x01;

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_GetByte(const uint8_t orderfOfIC, uint8_t *byte){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}
	if(orderfOfIC > pIC74595->numberOfIC){
		return IC74595_INVALID_ARG;
	}

	*byte = pIC74595->data[orderfOfIC];

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_GetAllOutIC(uint8_t *data){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	data = pIC74595->data;

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_GetAssignPin(const IC74595_pinNameTypedef choosePin, IC74595_pinInfoTypedef *pin){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	switch(choosePin) {

	case IC74595_SER:
		*pin = pIC74595->SER;
		break;

	case IC74595_SRCLK:
		*pin = pIC74595->SRCLK;
		break;

	case IC74595_RCLK:
		*pin = pIC74595->RCLK;
		break;

	case IC74595_OE:
		*pin = pIC74595->OE;
		break;
	default:
		return IC74595_INVALID_ARG;
		break;
	}

	return IC74595_OK;
}



IC74595_statusTypedef IC74595_CheckToggleAllPin(const uint32_t timeSwitch, uint8_t repeat){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	uint8_t *dataShiftOut = (uint8_t *)malloc(sizeof(uint8_t) * pIC74595->numberOfIC);
	if(dataShiftOut == NULL) {
		return IC74595_ERROR;
	}

	while(repeat == 0 ? 1 : repeat--){
		memset(dataShiftOut, 0xff, pIC74595->numberOfIC * sizeof(uint8_t));
		IC74595_SetAllOutIC(dataShiftOut);
		IC74595_ShiftOut();
		HAL_Delay(timeSwitch);

		IC74595_ResetAllOutIC();
		IC74595_ShiftOut();
		HAL_Delay(timeSwitch);
	}
	free(dataShiftOut);

	return IC74595_OK;
}
IC74595_statusTypedef IC74595_CheckOneByOneAllPin(const uint16_t timeSwitchPin, uint8_t repeat){
	if(pIC74595 == NULL){
		return IC74595_ERROR;
	}

	uint8_t *dataShiftOut = (uint8_t *)malloc(sizeof(uint8_t) * pIC74595->numberOfIC);
	if(dataShiftOut == NULL) {
		return IC74595_ERROR;
	}
	memset(dataShiftOut, 0x00, pIC74595->numberOfIC * sizeof(uint8_t));

	while(repeat == 0 ? 1 : repeat--){
		for(uint8_t orderOfIC = 0; orderOfIC < pIC74595->numberOfIC; orderOfIC++){
			for (uint8_t orderOfBit = 0; orderOfBit < 8; orderOfBit++) {
				dataShiftOut[orderOfIC] = (0x01) << orderOfBit;
//				dataShiftOut[pIC74595->numberOfIC - 1 - orderOfIC] = (0x01) << orderOfBit;
				IC74595_SetAllOutIC(dataShiftOut);
				IC74595_ShiftOut();
				HAL_Delay(timeSwitchPin);
			}
			dataShiftOut[orderOfIC] = 0x00;
		}
	}
	return IC74595_OK;
}


//* End of file
