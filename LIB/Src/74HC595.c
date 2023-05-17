#include "main.h"
#include "74HC595.h"

IC74595_statusTypedef IC74595_InitPinAndNumberOfIC(IC74595_typedef *IC74595, uint8_t numberOfIC){

	return IC74595_OK;
}

IC74595_statusTypedef IC74595_ShiftOut(uint8_t *pData){
	if(pData == NULL) {
		return IC74595_INVALID_ARG;
	}
	for(uint8_t j = 0; j < numberOfIC74595; j++){
		for (uint8_t i = 0; i < 8; i++) {
			HAL_GPIO_WritePin(SER_GPIO_Port, SER_Pin, (pData[j] >> i) & 0x01);
			//nhịp 1 xung cạnh xuống đưa dữ liệu vào tầng lưu trữ
			HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, 0);
			HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, 1);
		}
	}
	HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, 0);
	HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, 1);
	return IC74595_OK;
}
